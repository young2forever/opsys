#include <stdio.h>
#include <pthread.h>
#include <assert.h>

int buffer;
int count = 0; //initially, empty

pthread_cond_t empty, fill;
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

void *producer(void *arg)
{
    int i;
    int loops = (int)arg;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);
        while (count == 1)
            pthread_cond_wait(&empty, &mutex); //生产者等待条件变量empty
        put(i);
        pthread_cond_signal(&fill); //生产者发送信号给变量fill
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
            pthread_cond_wait(&fill, &mutex); //消费者等待fill
        int tmp = get();
        pthread_cond_signal(&empty); //消费者发送信号给变量empty
        pthread_mutex_unlock(&mutex);
        printf("%d\n", tmp);
    }
}