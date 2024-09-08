#include <stdio.h>
#include <semaphore.h>

sem_t s;
sem_init(&s, 0, 1); //声明一个信号量s，通过第三个参数初始化为1，第二个参数表示信号量是在同一进程的多个线程共享

/*
int sem_wait(sem_t *s)
{
    decrement the value of semaphore s by one wait if value of semaphore s is negative
}

int sem_post(sem_t *s)
{
    increment the value of semaphore s by one if there are one or more threads waiting, wake one
}
*/