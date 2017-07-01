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
int     socket_fd,new_fd;
struct sockaddr_in    server_addr;/*定义server套接口数据结构*/
struct sockaddr_in    client_addr;/*定义client套接口数据结构*/
int     sin_size,port_number;
pthread_t thread_id;
int main(void)
{
	
	char   *client_ip = NULL;/*获取的client的ip*/
	char    buf[BUF_MAX_SIZE];
	int     ret = 0;
	
	
	bzero(&server_addr,sizeof(server_addr));/*清零结构体*/
	bzero(&client_addr,sizeof(client_addr));/*清零结构体*/
	bzero(buf,sizeof(buf));/*清零结构体*/
	
	port_number = PORT_NUM;/*给定端口号*/
	
	socket_fd = socket(AF_INET,SOCK_STREAM,0);/*创建socket描述符*/
	if(socket_fd == -1)
	{
		DEBUG_INFO("socket failed error = %s",strerror(errno));
		return -1;
	}
	DEBUG_INFO("socket ok !");
	
	/*服务器端填充socketaddr*/
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);/*指明本地所有IP，本地任何网卡来的信息都可以*/
	server_addr.sin_port = htons(port_number);
	
	/*绑定socket*/
	ret = bind(socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret == -1)
	{
		DEBUG_INFO("bind failed error = %s",strerror(errno));
		return -1;
	}
	DEBUG_INFO("bind ok !");
	
	/*监听端口*/
	ret = listen(socket_fd,5);
	if(ret == -1)
	{
		DEBUG_INFO("listen failed error = %s",strerror(errno));
		return -1;
	}
	DEBUG_INFO("listen ok !");
	
	
	
	/*服务器阻塞等待客户端连接服务器*/
	
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		new_fd = accept(socket_fd,(struct sockaddr*)(&client_addr),&sin_size);
		DEBUG_INFO("new_fd = %d",new_fd);
		if(new_fd == -1)
		{
			DEBUG_INFO("accept failed error = %s",strerror(errno));
			return -1;
		}
		DEBUG_INFO("accept OK!");		
		DEBUG_INFO("server get connect from ip = %s",inet_ntoa(client_addr.sin_addr));
		DEBUG_INFO("server get connect from port = %d",ntohs(client_addr.sin_port));
		
		/*创建服务线程*/
		
		ret = pthread_create(&thread_id,NULL,(void *)recv_thread,buf);
		if(ret == 0)
		{
			DEBUG_INFO("pthread_create OK!");
		}
		
		
	}	
	
	
	return 0;
}

void recv_thread(char* buf)
{
	int ret = 0;
	while(1)
	{
		DEBUG_INFO("in recv_thread!");
		ret = recv(new_fd,buf,BUF_MAX_SIZE,MSG_PEEK);
		
		if(ret == -1)
		{
			DEBUG_INFO("recv failed error = %s",strerror(errno));
			return ;
		}
		if(ret > 0)
		{
			ret = recv(new_fd,buf,BUF_MAX_SIZE,0);
			DEBUG_INFO("server rev OK ! num = %d  content = %s",ret,buf);
			ret = send(new_fd,buf,strlen(buf)+1,0);
			if(ret > 0)
			{
				DEBUG_INFO("server send OK ! num = %d  content = %s",ret,buf);
			}
			if(ret < 0)
			{
				DEBUG_INFO("server send failed error = %s",strerror(errno));
			}
		}
			
	}
	return;
}
















































































