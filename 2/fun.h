struct user	//用户名和密码
{
	char name[20];
	char code[32];
	int flag;
	struct user* next;
};


struct msg	//在线用户的信息结构体（时时更新）
{
	int fd;			//拥有者的服务器端的连接套接字
	char from_name[20];	//信息的拥有者
	char to_name[20];	//信息的读者
	char info[1024];	//拥有者 时间
	char message[1024];	//信息内容
	struct msg *next;
};

