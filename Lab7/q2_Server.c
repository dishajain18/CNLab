#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define PORTNO 10201
#define MAX_CLIENTS 3

int clients[MAX_CLIENTS];
int client_count = 0;

void* handle_client(void* arg) {
    int newsockfd = *(int*)arg;
    free(arg);
    char buf[256];
    while (1) {
        int h = read(newsockfd, buf, sizeof(buf) - 1);
        if (h <= 0) 
			break;
        buf[h] = '\0';

        char output[300];
        sprintf(output, "sent from %d: %s", newsockfd, buf);
        printf("%s\n", output);
        for (int l = 0; l < client_count; l++) {
            write(clients[l], output, strlen(output)); // Send the message to all clients
        }
    }
    return NULL;
}

int main() {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in seraddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    seraddr.sin_port = htons(PORTNO);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    if (listen(sockfd, 3) < 0) {
        perror("ERROR on listen");
        exit(1);
    }

    while (1) {
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;
        }
        printf("created %d\n",newsockfd);
        if (client_count < MAX_CLIENTS) {
            clients[client_count++] = newsockfd;
            int* sockfd_ptr = malloc(sizeof(int));
            *sockfd_ptr = newsockfd;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, sockfd_ptr);
        } else {
            printf("Max clients reached. Connection refused.\n");
            close(newsockfd);
        }
    }

    close(sockfd);
    return 0;
}
