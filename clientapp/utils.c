#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

void chat_error(const char *msg)
{
    perror(msg);
    exit(0);
}

int send_msg(int sockfd, const char *pmt)
{
    char buffer[MSG_MSIZE];
    char buf1[MSG_MSIZE];
    int n;
    char *pos;
    int ifq;

    printf("%s", pmt);
    bzero(buf1, MSG_MSIZE);
    bzero(buffer, MSG_MSIZE);

    sprintf(buffer, "%s", pmt);
    fgets(buf1, MSG_MSIZE - 1 - strlen(buf1), stdin);

    /* remove \n */
    if ((pos=strchr(buf1, '\n')) != NULL) *pos = '\0';

    /* if send quit sig */
    if (strcmp(buf1, "\\quit") == 0) ifq = 1;
    else ifq = 0;

    strcat(buffer, buf1);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) chat_error("ERROR writing to socket");

    return ifq;
}

void recv_msg(int sockfd, char buffer[])
{
    int n;

    bzero(buffer, MSG_MSIZE);
    n = read(sockfd, buffer, MSG_MSIZE - 1);
    if (n < 0) chat_error("ERROR reading from socket");
}

int if_quit(char *msg, const char *quit_msg)
{
    if (strcmp(msg, quit_msg) == 0) return 1;
    else return 0;
}
