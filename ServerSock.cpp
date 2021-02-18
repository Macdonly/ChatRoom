#include <sys/signal.h>
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
	int err = ::bind(fd,(struct sockaddr*)&m_addr,sizeof(m_addr)); //三个参数是(fd,地址，地址大小)
	if(err >= 0) //成功时返回0，失败时返回-1；
	{
	    cout << "bind succeed." << err<<endl;
	}
	else
	{
	    cout << "bind failed " << __FILE__ << " " << __FUNCTION__ << endl;
	    exit(0);
	}
    }

    void ServerSock::listen()
    {
	int err = ::listen(fd,5);//参数5表示内核监听队列的最大长度为5，超过5则不受理新的客户连接
	signal(SIGCHLD,SIG_IGN);	
	if(err >= 0)
	{
	    cout << "listen succeed." << err<<endl;
	}
	else
	{
	    cout << "listen failed" << endl;
	    exit(0);
	}
    }
    void ServerSock::accept()
    {
	//signal(SIGCHLD,SIG_IGN);	
	cout << "pid:" << getpid() << endl;	
	sockaddr_in client;
	socklen_t client_addrlen = sizeof(client);
	int err = ::accept(fd,(struct sockaddr*)&client,&client_addrlen);
	//cout << err << endl;
	if(err >= 0)
	{
	    cout << "connect succeeded "<< endl;
	    cout << "client port: "<<ntohs(client.sin_port) << endl;

	    int ipid = fork();
	    if(!ipid)
	    {
		recv(err);
		exit(0);
	    }

	}
	else
	{
	    exit(0);
	    cout <<"connect failed" << endl;
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
		memset(recvBuf,'\0',20);
	    }
	    else
	    {
		if(close(err) == 0)
		{
		    cout << "client has closed the connection" << endl;	
		}
		break;
	    }
	}
    }

}
