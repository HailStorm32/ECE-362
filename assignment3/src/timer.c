/* 
 * Name: Demetri Van Sickle
 * Class: ECE 362
 * Date: 1/27/22
 */
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "../include/itoa.h"

void printError(const uint8_t errorType, const char* errorMsg);
long getTime();
void print(char *msg);

int main(int argc, char *argv[])
{
    char *cmdArgs[20]; 
    char timeRanStr[10];
    long startTime = 0;
    long totalTime = 0;
    int forkPid = -1;

    //Initalize array
    memset(cmdArgs, '\0', sizeof(cmdArgs));

    //Throw error if we have less than 3 arguments
    if(argc < 2)
    {
        printError(0, "");
    }

    //Copy over the arguments
    for(uint8_t indx = 1; indx < argc; indx++)
    {
        cmdArgs[indx - 1] = argv[indx]; 
    }

    //Get current time stamp
    startTime = getTime();

    //Fork off a new process
    forkPid = fork();

    if(forkPid < 0)
    {
        printError(1, "\nError fork failed\n"); 
    }
    else if(forkPid == 0)//If child process
    {
        //Execute the command
        if(execvp(cmdArgs[0], cmdArgs) <= -1)
        {
            printError(1, "Error running command: \n ");
        }
        exit(0);
    }
    else
    {
        //Wait for child to finish
        wait(forkPid);

        totalTime = getTime() - startTime;

        print("Time to run: ");
        print(itoa(totalTime, timeRanStr, 10));
        print(" seconds\n");
    }
    return 0;
}

void printError(const uint8_t errorType, const char* errorMsg)
{
    switch(errorType)
    {
        case 0:
            write(STDERR_FILENO, "Error: Invalid argument(s)\n", 28 );
            write(STDERR_FILENO, "Usage: timer [string] [string] ...\n", 36);
            break;
        case 1:
            write(STDERR_FILENO, errorMsg, strlen(errorMsg));
            write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
            write(STDERR_FILENO, "\n", 1);
            break;
    }
    exit(1);
}

long getTime()
{
    long timeVar = 0;

    if((timeVar = time(NULL)) <= 0)
    {
        printError(1, "Error getting time: \n");
    }
    return timeVar;
}

void print(char *msg)
{
    if(write(STDOUT_FILENO, msg, strlen(msg)) <= 0)
    {
        printError(1, "Error writing to STDOUT: \n");
    }
}
