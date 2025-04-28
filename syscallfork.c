#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {

        pid_t id, childid;
        id = getpid();

        childid = fork();

        if( childid > 0 ) {
        
                printf("In parent process\n");
                printf("Process ID: %d\nChild's ID: %d\n", getpid(), childid);

        } else if( childid == 0 ) {
        
                printf("In child process\n");
                printf("Process ID: %d\nParent's ID: %d\n", getpid(), getppid());
        
        }
        else {
                printf("Error encountered\n");
        }

        return 0;
}