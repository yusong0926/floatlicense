#include "client_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void helloworld() {
    sleep(10);
}


void *heartbeat(void *sockfd) {
    char key[] = "uofc$bos$alive$";
   // char key[] = "l$";
    int n;
    int fd = (int)sockfd;
    while(n >=0 ){
       n = write(fd, key, MSG_MSIZE);
       sleep(5);
    }

    printf("error in heartbeat");
}


int main(int argc, char *argv[])
{
    char key[20];
    char software[20];
    int available;
    printf("input your softwarename:");
    scanf("%s", software);
    printf("input your key:");
    scanf("%s", key);
    int portno = atoi(argv[2]);
    char* saddr = argv[1];
    
    
    printf("start initiate socket\n");
    int sockfd = init_socket(portno, saddr);


    char checkout_key[MSG_MSIZE];
    char release_key[MSG_MSIZE]; 
    /*
    bzero(checkout_key, MSG_MSIZE * sizeof(char));
    bzero(release_key, MSG_MSIZE * sizeof(char));
    */
    memset(checkout_key, '\0', MSG_MSIZE * sizeof(char));
    memset(release_key, '\0', MSG_MSIZE * sizeof(char));
    strcat(checkout_key, HEADER);
    strcat(checkout_key, "$");
    strcat(checkout_key, software);
    strcat(checkout_key, "$checkout$");
    strcat(checkout_key, key);
    strcat(checkout_key, "$");

    strcat(release_key, HEADER);
    strcat(release_key, "$");
    strcat(release_key, software);
    strcat(release_key, "$release$");
    strcat(release_key, key);
    strcat(release_key, "$");

    
    printf("start  service: %s\n", checkout_key);
    
    available = checkout(sockfd, checkout_key);
    pthread_t thread;
    int rc = pthread_create(&thread, NULL, heartbeat, (void *)sockfd);

    if(available){
        printf("ok, you have license available, you can run hello world now\n");
        printf("start running simulation............\n");
        helloworld();
        printf("start release license: release_key: %s\n", release_key);
        release(sockfd, release_key);
    } else {
        printf("no license available");
    }

    return 0;
}
