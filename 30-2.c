#include <stdio.h>
#include <pthread.h>
#include <assert.h>

int buffer;
int count = 0; //initially, empty

pthread_cond_t cond;
pthread_mutex_t mutex;

void put(int value)
{
    assert(count == 0);
    count = 1; //把count设置为1表示缓冲区满了
    buffer = value;
}

int get()
{
    assert(count == 1);
    count = 0; //把count设置为0表示缓冲区清空
    return buffer;
}

/* first try, but has problem when has two consumer
void *producer(void *arg)
{
    int i;
    int loops = (int)arg;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);                         //p1
        if (count == 1)                                     //p2
            pthread_cond_wait(&cond, &mutex);               //p3
        put(i);                                             //p4
        pthread_cond_signal(&cond);                         //p5
        pthread_mutex_unlock(&mutex);                       //p6
    }
}

void *consumer(void *arg)
{
    int i;
    int loops = (int)arg;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);                         //c1
        if (count == 0)                                     //c2
            pthread_cond_wait(&cond, &mutex);               //c3
        int tmp = get();                                    //c4
        pthread_cond_signal(&cond);                         //c5
        pthread_mutex_unlock(&mutex);                       //c6
        printf("%d\n", tmp);
    }
}
*/

/* second try, replace if by while */
void *producer(void *arg)
{
    int i;
    int loops = (int)arg;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);                         //p1
        while (count == 1)                                  //p2
            pthread_cond_wait(&cond, &mutex);               //p3
        put(i);                                             //p4
        pthread_cond_signal(&cond);                         //p5
        pthread_mutex_unlock(&mutex);                       //p6
    }
}

void *consumer(void *arg)
{
    int i;
    int loops = (int)arg;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);                         //c1
        while (count == 0)                                  //c2
            pthread_cond_wait(&cond, &mutex);               //c3
        int tmp = get();                                    //c4
        pthread_cond_signal(&cond);                         //c5
        pthread_mutex_unlock(&mutex);                       //c6
        printf("%d\n", tmp);
    }
}