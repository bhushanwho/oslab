#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    int fd[2], n;
    char buff1[100], buff2[100];

    if ( pipe( fd ) < 0 ) {
        printf("unable to create pipe");
        exit(1);
    }

    // read from STDIN (0) into buff1
    printf("enter text to put on pipe:\n");
    n = read( 0, buff1, 100 );
    
    // write buff1 to PIPE [1] from
    write( fd[1], buff1, n );

    // read from PIPE into buff2
    read( fd[0], buff2, n );

    // write from buff2 to STDOUT (1)
    write( 1, buff2, n );

    return 0;

}