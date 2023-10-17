/* This is a simple example to understand process synchronization using semaphores */
/* This example uses SysV Semaphore */
/*Refer: https://stackoverflow.com/questions/43548740/counting-semaphore-using-sysv*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int sem_id;
struct sembuf sops;

void thread_1(void)
{
    /*Wait on Set1 of Semaphore*/
    sops.sem_num = 1;	/* Set Number 1 */
    sops.sem_op = -1;
    sops.sem_flg = 0;

    if(semop(sem_id, &sops, 1) < 0)
        perror("Semop Wait In thread 3");
    else
        printf("Hello From thread 1\n");
}


void thread_2(void)
{
    /*Wait on Set0 of Semaphore*/
    sops.sem_num = 0;	/* Set Number 0 */
    sops.sem_op = -1;
    sops.sem_flg = 0;

    if(semop(sem_id, &sops, 1) < 0)
        perror("Semop Wait In thread 2");
    else
        printf("Hello from thread 2\n");

    /*Post on Set1 of Semaphore*/        
    sops.sem_num = 1;	/* Set Number 1 */
    sops.sem_op = 1;
    sops.sem_flg = 0;

    if(semop(sem_id, &sops, 1) < 0)
        perror("Semop Post In thread 2");
}

void thread_3(void)
{
    printf("Hello from thread 3\n");

    /*Post operation on Set0 of semaphore*/
    sops.sem_num = 0;	/* Set Number 0 */
    sops.sem_op = 1;
    sops.sem_flg = 0;

    if(semop(sem_id, &sops, 1) < 0)
        perror("Semop In thread 3");
    else
    { ; }
}

int main(void)
{
    void (*funct[]) = {thread_1, thread_2, thread_3};

    key_t semkey;
    char i;       
    pthread_t thread_id[3];

    semkey = ftok("/tmp", 'a'); /* Creates a System V IPC key*/

    if(semkey < 0)
        perror("Cannot Create Semaphore Key");
    else
    {
        sem_id = semget(semkey, 2, (IPC_CREAT|IPC_EXCL|0666)); /*Create Set of 2 Semaphore for Same semaphore key*/
        if(sem_id < 0)
            perror("Cannot create semaphore\n");
        else
        {
            ;
        }
    }

    for(i = 0; i < 3; i++)
    {
        if(pthread_create(&thread_id[i], NULL, funct[i], NULL) < 0)
            perror("Cannot Create thread\n");
    }

    for(i = 0; i < 3; i++)
        pthread_join(thread_id[i], NULL);

    if(semctl(sem_id, 0, IPC_RMID, NULL) == -1)
        perror("semctl");

    return 0;
}

