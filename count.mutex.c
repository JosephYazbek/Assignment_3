#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

long iterations = 0;
long threadcount;
long length;
long count = 0;
long *array;
pthread_mutex_t mutex;

/*
Requirement: X
Output: returns the true number of 1's in the array
*/
long count1s()
{
    int counter = 0;
    int i = 0;
    for (i = 0; i < length; i++)
    {
        if (array[i] == 1)
        {
            counter++;
        }
    }
    return counter;
}

/*
Requirement: the id of the thread 
Output: adds the number of 1's the thread counted from its portion if the array
*/
void *ThreadCount1s(void *rank)
{

    long i;
    long rrank = (long *)rank;
    // printf("\nrank: %ld",rrank);
    // printf("\n-----------");
    long portion = length / threadcount;
    // printf("\nPortion: %d",portion);
    // printf("\n-----------");
    long from = rrank * portion;
    long to = from + portion;

    // printf("\nfrom:%d  to:%d",from,to);
    // printf("\n-----------");

    pthread_mutex_lock(&mutex);
    for (i = from; i < to; i++)
    {
        if (length - to < portion)
        { // if length can't be divided equally to the threads, add the remaining elements to the last thread.
            to = length;
        }
        if (array[i] == 1)
        {
            count++;
        }
        iterations++;
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{
    printf("Enter the length of the array: ");
    scanf("%ld", &length);
    printf("\nEnter the number of threads: ");
    scanf("%ld", &threadcount);
    srand(time(0));
    clock_t start_thread, end_thread, start, end;
    double time_thread, time;
    array = (long *)malloc(sizeof(long) * length);
    for (long i = 0; i < length; i++)
    {
        array[i] = rand() % 6;
    }

    printf("\n");
    start = clock();
    printf("\nReal count = %ld", count1s());
    end = clock();
    time = ((double)(end - start)) / CLOCKS_PER_SEC * 1.0;
    printf("\nTotal time taken without threads = %f", time);

    pthread_mutex_init(&mutex, NULL);
    pthread_t t[threadcount];
    start_thread = clock();
    printf("\nThreadcount: %ld", threadcount);
    for (long i = 0; i < threadcount; i++)
    {
        pthread_create(&(t[i]), NULL, &ThreadCount1s, (void *)i);
        // printf("\n%d",i);
    }

    for (long i = 1; i < threadcount; i++)
    {
        pthread_join(t[i], NULL);
    }

    end_thread = clock();
    time_thread = ((double)(end_thread - start_thread)) / CLOCKS_PER_SEC * 1.0;
    printf("\nTotal time taken using threads = %f", time_thread);
    printf("\ncount of 1's using %ld threads = %ld\n", threadcount, count);
    printf("Iterations = %ld\n", iterations);
    return 0;
}
