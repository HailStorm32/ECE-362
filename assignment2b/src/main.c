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
void readLine(int fileFD, char* line);

int main(int argv, char* argc[])
{
    bool hasFileName = false;
    bool hasNumOfChars = false;
    int numOfChars = 0;
    int fileFD = -1;
    char buffer[10];
    char* line = (char*)malloc(sizeof(char)*100);

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
                if((fileFD = open(argc[indx], O_APPEND | O_RDWR )) <= 0)
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
        //readLine(fileFD, line);
        //printf("\nTEST: %s\n\n", line);

        for(uint8_t indx = 0; indx != numOfChars; indx++)
        {
            //memset(line, '9', strlen(line));
            readLine(fileFD, line);
            if(write(STDOUT_FILENO, line, strlen(line)) <= -1)
            {
                printError(1, "\nError: Failed to write to stdout\n");
            }
            printf("\n");
        }
        return 1;
    }
    else if(hasNumOfChars && !hasFileName)
    {
        for(uint8_t indx = 0; indx != numOfChars; indx++)
        {
            //memset(line, '9', strlen(line));
            readLine(STDIN_FILENO, line);
            if(write(STDOUT_FILENO, line, strlen(line)) <= -1)
            {
                printError(1, "\nError: Failed to write to stdout\n");
            }
            printf("\n");
        }
    }
    else if(!hasNumOfChars && hasFileName)
    {

        readLine(fileFD, line);
        if(write(STDOUT_FILENO, line, strlen(line)) <= -1)
        {
            printError(1, "\nError: Failed to write to stdout\n");
        }
        printf("\n");
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

void readLine(int fileFD, char* line)
{
    bool foundNewline = false;
    uint8_t lineIndx = 0;
    const uint8_t BUFF_SIZE = 10;

    uint8_t buffIndx = 0;
    static char buffer[BUFF_SIZE];
    static bool firstRun = false;

    /*printf("BUFFER ENTER: ");
    for(int i = 0; i < BUFF_SIZE; i++)
    {
        printf("%c, ", buffer[i]);
    }
    printf("\n");*/

    memset(line, '\0', strlen(line));

    //Gather data until we hit a newline
    while(!foundNewline)
    {
        if(!firstRun)
        {
            //        memset(buffer, '\0', strlen(buffer));

            //Read 10bytes
            if(read(fileFD, buffer, 10) == 0)
            {
                printError(1, "\nError: Failed to read any bytes\n");
            }
            firstRun = true;
        }
        /*printf("BUFFER: ");
        for(int i = 0; i < BUFF_SIZE; i++)
        {
            printf("%c, ", buffer[i]);
        }
        printf("\n");*/

        //Save the line if we already have it in the buffer
        if(strchr(buffer, '\n') != NULL)
        {
            while(buffer[buffIndx] != '\n')
            {
                line[lineIndx] = buffer[buffIndx];
                /*printf("\n\nL:%c | B:%c\n\n", line[lineIndx], buffer[buffIndx]);
                printf("\nLINE: ");
                for(int i = 0; i < 100; i++)
                {
                    printf("%c, ", line[i]);
                }*/

                lineIndx++;
                buffIndx++;
            }
            foundNewline = true;

            //Increment the buffIndx so as not to point at the newline
            buffIndx++;

            //Reset buffIndx if past end of buffer and clear the buffer
            if(buffIndx >= BUFF_SIZE)
            {
                buffIndx = 0;
                memset(buffer, '\0', strlen(buffer));
            }
            else if(buffIndx <= BUFF_SIZE - 1)
            {
                int baseIndx = 0;

                //shift the array down
                while(baseIndx < BUFF_SIZE)
                {
                    if(buffIndx < BUFF_SIZE)
                    {
                        buffer[baseIndx] = buffer[buffIndx];
                        buffer[buffIndx] = '\0';
                    }
                    else
                    {
                        buffer[baseIndx] = '\0';
                    }

                    buffIndx++;
                    baseIndx++;
                }
            }
        }
        //if we dont have any newline char, save whats in the buffer to the line
        //and get another chunck of data
        else
        {
            //Write the whole buffer to line
            while(buffer[buffIndx] != '\0')
                //while(buffIndx < BUFF_SIZE)
            {
                line[lineIndx] = buffer[buffIndx];

                lineIndx++;
                buffIndx++;
            }

            //memset(buffer, '\0', strlen(buffer));
            buffIndx = 0;

            //Read 10bytes
            if(read(fileFD, buffer, 10) == 0)
            {
                printError(1, "\nError: Failed to read any bytes\n");
            }
        }
    }

}
