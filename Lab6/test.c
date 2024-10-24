#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
 int x=fork();
 if(x==0)
 {
   printf("child\n");
   sleep(1);
   printf("child\n");
 }
 if(x>0)
 {
   printf("parent\n");
   printf("parent\n");
   sleep(1);
   printf("parent\n");
   printf("parent\n");
   printf("parent\n");
   printf("parent\n");
  }
 }
