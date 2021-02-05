#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main(int argc, char** argv)
    {
	if(argc != 3)
	{
	    cout <<"Please enter like this: port ip." << argc << endl;
	}
	else
	{
	    int clientFd = socket(AF_INET,SOCK_STREAM,0);
	    sockaddr_in clientAddr;
	    clientAddr.sin_family = AF_INET;
	    clientAddr.sin_port = htons(atoi(argv[1]));
	    clientAddr.sin_addr.s_addr = inet_addr(argv[2]);
	    //bind(clientFd,(struct sockaddr*)&clientAddr,sizeof(clientAddr));
	    int err = connect(clientFd,(struct sockaddr*)&clientAddr,sizeof(clientAddr));
        if(err != -1)
        {
            cout <<"connect succeeded" << endl;        
        }
        else
        {
            cout <<"connect failed" << endl;        
        }
	    close(clientFd);

	}
		
	return 0;
    
    }
