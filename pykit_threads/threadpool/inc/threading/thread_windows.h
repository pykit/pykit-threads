#ifndef THREAD_POSIX_H_
#define THREAD_POSIX_H_
#include <windows.h>

#define WIN32

typedef struct {
    HANDLE handle;
} Thread;

typedef struct {
    CRITICAL_SECTION cs;
} Mutex;

typedef struct {
    HANDLE  sema;
    HANDLE  semacount;
    int     waiters;
    Mutex  *mutex;
    Mutex   mutex_waiter;
} Signal;

#endif /* THREAD_POSIX_H_ */
