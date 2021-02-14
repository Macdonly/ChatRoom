#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
using namespace std;

int main(int argc, char** argv)
    {
        	if(argc != 3)
        	{
        	    cout <<"Please enter like this: port ip." << argc << endl;
        	}
        	else
        	{
        	    int clientFd = socket(PF_INET,SOCK_STREAM,0);
        	    sockaddr_in clientAddr;
        	    clientAddr.sin_family = AF_INET;
        	    clientAddr.sin_port = htons(atoi(argv[1]));
        	    clientAddr.sin_addr.s_addr = inet_addr(argv[2]);
        	    //bind(clientFd,(struct sockaddr*)&clientAddr,sizeof(clientAddr));
        	    int err = connect(clientFd,(struct sockaddr*)&clientAddr,sizeof(clientAddr));
        	    //char [] sendBuf ={0};
        	    char recvBuf[20] = {0};
        	    char sendBuf[20] = {0};
            	if(err != -1)
             {
                cout <<"connect succeeded" << endl;        
             }
             else
             {
                 cout <<"connect failed" << endl;        
             }
            
            	while(1)
            	{
            	    cout << "请输入" << endl;
            	    cin.getline(sendBuf,20); //之前用的get 只阻塞第一次，getline就可以每次都阻塞啦
            	    if(strcmp(sendBuf,"exit")==0)
            	    {
                		close(clientFd);
                		break; 
            	    }
            	    else
            	    {
                		cout <<"已发送字节：" <<send(clientFd,sendBuf,strlen(sendBuf),0) << endl;
                		memset(sendBuf,'\0',20);
                		cin.clear();
            	    }
            	    if(::recv(clientFd,recvBuf,sizeof(recvBuf),0))
            	    {
                     cout << "收到回发：" ;
                		for(int i=0; i<sizeof(recvBuf); i++)
                		{
                		    cout << recvBuf[i];
                		}
                		cout << endl;	
            	    }
            		else
            		{
            		    cout << "未收到回发" << endl;
            		}
            }
            
        	}
        		
        	return 0;
            
    }
