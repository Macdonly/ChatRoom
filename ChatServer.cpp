#include "ChatServer.h"
#include <iostream>
#include <sys/epoll.h>
using namespace std;
namespace ChatRoom
{
    ChatServer::ChatServer() : serversock(10000,"192.168.232.128"){}


    void ChatServer::run()
    {
	serversock.bind();
	serversock.listen();

	int epollfd = epoll_create(5);
	serversock.addfd(epollfd, serversock.fd, true);

	epoll_event events[5];

	while(1)
	{
	    int ret = epoll_wait(epollfd,events,5,-1);
	    if(ret < 0)
	    {
		cout << "epoll failure" << endl; 
		break; 
	    }
	    serversock.lt(events, ret, epollfd, serversock.fd);	    
	
	}
	shutdown(serversock.fd, SHUT_RDWR);

    }
}

