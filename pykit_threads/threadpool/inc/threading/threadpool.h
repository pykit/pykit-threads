#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <threading/threading.h>

/* Queue */

typedef struct {
    Mutex  mutex;          /* lock for all signals and stats */
    Signal signal_put;     /* signal for producer thread */
    Signal signal_get;     /* signal for consumer thread */
    Signal signal_join;    /* signal for joining thread */
    int    capacity;       /* capacity of the queue */
    int    size;           /* # of item in the queue */
    int    head, tail;     /* head and tail of the queue */
    int    pending_put;    /* # of producer waiting for free space */
    int    pending_task;   /* # of in-process task */
    void** items;
} Queue;


/*
Init a queue with capacity.
*/
int
Queue_init(Queue* self, int capacity);

/*
Close the queue.
*/
void
Queue_close(Queue* self);

/*
Wait for all task to complete.
*/
void
Queue_join(Queue* self);

/*
Put item into the queue.
Blocking if the queue is full.
*/
void
Queue_put(Queue* self, void* item);

/*
Get item from the queue.
Must be paired with Queue_taskdone.
*/
void*
Queue_get(Queue* self);

/*
Signal completion of a task.
Must be paired with Queue_get.
*/
void
Queue_taskdone(Queue* self);

/*****************************************************************************/

/* ThreadPool */


typedef struct {
    void (*func)(void*);
    void *args;
} Task;

typedef struct {
    Thread *thread;
    Queue  *queue;
} ThreadInfo;

typedef struct {
    Queue       queue;
    Thread     *threads;
    ThreadInfo *infos;
    int         numthreads;
} ThreadPool;

/* 
Init a thread pool.
capacity: queue capacity
numthreads: number of threads to spawn.
*/
int
ThreadPool_init(ThreadPool* self, int capacity, int numthreads);

/*
Asynchronously execute task (func and args) in a worker thread.
Blocking if the queue is full.
Given though the task is placed into a queue,
there is no guarantee a earlier enqueued task is executed first.
To ensure all enqueued tasks are completed, use ThreadPool_join.
*/
void
ThreadPool_enqueue(ThreadPool* self, void (*func)(void*), void* args);

/*
Block until the thread pool is empty.
*/
void
ThreadPool_join(ThreadPool* self);

/*
Close the thread pool.
*/
void
ThreadPool_close(ThreadPool* self);


#endif /* THREADPOOL_H_ */
