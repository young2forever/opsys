#include <stdio.h>

typedef struct lock_t
{
    int flag;
} lock_t;

void init(lock_t *mutex) 
{
    // 0 -> lock is available, 1 -> held
    mutex->flag = 0;
}

void lock(lock_t *mutex)
{
    while (mutex->flag == 1) // TEST the flag
        ; // spin-wait (do nothing)
    mutex->flag = 1; //now SET it!
}

void unlock(lock_t *mutex)
{
    mutex->flag = 0;
}

/* 
flag == 0
Thread1                                    Thread2
call lock()
while (flag == 1)
interrupt: switch to Thread2
                                            call lock()
                                            while (flag == 1)
                                            flag = 1;
                                            interrupt: switch to Thread1
flag = 1; // set flag to 1(too!)                                
 */