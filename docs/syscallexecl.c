#include <unistd.h>
#include <stdio.h>

int main() {

        printf("Using execl, pwd\n");
        execl( "/bin/pwd", "pwd", (char*)NULL );

        return 0;

}