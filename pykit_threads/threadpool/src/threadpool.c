#include <threading/threadpool.h>
#include <stdlib.h>

int
Queue_init(Queue* self, int capacity)
{
    if ( Mutex_init(&self->mutex) ) {
        return 1;
    }

    if ( Signal_init(&self->signal_put, &self->mutex) ) {
        Mutex_close(&self->mutex);
        return 1;
    }

    if ( Signal_init(&self->signal_get, &self->mutex) ) {
        Signal_close(&self->signal_put);
        Mutex_close(&self->mutex);
        return 1;
    }

    if ( Signal_init(&self->signal_join, &self->mutex) ) {
        Signal_close(&self->signal_put);
        Signal_close(&self->signal_get);
        Mutex_close(&self->mutex);
        return 1;
    }
    
    self->capacity = capacity;
    self->size = 0;
    self->head = self->tail = 0;
    self->pending_put = 0;
    self->pending_task = 0;
    self->items = malloc(sizeof(void*) * capacity);
    
    return 0;
}

void
Queue_close(Queue* self)
{
    Signal_close(&self->signal_put);
    Signal_close(&self->signal_get);
    Mutex_close(&self->mutex);
    free(self->items);
}

void
Queue_join(Queue* self)
{
    Mutex_lock(&self->mutex);
    while ( self->size > 0 || self->pending_put > 0 || self->pending_task > 0) {
        Signal_wait(&self->signal_join);
    }
    Mutex_unlock(&self->mutex);
}

void
Queue_put(Queue* self, void* item)
{
    Mutex_lock(&self->mutex);

    /* register pending put */
    self->pending_put += 1;

    /* wait for free space */
    while ( self->size >= self->capacity ) {
        Signal_wait(&self->signal_put);
    }
    /* unregister pending put */
    self->pending_put -= 1;

    self->items[self->tail] = item;
    self->tail = (self->tail + 1) % self->capacity;
    self->size += 1;

    /* signal consumer */
    Signal_signal(&self->signal_get);
    Mutex_unlock(&self->mutex);
}

void*
Queue_get(Queue* self)
{
    void* item = 0;
    Mutex_lock(&self->mutex);

    /* wait until queue becomes non empty */
    while ( self->size <= 0 ) {
        Signal_wait(&self->signal_get);
    }

    item = self->items[self->head];

    self->head = (self->head + 1) % self->capacity;
    self->size -= 1;
    self->pending_task += 1;
    /* signal producer */
    Signal_signal(&self->signal_put);

    Mutex_unlock(&self->mutex);
    return item;
}

void
Queue_taskdone(Queue* self)
{
    /* lock */
    Mutex_lock(&self->mutex);
    self->pending_task -= 1;
    /* signal joinning threads */
    Signal_broadcast(&self->signal_join);
    /* unlock */
    Mutex_unlock(&self->mutex);
}

#ifdef WIN32
#define EXIT_THREAD return 0;
unsigned __stdcall
#else
#define EXIT_THREAD return;
void
#endif
ThreadPool_thread_run(ThreadInfo* pinfo)
{
    Task task;
    ThreadInfo info = *pinfo;
    
	Thread_detach(info.thread); /* mark daemon thread */

    /* run forever */
    while ( 1 ) {
        /* get task from queue */
        Task* ptask = (Task*)Queue_get(info.queue);
        if (ptask == NULL) { /* stop thread if task is NULL */
            Queue_taskdone(info.queue);
            break;
        }
        /* clone task to local stack and free the task */
        task = *ptask;
        free(ptask);

        /* execute the task */
        task.func(task.args);

        Queue_taskdone(info.queue);
    }
    EXIT_THREAD;
}

int
ThreadPool_init(ThreadPool* self, int capacity, int numthreads)
{
    int i, err;

    /* setup queue */
    if ( Queue_init(&self->queue, capacity) ) {
        return 1;
    }

    self->threads = malloc(sizeof(Thread) * numthreads);
    self->infos = malloc(sizeof(ThreadInfo) * numthreads);
    self->numthreads = numthreads;

    /* start threads */
    for ( i = 0; i < numthreads; ++i ) {
        self->infos[i].queue = &self->queue;
        self->infos[i].thread = &self->threads[i];

        err = Thread_init(self->infos[i].thread,
                          (void(*)(void*))ThreadPool_thread_run,
                          &self->infos[i]);
        if ( err ) {
            /* threads that has started will leaked */
            Queue_close(&self->queue);
            free(self->threads);
            free(self->infos);
            return 1;
        }
    }

    return 0;
}

void
ThreadPool_enqueue(ThreadPool* self, void (*func)(void*), void* args)
{
    /* alloc task; free inside consumer */
    Task *task = malloc(sizeof(Task));
    task->func = func;
    task->args = args;
    Queue_put(&self->queue, task);
}

void
ThreadPool_join(ThreadPool* self)
{
    Queue_join(&self->queue);
}

void
ThreadPool_close(ThreadPool* self)
{
    int i;
    /* send NULL task to kill all threads */
    for ( i = 0; i < self->numthreads; ++i ) {
        Queue_put(&self->queue, NULL);
    }
    /* wait for all task to close */
    Queue_join(&self->queue);
    /* teardown */
    Queue_close(&self->queue);
    free(self->infos);
    free(self->threads);
}

