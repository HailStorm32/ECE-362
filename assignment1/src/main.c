#include <cstdio>
#include <cstdint>
#include <climits>
#include <string.h>

int getScore(const char *str);

struct optionFlags
{
    bool highest;
    bool lowest;
};

int main(int argv, char *argc[])
{
    uint8_t sortedIndxes2Prnt[20];
    bool invalidArg = false;
    bool isOutOfOrder = true;
    uint8_t tempIndx = 0;
    uint8_t indxHolder = 0;
    uint8_t totalNumOfStr = 0;
    optionFlags flags;

    //Inintialize option flags
    flags.highest = false;
    flags.lowest = false;

    //Initialize array
    for(uint8_t indx = 0; indx < 20; indx++)
    {
        sortedIndxes2Prnt[indx] = 255;
    }

    //Only continue if we have arguments
    if(argv <= 1)
    {
        invalidArg = true;
    }
    else
    {
        bool hasStr = false;
        bool haveOpt = true;

        //Cycle through the arguments
        for(uint8_t indx = 1; indx < argv; indx++)
        {
            //Check to see if an option
            if(argc[indx][0] == '-')
            {
                uint8_t indx2 = 1;
                haveOpt = false;

                //Get and record each option (also works if they are grouped)
                //While char is still a letter
                while((argc[indx][indx2] >= 97 && argc[indx][indx2] <= 122) ||
                        (argc[indx][indx2] >= 65 && argc[indx][indx2] <= 90))
                {
                    //Log which option was given
                    switch(argc[indx][indx2])
                    {
                        case 'h':
                            haveOpt = true;
                            flags.highest = true;
                            break;
                        case 'l':
                            haveOpt = true;
                            flags.lowest = true;
                            break;
                    }
                    indx2++;
                }
            }
            //Make sure there is atleast one string
            else if(argc[indx][0] != '-' && argc[indx][0] != '\0')
            {
                hasStr = true;
                totalNumOfStr++;
            }
        }

        //If we never got a valid option with a '-' 
        //or a string set the flag
        if(haveOpt == false || hasStr == false)
        {
            invalidArg = true;
        }
    }

    //Exit if invalid arguments
    if(invalidArg)
    {
        printf("Error: Invalid argument(s)\n");
        printf("Usage: filter [-l] [-h] string string ...\n");
        return 1;
    }

    tempIndx = 0;

    //Get the indexes of where strings are stored in the argc array
    for(uint8_t indx = 1; indx < argv; indx++)
    {
        //Only get non-options
        if(argc[indx][0] != '-')
        {
            sortedIndxes2Prnt[tempIndx] = indx;
            tempIndx++;
        }
    }

    //Sort the strings numerically based on the total ascii value of the letters
    //Continue to loop until we can go though without having to reorder
    while(isOutOfOrder)
    {
        isOutOfOrder = false;

        //Cylcle through the list
        for(uint8_t currIndxPtr = 0; 
                currIndxPtr < totalNumOfStr - 1; currIndxPtr++)
        {
            //If the current and adjacent cell are out of order
            if(getScore(argc[sortedIndxes2Prnt[currIndxPtr]]) > 
                    getScore(argc[sortedIndxes2Prnt[currIndxPtr + 1]]))
            {
                //Store the current value in the cell
                indxHolder = sortedIndxes2Prnt[currIndxPtr]; 
                
                //Move value from the right cell into current cell
                sortedIndxes2Prnt[currIndxPtr] = 
                    sortedIndxes2Prnt[currIndxPtr + 1]; 

                //Move stored value into right cell
                sortedIndxes2Prnt[currIndxPtr + 1] = indxHolder; 
                
                isOutOfOrder = true;
            }
        }
    }


    //Begin running options

    if(flags.highest)
    {
        printf("high: %s\n", argc[sortedIndxes2Prnt[totalNumOfStr - 1]]);
    }

    if(flags.lowest)
    {
        printf("low: %s\n", argc[sortedIndxes2Prnt[0]]);
    }

    //No options given, print all strings
    if(!flags.lowest && !flags.highest)
    {
        int lastPrinted = 0;
        
        for(uint8_t indx = 0; indx < totalNumOfStr; indx++)
        {
            //Dont print a duplicate value
            if(lastPrinted != getScore(argc[sortedIndxes2Prnt[indx]]))
            {
                printf("%s\n", argc[sortedIndxes2Prnt[indx]]);
                lastPrinted = getScore(argc[sortedIndxes2Prnt[indx]]);
            }
        }
    }
    return 0;
}

int getScore(const char *str)
{
    uint8_t indx = 0;
    int score = 0;

    while(str[indx] != '\0')
    {
        score += str[indx];
        indx++;
    }

    return score;
}
