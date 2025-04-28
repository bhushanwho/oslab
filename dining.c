#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define N 5
#define SHM_KEY 9999
#define SEM_KEY 8888

enum { THINKING, HUNGRY, EATING };

// semaphore indices
#define MUTEX 0
#define PHIL(i) (1 + i)  // semaphore index for each philosopher

struct shared {
    int state[N];  // state of each philosopher
};

// semaphore wrapper
void sem_op(int semid, int sem_num, int op) {
    struct sembuf s = { sem_num, op, 0 };
    semop(semid, &s, 1);
}

void test(int i, int semid, struct shared* sh) {
    int left = (i + N - 1) % N;
    int right = (i + 1) % N;

    if (sh->state[i] == HUNGRY &&
        sh->state[left] != EATING &&
        sh->state[right] != EATING) {

        sh->state[i] = EATING;
        printf("Philosopher %d starts EATING\n", i);
        sem_op(semid, PHIL(i), 1); // up
    }
}

void take_forks(int i, int semid, struct shared* sh) {
    sem_op(semid, MUTEX, -1); // lock

    sh->state[i] = HUNGRY;
    printf("Philosopher %d is HUNGRY\n", i);

    test(i, semid, sh);

    sem_op(semid, MUTEX, 1);  // unlock

    sem_op(semid, PHIL(i), -1); // down (wait if not allowed to eat yet)
}

void put_forks(int i, int semid, struct shared* sh) {
    sem_op(semid, MUTEX, -1); // lock

    sh->state[i] = THINKING;
    printf("Philosopher %d puts forks down\n", i);

    int left = (i + N - 1) % N;
    int right = (i + 1) % N;

    test(left, semid, sh);
    test(right, semid, sh);

    sem_op(semid, MUTEX, 1); // unlock
}

void philosopher(int i, int semid, struct shared* sh) {
    for( int j = 0; j < 2; j++ ) {
        printf("Philosopher %d is THINKING\n", i);
        sleep(1);

        take_forks(i, semid, sh);

        sleep(2); // simulate EATING

        put_forks(i, semid, sh);
    }
}

int main() {
    // Shared memory
    int shmid = shmget(SHM_KEY, sizeof(struct shared), IPC_CREAT | 0666);
    struct shared* sh = shmat(shmid, NULL, 0);

    // Init states
    for (int i = 0; i < N; i++) {
        sh->state[i] = THINKING;
    }

    // Semaphores: 1 mutex + N philosopher semaphores
    int semid = semget(SEM_KEY, 1 + N, IPC_CREAT | 0666);

    // Init semaphores
    semctl(semid, MUTEX, SETVAL, 1);
    for (int i = 0; i < N; i++) {
        semctl(semid, PHIL(i), SETVAL, 0);
    }

    // Spawn N philosophers
    for (int i = 0; i < N; i++) {
        if (fork() == 0) {
            philosopher(i, semid, sh);
            exit(0);
        }
    }

    // Parent waits forever
    for (int i = 0; i < N; i++)
        wait(NULL);

    // Cleanup (never reached here unless modified)
    shmdt(sh);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
