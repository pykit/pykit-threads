#include <stdlib.h>
#include <threading/threading.h>

int
Thread_init(Thread* self, void (*target)(void*), void* args)
{
    if ( pthread_create(&self->thread_id, 0, (void*(*)(void*))target, args) ) {
        return 1;
    }
    return 0;
}

void
Thread_detach(Thread* self)
{
    pthread_detach(pthread_self());
}

/*****************************************************************************/

int
Mutex_init(Mutex* self)
{
    if ( pthread_mutex_init(&self->mutex, 0) ) {
        return 1;
    }
    return 0;
}

void
Mutex_close(Mutex* self)
{
    pthread_mutex_destroy(&self->mutex);
}

void
Mutex_lock(Mutex* self)
{
    pthread_mutex_lock(&self->mutex);
}

void
Mutex_unlock(Mutex* self)
{
    pthread_mutex_unlock(&self->mutex);
}

/*****************************************************************************/

int
Signal_init(Signal* self, Mutex* mutex)
{
    self->mutex = mutex;
    if ( pthread_cond_init(&self->cond, 0) ) {
        return 1;
    }
    return 0;
}

void
Signal_close(Signal* self)
{
    pthread_cond_destroy(&self->cond);
}

void
Signal_wait(Signal* self)
{
    pthread_cond_wait(&self->cond, &self->mutex->mutex);
}

void
Signal_signal(Signal* self)
{
    pthread_cond_signal(&self->cond);
}

void
Signal_broadcast(Signal* self)
{
    pthread_cond_broadcast(&self->cond);
}

