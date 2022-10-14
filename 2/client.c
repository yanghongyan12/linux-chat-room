#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "fun.h"
#include "color.h"

#define RET_OK 	0
#define RET_ERR	-1
#define LISTEN_QUEUE_NUM 5
#define BUFFER_SIZE 1024
#define ECHO_PORT 50013

int q;
int flag1=0;
int flag = 0;		//1是成功，0是失败。
//注册
void sign_in(int sockfd)
{
	struct user rgist={"q","q",1};	 //保存注册信息的结构体;
	char code[20];
	
	printf(YELLOW "\t********************************************************\n" NONE);	
	printf(YELLOW "\t*               Please Register                        *\n" NONE);
	printf(YELLOW "\t*------------------------------------------------------*\n" NONE);
	printf(L_BLUE "username:" NONE);
	scanf("%s",rgist.name);
	printf(L_BLUE "password:" NONE);
	scanf("%s",rgist.code);
	printf(L_BLUE "confirm password:" NONE);
	scanf("%s",code);

	if(strcmp(rgist.code,code)==0)
	{
		send(sockfd, &rgist, sizeof(struct user), 0);	//发送用户名和密码
		recv(sockfd, &flag, sizeof(int), 0);		//接收服务器的信息flag
		//printf("flag=%d\n",flag);
		if(flag == 1)//注册成功;
		{
			printf("Registered successfully!\n");
		}
		else //用户重复注册;
		{
			printf("The user has ben registered!\n");
			
		}
	}
	else{
		printf(RED "The two passwords are different!Please input again!\n" NONE);
	}
}
//登录
void login_in(int sockfd, int *p_onlinefd,char *username)
{
	struct user me={"q","q",0};	
	
	printf(YELLOW "\t********************************************************\n" NONE);	
	printf(YELLOW "\t*               Please Login                           *\n" NONE);
	printf(YELLOW "\t*------------------------------------------------------*\n" NONE);
	printf(L_BLUE "username:" NONE);
	scanf("%s",me.name);
	printf(L_BLUE "password:" NONE);
	scanf("%s",me.code);
	printf("Logging on to network,please wait for a moment...\n");

	send(sockfd, &me, sizeof(struct user), 0);	//发送用户名和密码
	
	recv(sockfd, &flag, sizeof(int), 0);		//接收服务器的信息flag

	if(flag == 1)//验证成功
	{
		strcpy(username, me.name);

		recv(sockfd, p_onlinefd, sizeof(int), 0);   
		flag1=1;
		sleep(1);
		//break;
	}
	else if(flag == -1)//重复登录
	{
		printf("The user has logged in!\n");
		sleep(2);
	}
	else	//验证失败
	{
		printf("Your username or password is incorrect,please log in again!\n");
		printf("After 2S, it will return to the first interface.\n");
		sleep(2); 
	}
}
void menu_main()
{
	printf(YELLOW "\t********************************************************\n" NONE);	
	printf(YELLOW "\t*           welcome to yunzhi chat room v1.0           *\n" NONE);
	printf(YELLOW "\t*------------------------------------------------------*\n" NONE);
	printf(YELLOW "\t*                     [1]Login                         *\n" NONE);
	printf(YELLOW "\t*------------------------------------------------------*\n" NONE);
	printf(YELLOW "\t*                     [2]Register                      *\n" NONE);
	printf(YELLOW "\t*------------------------------------------------------*\n" NONE);
	printf(YELLOW "\t*                     [3]Exit                          *\n" NONE);
	printf(YELLOW "\t********************************************************\n" NONE);
}
//功能界面
void menu_fun(char *username)
{
	printf("login successfully\n");
	system("clear");
	printf(L_BLUE "\t%s   hello!\n" NONE,username);
	printf(YELLOW "--------------------Direction For Use--------------------\n" NONE);
	printf(YELLOW "Private    chat ->format object name:content\n" NONE);
	printf(YELLOW "Multi-user chats->farmat object name:object name:content\n" NONE);
	printf(YELLOW "group      chat ->farmat all:content\n" NONE);
	printf(YELLOW "quit       chat ->quit\n" NONE);
	printf(YELLOW "---------------------------------------------------------\n" NONE);
}


/*功能：用户登录
参数：sockfd	 	客户端TCP套接字
      p_onlinefd	服务器TCP连接后套接字的指针
      username		登录名
      onlinename 	在线用户列表
返回值：成功返回0；失败返回-1*/
int client_login (int sockfd, int *p_onlinefd, char *username, char *onlinename)
{
	
	int a=0;
	while(1)
	{
		if(flag1==1){
			break;
		}
		system("clear");
		menu_main();
		printf(L_BLUE "Please choose:" NONE);
		scanf("%d",&a);
		setbuf(stdin,NULL);
		//getchar();
		switch(a)
		{
			case 1:
				system("clear");
				login_in(sockfd,p_onlinefd,username);
				break;
			case 2:
				system("clear");
				sign_in(sockfd);
				sleep(2);
				break;
			case 3:
				kill(q+2,SIGKILL);
				kill(q+3,SIGKILL);
				kill(q+4,SIGKILL);
				exit(0);
				break;
			default:
				break;
		}
	}
	menu_fun(username);
	recv(sockfd, onlinename, 1024, 0);	

	return 0;
}


int main (int argc, char *argv[])
{
	int sock;			//套接字
	struct sockaddr_in servaddr;	
	int len;	
	char buf[BUFFER_SIZE];	
	
	int onlinefd;		//服务器端的TCP连接后的套接字
	char username[20];	//用户名
	char onlinename[1024];	//在线用户列表

	struct msg msgs;	

	struct tm *ptm;		//时间有关
	time_t ts;
	char tim[50];

	
	//创建socket套接字
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}
	
	//设置IP 和 端口号
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(50013); 
	
	//连接服务器
	if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect");
		return -1;
	}
	//用户登录
	if(client_login(sock, &onlinefd, username, onlinename) != 0)
	{
		close(sock);
		return -1;
	}
	printf("%s\n", onlinename);	//打印在线用户名单

	//创建进程
	pid_t pid;
	if((pid = fork()) < 0)
	{
		return -1;
	}
	if(pid == 0)	//该子进程接收消息
	{
		while(1)
		{
			bzero(&msgs, sizeof(msgs));
			len = recv(sock, &msgs, sizeof(msgs), 0);

			if(len > 0)
			{	
				printf("%s", msgs.info);
				printf("%s", msgs.message);
				sleep(1);	
				printf("---------------------------------------------------------");
			}
			else if(len < 0)
			{
				perror("recv : ");
				break;
			}
			else
			{
				printf("Sorry,the server is down\n");
				//结束父进程
				kill(getppid(), SIGKILL);
				break;				
			}
		}
	}	
	else	//父进程发送消息（只向该用户的服务器处理程序 发送）	
	{

		while(1)
		{
			bzero(buf, BUFFER_SIZE);
			bzero(&msgs, sizeof(struct msg));

			msgs.fd = onlinefd;
			strcpy(msgs.from_name, username);

			if(fgets(buf, BUFFER_SIZE, stdin) == NULL)
			{
				if(ferror(stdin))
				{
					perror("stdin");
					break;
				}
			}
			strcpy(msgs.message, buf);			
			//判断是否退出
			if(!strncasecmp(buf, "quit", 4))
			{
				send(sock, &msgs, sizeof(msgs), 0);
				printf("Program will now exit！Bye!\n");
				sleep(2);
				system("clear");
				kill(0,SIGKILL);
				break;
			}
			
			ts = time(NULL);			//格式化时间
			ptm = localtime(&ts);

			sprintf(tim, "(%d-%d-%d %02d:%02d:%02d):", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
			sprintf(msgs.info, "\n%s %s", username, tim);
			if(send(sock, &msgs, sizeof(msgs), 0) < 0)
			{
				printf("send ! %s", strerror(errno));
				break;
			}
		}
		
		//结束子进程	
		kill(pid, SIGKILL);	
	}

	close(sock);	
	
	return 0;
}

