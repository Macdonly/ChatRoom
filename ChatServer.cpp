#include "ChatServer.h"
#include <iostream>
using namespace std;
namespace ChatRoom
{
    ChatServer::ChatServer() : serversock(10000,"192.168.232.128"){}


    void ChatServer::run()
    {
	serversock.bind();
	serversock.listen();
	serversock.select(); 
	cout << "accept() excuted" << endl;

    }
}

