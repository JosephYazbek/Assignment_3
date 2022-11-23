#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
int *array;
long length;
int totalCount;
int numberThreads;
const int cachefill = 126896;

struct argument{
    int count;
    int start;
    int end;
    int id;
    char dummy[]; //cachefill
};

void* count1s (void* start) {
    struct argument* argument = (struct argument*)start;
    int i;
    if(argument->id == numberThreads - 1){
        for (i = argument->start; i < length; i++) {
            if (array[i] == 1) {
                argument->count++;
            }
        }
    }
    else {
        for (i = argument->start; i < argument->end; i++) {
            if (array[i] == 1) {
                argument->count++;
            }
        }
    }
    return (void*) argument;
}
int main() {
    struct timeval start, end;
    struct argument* arg;
    int ones = 0;
    srand(time(NULL));
    printf("Size of the array: ");
    scanf("%ld", &length);
    array = (int *) malloc(length * sizeof(int));
    for (int i = 0; i < length; i++)
        array[i] = (rand() % 6);
    for (int i = 0; i < length; i++) {
        if (array[i] == 1)
            ones++;
    }
    printf("Threads used in program: ");
    scanf("%d", &numberThreads);
    pthread_t threads[numberThreads];
    gettimeofday(&start, 0);
    for (int i = 0; i < numberThreads; i++) {
        arg = malloc(4 * sizeof(int) + cachefill);
        arg->count = 0;
        arg->start = i * length/numberThreads;
        arg->end = arg->start + length/numberThreads;
        arg->id = i;
        for(int i = 0; i < cachefill; i++){
            arg->dummy[i] = 'a';
        }
        pthread_create(&threads[i], NULL, count1s, (void *) arg);
    }
    for (int i = 0; i < numberThreads; i++) {
        pthread_join(threads[i], (void**) &arg);
        totalCount += arg->count;
        free(arg);
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - start.tv_sec;
    long ms = end.tv_usec - start.tv_usec;
    double elapsed = seconds + ms * pow(10, -6);
    free(array);
    printf("Answer produced by the threads is: %d\n", totalCount);
    printf("Time taken: %fs\n", elapsed);
    printf("Real answer is: %d", ones);
}