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

volatile int sigquit = 0;

void signalhandler(int sig) {
    if (sig == SIGALRM || sig == SIGINT) {
        sigquit = 1;
    }
}

void memorycleanup(struct shmmem* ptr, int shmid, int msgqID) {
    if (shmdt(ptr) == -1) {
        perror("shm detach");
        exit(1);
    }
    if (shmid != -1) {
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shm destroy");
            exit(1);
        }
    }
    if (msgqID != -1) {
        if (msgctl(msgqID, IPC_RMID, NULL) == -1) {
            perror("msgq destroy");
            exit(1);
        }
    }
}

void memorycleanupnoptr(int shmid, int msgqID) {
    if (shmid != -1) {
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shm destroy");
            exit(1);
        }
    }
    if (msgqID != -1) {
        if (msgctl(msgqID, IPC_RMID, NULL) == -1) {
            perror("msgq destroy");
            exit(1);
        }
    }
}

int main(int argc, char **argv)
{
    int options;
    FILE *fp;
    int MAX_CHILDREN = 5;
    int MAX_TIME = 20;
    char filename[80];
    int children[100];
    int childind;

    signal(SIGINT, signalhandler);
    signal(SIGALRM, signalhandler);

    for (childind = 0; childind < 100; childind++) {
        children[childind] = 0;
    }
    childind = 0;

    opterr = 0;
    while((options = getopt(argc, argv,":hc:l:t:")) != -1)
    {
        switch(options)
        {
            case 'h':
                printf("Usage %s [-h] [-c x] [-l filename] [-t z]\n", argv[0]);
                printf("Where: \n");
                printf("-c x:        Controls how many children are allowed to spawn (default 5)\n");
                printf("-l filename: Log file you want to use\n");
                printf("-t z:        Controls how long before the system will time out (default 20\n");
                exit(1);
                break;

            case 'c':
                MAX_CHILDREN = atoi(optarg);
                if(MAX_CHILDREN > 20)
                {
                    printf("Value entered is greater than allowed, setting to default (20)");
                    MAX_CHILDREN = 20;
                }
                else if(MAX_CHILDREN < 0)
                {
                    printf("invalid argument, type '%s -h' for usage/help message\n", argv[0]);
                    exit(1);
                }
                break;
            case 'l':
                strcpy(filename, optarg);
                if ((fp = fopen(filename, "a+")) == NULL) {
                    perror("fopen");
                    exit(1);
                }
                break;
            case 't':
                MAX_TIME = atoi(optarg);
                if(MAX_TIME <= 0)
                {
                    printf("invalid argument, type '%s -h' for usage/help message\n", argv[0]);
                    exit(1);
                }
                break;
            case ':':
                printf("option -%c requires an argument, type '%s -h' for usage/help message\n", optopt, argv[0]);
                exit(1);
                break;
            default:
                printf("unknown option -%c, type '%s -h' for usage/help message\n", optopt, argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (optind < argc) {
        printf("invalid argument %s, type '%s -h' for usage/help message\n", argv[optind], argv[0]);
    }

    alarm(MAX_TIME);

    key_t key;
    int shmid;
    struct shmmem* ptr;
    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((shmid = shmget(key, 1048, 0600|IPC_CREAT|IPC_EXCL)) == -1) {
        perror("shmget");
        exit(1);
    }
    if ((ptr = (struct shmmem*) shmat(shmid, NULL, 0)) == (void*)-1) {
        perror("shmat");
        memorycleanupnoptr(shmid, -1);
        exit(1);
    }

    int msgqkey;
    int msgqID;
    if ((msgqkey = ftok(".", 'C')) == -1) {
        perror("ftok");
        memorycleanup(ptr, shmid, -1);
        exit(1);
    }
    if ((msgqID = msgget(msgqkey, 0644|IPC_CREAT|IPC_EXCL)) == -1) {
        perror("msgget - p");
        memorycleanup(ptr, shmid, -1);
        exit(1);
    }

    int max = 0;
    int procCounter = 0;
    int wstatus;
    struct msgmem msg;
    msg.mtype = 1;
    ptr->clockSec = 0;
    ptr->clockNano = 0;
    ptr->pgid = 0;
    
    //unlock critical section for children
    if(msgsnd(msgqID, &msg, 0, 0) == -1)
    {
        perror("msgsnd");
        memorycleanup(ptr, shmid, msgqID);
        exit(1);
    }

    while(max < 100 && ptr->clockSec < 2)
    {
        if (sigquit) break;

        if(procCounter < MAX_CHILDREN)
        {
            const pid_t myppid = fork();
            switch(myppid)
            {
                case -1:
                    perror("fork");
                    memorycleanup(ptr, shmid, msgqID);
                    exit(1);
                    break;
                case 0:
                    if(execlp("ossrcv", "ossrcv", NULL) == -1)
                    {
                        perror("execlp");
                        memorycleanup(ptr, shmid, msgqID);
                        exit(1);
                    }
                    break;
                default:
                    procCounter++;
                    children[childind++] = myppid;
                    break;
            }
            fprintf(fp, "Creating child with id: %d. At %d Seconds and %d Nanoseconds.\n", myppid, ptr->clockSec, ptr->clockNano);
            max++;
        }
        
        if(ptr->pgid != 0)
        {
            fprintf(fp, "Terminating child with id: %d. At %d Seconds and %d Nanoseconds.\n", ptr->pgid, ptr->clockSec, ptr->clockNano);
            waitpid(ptr->pgid, &wstatus, 0);
            procCounter--;
            ptr->pgid = 0;
            //send message to append to msgque
            if(msgsnd(msgqID, &msg, 0, 0) == -1)
            {
                perror("msgsnd");
                memorycleanup(ptr, shmid, msgqID);
                exit(1);
            }
        }

        ptr->clockNano += 1000;
        while(ptr->clockNano > 1e9)
        {
            ptr->clockNano -= (int)1e9;
            ptr->clockSec++;
        }
    }

    fprintf(fp, "Terminating after starting %d children, at %d Seconds and %d Nanoseconds.\n", childind, ptr->clockSec, ptr->clockNano); 

    while(procCounter > 0)
    {
        int i;
        for (i = 0; i < childind; i++) {
            if (kill(children[i], SIGINT) == -1 && errno != ESRCH) {
                perror("kill");
                memorycleanup(ptr, shmid, msgqID);
                exit(1);
            }
        }
        waitpid(-1, &wstatus, 0);
        procCounter--;
    }

    if (shmdt(ptr) == -1) {
        perror("shm detach");
        exit(1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shm destroy");
        exit(1);
    }
    if (msgctl(msgqID, IPC_RMID, NULL) == -1) {
        perror("msgq destroy");
        exit(1);
    }
    return 0;
}
