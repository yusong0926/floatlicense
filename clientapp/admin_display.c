#include "client_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
    char key[20];
    char software[20];
    char company[20];
    char purchased_num[10];
    char reserved_num[10];
    int available;
    printf("software:");
    scanf("%s", software);
    printf("key:");
    scanf("%s", key);
    int portno = atoi(argv[2]);
    char* saddr = argv[1];
    
    
    printf("start initiate socket\n");
    int sockfd = init_socket(portno, saddr);

    char checkout_key[MSG_MSIZE];
    /*
    bzero(checkout_key, MSG_MSIZE * sizeof(char));
    bzero(release_key, MSG_MSIZE * sizeof(char));
    */
    memset(checkout_key, '\0', MSG_MSIZE * sizeof(char));
    strcat(checkout_key, HEADER);
    strcat(checkout_key, "$");
    strcat(checkout_key, software);
    strcat(checkout_key, "$display$");
    strcat(checkout_key, key);
    strcat(checkout_key, "$");

     
    printf("start admin service: %s\n", checkout_key);
    
    available = display(sockfd, checkout_key);

    if(available){
        printf("ok");
    } else {
        printf("no ");
    }

    return 0;
}
