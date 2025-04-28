#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

int main() {

        const char* file = "p1.txt";
        struct stat sb;

        if( stat( file, &sb ) == -1 ) {
                perror("Error");
                return -1;
        }

        printf("Mode: %lo (octal)\n", (unsigned long) sb.st_mode);
        printf("File: %s\nSize: %ld\nBlocks: %ld\nIO Block: %d\n", file, sb.st_size, sb.st_blocks, sb.st_blksize);
        printf("Directory: "); sb.st_mode == S_IFDIR ? printf("Yes\n") : printf("No\n");

        printf("Last status change:       %s", ctime(&sb.st_ctime));
        printf("Last file access:         %s", ctime(&sb.st_atime));
        printf("Last file modification:   %s", ctime(&sb.st_mtime));

        return 0;
}