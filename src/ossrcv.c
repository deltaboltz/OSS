//
// Created by Connor on 10/13/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define PERMS 0644

struct msgmem
{
    long mtype;
    char mtext[200];
};
struct msgmem msg;

typedef struct
{
    pid_t pgid;
    int clockSec;
    int clockNano;
}shmmem;
shmmem* ptr;

int shmid;
double timeout;

float nextrand(int maxNano);

int main(void)
{
    srand(getpid());
    timeout = rand() * 1e6;

    key_t key;
    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    shmid = shmget(key, 1048, 0600|IPC_CREAT|IPC_EXCL);
    if(shmid == -1)
    {
        //freeshm();
        perror("shmid");
        return 1;
    }


    return 0;
}

float nextrand(int maxNano)
{
    int increaseNano = rand() % maxNano;
    shmmem copy;
    (&copy)->clockSec = ptr->clockSec;
    (&copy)->clockNano = ptr->clockNano;
    ptr->clockNano -= increaseNano;

    while(ptr->clockNano > 1e9)
    {
        ptr-> clockNano -= (int)1e9;
        ptr->clockSec += 1;
    }
    return ((float)((int)(&copy)));
}