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
    argStruct_t *args = (argStruct_t*)givenArgs;
    //printf("rowStart: %d\nrowEnd: %d\nindx: %d", args->rowStart, args->rowEnd, args->resultIndx);

    //Search the rows we were given
    for(uint16_t row=args->rowStart; row < args->rowEnd; row++)
    {
        for(uint16_t col=0; col < Cols; col++)
        {
            threadResults[args->resultIndx] += checkForMatch(row,col);
        }
    }
}

int main(int argc, char *argv[]) 
{
    int sequencesFound = 0;
    uint16_t rowStart = 0;
    uint16_t numOfTasks = 0;
    int rc; 

    if(argc <= 1){ printf("\nInvalid Arguments\n"); exit(-1);}

    for( argc--, argv++; argc > 0; argc-=2, argv+=2  ) {
        if      (strcmp(argv[0], "-s" ) == 0 ) srand ( atoi(argv[1]) );
        else if (strcmp(argv[0], "-r" ) == 0 ) Rows = atoi(argv[1]);
        else if (strcmp(argv[0], "-c" ) == 0 ) Cols = atoi(argv[1]);
        else if (strcmp(argv[0], "-l" ) == 0 ) Detect_len = atoi(argv[1]);
        else if (strcmp(argv[0], "-t" ) == 0 ) numThreads = atoi(argv[1]);
        else { printf("\nInvalid Arguments\n"); exit(-1); }
    }

    makeAnImage();

    switch(numThreads)
    {
        case 1:
            argStruct_t args;

            numOfTasks = Rows/1;
            
            args.resultIndx = 0;
            args.rowStart = rowStart;
            args.rowEnd = rowStart + numOfTasks;

            sequenceFind(&args);
            break;
        case 2:
            pthread_t thread1, thread2;

            numOfTasks = Rows/2;

            //Create structs for the arguments passed to the threads
            argStruct_t *thread1Args = malloc(sizeof(argStruct_t));
            argStruct_t *thread2Args = malloc(sizeof(argStruct_t));
            
            //Set arguments for thread 1
            thread1Args->resultIndx = 0;
            thread1Args->rowStart = rowStart;
            thread1Args->rowEnd = rowStart + numOfTasks;

            //Create thread 1 and return an error if fails
            rc = pthread_create(&thread1, NULL, sequenceFind, (void*)thread1Args);
            assert(rc == 0);

            rowStart += numOfTasks;//Move the starting row by the # of tasks

            //Set arguments for thread 2
            thread2Args->resultIndx = 1;
            thread2Args->rowStart = rowStart;
            thread2Args->rowEnd = rowStart + numOfTasks;

            //Create thread 1 and return an error if fails
            rc = pthread_create(&thread2, NULL, sequenceFind, (void*)thread2Args);
            assert(rc == 0);

            //Wait for threads to finish
            rc = pthread_join(thread1, NULL); assert(rc == 0);
            rc = pthread_join(thread2, NULL); assert(rc == 0);

            break;
        case 4:
            break;
        case 8:
            break;
        case 16:
            break;
        default:
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
