#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex;

void printArr(int array[]) 
{
    for (int i = 0; i < 10; i++) 
    {
        printf("%d", array[i]);
    }
    printf("\n");
}

void *print(void *array)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 10; i++)
    {
        ((int *)array)[i] = ((int *)array)[i] + i;
    }
    printArr((int *)array);
    for (int i = 0; i < 10; i++)
    {
        ((int *)array)[i] = ((int *)array)[i] - i;
    }
    printArr((int *)array);
    printf("end\n");
    pthread_mutex_unlock(&mutex);
    return (0);
}

int main() 
{
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    printf("MUTEX: %ld|\n", mutex.__sig);
    pthread_create(&t1, NULL, print, (void *)array);
    pthread_create(&t2, NULL, print, (void *)array);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}