#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define BUFFER_SIZE 10
#define SHM_KEY 1234
#define SEM_KEY 5678

// semaphore indices
enum { MUTEX=0, EMPTY, FULL };

// the shared memory, the buffer basically
struct shared {
    int buffer[ BUFFER_SIZE ];
    int in;
    int out;
};

// a neat wrapper for semop( int semid, struct sembuf* sop, int* nsop )
void sem_op( int semid, int sem_num, int op ) {

    struct sembuf s;
    s.sem_num = sem_num;
    s.sem_op = op;
    s.sem_flg = 0;

    semop( semid, &s, 1 );
}


void producer( int semid, struct shared* sh ) {

    // while( true ) but we're being genereous, basically
    for( int i = 0; i < 10; i++ ) {

        // down( empty )
        sem_op( semid, EMPTY, -1 );

        // lock( mutex )
        sem_op( semid, MUTEX, -1 );

        sh->buffer[ sh->in ] = i;
        printf("produced: %d\n", i);
        sh->in = (sh->in + 1) % BUFFER_SIZE;

        // unlock( mutex )
        sem_op( semid, MUTEX, 1 );

        // up( full )
        sem_op( semid, FULL, 1 );
        
        // simulating the process of "producing"
        sleep(1);
    }

}

void consumer( int semid, struct shared* sh ) {

    // while( true ) but it's past 1 and i'm dying to sleep
    for( int i = 0; i < 10; i++ ) {

        // down( full )
        sem_op( semid, FULL, -1 );

        // lock( mutex )
        sem_op( semid, MUTEX, -1 );

        int item = sh->buffer[ sh->out ];
        sh->out = (sh->out + 1) % BUFFER_SIZE;
        printf("consumed: %d\n", item);

        // unlock( mutex )
        sem_op( semid, MUTEX, 1 );
        
        // simulating the process of "consuming"
        sleep(2);

    }

}

int main() {

    int shmid = shmget( SHM_KEY, sizeof(struct shared) , IPC_CREAT|0666 );
    struct shared* sh = shmat( shmid, NULL, 0 );
    sh->in = sh->out = 0;

    // setting up semaphores
    int semid = semget( SEM_KEY, 3, IPC_CREAT|0666 );

    // set FULL = 0, EMPTY = N, MUTEX = 1;
    semctl( semid, FULL, SETVAL, 0 );
    semctl( semid, EMPTY, SETVAL, BUFFER_SIZE );
    semctl( semid, MUTEX, SETVAL, 1 );

    pid_t pid = fork();
    if( pid == 0  ) {
        consumer( semid, sh );
        exit(0);
    } else {
        producer( semid, sh );
        wait( NULL );

        // delete shared memory
        shmdt( sh );
        shmctl( shmid, IPC_RMID, NULL );
        semctl( semid, 0, IPC_RMID );

    }

    return 0;

}