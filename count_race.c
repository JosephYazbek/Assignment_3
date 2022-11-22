#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

long threadcount;
long length;
long count = 0;
long *array;
/*
requirement: X
Output: returns the true number of 1's in the array
*/
long count1s()
{
   int i = 0;
   for (i = 0; i < length; i++)
   {
      if (array[i] == 1)
      {
         count++;
      }
   }
   return count;
}
/*
requirement: the id of the thread 
Output: adds the number of 1's the thread counted from its portion if the array
*/
void *ThreadCount1s(void *rank)
{
   long rrank = (long)rank;
   long portion = length / threadcount;
   long from = rrank * portion;
   long to = from + portion;
   for (long i = from; i < to; i++)
   {
      if (length - to < portion)
      { // if length can't be divided equally to the threads, add the remaining elements to the last thread.
         to = length;
      }
      if (array[i] == 1)
      {
         count++;
      }
   }
}
int main()
{
   printf("Length of the array: ");
   scanf("%ld", &length);
   printf("\nNumber of threads: ");
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
   end = clock();
   time = ((double)(end - start)) / CLOCKS_PER_SEC * 1.0;
   printf("\nTotal time taken without threads = %f", time);
   printf("\nReal count = %ld", count1s());
   count = 0;
   pthread_t t[threadcount];
   start_thread = clock();
   for (long i = 0; i < threadcount; ++i)
   {
      pthread_create(&t[i], NULL, &ThreadCount1s, (void *)i);
   }

   for (long i = 0; i < threadcount; ++i)
   {
      pthread_join(t[i], NULL);
   }
   end_thread = clock();
   time_thread = ((double)(end_thread - start_thread)) / CLOCKS_PER_SEC * 1.0;
   printf("\nTotal time taken using threads = %f", time_thread);
   printf("\ncount of 1's using %d threads = %d\n", threadcount, count);
   return 0;
}
