#ifndef THREAD_POSIX_H_
#define THREAD_POSIX_H_

#include <pthread.h>

typedef struct {
    pthread_t thread_id;
} Thread;

typedef struct {
    pthread_mutex_t mutex;
} Mutex;

typedef struct {
    pthread_cond_t cond;
    Mutex *mutex;
} Signal;

#endif /* THREAD_POSIX_H_ */
