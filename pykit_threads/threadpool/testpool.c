#include <stdio.h>
#include <threading/threadpool.h>

void eat(void* args) {
    printf("eat %s\n", (char*)args);
}

int main()
{
    int i, t;
    ThreadPool pool;

    char *fruits[] = {
        "apple", "orange", "lemon", "mango",
        "berry", "banana", "lime"
    };

    const int count = sizeof(fruits) / sizeof(char*);

    ThreadPool_init(&pool, 5, 4);

	for ( t = 0; t < 5; ++ t ){
        for ( i = 0; i < count; ++i ){
            printf("enqueue %d: %s\n", i, fruits[i]);
            ThreadPool_enqueue(&pool, eat, fruits[i]);
        }
        puts("joining");
        ThreadPool_join(&pool);
    
        
    }
    puts("closing");
    ThreadPool_close(&pool);
    puts("ok");
    return 0;
}
