//
// Created by Connor on 10/18/2020.
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

int main(int argc, char **argv)
{
    int options;
    int msgid;
    unsigned long len;

    key_t key;

    while((options = getopt(argc, argv,"hc:l:t:")) != 1)
    {
        switch(options)
        {
            case 'h':
                printf("Usage ./oss -h OR -c x -l filename -t z\n");
                printf("Where: \n");
                printf("-c x:        Controls how many children are allowed to spawn (default 5)\n");
                printf("-l filename: Log file you want to use\n");
                printf("-t z:         controls how long before the system will time out (default 20\n");

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
}