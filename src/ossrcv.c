//
// Created by Connor on 10/13/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/msg.h>
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

int main(int argc, char **argv)
{
    int msqid = 0;
    int toend;
    key_t key;

    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }

    if((msqid = msgget(key, PERMS)) == -1)
    {
        perror("msgget");
        exit(1);
    }
    printf("message q: ready to recieve\n");

    for(;;)
    {
        if(msgrcv(msqid, &msg, sizeof(msg.mtext), 0, 0) == -1)
        {
            perror("msgrcv");
            exit(1);
        }
        printf("recvd: \"%s\" \n", msg.mtext);
        toend = strcmp(msg.mtext, "end");
        if(toend == 0)
        {
            break;
        }
    }

    printf("message q: done rcving\n");
    //system("rm msgq.txt");
    return 0;
}