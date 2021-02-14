#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "ServerSock.h"
using namespace std;
namespace ChatRoom
{
    //构造函数初始化四个成员变量
    ServerSock::ServerSock(int port, const char* ip) : m_port(port), m_IP(ip)
    {
	fd = socket(PF_INET,SOCK_STREAM,0); //PF_INET表示TCP连接，SOCK_STREAM表示基于流，第三个参数一般为0
	bzero(&m_addr,sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);
	//m_addr.sin_addr.s_addr = ::inet_addr(ip);
	//inet_pton(AF_INET,ip,&m_addr.sin_addr);
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    void ServerSock::bind()
    {
	if(::bind(fd,(struct sockaddr*)&m_addr,sizeof(m_addr)) >= 0) //三个参数是(fd,地址，地址大小)
	{
	    //成功时返回0，失败时返回-1；
	    cout << "bind succeed." <<endl;
	}
	else
	{
	    perror("bind"); 
	    exit(0);
	}
    }

    void ServerSock::listen()
    {
	//参数5表示内核监听队列的最大长度为5，超过5则不受理新的客户连接
	if(::listen(fd,5) >= 0)
	{
	    cout << "listen succeed." << endl;
	}
	else
	{
	    perror("listen"); 
	    exit(0);
	}
    }
    void ServerSock::accept()
    {
	sockaddr_in client;
	socklen_t client_addrlen = sizeof(client);
	int err = ::accept(fd,(struct sockaddr*)&client,&client_addrlen);
	//cout << err << endl;
	if(err >= 0)
	{
	    cout << "connect succeeded "<< endl;
	    cout << "client port: "<<ntohs(client.sin_port) << endl;
	    recv(err);
	}
	else
	{
	    exit(0);
	    perror("accept");	
	}

    }

    void ServerSock::recv(int rwsocket)
    {
	//char* recvBuf = new char[20];
	char recvBuf[20];
	memset(recvBuf,'\0',20);
	while(1)
	{	
	    int err = ::recv(rwsocket,recvBuf,19,0);
	    if(err > 0)
	    {
		cout <<"接收的字节数："<< err << endl;
		for(int i=0; i<sizeof(recvBuf); i++)
		{
		    cout << recvBuf[i];
		}
		cout << endl;
	    }
	    else
	    {
		break;
	    }
	    memset(recvBuf,'\0',20);
	}
    }

    void ServerSock::select()
    {
	cout << "fd:" << fd << endl;
	fd_set fdset;
	int conn_count = 0;//当前连接数量
	const int pending_count = 10;//可以接受的最大连接数量
	int connected_fd[pending_count];
	int maxfd = fd;	
	memset(connected_fd,0,sizeof(connected_fd));	
	char recvBuf[20];
	memset(recvBuf,'\0',sizeof(recvBuf));
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;	
	while(1)
	{
	    FD_ZERO(&fdset); 
	    FD_SET(fd,&fdset);
	    for(int i=0; i<conn_count; i++)//将新建立的连接加入fd集合中
	    {
		if(connected_fd[i] != 0)
		{
		    FD_SET(connected_fd[i],&fdset);
		}
	    } 
	    int err = ::select(maxfd+1,&fdset,NULL,NULL,NULL); 
	    for(int i=0; i<conn_count; i++)//遍历创建的连接
	    {
		if(FD_ISSET(connected_fd[i],&fdset)) 
		{
		    if(::recv(connected_fd[i],recvBuf,sizeof(recvBuf)-1,0))
		    {
			for(int j=0; j<strlen(recvBuf); j++)
			{
			    cout << recvBuf[j];    	
			}	    
			cout << endl; 
			memset(recvBuf,'\0',strlen(recvBuf)); 
		    }
		    else
		    {
			perror("client has close the connect");
			shutdown(connected_fd[i],SHUT_RDWR);	
			FD_CLR(connected_fd[i],&fdset); 
			connected_fd[i] = 0; 
		    }			

		}

	    }
	    if(FD_ISSET(fd,&fdset))
	    {	
		sockaddr_in clientaddr;
		socklen_t length;
		int new_fd = ::accept(fd,(struct sockaddr*)&clientaddr,&length);
		if(new_fd < 0)
		{
		    cout << "连接失败" << endl;
		}
		else
		{
		    cout << "连接成功" << endl;
		    cout << "客户端ip:" << inet_ntoa(clientaddr.sin_addr) << endl;
		    cout << "客户端port:" << ntohs(clientaddr.sin_port) << endl; 
		    connected_fd[conn_count++] = new_fd;
		    if(new_fd > maxfd)	
		    {
			maxfd = new_fd; 
		    }	
		}
	    }

	}	    



    }	


}
