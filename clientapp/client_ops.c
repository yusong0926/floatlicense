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

    char buffer[MSG_MSIZE];
    printf("send data: %s\n", key);
    
    int n = write(sockfd, key, MSG_MSIZE);
    if (n < 0) chat_error("ERROR writing to socket");
    /* receive message */
    memset(buffer, 0, MSG_MSIZE);
    n = read(sockfd, buffer, MSG_MSIZE);
    trim_newline(buffer);
    printf("receving message from server");
    printf("result: %s\n", buffer);
    if (strcmp(buffer, "accept") == 0) {
        return 1;
    }
    else {
        return 0;
    }
}
int update(int sockfd, const char* key)
{

    char buffer[MSG_MSIZE];
    printf("send data: %s\n", key);
    
    int n = write(sockfd, key, MSG_MSIZE);
    if (n < 0) chat_error("ERROR writing to socket");
    /* receive message */
    memset(buffer, 0, MSG_MSIZE);
    n = read(sockfd, buffer, MSG_MSIZE);
    trim_newline(buffer);
    printf("receving message from server");
    printf("result: %s\n", buffer);
    if (strcmp(buffer, "accept") == 0) {
        return 1;
    }
    else {
        return 0;
    }
}


void trim_newline(char* str)
{
    int i = 0;
    for (i = 0; i < MSG_MSIZE; i++) {
        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
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
    trim_newline(buffer);
    if (strcmp(buffer, "release") == 0) {
        return 1;
    }
    else {
        return 0;
    }
    return 0;
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        *(result + idx) = 0;
    }

    return result;

}

int display(int sockfd, const char* key)
{
    int x = 1000;
    char buffer[MSG_MSIZE*x];
    printf("send data: %s\n", key);
    
    int n = write(sockfd, key, MSG_MSIZE);
    if (n < 0) chat_error("ERROR writing to socket");
    /* receive message */
    memset(buffer, 0, MSG_MSIZE*x);
    n = read(sockfd, buffer, MSG_MSIZE*x);
    trim_newline(buffer);
    printf("receving message from server\n");
    char** tokens = str_split(buffer, '$');
    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            printf("%s\n", *(tokens + i));
            free(*(tokens + i));
        }
        printf("\n");
        free(tokens);
    }
//    displayJson(buffer);
    return 1;
}

void displayJson(const char* buffer)
{
    



}


