#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
struct argument{
    int start;
    int end;
    int id;
};
struct argument *ptr_arg;
int *array;
long length;
int totalCount;
int numberThreads;

void* count1s (void* start) {
    struct argument* args = (struct argument*)start;
    int* count = malloc(sizeof (int));
    int counter = 0;
    int i;
    if(args->id == numberThreads - 1)
        args->end = length;
    for (i = args->start; i < args->end; i++) {
        if (array[i] == 1) {
            counter++;
        }
    }
    free(start);
    *count = counter;
    return (void*) count;
}
int main() {
    struct timeval start, end;
    int* count;
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
        struct argument* args = malloc(3 * sizeof (int));
        args->start = i * length/numberThreads;
        args->end = args->start + length/numberThreads;
        args->id = i;
        pthread_create(&threads[i], NULL, count1s, (void *) args);
    }
    for (int i = 0; i < numberThreads; i++) {
        pthread_join(threads[i], (void**) &count);
        totalCount += *count;
        free(count);
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