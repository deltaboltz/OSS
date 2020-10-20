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
};
struct msgmem msg;

typedef struct
{
    pid_t pgid;
    int clockSec;
    int clockNano;
}shmmem;
shmmem* ptr;


double timeout;

float nextrand(int maxNano);
float tofloat();


int main(void)
{
    srand(getpid());
    timeout = rand() % 100000;

    key_t key;
    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    int shmid =  shmget(key, 1048, 0600 | IPC_CREAT | IPC_EXCL);
    if(shmid == -1)
    {
        //freeshm();
        perror("shmid");
        return 1;
    }

    while((ptr->clockNano < timeout) && (ptr->clockSec < timeout));

    if(msgrcv(2, &msg, 0, 0, 0) == -1)
    {
        perror("msgrcv");
    }

    ptr->pgid = getpid();

    if(shmdt(shmid) == -1)
    {
        perror("shmdt");
    }


    return 0;
}



