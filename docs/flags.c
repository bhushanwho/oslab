#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
        const char* file = "p1.txt";
        int fd = open( file, O_RDWR );

        if( fd == -1 ) { perror("Error"); return -1; }

        int flags = fcntl(fd, F_GETFL);

        if( flags == -1 ) {
                close(fd);
                perror("Error");
                return -1;
        }

        printf("File flags for %s\n", file);

        if( flags & O_RDONLY ) { printf("O_RDONLY\tRead-only\n"); }
        if( flags & O_WRONLY ) { printf("O_WRONLY\tWrite-only\n"); }
        if( flags & O_RDWR ) { printf("O_RDWR\tRead-Write\n"); }
        if( flags & O_APPEND ) { printf("O_APPEND\tAppend\n"); }
        if( flags & O_NONBLOCK ) { printf("O_NONBLOCK\tNon-blocking\n"); }
        if( flags & O_SYNC ) { printf("O_SYNC\tSynchronous Writes\n"); }

        close(fd);

        return 0;
}