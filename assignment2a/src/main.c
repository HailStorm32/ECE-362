/*
 * Name: Demetri Van Sickle
 * Class: ECE 362
 * Date: 1/17/22
 * Description: Echos data written by the user while also loging what is said 
 *      to a file called output.log
 */
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <cstdint>


void printError(const char* errroMsg);


int main(int argv, char* argc[])
{
    char *inputBuff = (char*)malloc(sizeof(char)*200);
    int logFD = -1;
    uint8_t numOfCharRead = -1;

    //Open (and create if needed) the log file
    if((logFD = open("output.log", O_APPEND | O_CREAT | O_WRONLY, S_IRWXU)) <= -1)
    {
        printError("\nError opening output.log: ");
    }

    //Write oppening line of ==== to log file
    if(write(logFD, "================\n", 17) <= -1)
    {
        printError("\nError writing to log file: ");
    }

    while(true)
    {
        //Read user input
        if((numOfCharRead = read(STDIN_FILENO, inputBuff, 200)) <= -1)
        {
            printError("\nError reading stdin: ");
        }

        //Exit if we recive a ^D
        if(inputBuff[0] == '\0')
        {
            close(logFD);
            return 0;
        }

        //Write what user said back to stdout
        if(write(STDOUT_FILENO, inputBuff, numOfCharRead) <= -1)
        {
            printError("\nError writing to stdout: ");
        }

        //Save what user said to the log
        if(write(logFD, inputBuff, numOfCharRead) <= -1)
        {
            printError("\nError writing to log file: "); 
        }

        //Clear the first char, in case we get a ^D. For some reason a ^D wont
        //override whats already in the buffer
        inputBuff[0] = '\0';
    }
}

void printError(const char* errorMsg)
{
    write(STDERR_FILENO, errorMsg, strlen(errorMsg));
    write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
    write(STDERR_FILENO, "\n", 1);
    exit(1);
}
