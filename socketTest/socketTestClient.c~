#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
/*如果没有则inet_ntoa会有问题*/
#include<arpa/inet.h>
/*线程的创建*/
#include <pthread.h>


#define    BUF_MAX_SIZE    1024
#define    PORT_NUM        9988
#define    DEBUG_INFO(fmt,arg...)\
	   printf("[Debug Info]:fun:%s line:%d,\
	   "fmt"\n",__FUNCTION__,__LINE__,##arg)

void recv_thread(char* buf);
int socket_fd;
char buf[BUF_MAX_SIZE];
struct sockaddr_in    server_addr;/*定义server套接口数据结构*/
int     sin_size,port_number;
struct hostent *host;
pthread_t thread_id;

int main(int argc,char* argv[])
{
	
	int     ret = 0;
	char* send_char = "Hello world! Hello Linux! Hello TCP/IP! Hello Socket! I'm here!";
	bzero(&server_addr,sizeof(server_addr));/*清零结构体*/
	bzero(buf,sizeof(buf));/*清零结构体*/
	
	/*检查输入参数*/
	if(argc != 3)
	{
		DEBUG_INFO("need intput 2 args:server name，port number！");
		return -1;
	}
	
	/*获取服务器name*/
	host = gethostbyname(argv[1]);
	if(host == NULL)
	{
		DEBUG_INFO("input server name error!");
		return -1;
	}
	
	/*获取服务器的端口*/
	port_number = atoi(argv[2]);
	if(port_number < 0)
	{
		DEBUG_INFO("input server port error!");
		return -1;
	}
	
	/*客户端填充socketaddr*/
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr = *((struct in_addr*)host->h_addr);/*指明本地所有IP，本地任何网卡来的信息都可以*/
	server_addr.sin_port = htons(port_number);
	
	/*建立socket描述符*/
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd ==-1)
	{
		DEBUG_INFO("socket error = %s!",strerror(errno));
		return -1;
	}
	DEBUG_INFO("socket OK!");
	
	/*客户端程序发起链接请求*/
	ret = connect(socket_fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
	if(ret == -1)
	{
		DEBUG_INFO("connect error = %s!",strerror(errno));
		return -1;
	}
	DEBUG_INFO("connect OK!");
	
	/*创建服务线程*/	
	ret = pthread_create(&thread_id,NULL,(void *)recv_thread,buf);
	
	/*发送数据测试*/
	while(1)
	{
		ret = send(socket_fd,send_char,strlen(send_char)+1,0);
		if(ret > 0)
		{
			DEBUG_INFO("send OK!");
		}
		sleep(2);
	}
	return 0;
}

void recv_thread(char* buf)
{
	int ret = 0;
	while(1)
	{
		DEBUG_INFO("in recv_thread!");
		ret = recv(socket_fd,buf,BUF_MAX_SIZE,MSG_PEEK);
		
		if(ret == -1)
		{
			DEBUG_INFO("recv failed error = %s",strerror(errno));
			return ;
		}
		if(ret > 0)
		{
			ret = recv(socket_fd,buf,BUF_MAX_SIZE,0);
			DEBUG_INFO("client rev OK ! num = %d  content = %s",ret,buf);
			#if 0
			ret = send(new_fd,buf,strlen(buf)+1,0);
			if(ret > 0)
			{
				DEBUG_INFO("server send OK ! num = %d  content = %s",ret,buf);
			}
			if(ret < 0)
			{
				DEBUG_INFO("server send failed error = %s",strerror(errno));
			}
			#endif
		}
			
	}
	return;
}












































































