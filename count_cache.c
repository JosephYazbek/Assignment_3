#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
int *array;
long length;
int total_count;
int numThreads;
const int cachefill = 127984;

struct argument
{
    int count;
    int start;
    int end;
    int id;
    char dummy[cachefill];
};

/*
    Requirement: void pointer.
    Output: returns number of 1's found.
    */

void *count1s(void *start)
{
    struct argument *argument = (struct argument *)start;
    int i;
    if (argument->id == numThreads - 1)
    {
        for (i = argument->start; i < length; i++)
        {
            if (array[i] == 1)
            {
                argument->count++;
            }
        }
    }
    else
    {
        for (i = argument->start; i < argument->end; i++)
        {
            if (array[i] == 1)
            {
                argument->count++;
            }
        }
    }
    return (void *)argument;
}
int main()
{
    struct timeval start, end;
    struct argument *arg;
    int ones = 0;
    srand(time(NULL));
    printf("Enter the size of the array: ");
    scanf("%ld", &length);
    array = (int *)malloc(length * sizeof(int));
    for (int i = 0; i < length; i++)
        array[i] = (rand() % 6);
    for (int i = 0; i < length; i++)
    {
        if (array[i] == 1)
            ones++;
    }
    printf("How many threads would you like to use: ");
    scanf("%d", &numThreads);
    pthread_t threads[numThreads];
    gettimeofday(&start, 0);
    for (int i = 0; i < numThreads; i++)
    {
        arg = malloc(4 * sizeof(int) + cachefill);
        arg->count = 0;
        arg->start = i * length / numThreads;
        arg->end = arg->start + length / numThreads;
        arg->id = i;
        for (int i = 0; i < cachefill; i++)
        {
            arg->dummy[i] = 'a';
        }
        pthread_create(&threads[i], NULL, count1s, (void *)arg);
    }
    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], (void **)&arg);
        total_count += arg->count;
        free(arg);
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - start.tv_sec;
    long ms = end.tv_usec - start.tv_usec;
    double elapsed = seconds + ms * pow(10, -6);
    free(array);
    printf("The answer produced by the threads is: %d\n", total_count);
    printf("Time taken: %fs\n", elapsed);
    printf("The real answer is: %d", ones);
}