#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define MAX 2
int buffer[MAX];
int fillx = 0;
int use = 0;
int count = 0;

void put(int value)
{
    buffer[fillx] = value;
    fillx = (fillx + 1) % MAX;
    count++;
}

int get()
{
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    count--;
    return tmp;
}

pthread_cond_t empty, fill;
pthread_mutex_t mutex;

void *producer(void *arg)
{
    int i;
    int loops = (int)arg;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);
        while (count == MAX)
            pthread_cond_wait(&empty, &mutex);
        put(i);
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer(void *arg)
{
    int i;
    int loops = (int)arg;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);
        while (count == 0)
            pthread_cond_wait(&fill, &mutex);
        int tmp = get();
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        printf("%d\n", tmp);
    }
}