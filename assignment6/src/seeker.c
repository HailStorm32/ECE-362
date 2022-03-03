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
            threadResults[args->resultIndx] += checkForMatch(row,col);
        }
    }
}

void spawnThreads(uint8_t numOfThreads, pthread_t* threads, argStruct_t* args)
{
    uint16_t numOfTasks = 0;
    uint16_t rowStart = 0;
    int rc; 

    numOfTasks = Rows/numOfThreads;

    //Cycle through and create all the threads
    for(uint8_t indx = 0; indx < numOfThreads; indx++)
    {
        //Set arguments for thread
        args[indx].resultIndx = indx;
        args[indx].rowStart = rowStart;
        args[indx].rowEnd = rowStart + numOfTasks;

        //Create thread and return an error if fails
        rc = pthread_create(&threads[indx], NULL, sequenceFind, (void*)&args[indx]);
        assert(rc == 0);

        rowStart += numOfTasks;//Move the starting row by the # of tasks
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

    if(numThreads == 1)
    {
        if(debugLvl >= 1)
        {
            printf("\n1 thread:\n");
        }

        pthread_t threads[1];
        argStruct_t *threadArgs[1];

        spawnThreads(1, threads, threadArgs);

        //Wait for threads to finish
        rc = pthread_join(threads[0], NULL); assert(rc == 0);
    }
    else if(numThreads == 2)
    {
        if(debugLvl >= 1)
        {
            printf("\n2 thread:\n");
        }

        pthread_t threads[2];
        argStruct_t *threadArgs[2];

        spawnThreads(numThreads, threads, threadArgs);

        //Wait for threads to finish
        for(uint8_t indx = 0; indx < numThreads; indx++)
        {
            rc = pthread_join(threads[indx], NULL); 
            assert(rc == 0);
        }
       /* uint16_t numOfTasks = 0;
        uint16_t rowStart = 0;
        int rc; 

        argStruct_t arg1, arg2;
        pthread_t thread1, thread2;

        numOfTasks = Rows/numThreads;

        //Set arguments for thread
        arg1.resultIndx = 0;
        arg1.rowStart = rowStart;
        arg1.rowEnd = rowStart + numOfTasks;

        //Create thread and return an error if fails
        rc = pthread_create(&thread1, NULL, sequenceFind, (void*)&arg1);
        assert(rc == 0);

        rowStart += numOfTasks;//Move the starting row by the # of tasks

        arg2.resultIndx = 1;
        arg2.rowStart = rowStart;
        arg2.rowEnd = rowStart + numOfTasks;

        //Create thread and return an error if fails
        rc = pthread_create(&thread2, NULL, sequenceFind, (void*)&arg2);
        assert(rc == 0);

        rc = pthread_join(thread1, NULL); 
        assert(rc == 0);
        rc = pthread_join(thread2, NULL); 
        assert(rc == 0);*/
    }
    else if(numThreads == 4)
    {
        if(debugLvl >= 1)
        {
            printf("\n4 thread:\n");
        }

        pthread_t threads[4];
        argStruct_t *threadArgs[4];

        spawnThreads(numThreads, threads, threadArgs);

        //Wait for threads to finish
        for(uint8_t indx = 0; indx < numThreads; indx++)
        {
            rc = pthread_join(threads[indx], NULL); 
            assert(rc == 0);
        }
    }
    else if(numThreads == 8)
    {
        if(debugLvl >= 1)
        {
            printf("\n8 thread:\n");
        }

        pthread_t threads[8];
        argStruct_t *threadArgs[8];

        spawnThreads(numThreads, threads, threadArgs);

        //Wait for threads to finish
        for(uint8_t indx = 0; indx < numThreads; indx++)
        {
            rc = pthread_join(threads[indx], NULL); 
            assert(rc == 0);
        }
    }
    else if(numThreads == 16)
    {
        if(debugLvl >= 1)
        {
            printf("\n16 thread:\n");
        }

        pthread_t threads[16];
        argStruct_t *threadArgs[16];

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
