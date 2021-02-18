#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/epoll.h>
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
	int setNonblock(int fd);
        void addfd(int epollfd, int fd, bool enable_et);	
	void lt(epoll_event* events, int number, int epollfd, int listenfd);
    };


}


#endif
