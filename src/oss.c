//
// Created by Connor on 10/18/2020.
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
    //char msgtxt;
};
struct msgmem msg;

typedef struct
{
    pid_t pgid;
    int clockSec;
    int clockNano;
}shmmem;
shmmem* ptr;

int MAX_CHILDREN = 5;
int MAX_TIME = 20;
char filename[80];
int shmid;

int main(int argc, char **argv)
{
    int options;
    unsigned long len;
    FILE *fp;

    key_t key;
    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }

    while((options = getopt(argc, argv,"hc:l:t:")) != 1)
    {
        switch(options)
        {
            case 'h':
                printf("Usage ./oss -h OR -c x -l filename -t z\n");
                printf("Where: \n");
                printf("-c x:        Controls how many children are allowed to spawn (default 5)\n");
                printf("-l filename: Log file you want to use\n");
                printf("-t z:        Controls how long before the system will time out (default 20\n");

            case 'c':
                MAX_CHILDREN = atoi(optarg);
                if(MAX_CHILDREN > 20)
                {
                    MAX_CHILDREN = 20;
                }
                else if(MAX_CHILDREN < 0)
                {
                    printf("invalid argument, type 'master -h' for usage/help message\n");
                    exit(1);
                }
                break;
            case 'l':
                strcpy(filename, optarg);
                if(!strcmp(filename, ""))
                {
                    printf("invalid argument, type 'master -h' for usage/help message\n");
                    exit(1);
                }
                else
                {
                    fp = fopen(filename, "a+");
                }
                break;
            case 't':
                MAX_TIME = atoi(optarg);
                if(MAX_TIME <= 0)
                {
                    printf("invalid argument, type 'master -h' for usage/help message\n");
                    exit(1);
                }
                break;
            default:
                printf("Use -h for help\n");
                return EXIT_FAILURE;
        }
    }




    shmid = shmget(key, 1048, 0600|IPC_CREAT|IPC_EXCL);

    if(shmid == -1)
    {
        //freeshm();
        perror("shmid");
        return 1;
    }

    ptr = (shmmem*) shmat(shmid, NULL, 0);
    if(ptr == (void*) -1)
    {
        //freeshm();
        perror("shmat");
        return 1;
    }


    int max = 0;
    int procCounter = 0;
    int currentID = 0;
    int wstatus;

    ptr->clockSec = 0;
    ptr->clockNano = 0;
    ptr->pgid = 0;

    while(max < 100 && ptr->clockSec < 2)
    {
        if(procCounter < MAX_CHILDREN)
        {
            const pid_t myppid = fork();
            switch(myppid)
            {
                case -1:
                    perror("fork");
                    exit(1);
                case 0:
                    if(execlp("ossrcv.c", "ossrcv", "\0") == -1)
                    {
                        perror("execlp");
                    }
                    break;
                default:
                    procCounter++;
            }
            fprintf(fp, "Creating child with id: %d. At %d Seconds and %d Nanoseconds.\n", ptr->pgid, ptr->clockSec, ptr->clockNano);


        }
        if(ptr->pgid != 0)
        {
            fprintf(fp, "Terminating child with id: %d. At %d Seconds and %d Nanoseconds.\n", ptr->pgid, ptr->clockSec, ptr->clockNano);
            waitpid(-1, &wstatus, 0);
            procCounter--;
            ptr->pgid = 0;
            //send message to append to msgque
        }

        ptr->clockNano += 100;
        while(ptr->clockNano > 1e9)
        {
            ptr->clockNano -= (int)1e9;
            ptr->clockSec++;
        }
    }

    while(procCounter > 0)
    {
        if(ptr->pgid != 0)
        {
            waitpid(-1, &wstatus, 0);
            procCounter--;
            ptr->pgid = 0;
        }
    }


}