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

#include <stdio.h>//DEBUG

void printError(const uint8_t errorType, const char* errorMsg);
void print(int fd, char *msg);
void readData(int fd, char* buff, int numOfBytes);

int main(int argc, char *argv[])
{
    int forkPid = -1;
    int sumPipe[2];//Pipe for the child doing the sum
    int productPipe[2];//Pipe for the child doing the product
    const int BUFF_SIZE = 100;
    char buff[BUFF_SIZE];

    //Create pipe line for comms between parent and sum child
    if(pipe(sumPipe) < 0)
    {
        printError(1, "Error cant create pipe\n");
    }
    
    //Fork process for first child
    if((forkPid = fork()) < 0)
    {
        printError(1, "Error couldnt fork\n");
    }
    
    if(forkPid == 0)//If child
    {
        //Close writing pipe
        close(sumPipe[1]);

        readData(sumPipe[0], buff, BUFF_SIZE);
        print(STDOUT_FILENO, buff);


        //Close reading pipe
        close(sumPipe[0]);
        exit(0);
    }
    else //If parent
    {
        //Create pipe line for comms between parent and product child
        pipe(productPipe);

        //Fork process for second child
        if((forkPid = fork()) < 0)
        {
            printError(1, "Error couldnt fork second time\n");
        }

        if(forkPid == 0)//If 2nd child
        {
            //Close writing pipe
            close(productPipe[1]);

            readData(productPipe[0], buff, BUFF_SIZE);
            print(STDOUT_FILENO, buff);


            //Close reading pipe
            close(productPipe[0]);
            exit(0);
        }
        else//If parent
        {
            //Close reading pipes
            close(sumPipe[0]);
            close(productPipe[0]);

            print(sumPipe[1], "Hello 1");
            print(sumPipe[1], "Hello 1a\n");
            print(sumPipe[1], "Hello 1b");
            print(productPipe[1], "Hello 2");

            //Close writing pipes
            close(sumPipe[1]);
            close(productPipe[1]);

            wait(NULL);
        }
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

void print(int fd, char *msg)
{
    if(write(fd, msg, strlen(msg)) <= 0)
    {
        printError(1, "Error writing to STDOUT: \n");
    }
}

void readData(int fd, char* buff, int numOfBytes)
{
    if(read(fd, buff, numOfBytes) < 0)
    {
        printError(1, "Error cant read data\n");
    }
}
