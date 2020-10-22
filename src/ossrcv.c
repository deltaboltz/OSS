/*
 * Connor Schultz
 * cdstyd@umsystem.edu
 *
 * 10/18/2020
 *
 * Files needed: oss.c
 *               ossrcv.c
 *
 * Files created: logfile (name given by user)
 *
 * Sources: https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_message_queues.htm
 *          https://www.geeksforgeeks.org/ipc-using-message-queues/
 *
 */
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

//message memory struct
struct msgmem
{
    long mtype;
};

//shared memory struct with pgid and time in Sec/Nano
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

    //Create the same shared memroy key between both processes
    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    //Create the shared memory ID with parent (key, 0, 0)
    if ((shmid = shmget(key, 0, 0)) == -1) {
        perror("shmget");
        exit(1);
    }

    //Attach the shared memory
    if ((ptr = shmat(shmid, NULL, 0)) == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    //Set up the shared message queue between the two processes
    int msgqkey;
    int msgqID;
    struct msgmem msg;
    if((msgqkey = ftok(".", 'C')) == -1)
    {
        perror("ftok");
        exit(1);
    }

    //Get the message queue id in order to check message queue
    if ((msgqID = msgget(msgqkey, 0660)) == -1) {
        perror("msgget - c");
        exit(1);
    }

    //Set a random timeout function for each child process
    float timeout = nextrand(ptr, 1e6);

    //Do a busy wait until the message q has a message
    while (tofloat(ptr) < timeout);

    //Receieve the message to go into the critical section
    if(msgrcv(msgqID, &msg, 0, 0, 0) == -1)
    {
        perror("msgrcv");
        exit(1);
    }

    ptr->pgid = getpid();

    //Detach the shared memory
    if(shmdt(ptr) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    return 0;
}

//Creates and increments the next timeout for a child process
float nextrand(struct shmmem* ptr, int maxNano) {
    float newSec = (float)(ptr->clockSec);
    int newNano = ptr->clockNano + (rand() % maxNano);
    while (newNano > 1e9) {
        newNano -= (int)1e9;
        newSec += 1;
    }
    return newSec + (float)newNano/(float)1e9;
}

//converts values into float using (float) casting
float tofloat(struct shmmem* ptr) {
    return (float)(ptr->clockSec) + (float)(ptr->clockNano)/(float)1e9;
}
