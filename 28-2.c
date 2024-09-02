#include <stdio.h>

typedef struct lock_t {
    int flag;
} lock_t;

int TestAndSet(int *old_ptr, int new)
{
    int old = *old_ptr; // fetch old value at old_ptr
    *old_ptr = new; //store 'new' into old_ptr
    return old; // return the old value
}

int CompareAndSwap(int *ptr, int expected, int new)
{
    int actual = *ptr;
    if (actual == expected)
        *ptr = new;
    return actual;
}

void init(lock_t *lock) 
{
    // 0 indicates that lock is available, 1 that it is held
    lock->flag = 0;
}

void lock(lock_t *lock)
{
    while (TestAndSet(&lock->flag, 1) == 1)
        ; // spin-wait (do nothing)

    // while (CompareAndSwap(&lock->flag, 0, 1) == 1)
    //     ; //spin
}

void unlock(lock_t *lock)
{
    lock->flag = 0;
}