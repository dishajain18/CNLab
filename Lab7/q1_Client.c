#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

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
	int len,result,sockfd,n=1;
	struct sockaddr_in address;
	char ch[256],buf[256];
	//Create a socket for the client
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//Name the socket as agreed with the server
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("172.16.48.93"); 
	address.sin_port=htons(10205);
	len = sizeof(address);
	//Connect your socket to the server’s socket
	result=connect(sockfd,(struct sockaddr *)&address,len);
	if(result==-1)
	{
		perror("\nCLIENT ERROR");
		exit(1);
	}
	//You can now read and write via sockfd (Logic for problem mentioned here)
	printf("\nEnter Key: ");
	int key;
	scanf("%d",&key);
	n=write(sockfd,&key,sizeof(key));
	if(n<0)
	{
		perror("Write failed");
		exit(-1);
	}
	int running=1;
	while(1)
	{
		printf("ENTER STRING: ");
		scanf(" %[^\n]s",ch); //ALWAYS PUT SPACE BEFORE SCANNING STRING IF YOU HAVE READ SOMETHING ELSE BEFORE IT
		if(strcmp(ch,"end")==0)
			running=0;
		encrypt(ch,key);
		n=write(sockfd,ch,strlen(ch));
		if(running==0) 
		{	close(sockfd);
			break;
		}
		if(n<0)
		{
			perror("Write failed");
			exit(-1);
		}
		printf("STRING SENT BACK FROM SERVER IS ... ");
		n=read(sockfd,buf,sizeof(buf));
		buf[n]=0;
		if(n<0)
		{
			perror("Read failed");
			exit(-1);
		}
		decrypt(buf,key);
		printf("%s\n",buf);
   }
	return 0;
}

