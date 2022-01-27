#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include <stdio.h> //FOR DEBUG
//#define DEBUG //FOR DEBUG

void printError(const uint8_t errorType, const char* errorMsg);
long getTime();
void print(char *msg);

int main(int argc, char *argv[])
{
    char *cmdArgs[20]; 
    long startTime = 0;
    long totalTime = 0;

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

        #ifdef DEBUG
        printf("\nArgs %d: %s \n", indx, cmdArgs[indx-1]);
        #endif
    }

    //Get current time stamp
    startTime = getTime();

    //Execute the command
    if(execvp(cmdArgs[0], cmdArgs) <= -1)
    {
        printError(1, "Error running command: \n ");
    }
    
    totalTime = getTime() - startTime;

    print("Time to run: ");
    print(itoa(totalTime,NULL, 10));
    print(" seconds\n");
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
