#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#define PORTNO 10205

void encrypt(char*buf,int key)
{
  for(int i=0;i<strlen(buf);i++)
  	buf[i]=(buf[i]+key)%256;
}

void decrypt(char*buf,int key)
{
  for(int i=0;i<strlen(buf);i++)
  	buf[i]=(buf[i]-key)%256;
}

int main()
{
	int sockfd,newsockfd,portno,clilen,n=1;char buf[256];
	struct sockaddr_in seraddr,cliaddr;
	int i,value;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("172.16.48.93"); 
	seraddr.sin_port = htons(PORTNO);
	bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
	// Create a connection queue, ignore child exit details, and wait for clients
	listen(sockfd,5);
	printf("Server waiting...\n");
	while(1){
		//Accept the connection
		clilen = sizeof(clilen);
		newsockfd=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
		//Fork to create a process for this client and perform a test to see whether
		//you’re the parent or the child:
		if(fork()==0){
			// If you’re the child, you can now read/write to the client on newsockfd.              
			int key;
			n = read(newsockfd,&key,sizeof(key));
			if(n<0)
			{
				perror("read failed");
				exit(-1);
			}
			while(1)
			{
				memset(buf, 0, sizeof(buf));
				n = read(newsockfd,buf,sizeof(buf));
				if(n<0)
				{
					perror("read failed");
					exit(-1);
				}			
				buf[n]=0;
				decrypt(buf,key);
				if(strcmp(buf,"end")==0)
					{
						printf("%d Client closed connection\n",getpid());
						close(newsockfd);
						break;
					}
				printf("\n%d Message from Client %s \n",getpid(),buf);
				encrypt(buf,key);
				n = write(newsockfd,buf,sizeof(buf));
				if(n<0)
				{
					perror("write failed");
					exit(-1);
				}
			}
			exit(0);
		}
		//Otherwise, you must be the parent and your work for this client is finished
		else
		close(newsockfd);
	}
}

