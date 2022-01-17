#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <cstdint>

#include <cstdio>


int main(int argv, char* argc[])
{
    char *inputBuff = (char*)malloc(sizeof(char)*200);
    int logFD = -1;
    uint8_t numOfCharRead = -1;
    uint8_t numOfCharWrt = -1;

    //Open (and create if needed) the log file
    logFD = open("output.log", O_APPEND | O_CREAT | O_WRONLY, S_IRWXU);

    //Send error if opening file failed
    if(logFD <= -1)
    {
        write(STDERR_FILENO, "\nError opening output.log: ", 27);
        write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
        write(STDERR_FILENO, "\n", 1);
        return 1;
    }

    //Write oppening line of ==== to log file
    if(write(logFD, "================\n", 17) <= -1)
    {
        write(STDERR_FILENO, "\nError writing to log file: ", 28);
        write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
        write(STDERR_FILENO, "\n", 1);
        return 1;
    }

    while(true)
    {
        //Read user input
        numOfCharRead = read(STDIN_FILENO, inputBuff, 200);

        //Send error if reading failed
        if(numOfCharRead <= -1)
        {
            write(STDERR_FILENO, "\nError reading stdin: ", 21);
            write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
            write(STDERR_FILENO, "\n", 1);
            return 1;
        }

        //Write what user said back to stdout
        if(write(STDOUT_FILENO, inputBuff, numOfCharRead) <= -1)
        {
            write(STDERR_FILENO, "\nError writing to stdout: ", 25);
            write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
            write(STDERR_FILENO, "\n", 1);
            return 1;
        }

        //Save what user said to the log
        if(write(logFD, inputBuff, numOfCharRead) <= -1)
        {
            write(STDERR_FILENO, "\nError writing to log file: ", 28);
            write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
            write(STDERR_FILENO, "\n", 1);
            return 1;
        }


    }

    return 0;
}
