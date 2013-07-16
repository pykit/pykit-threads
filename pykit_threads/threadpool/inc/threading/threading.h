/* 
Continuum Analytics, Inc

Portion (esp win32) of the portable thread API is 
adapted from dlib <http://dlib.net/>.

A portable threading and thread pool library written in ANSI C.
It is written is a object oriented style.
The API is designed to look like Python threading and Queue packages in the 
standard library.


Style Notes:

    - Functions return 0 on success; non-zero value on error.
    - *_init functions are constructor that takes an uninitialized 
      object struct.
    - *_close functions are destructor.
*/

#ifndef THREADING_H_
#define THREADING_H_
#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN32__) 
    #include <threading/thread_windows.h>
#else
    #include <threading/thread_posix.h>
#endif

/* 
Init and execute a function in a new thread.
*/
int
Thread_init(Thread* self, void (*target)(void*), void* args);

/*
Detach the thread.
Release thread handle on win32.
Safe to call while the thread is still running.
*/
void
Thread_detach(Thread* self);

/*****************************************************************************/

/* Mutex */

int
Mutex_init(Mutex* self);

void
Mutex_close(Mutex* self);

void
Mutex_lock(Mutex* self);

void
Mutex_unlock(Mutex* self);

/*****************************************************************************/

/* Signal */

int
Signal_init(Signal* self, Mutex* mutex);

void
Signal_close(Signal* self);

/* 
Wait for a signal
*/
void
Signal_wait(Signal* self);

/*
Signal at least one thread
*/
void
Signal_signal(Signal* self);

/*
Signal all waiting threads.
*/
void
Signal_broadcast(Signal* self);


#endif /* THREADING_H_ */
