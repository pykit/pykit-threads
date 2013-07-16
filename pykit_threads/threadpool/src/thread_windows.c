/*
Adapted from dlib <http://dlib.net/>
*/
#include <stdlib.h>
#include <threading/threading.h>
#include <windows.h>
#include <process.h>

int
Thread_init(Thread* self, void (*target)(void*), void* args)
{
    /* 
    Use _beginthreadex to spawn a new thread.
    The simpler version _beginthread automatically closes the handle when the
    thread exit.  Since our thread can run forever, it MAY (not sure) cause
    leakage or zombie thread.  It seems explicitly calling CloseHandle is safer.
    */
    self->handle = (HANDLE)_beginthreadex(NULL, 0, target, args, 0, NULL);
	if ( self->handle == 0 ) {
		return 1;
	}
	return 0;
}

void
Thread_detach(Thread* self)
{
    CloseHandle(self->handle);
}

int
Mutex_init(Mutex* self)
{
    InitializeCriticalSection(&self->cs);
	return 0;
}

void
Mutex_close(Mutex* self)
{
    DeleteCriticalSection(&self->cs);
}

void
Mutex_lock(Mutex* self)
{
    EnterCriticalSection(&self->cs);
}

void
Mutex_unlock(Mutex* self)
{
    LeaveCriticalSection(&self->cs);
}



int
Signal_init(Signal* self, Mutex* mutex)
{
    self->sema = CreateSemaphore(NULL, 0, 100000000, NULL);
    
    if ( self->sema == NULL ) {
        return 1;
    }

    self->semacount = CreateSemaphore(NULL, 0, 100000000, NULL);

    if ( self->semacount == NULL ) {
        CloseHandle(self->sema);
        return 1;
    }

    if ( Mutex_init(&self->mutex_waiter) ) {
        CloseHandle(self->sema);
        CloseHandle(self->semacount);
        return 1;
    }
    
    self->waiters = 0;
    self->mutex = mutex;
	
	return 0;
}

void
Signal_close(Signal* self)
{
    CloseHandle(self->sema);
    CloseHandle(self->semacount);
    Mutex_close(&self->mutex_waiter);
}

void
Signal_wait(Signal* self)
{
    Mutex_lock(&self->mutex_waiter);
    ++self->waiters;
    Mutex_unlock(&self->mutex_waiter);

    Mutex_unlock(self->mutex);
    WaitForSingleObject(self->sema, INFINITE);

    ReleaseSemaphore(self->semacount, 1, NULL);

    Mutex_lock(self->mutex);
}

void
Signal_signal(Signal* self)
{
    Mutex_lock(&self->mutex_waiter);
    if ( self->waiters > 0 ){
        -- self->waiters;
        ReleaseSemaphore(self->sema, 1, NULL);
        WaitForSingleObject(self->semacount, INFINITE);
    }
    Mutex_unlock(&self->mutex_waiter);
}

void
Signal_broadcast(Signal* self)

{
    int i;
    Mutex_lock(&self->mutex_waiter);
    if ( self->waiters > 0 ){
        ReleaseSemaphore(self->sema, self->waiters, NULL);

        for ( i = 0; i < self->waiters; ++i ) {
            WaitForSingleObject(self->semacount, INFINITE);
        }

        self->waiters = 0;
    }
    Mutex_unlock(&self->mutex_waiter);
}
