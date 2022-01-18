/*
 * Name: Demetri Van Sickle
 * Class: ECE 362
 * Date: 1/17/22
 * Description: A program that will take a filename and an integer value 
 *      as arguments. The program will then read the file and output the 
 *      number of lines specified by the integer value. 
 *      IF no file name is given, it will read from the user input. 
 *      IF no integer value is given, it will only read the first line
 */
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstdint>
#include <cstdio> //FOR DEBUG
#include <stdlib.h>
#include <cstring>

void printError(const uint8_t errorType, const char* errorMsg);

int main(int argv, char* argc[])
{
    bool hasFileName = false;
    bool hasNumOfChars = false;
    int numOfChars = 0;
    int fileFD = -1;

    if(argv == 2 || argv == 3)
    {
        //Cycle through arguments
        for(uint8_t indx = 1; indx < argv; indx++)
        {
            //Check to see if an option
            if(argc[indx][0] == '-')
            {
                if((numOfChars = abs(atoi(argc[indx]))) == 0)
                {
                    printError(0, "");
                }
                hasNumOfChars = true;
            }
            //If we have a file name
            else if(argc[indx][0] != '-' && argc[indx][0] != '\0')
            {
                //Try and open file And Report error if unable to open
                if((fileFD = open(argc[indx], O_APPEND | O_WRONLY, S_IRWXU)) < 0)
                {
                    printError(1, "Error opening file: ");
                }
                hasFileName = true;
            }
        }
    }
    else
    {
        printError(0, "");
    }

    //Run the options
    if(hasNumOfChars && hasFileName)
    {
    }
    else if(hasNumOfChars && !hasFileName)
    {
    }
    else if(!hasNumOfChars && hasFileName)
    {
    }
}

void printError(const uint8_t errorType, const char* errorMsg)
{
    switch(errorType)
    {
        case 0:
            write(STDERR_FILENO, "Error: Invalid argument(s)\n", 28 );
            write(STDERR_FILENO, "Usage: header filename [-wholeNumber]\n", 39);
            break;
        case 1:
            write(STDERR_FILENO, errorMsg, strlen(errorMsg));
            write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
            write(STDERR_FILENO, "\n", 1);
            break;
    }
    exit(1);
}
