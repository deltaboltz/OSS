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

struct msgmem
{
    long mtype;
};

struct shmmem
{
    pid_t pgid;
    int clockSec;
    int clockNano;
};

float nextrand(struct shmmem* ptr, int maxNano);
float tofloat(struct shmmem* ptr);

void signalhandler(int sig) {
    if (sig == SIGINT) exit(1);
}

int main(void)
{
    srand(getpid());
    key_t key;
    int shmid;
    struct shmmem* ptr;
    signal(SIGINT, signalhandler);
    
    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((shmid = shmget(key, 0, 0)) == -1) {
        perror("shmget");
        exit(1);
    }
    if ((ptr = shmat(shmid, NULL, 0)) == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    int msgqkey;
    int msgqID;
    struct msgmem msg;
    if((msgqkey = ftok(".", 'C')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((msgqID = msgget(msgqkey, 0660)) == -1) {
        perror("msgget - c");
        exit(1);
    }
    
    float timeout = nextrand(ptr, 1e6);
    while (tofloat(ptr) < timeout);
    // block until msg queue has a message
    if(msgrcv(msgqID, &msg, 0, 0, 0) == -1)
    {
        perror("msgrcv");
        exit(1);
    }

    ptr->pgid = getpid();

    if(shmdt(ptr) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    return 0;
}

float nextrand(struct shmmem* ptr, int maxNano) {
    float newSec = (float)(ptr->clockSec);
    int newNano = ptr->clockNano + (rand() % maxNano);
    while (newNano > 1e9) {
        newNano -= (int)1e9;
        newSec += 1;
    }
    return newSec + (float)newNano/(float)1e9;
}

float tofloat(struct shmmem* ptr) {
    return (float)(ptr->clockSec) + (float)(ptr->clockNano)/(float)1e9;
}
