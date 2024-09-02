#include <stdio.h>
#include <pthread.h>

int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thr_exit()
{
    pthread_mutex_lock(&m);
    done = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

void *child(void *arg)
{
    printf("child\n");
    thr_exit();
    return NULL;
}

void thr_join()
{
    pthread_mutex_lock(&m);
    while (done == 0)
        pthread_cond_wait(&c, &m); //释放锁，并让调用县城休眠(原子的);线程被唤醒之后，重新获取锁，再返回调用者
    pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[])
{
    printf("parent:begin\n");
    pthread_t p;
    pthread_create(&p, NULL, child, NULL);
    thr_join();
    printf("parent:end\n");
    return 0;
}

/*
如果去掉done，代码变成如下形式
void thr_exit()
{
    pthread_mutex_lock(&m);
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

void thr_join()
{
    pthread_mutex_lock(&m);
    pthread_cond_wait(&c, &m); 
    pthread_mutex_unlock(&m);
}
假设子线程立即运行并调用thr_exit()，此时子线程发送信号，但是当前没有在条件变量上睡眠等待的线程。
父线程运行时，就会调用wait并卡在那里

void thr_exit()
{
    done = 1
    pthread_cond_signal(&c);
}

void thr_join()
{
    if (done == 0)
        pthread_cond_wait(&c); 
}
假设父进程调用thr_join(),然后检查完done的值为0，试图睡眠，但在调用wait进入睡眠之前，父进程被中断。
子线程修改done为1，发出信号，同样没有等待线程，父进程再次运行时，就会长眠不醒。
*/