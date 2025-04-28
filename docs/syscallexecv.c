#include <unistd.h>
#include <stdio.h>

int main() {
				
				printf("Using execv, ls -l\n");
        char* args[] = { "ls", "-l", NULL };
        execv( "/bin/ls", args );

}