#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main() {

        const char* filename = "p1.txt";
        const char* contents = "hello file!";

        size_t data;

        int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0644);
        if( fd == -1 ) { perror("Error"); }

        else {
                data = write( fd, contents, strlen(contents) );
                close(fd);
        }

        fd = open(filename, O_RDONLY, 0644);
        if( fd == -1 ) { perror("Error"); }

        else {
                char read_contents[100];
                read( fd, read_contents, data );
                close(fd);

                printf("Read from file:\n%s\n", read_contents);
        }

        return 0;
}