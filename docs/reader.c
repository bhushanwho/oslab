#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY 1234
#define SEM_KEY 5678

enum { MUTEX = 0, WRT }; // semaphores: read_count lock, writer lock

struct shared {
    int read_count;
    int data; // shared data
};

void sem_op(int semid, int sem_num, int op) {
    struct sembuf s;
    s.sem_num = sem_num;
    s.sem_op = op;
    s.sem_flg = 0;
    semop(semid, &s, 1);
}

void reader(int semid, struct shared* sh, int id) {
    for (int i = 0; i < 5; i++) {
        sem_op(semid, MUTEX, -1);
        sh->read_count++;
        if (sh->read_count == 1)
            sem_op(semid, WRT, -1); // first reader locks writers
        sem_op(semid, MUTEX, 1);

        printf("Reader %d reads: %d\n", id, sh->data);
        sleep(1);

        sem_op(semid, MUTEX, -1);
        sh->read_count--;
        if (sh->read_count == 0)
            sem_op(semid, WRT, 1); // last reader unlocks writers
        sem_op(semid, MUTEX, 1);

        sleep(1);
    }
}

void writer(int semid, struct shared* sh, int id) {
    for (int i = 0; i < 5; i++) {
        sem_op(semid, WRT, -1);
        sh->data += 10;
        printf("Writer %d writes: %d\n", id, sh->data);
        sem_op(semid, WRT, 1);

        sleep(2);
    }
}

int main() {
    int shmid = shmget(SHM_KEY, sizeof(struct shared), IPC_CREAT | 0666);
    struct shared* sh = shmat(shmid, NULL, 0);
    sh->read_count = 0;
    sh->data = 100;

    int semid = semget(SEM_KEY, 2, IPC_CREAT | 0666);
    semctl(semid, MUTEX, SETVAL, 1);
    semctl(semid, WRT, SETVAL, 1);

    for (int i = 0; i < 2; i++) {
        if (fork() == 0) {
            reader(semid, sh, i);
            exit(0);
        }
    }

    for (int i = 0; i < 2; i++) {
        if (fork() == 0) {
            writer(semid, sh, i);
            exit(0);
        }
    }

    for (int i = 0; i < 4; i++)
        wait(NULL);

    shmdt(sh);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
