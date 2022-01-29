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
#define DEBUG //DEBUG

const int BUFF_SIZE = 100;
const int NUM_BUFF_SIZE = 500;

void printError(const uint8_t errorType, const char* errorMsg);
void print(int fd, char *msg);
int readData(int fd, char* buff, int numOfBytes);
bool validateString(char *charBuffer, int numOfChars);
uint8_t parseData(char *charBuffer, int *numArray, int numOfChars);

int main(int argc, char *argv[])
{
    int forkPidSum = -1;
    int forkPidProduct = -1;
    int sumPipe[2];//Pipe for the child doing the sum
    int productPipe[2];//Pipe for the child doing the product
    char buff[NUM_BUFF_SIZE];
    int numOfCharRead = -1;
    int numbers[BUFF_SIZE/2];
    uint8_t numOfNums = 0;

    //Create pipe line for comms between parent and sum child
    if(pipe(sumPipe) < 0)
    {
        printError(1, "Error cant create pipe\n");
    }

    //Fork process for first child (sum child)
    if((forkPidSum = fork()) < 0)
    {
        printError(1, "Error couldnt fork\n");
    }

    if(forkPidSum == 0)//If sum child
    {
        int sumTotal = 0;

        //Close writing pipe b/c we dont need it
        close(sumPipe[1]);
        numOfCharRead = readData(sumPipe[0], buff, NUM_BUFF_SIZE);

        //If we got signal from parent to exit
        if(strncmp(buff, "quit", 3) == 0)
        {
            printf("QUIT");
            memset(buff, '\0', NUM_BUFF_SIZE);
            itoa(sumTotal, buff, 10); 

            print(STDOUT_FILENO, "\nSum: ");
            print(STDOUT_FILENO, buff);
            print(STDOUT_FILENO, "\n");

            //Close reading pipe and exit
            close(sumPipe[0]);
            close(sumPipe[1]);
            exit(0);
        }

        //Extract numbers from buffer
        numOfNums = parseData(buff, numbers, numOfCharRead);

        //Sum all the numbers
        for(uint8_t indx = 0; indx < numOfNums; indx++)
        {
            sumTotal += numbers[indx];
            printf("%d, ", numbers[indx]);
        }
        memset(buff, '\0', NUM_BUFF_SIZE);
        itoa(sumTotal, buff, 10); 

        print(STDOUT_FILENO, "\nSum:");
        print(STDOUT_FILENO, buff);
        print(STDOUT_FILENO, "\n");

        //Close reading/writing pipes and exit
        close(sumPipe[0]);
        close(sumPipe[1]);
        exit(0);
    }
    else //If parent
    {
        //Create pipe line for comms between parent and product child
        pipe(productPipe);

        //Fork process for second child (product child)
        if((forkPidProduct = fork()) < 0)
        {
            printError(1, "Error couldnt fork second time\n");
        }

        if(forkPidProduct == 0)//If product child
        {
            int productTotal = 1;

            //Close writing pipe b/c we dont need it
            close(productPipe[1]);
            numOfCharRead = readData(productPipe[0], buff, NUM_BUFF_SIZE);

            //If we got signal from parent to exit
            if(strncmp(buff, "quit", 3) == 0)
            {
                printf("QUIT");
                memset(buff, '\0', NUM_BUFF_SIZE);
                itoa(productTotal, buff, 10); 

                print(STDOUT_FILENO, "\nProduct: ");
                print(STDOUT_FILENO, buff);
                print(STDOUT_FILENO, "\n");

                //Close reading pipe and exit
                close(productPipe[0]);
                close(productPipe[1]);
                exit(0);
            }

            //Extract numbers from buffer
            numOfNums = parseData(buff, numbers, numOfCharRead);

            //Multiply all the numbers
            for(uint8_t indx = 0; indx < numOfNums; indx++)
            {
                productTotal *= numbers[indx];
                printf("%d, ", numbers[indx]);
            }
            printf("PROT: %d\n", productTotal);
            memset(buff, '\0', NUM_BUFF_SIZE);
            itoa(productTotal, buff, 10); 
            printf("PROT_str: %s\n", buff);

            print(STDOUT_FILENO, "\nProduct:");
            print(STDOUT_FILENO, buff);
            print(STDOUT_FILENO, "\n");

            //Close reading/writing pipes and exit
            close(productPipe[0]);
            close(productPipe[1]);
            exit(0);
        }
        else//If parent
        {
            char cumulativeBuff[NUM_BUFF_SIZE];

            //Close reading pipes b/c we dont need them
            close(sumPipe[0]);
            close(productPipe[0]);

            memset(buff, '\0', BUFF_SIZE);

            //Get data from user
            while(true)
            {
                numOfCharRead += readData(STDIN_FILENO, buff, BUFF_SIZE);

                //Exit if we recive a ^D
                if(buff[0] == '\0' )
                {
                    break;
                }

                //Add on to the cumulitave buffer 
                strcat(cumulativeBuff, buff);
                memset(buff, '\0', BUFF_SIZE);
            }
            printf("strNum: %s\n", cumulativeBuff);

            //Validate data
            if(!validateString(cumulativeBuff, numOfCharRead))
            {
                printError(0,"");
            }
            else
            {
                print(sumPipe[1], cumulativeBuff);
                wait(NULL);
                print(productPipe[1], cumulativeBuff);
            }
            wait(NULL);

            //Close writing pipes
            close(sumPipe[1]);
            close(productPipe[1]);
        }
    }
    return 0;
}

uint8_t parseData(char *charBuffer, int *numArray, int numOfChars)
{
    char strNumber[BUFF_SIZE];
    uint8_t buffIndx = 0;
    uint8_t strNumIndx = 0;
    uint8_t rtnArrayIndx = 0;
    uint8_t numOfNums = 0;

    //"zero" the arrays arrays
    for(uint8_t indx = 0; indx < BUFF_SIZE/2; indx++)
    {
        numArray[indx] = 0;
    }
    memset(strNumber, '\0', BUFF_SIZE);

    //Get all the numbers out of the string
    do
    {
        //Get the number
        while((charBuffer[buffIndx] >= 48 && charBuffer[buffIndx] <= 57) 
                || charBuffer[buffIndx] == 45)
        {
            strNumber[strNumIndx] = charBuffer[buffIndx];
            buffIndx++;
            strNumIndx++;
        }

        //Move buffIndx to next number (move past the space) 
        while(charBuffer[buffIndx] == 32 || charBuffer[buffIndx] == '\n')
        {
            buffIndx++;
        }

        //Save the number
        numArray[rtnArrayIndx] = atoi(strNumber);

        //Dont count zeros
        if(numArray[rtnArrayIndx] != 0)
        { 
            numOfNums++;
        }
        rtnArrayIndx++;
        strNumIndx = 0;

        memset(strNumber, '\0', BUFF_SIZE);
    }while(charBuffer[buffIndx] != '\0');
    return numOfNums;
}

void printError(const uint8_t errorType, const char* errorMsg)
{
    switch(errorType)
    {
        case 0:
            write(STDERR_FILENO, "Error: must input numbers seperated by a space\n", 47);
            write(STDERR_FILENO, "Usage: 13 -12 4 ...\n", 20);
            break;
        case 1:
            write(STDERR_FILENO, errorMsg, strlen(errorMsg));
            write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
            write(STDERR_FILENO, "\n", 1);
            exit(1);
            break;
    }
}

void print(int fd, char *msg)
{
    if(write(fd, msg, strlen(msg)) <= 0)
    {
        printError(1, "Error writing to STDOUT: \n");
    }
}

int readData(int fd, char* buff, int numOfBytes)
{
    int charsRead = -1;
    if((charsRead = read(fd, buff, numOfBytes)) < 0)
    {
        printError(1, "Error cant read data\n");
    }
    return charsRead;
}

bool validateString(char *charBuffer, int numOfChars)
{
    int numOfSpaces = 0;
    int numOfNums = 0;

    //Count the number of spaces and numbers
    for(int indx = 0; indx < numOfChars; indx++)
    {
        if(charBuffer[indx] == 32 || charBuffer[indx] == '\n')
        {
            numOfSpaces++;
        }
        else if(charBuffer[indx] >= 48 && charBuffer[indx] <= 57)
        {
            numOfNums++; 
        }
    }

    //Fail if we dont have at least to operands (two #s seperated by a space)
    if(numOfSpaces < 1 || numOfNums < 2)
    {
#ifdef DEBUG
        printf("\nERROR: numOfSpaces < 1 || numOfNums < 2\n");
        printf("BUFF: %s\n", charBuffer);
        printf("# of spaces: %d\n", numOfSpaces);
        printf("# of #: %d\n", numOfNums);
#endif
        return false;
    }

    //Check to see if user string contains non numbers 
    for(int indx = 0; indx < numOfChars - 1; indx++)
    {
        //Make sure current char is a number, minus sign, space or newline 
        if((charBuffer[indx] < 48 || charBuffer[indx] > 57) 
                && !((charBuffer[indx] == 32) || (charBuffer[indx] == 45)
                    || (charBuffer[indx] == '\n') || (charBuffer[indx] == '\0')))
        {
#ifdef DEBUG
            int charVal = 0;
            charVal = charBuffer[indx];
            printf("\nERROR: invalid char: %d\n", charVal);
#endif

            return false;
        }
    }
    return true;
}
