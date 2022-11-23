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
int count;
int numberThreads;
pthread_mutex_t lock;

void* count1s (void* start) {
    int i;
    struct argument* args = (struct argument*) start;
    if(args->id == numberThreads - 1) {
        for (i = args->start; i < length; i++) {
            if (array[i] == 1) {
                pthread_mutex_lock(&lock);
                count++;
                pthread_mutex_unlock(&lock);
            }
        }
    }
    else {
        for (i = args->start; i < args->end; i++) {
            if (array[i] == 1) {
                pthread_mutex_lock(&lock);
                count++;
                pthread_mutex_unlock(&lock);
            }
        }
    }
    free(start);
}
int main() {
    pthread_mutex_init(&lock, NULL);
    struct timeval start, end;
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
        ptr_arg = malloc(3 * sizeof (int));
        ptr_arg->start = i * floor((double)length/numberThreads);
        ptr_arg->end = ptr_arg->start + floor((double)length / numberThreads);
        ptr_arg->id = i;
        pthread_create(&threads[i], NULL, count1s, (void *) ptr_arg);
    }
    for (int i = 0; i < numberThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - start.tv_sec;
    long ms = end.tv_usec - start.tv_usec;
    double elapsed = seconds + ms * pow(10, -6);
    free(array);
    printf("Answer produced by the threads is: %d\n", count);
    printf("Time taken: %fs\n", elapsed);
    printf("Real answer is: %d", ones);
}