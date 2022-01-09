#include <cstdio>
#include <cstdint>
#include <climits>

int getScore(const char *str);

struct optionFlags
{
    bool highest;
    bool lowest;
};

int main(int argv, char *argc[])
{
    bool invalidArg = false;
    int currHighScore = 0;
    uint8_t currHighestStr = 0;
    int currLowScore = INT_MAX;
    uint8_t currLowestStr = 0;
    optionFlags flags;

    //Inintialize option flags
    flags.highest = false;
    flags.lowest = false;

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


    //Begin running options

    if(flags.highest)
    {
        //Cycle through arguments 
        for(uint8_t indx = 1; indx < argv; indx++)
        {
            //Only get non-options and compare scores
            if(argc[indx][0] != '-')
            {
                int currScore = 0;

                currScore = getScore(argc[indx]);

                //Remember the index for the string 
                //if it it has the largest score
                if(currHighScore < currScore)
                {
                    currHighScore = currScore;
                    currHighestStr = indx;
                }
            }
        }

        printf("high: %s\n", argc[currHighestStr]);
    }

    if(flags.lowest)
    {
        //Cycle through arguments 
        for(uint8_t indx = 1; indx < argv; indx++)
        {
            //Only get non-options and compare scores
            if(argc[indx][0] != '-')
            {
                int currScore = 0;

                currScore = getScore(argc[indx]);

                //Remember the index for the string 
                //if it it has the largest score
                if(currLowScore > currScore)
                {
                    currLowScore = currScore;
                    currLowestStr = indx;
                }
            }
        }

        printf("low: %s\n", argc[currLowestStr]);
    }

    //No options given, print all strings
    if(!flags.lowest && !flags.highest)
    {
        for(uint8_t indx = 1; indx < argv; indx++)
        {
            //Only print non-options
            if(argc[indx][0] != '-')
            {
                printf("\n%s ", argc[indx]);
            }
        }
    }

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
