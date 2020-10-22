/*
 * Connor Schultz
 * cdstyd@umsystem.edu
 *
 * 10/28/2020
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

//Memory clean up incase we end early
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

//Memory clean up in case we end early AND do not have the ptr value set
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

//C does not allow for overloaded functions so had to do two different memory dumps

int main(int argc, char **argv)
{
    //initialize all variables
    int options;
    FILE *fp;
    int MAX_CHILDREN = 5;
    int MAX_TIME = 20;
    char filename[80];

    //Child array that holds all children IDs
    int children[100];
    int childind;
    //---------------------------------------

    //Early end or alarm handler
    signal(SIGINT, signalhandler);
    signal(SIGALRM, signalhandler);


    //Initialize the entire child ID array
    //C does not initialize arrays for you so it would have garbage data otherwise
    for (childind = 0; childind < 100; childind++) {
        children[childind] = 0;
    }
    //-----------------------------------------------------------------------------

    //Reset the child index to zero
    childind = 0;

    //Set opterr to 0 to keep the getopt from spitting out its own errors
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

    //Create the shared memory key, shmid, and shmmem ptr
    key_t key;
    int shmid;
    struct shmmem* ptr;
    if((key = ftok(".", 'B')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    //We create the key in this file for the child to use
    if ((shmid = shmget(key, 1048, 0600|IPC_CREAT|IPC_EXCL)) == -1) {
        perror("shmget");
        exit(1);
    }
    if ((ptr = (struct shmmem*) shmat(shmid, NULL, 0)) == (void*)-1) {
        perror("shmat");
        memorycleanupnoptr(shmid, -1);
        exit(1);
    }

    //Create the message queue
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

    //Start of child processes
    int max = 0;
    int procCounter = 0;
    int wstatus;
    struct msgmem msg;

    //mtype does not matter in this project
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

    //While loop to make 100 children or until sim clock gets to 2 seconds
    while(max < 100 && ptr->clockSec < 2)
    {
        //If the user ^C the program must quit the loop
        if (sigquit) break;

        if(procCounter < MAX_CHILDREN)
        {
            const pid_t myppid = fork();
            //Switch case for if the child doesn't fork or if it does then execlp
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
                    //Increment the procCounter and set the next children array element to the myppid
                default:
                    procCounter++;
                    children[childind++] = myppid;
                    break;
            }
            //Print to the log file
            fprintf(fp, "Creating child with id: %d. At %d Seconds and %d Nanoseconds.\n", myppid, ptr->clockSec, ptr->clockNano);
            max++;
        }

        //Once the child is done then we print to the log file again
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

        //Increment the sim clock, had to do +1000 nanoSec in order to sometimes reach the 2 second mark
        ptr->clockNano += 1000;
        while(ptr->clockNano > 1e9)
        {
            ptr->clockNano -= (int)1e9;
            ptr->clockSec++;
        }
    }

    fprintf(fp, "Terminating after starting %d children, at %d Seconds and %d Nanoseconds.\n", childind, ptr->clockSec, ptr->clockNano); 

    //Start killing the remaining children
    while(procCounter > 0)
    {
        int i;
        for (i = 0; i < childind; i++) {
            //This is where the children array come into play
            if (kill(children[i], SIGINT) == -1 && errno != ESRCH) {
                perror("kill");
                memorycleanup(ptr, shmid, msgqID);
                exit(1);
            }
        }
        waitpid(-1, &wstatus, 0);
        procCounter--;
    }

    //clean up all the memory that is left over
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
