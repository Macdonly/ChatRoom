#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#ifndef SERVERSOCK_H
#define SERVERSOCK_H

namespace ChatRoom
{
    //ServerSock类是聊天室服务端使用的socket
    class ServerSock
    {
    public:
	int fd;
	sockaddr_in m_addr;
	const char* m_IP;
	int m_port;
    public:
	ServerSock(int port, const char* ip);	
	void bind();
	void listen();
	void accept();
	void recv(int rwsocket);
	void select();
    };


}


#endif
