cdef extern from "threading/threading.h":
    ctypedef struct Thread:
        pass

    int Thread_init(Thread* self, void (*target)(void*), void* args)
    void Thread_detach(Thread* self)


cdef extern from "threading/threadpool.h":
    ctypedef struct ThreadPool:
        pass

    int ThreadPool_init(ThreadPool *, int capacity, int numthreads)
    void ThreadPool_enqueue(ThreadPool* self, void (*func)(void*), void* args)
    void ThreadPool_join(ThreadPool* self)
    void ThreadPool_close(ThreadPool* self)
