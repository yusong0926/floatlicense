#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "utils.h"

int init_socket(int portno, const char* saddr)
{
    /*initialization*/
    int sockfd;
    /*sockaddr_in: structures for handling internect addresses*/
    struct sockaddr_in serv_addr; 
    /*hostent: represent an entry in the hosts database*/
    struct hostent *server;

    /*create a socket, return a file descriptor*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) chat_error("ERROR opening socket");

    /*connect() server*/
    server = gethostbyname(saddr);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr,(char *)server->h_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        chat_error("ERROR connecting");
        return -1;
    }
    else {
        return sockfd;
    }
}


int checkout(int sockfd, const char* key)
{

    /* send buffer *//*
    char buffer[MSG_MSIZE];
    strcpy(buffer, HEADER);
    strcpy(buffer, "$");
    strcpy(buffer, "checkout");
    strcpy(buffer, "$");
    strcpy(buffer, key);
    strcpy(buffer, "$");
    */
    char buffer[MSG_MSIZE];
    printf("send data: %s\n", key);
    
    int n = write(sockfd, key, MSG_MSIZE);
    if (n < 0) chat_error("ERROR writing to socket");
    /* receive message */
    memset(buffer, 0, MSG_MSIZE);
    n = read(sockfd, buffer, MSG_MSIZE);
    printf("receving message from server");
    printf("result: %s\n", buffer);
    if (strcmp(buffer, "accept") == 0) {
        return 1;
    }
    else {
        return 0;
    }
    return 0;
}


int release(int sockfd, const char* key)
{   
    /* send buffer */
    char buffer[MSG_MSIZE];

    int n = write(sockfd, key, MSG_MSIZE);
    if (n < 0) chat_error("ERROR writing to socket");
    /* receive message */
    memset(buffer, 0, MSG_MSIZE);
    n = read(sockfd, buffer, MSG_MSIZE);
    printf("receving message from server\n");
    printf("result: %s\n", buffer);
    if (strcmp(buffer, "release") == 0) {
        return 1;
    }
    else {
        return 0;
    }
    return 0;
}
