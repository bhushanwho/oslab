// server.c

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
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

	// creating the shared memory segment
	if ( 
		( shmid = shmget( key, SHMSIZE, IPC_CREAT | 0666 ) )  < 0 
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

	s = shm;
	printf("enter text: ");
	scanf("%s", s);

	// waits till another process changes first character to '*'
	while( *shm != '*' ) 
		sleep(1);

	exit(0);

}	
