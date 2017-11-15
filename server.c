#include<stdio.h>
#include<sys/types.h>//socket
#include<sys/socket.h>//socket
#include<string.h>//memset
#include<stdlib.h>//sizeof
#include<netinet/in.h>//INADDR_ANY
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 47014
#define MAXSZ 100
int main()
{
 int sockfd;//to create socket
 int newsockfd;//to accept connection
 //int pid;
 struct sockaddr_in serverAddress;//server receive on this address
 struct sockaddr_in clientAddress;//server sends to client on this address

 int n;
 char msg[MAXSZ];
 
 int clientAddressLength;
 int pid;

 //create socket
 sockfd=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
 serverAddress.sin_port=htons(PORT);

 //bind the socket with the server address and port
 bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));

 //listen for connection from client
 listen(sockfd,5);

 while(1)
 {
  //parent process waiting to accept a new connection
  printf("\nserver waiting for new client connection:\n");
  clientAddressLength=sizeof(clientAddress);
  newsockfd=accept(sockfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
  printf("connected to client: %s\n",inet_ntoa(clientAddress.sin_addr));

  //child process is created for serving each new clients
  pid=fork();
  if(pid==0)//child process rec and send
  {
   //rceive from client
   while(1)
   {
    n=recv(newsockfd,msg,MAXSZ,0);
    if(n==0)
    {
     close(newsockfd);
     break;
    }
    printf("Details = %s\n", msg);
       if(msg[0]=='l'&& msg[1]=='i'&& msg[2] == 's' && msg[3] == 't')
    	{
		printf(" list received\n");
		int a;
		pid = fork();
 		if(pid == 0)
		{
		
		char *args[] = {"ls", NULL};
		a = execv("/bin/ls", args);
                exit(1);
		}
		else
		{
			wait(NULL);
		}
	}
	
	else if(msg[0]=='c'&& msg[1]=='d')
    	{
		printf(" list received\n");
		int a,i=0;
		char point[MAXSZ];
		
		printf("current working directory is  %s \n", getcwd(point,MAXSZ));
		
		char directory[MAXSZ] = "\0";
		char new_dire[MAXSZ] = "\0";
		strncpy(directory,msg+3,strlen(msg));
		strncpy(new_dire,directory,strlen(directory)-1);
		a = chdir(new_dire);
		printf("NEW current  working directory is %s \n", getcwd(point,MAXSZ));
		
              
	}
	else if(msg[0] == 'd' && msg[1] == 'o' && msg[2] == ' ' && msg[3] == 'n' && msg[4] == 'o')
	{
		char dest[MAXSZ] = "I am doing nothing  ";
		int i = strlen(dest);
		dest[i]=0;
		send(newsockfd,dest,i,0);
		continue;
		
	}
	
	else if(msg[0] == 'q' && msg[1] == 'u')
	{
	close(newsockfd);
	exit(1);
	}

	else 
	{
		char dest[MAXSZ] = "What?                ";
		int i = strlen(dest);
		dest[i]=0;
		send(newsockfd,dest,i,0);
		continue;
	
	}

    char dest[MAXSZ] = "Okay PORT            ";
    int i = strlen(dest);
    dest[i]=0;
    send(newsockfd,dest,i,0);
    msg[n]=0;
    printf("Received command:%s\n",msg);

     
   }//close interior while
  exit(0);
  }
  else
  {
   close(newsockfd);//sock is closed BY PARENT
  }
 }//close exterior while

 return 0;

}
