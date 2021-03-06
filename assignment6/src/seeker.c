/*
 * Name: Demetri Van Sickle
 * Class: ECE 362
 * Date: 3/1/22
 * modified example code
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>

#define MAX_ROWS     16000
#define MAX_COLS     16000
#define DETECT_LEN      20
#define DEFAULT_THREADS 1

int Rows        = MAX_ROWS;
int Cols        = MAX_COLS;
int Detect_len  = DETECT_LEN;
int Image[MAX_ROWS][MAX_COLS];
int numThreads = DEFAULT_THREADS;
uint8_t debugLvl = 0;
int threadResults[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

typedef struct
{
    uint8_t resultIndx;
    uint16_t rowStart;
    uint16_t rowEnd;
}argStruct_t;

int checkForMatch(int row, int col) {
    int r,c, length;
    int detect = 0;

    for(length=0, c=col; c < Cols; c++)
        if( Image[row][c] == 1 )
        { if( ++length == Detect_len ) { detect++; break; } }
        else break;

    for(length=0, r=row; r < Rows; r++)
        if( Image[r][col] == 1 )
        { if( ++length == Detect_len ) { detect++; break; } }
        else break;

    return detect;
}

void makeAnImage() {
    for(int row=0; row < Rows; row++)
        for(int col=0; col< Cols; col++)
            Image[row][col] = rand() % 2;
}

void sequenceFind(void *givenArgs)
{
    int numOfMatches = 0;

    if(debugLvl >= 2)
    {
        pthread_t tid;
        tid = pthread_self();
        printf("\nthread ID: %lu", (unsigned long)tid);
    }

    argStruct_t *args = (argStruct_t*)givenArgs;

    if(debugLvl >= 3)
    {
        printf("\n\nrowStart: %d\nrowEnd: %d\nindx: %d\n\n\n", args->rowStart, args->rowEnd, args->resultIndx);
    }

    //Search the rows we were given
    for(uint16_t row=args->rowStart; row < args->rowEnd; row++)
    {
        for(uint16_t col=0; col < Cols; col++)
        {
            numOfMatches += checkForMatch(row,col);
        }
    }
    threadResults[args->resultIndx] = numOfMatches;
}

void spawnThreads(uint8_t numOfThreads, pthread_t* threads, argStruct_t* args)
{
    uint16_t rowsPerThread = 0;
    uint16_t rowStart = 0;
    int rc; 

    rowsPerThread = Rows/numOfThreads;

    //Cycle through and create all the threads
    for(uint8_t indx = 0; indx < numOfThreads; indx++)
    {
        //Set arguments for thread
        args[indx].resultIndx = indx;
        args[indx].rowStart = rowStart;
        args[indx].rowEnd = rowStart + rowsPerThread;

        //Create thread and return an error if fails
        rc = pthread_create(&threads[indx], NULL, sequenceFind, (void*)&args[indx]);
        assert(rc == 0);

        rowStart += rowsPerThread;//Move the starting row by the # of tasks
    }
}

int main(int argc, char *argv[]) 
{
    int sequencesFound = 0;
    int rc; 

    //Error if we dont have arguments
    if(argc <= 1){ printf("\nInvalid Arguments\n"); exit(-1);}

    for( argc--, argv++; argc > 0; argc-=2, argv+=2  ) {
        if      (strcmp(argv[0], "-s" ) == 0 ) srand ( atoi(argv[1]) );
        else if (strcmp(argv[0], "-r" ) == 0 ) Rows = atoi(argv[1]);
        else if (strcmp(argv[0], "-c" ) == 0 ) Cols = atoi(argv[1]);
        else if (strcmp(argv[0], "-l" ) == 0 ) Detect_len = atoi(argv[1]);
        else if (strcmp(argv[0], "-t" ) == 0 ) numThreads = atoi(argv[1]);
        else if (strcmp(argv[0], "-d" ) == 0 ) debugLvl = atoi(argv[1]);
        else { printf("\nInvalid Arguments\n"); exit(-1); }
    }

    makeAnImage();

    //Only run if we were given a valid thread count
    if((((numThreads % 2) == 0) && (numThreads > 0 && numThreads < 17)) 
            || numThreads == 1)
    {
        if(debugLvl >= 1)
        {
            printf("\n%d thread:\n", numThreads);
        }

        pthread_t threads[numThreads];
        argStruct_t *threadArgs[numThreads];

        spawnThreads(numThreads, threads, threadArgs);

        //Wait for threads to finish
        for(uint8_t indx = 0; indx < numThreads; indx++)
        {
            rc = pthread_join(threads[indx], NULL); 
            assert(rc == 0);
        }
    }
    else
    {
        printf("\nInvalid number of threads");
        exit(-1);
    }


    //Add all the counts from the threads
    for(uint8_t indx = 0; indx < 16; indx++)
    {
        sequencesFound += threadResults[indx];
    }

    printf("\nTOTAL DETECTED: %d\n", sequencesFound);

    exit(0);
}
