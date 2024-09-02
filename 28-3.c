#include <stdio.h>

typedef struct lock_t {
    int flag;
} lock_t;

int LoadLinked(int *ptr)
{
    return *ptr;
}

int StoreConditional(int *ptr, int value)
{
    if (no one has updated *ptr since the LoadLinked to this address)
    {
        *ptr = value;
        return 1; // success
    }
    else
        return 0; // failed to update
}

void lock(lock_t *lock)
{
    while (1)
    {
        while (LoadLinked(&lock->flag) == 1)
            ; //spin until it's zero
        if (StoreConditional(&lock->flag, 1) == 1)
            return; // if set-it-to-1 was a success: all done
                    // otherwise: try it all over again
    }
}

void unlock(lock_t *lock)
{
    lock->flag = 0;
}

/* 
flag == 0
Thread1                                    Thread2
call lock()
while (LoadLinked(&lock->flag) == 1) brk
interrupt: switch to Thread2
                                            call lock()
                                            while (LoadLinked(&lock->flag) == 1) brk
                                            if (StoreConditional(&lock->flag, 1) == 1) ret
                                            flag == 1
                                            interrupt: switch to Thread1

if (StoreConditional(&lock->flag, 1) == 1)
skip and enter next while loop                           
 */