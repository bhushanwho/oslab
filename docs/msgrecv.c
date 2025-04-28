// receiver.c

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
    int msg_id;

    if( 
        ( msg_id = msgget( key, 0666 ) ) < 0
    ) {
        perror("error opening msg queue");
        exit(1);
    }

    if(
        ( msgrcv( msg_id, &message, sizeof(message.msg_text), 1, 0 ) ) 
    )

    printf("message received from queue: %s\n", message.msg_text);
    return 0;

}