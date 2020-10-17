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
    char msgtxt[200];
    pid_t ppid;
};
struct msgmem msg;

int MAX_CHILDREN = 5;
int MAX_TIME = 20;
char filename[80];


int main(int argc, char ** argv)
{
    int options;
    int msgid;
    unsigned long len;

    key_t key;

    //system("touch msgq.txt");
    while((options = getopt(argc, argv,"hc:l:t:")) != 1)
    {
        switch(options)
        {
            case 'h':
                printf("Usage ./oss -h OR -c x -l filename -t z\n");
                printf("Where: \n");
                printf("-c x:  Controls how many children are allowed to spawn (default 5)\n");
                printf("-l filename: Log file you want to use\n");
                printf("-t z: controls how long before the system will time out (default 20\n");

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
                }
                break;
            case 't':
                MAX_TIME = atoi(optarg);
                if(MAX_TIME <= 0)
                {
                    printf("invalid argument, type 'master -h' for usage/help message\n");
                }
                break;
            default:
                printf("Use -h for help\n");
                return EXIT_FAILURE;
        }
    }

    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if((msgid = msgget(key,PERMS|IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    printf("The message id is: %d" ,msgid);
    printf("\n");
    printf("ready to send\n");

    msg.mtype=1;

    while(fgets(msg.msgtxt, sizeof msg.msgtxt, stdin) != NULL)
    {
        len = strlen(msg.msgtxt);

        if(msg.msgtxt[len-1] == '\n')
        {
            msg.msgtxt[len-1] = '\0';
        }
        if(msgsnd(msgid, &msg, len+1, 0) == -1)
        {
            perror("msgsnd");
        }
    }
    strcpy(msg.msgtxt, "end");
    len = strlen(msg.msgtxt);
    if(msgsnd(msgid, &msg, len+1, 0) == -1)
    {
        perror("msgsnd");
    }

    if(msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }
    printf("message queue: done sending messages");
    return 0;
}
