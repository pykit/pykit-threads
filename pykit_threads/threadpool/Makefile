CC = gcc
CFLAGS = -O3 -Wall -ansi -pedantic -Iinc
OBJS = src/threading.o src/threadpool.o

all: testqueue testpool

src/threading.c: inc/threading/threading.h inc/threading/thread_posix.h inc/threading/thread_windows.h
src/threadpool.c: inc/threading/threadpool.h inc/threading/threading.h

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $+

testqueue: testqueue.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $+

testpool: testpool.c  $(OBJS)
	$(CC) $(CFLAGS) -o $@ $+

clean:
	rm -f testpool testqueue libthreading
	rm -f src/*.o
