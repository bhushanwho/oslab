// client.c

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSIZE 27

int main() {

	char c;
	int shmid;
	key_t key;

	char* shm;
	char* s;

	key = 1234;

	// locating the segment
	// no need of IPC_CREAT
	
	if ( 
		( shmid = shmget( key, SHMSIZE, 0666 ) )  < 0 
	   ) {
		perror("could not shmget");
		exit(1);
	}

	// attaching shared segment to current data space
	if (
		( shm = shmat( shmid, NULL, 0 ) ) == (char *) -1 
	   ) {
		perror("could not shmat");
		exit(1);
	}

	printf("contents of shared memory: \n");	
	for( s = shm; *s != 0; s++ ) {
		printf("%c", *s);
	}
	printf("\n");


	// change first char to '*' to indicate segment was read
	*shm = '*';

	exit(0);

}	
