/*
    Simple thread runtime wrapper around pykit_threads/threadpool.
    pykit.lower.lower_threads maps thread(pool)_ instructions onto the
    runtime.
*/

#include <stdlib.h>

#include "threading.h"
#include "threadpool.h"

Thread *thread_start(void *func, void *args) {
    Thread *thread = malloc(sizeof(Thread));
    if (thread && Thread_init(thread, func, args) != 0) {
        free(thread);
    }
    return thread;
}

void thread_join(Thread *thread) {
    Thread_detach(thread);
    free(thread);
}

int threadpool_start(int n) {
    ThreadPool *tp = malloc(sizeof(ThreadPool));
    if (tp && ThreadPool_init(tp, n * 5, n) != 0) {
        free(tp);
    }
    return tp;
}

void threadpool_submit(ThreadPool *threadpool, void *func, void *args) {
    ThreadPool_enqueue(threadpool, func, args);
}

void threadpool_join(ThreadPool *threadpool) {
    ThreadPool_join(threadpool);
}

void threadpool_close(ThreadPool *threadpool) {
    ThreadPool_close(threadpool);
    free(threadpool);
}