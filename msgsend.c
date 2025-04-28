// sender.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_SIZE 100

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {

    int key = 1234;

    struct msg_buffer message;
    message.msg_type = 1;

    int msg_id;

    // create or open
    if( 
        ( msg_id = msgget( key, IPC_CREAT|0666 ) ) < 0
    ) {
        perror("error creating msg queue");
        exit(1);
    }

    printf("message:\n");
    fgets( message.msg_text, MSG_SIZE, stdin );

    if(
        ( msgsnd( msg_id, &message, sizeof(message.msg_text), 0 ) ) 
    )

    printf("message sent to queue\n");
    return 0;

}