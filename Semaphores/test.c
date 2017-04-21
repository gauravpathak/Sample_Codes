#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

static int sem0, sem1;
#define POST1(Sem) semop(Sem, &(struct sembuf){0,1,0}, 1)
#define WAIT1(Sem) semop(Sem, &(struct sembuf){0,-1,0}, 1)

static void* t0(void *unused) { WAIT1(sem1); puts("hello from t0"); return 0; }
static void* t1(void *unused) { WAIT1(sem0); puts("hello from t1"); POST1(sem1); return 0; }
static void* t2(void *unused) { puts("hello from t2"); POST1(sem0); return 0; }

int main(void)
{
    key_t sem0_k, sem1_k;
    sem0_k = ftok("/tmp", '0');
    sem1_k = ftok("/tmp", '1');

    sem0 = semget(sem0_k, 1, (IPC_CREAT|IPC_EXCL|0666));
    sem1 = semget(sem1_k, 1, (IPC_CREAT|IPC_EXCL|0666));

    pthread_t tids[3];
    pthread_create(tids+2, NULL, t2, NULL);
    sleep(1);
    pthread_create(tids+1, NULL, t1, NULL);
    sleep(1);
    pthread_create(tids+0, NULL, t0, NULL);

    for(int i = 0; i < 3; i++)
        pthread_join(tids[i], NULL);

    semctl(sem0, 0, IPC_RMID, NULL);
    semctl(sem1, 0, IPC_RMID, NULL);

    return 0;
}
