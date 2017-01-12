#ifndef ELLA_WU
#define ELLA_WU

#define MSG_MSIZE 200 
#define KEY_SIZE 100 
#define HEADER "uofc"
#define HEADER_SIZE 19 

#ifdef __cplusplus
extern "C" {
#endif

void chat_error(const char *msg);
int send_msg(int sockfd, const char *pmt);
void recv_msg(int sockfd, char buffer[]);
int if_quit(char *msg, const char *quit_msg);

#ifdef __cplusplus
}
#endif
#endif
