#include <sys/types.h>
#include <ctype.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
#include <signal.h>		
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include "fun.h"


#define LISTEN_QUEUE_NUM 5
#define BUFFER_SIZE 1024
#define ECHO_PORT 50013


struct user client;	//用户名和密码。
struct msg *head;	//在线用户的链表头	
//struct user *h;	//已注册用户信息的链表头	
pthread_mutex_t mutex_1;	//互斥锁，用于在线用户链表/聊天记录文件


//删除指定用户（描述符为fd）
void delete_user (struct msg *head, int fd)
{
	struct msg *p, *s;

	for(p = head; p != NULL; p = p->next)
	{
		if(p->next->fd == fd)
		{
			s = p->next;
			p->next = s->next;
			free(s);

			break;
		}
	}
}



//线程执行函数
void thread_routine (void *param)
{
	int new_sock = *((int *)param);		//连接套接字
	FILE *logsfp;				
	int byteread;	
	char buf[4000];	
	char onlinename[BUFFER_SIZE];		//在线用户列表
	int onlinefd;				//发送给客户端的连接套接字	

	struct timeval timeout;			//select等待时间
	fd_set readset, r_readset;
	int maxfd;
	int nfound;
		
	//截取名字
	int k;
	int i;					//循环变量
	char *getname[20];
	char *q;
	
	struct msg msgs;
	struct msg *s, *p;
	struct msg tmp;				//退出信息

	getname[0] = (char *)malloc(40);
	getname[1] = (char *)malloc(40);
	getname[2] = (char *)malloc(40);
	getname[3] = (char *)malloc(40);
	getname[4] = (char *)malloc(40);
	getname[5] = (char *)malloc(40);
	getname[6] = (char *)malloc(40);
	getname[7] = (char *)malloc(40);	//除了all，若群聊：只能和<=7个人聊

	//发送当前的套接字
	onlinefd = new_sock;
	if(send(new_sock, &onlinefd, sizeof(int), 0) < 0)
	{
		perror("sssend2 : ");
		pthread_exit(NULL);
	}

	pthread_mutex_lock(&mutex_1);
	//申请节点，插入链表
	s = (struct msg *)malloc(sizeof(struct msg));
	strcpy(s->from_name, client.name);
	s->fd = new_sock;
	s->next = head->next;
	head->next = s;
	
	//向当前用户发送消息“在线用户列表”
	bzero(onlinename, sizeof(onlinename));
	strcpy(onlinename, "online people:");
	for(p = head->next; p != NULL; p = p->next)
	{
		strcat(onlinename, p->from_name);//将在线的人追加到onlinename数组中
		strcat(onlinename, " ");	
	}
	send(new_sock, onlinename, sizeof(onlinename), 0);	


	//向其他用户发送信息“当前用户上线了”
	sprintf(tmp.message, "\n%son the line，you can talk to him/her now\n", client.name);
	for(p = head->next; p != NULL; p = p->next)
	{	
		if(p->fd != new_sock)
		{
			if(send(p->fd, &tmp, sizeof(struct msg), 0) < 0)
				perror("send : ");
		}								
	}
	pthread_mutex_unlock(&mutex_1);
	
	FD_ZERO(&readset);//文件描述符清空
	FD_SET(new_sock, &readset);//文件描述符集合中增加一个新的文件描述符

	maxfd = new_sock + 1;
	//多路复用
	while(1)	
	{
		r_readset = readset;

		timeout.tv_sec = 0;//以秒为单位
		timeout.tv_usec = 500000;//以毫秒为单位

		if((nfound = select(maxfd, &r_readset, (fd_set *)0, (fd_set *)0, &timeout)) < 0)//检查是否可读
		{
			perror("select");
			continue;
		}
		else if(nfound == 0)
		{
			continue;
		}

		if(FD_ISSET(new_sock, &r_readset))///检查new_sock是否在这个集合里面, 
		{
			//接收消息
			byteread = recv(new_sock, &msgs, sizeof(msgs), 0);		
			if(byteread < 0)	//recv出错
			{
				perror("recv -- :");
				break;
			}
			else if(byteread == 0)	//连接中止，发送下线信息
			{
				break;
			}	
			//若没“：”，无法处理，直接下一次。				
			if(strrchr(msgs.message, ':') == NULL)	
			{
				continue;
			}
	
			//截取聊天信息中的用户名和内容	
			k = 0;
			strcpy(getname[k], strtok(msgs.message, ":"));

			k++;
			while((q = strtok(NULL, ":")))
			{
				strcpy(getname[k], q);
				k++;
			}

			strcpy(msgs.message, getname[k - 1]);

			pthread_mutex_lock(&mutex_1);
			//记录聊天情况
			if((logsfp = fopen("log.txt", "a")) == NULL)	
			{
				perror("open file : ");	
			}


			for(i = 0; i < k-1; i++)
			{
				sprintf(buf, "\n%s to %s %s %s\n", msgs.from_name, getname[i], msgs.info, msgs.message);
				fprintf(logsfp, "%s", buf);	//写入文件

				//向一个人发送消息
				for(p = head->next; p != NULL; p = p->next)
				{
					if(strcmp(getname[i], p->from_name) == 0)
					{
						send(p->fd, &msgs, sizeof(msgs), 0);
					}
				}
			}

			//向所有人发送消息
			if(strcmp(getname[0], "all") == 0)
			{
				for(p = head->next; p != NULL; p = p->next)
				{
					if(strcmp(msgs.from_name, p->from_name) != 0)
					{
						send(p->fd, &msgs, sizeof(msgs), 0);
					}
				}												
			}

			fclose(logsfp);
			pthread_mutex_unlock(&mutex_1);			
		}
	}
	pthread_mutex_lock(&mutex_1);
	//清理用户（下线的）
	delete_user(head, new_sock);
	close(new_sock);
	
	//发送下线消息
	sprintf(tmp.message, "\n%s 下线了\n", msgs.from_name);
	for(p = head->next; p != NULL; p = p->next)
	{
		send(p->fd, &tmp, sizeof(tmp), 0);
	}
	pthread_mutex_unlock(&mutex_1);

	pthread_exit(NULL);
}

/*
功能：登录验证
返回值：成功返回1；名字或密码不对，失败返回0；重复登录，返回-1；
*/
void anybody_register(int newfd)
{
	int flag=1;
	FILE* fp;
	int i=0;
	struct user usr[300];

	if((fp=fopen("user.dat", "r")) == NULL)	//user.dat存放着所有的用户名和密码
	{
		perror("open");
		exit(-1);
	}
	for(i = 0; i < 300; i++)
	{
		fscanf(fp, "%s %s", usr[i].name, usr[i].code);

		if((strcmp(client.name, usr[i].name) == 0))
		{
			 flag=-1;
			 break;
		}
	}
	fclose(fp);
	if(flag!=-1)
	{
		if((fp=fopen("user.dat","a+"))==NULL)
		{
			perror("open");
			exit(-1);
		}
		fprintf(fp," %s %s",client.name,client.code);//未完待续;
		printf("The registration information is:%s %s\n",client.name,client.code);
		fclose(fp);
	}
	if(send(newfd, &flag, sizeof(flag), 0) < 0)
	{
		perror("send1 : ");
	}
	
}
int anybody_login (int newfd, struct msg *head)
{
	struct user users[300];
	int i;
	struct msg *p;
	int flag = 0;	//核对用户名和密码的标志位，0表示验证失败，1表示验证成功。
	FILE *fp;

	
	for(p = head->next; p != NULL; p = p->next)
	{
		if(!strcmp(p->from_name, client.name))	//重复登录，返回-1
		{
			flag = -1;
			break;
		}
	}

	if(flag != -1)//验证账号和密码是否一致
	{
		
		if((fp=fopen("user.dat", "r")) == NULL)	//user.dat存放着所有的用户名和密码
		{
			perror("open");
			exit(-1);
		}

		for(i = 0; i < 300; i++)
		{
			fscanf(fp, "%s %s", users[i].name, users[i].code);

			if((strcmp(client.name, users[i].name) == 0) && (strcmp(client.code, users[i].code) == 0))
			{
				 flag=1;
				 break;
			}
		}

		fclose(fp);
	}

	if(send(newfd, &flag, sizeof(flag), 0) < 0)
	{
		perror("send1 : ");
	}

	return flag;
}


int main (int argc, char *argv[])
{
	struct sockaddr_in servaddr, remote;	
	uint32_t addrlen;		
	int flag;				//登录成功与否的标志位
	pthread_t thread_id;	//声明线程ID;
	int request_sock;		//监听套接字
	int new_sock;			//连接套接字
	int i=0;


	//创建在线用户链表的头节点
	head = (struct msg *)malloc(sizeof(struct msg));	
	head->next = NULL;	
	
	//初始化互斥锁	
	if(pthread_mutex_init(&mutex_1, NULL) < 0)
	{
		printf("mutex1 init error\n");
		return -1;
	}

	//创建套接字
	if((request_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("socket:");
		return -1;
	}
	
	memset(&servaddr, 0, sizeof(servaddr));//char初始化函数(作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法)；
	servaddr.sin_family = AF_INET;//表明服务器地址协议类型为：TCP/IP类型；
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(50013);
	
	int optval = 1;
	if (setsockopt(request_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}
		
	//绑定IP 和 

	if(bind(request_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)	
	{
		perror("bind");
		return -1;
	}
	
	//监听
	if(listen(request_sock, LISTEN_QUEUE_NUM) < 0)
	{
		perror("listen");
		return -1;
	}

	while(1)
	{
		addrlen = sizeof(remote);
		
		//有人登录，进行连接。
		if((new_sock = accept(request_sock, (struct sockaddr *)&remote, &addrlen)) < 0)
		{
			
			perror("accept");
			continue;
		}
		printf("Receives the request from the client for the %d time.\n",++i);			
		recv(new_sock, &client, sizeof(client), 0);//接收从客户端发送过来的struct user结构体信息;
		switch(client.flag)
		{
			case 0:
				//登录验证
				flag = anybody_login(new_sock,head);
				//登录失败
				if(flag != 1)	
				{
					close(new_sock);
					continue;	
				}
				if(pthread_create(&thread_id, NULL, (void *)thread_routine, &new_sock) != 0)
				{
					close(new_sock);	//若失败，主进程关掉连接套接字
				}
				break;
			case 1:			
				anybody_register(new_sock);
				break;
			default :
				break;
		}
	}

	return 0;	
}







