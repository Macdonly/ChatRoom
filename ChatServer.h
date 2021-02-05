#ifndef CHATSERVER_H
#define CHATSERVER_H
#include <sys/socket.h>
#include <iostream>
#include "ServerSock.h"
using namespace std;

namespace ChatRoom
{
    class ChatServer
    {
    protected:
	ServerSock serversock;
    public:
	ChatServer();
	void run();
    };	    



}
#endif

