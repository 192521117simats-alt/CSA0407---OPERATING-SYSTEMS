#include "thread_lab.h"
enum { THREADS = 4, INCREMENTS = 10000 };
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int counter;

static void *increment(void *arg) {
    (void)arg;
    for (int i = 0; i < INCREMENTS; ++i) {
        check_thread(pthread_mutex_lock(&lock));
        ++counter;
        check_thread(pthread_mutex_unlock(&lock));
    }
    return NULL;
}
int main(void) {
    pthread_t threads[THREADS];
    for (int i = 0; i < THREADS; ++i)
        check_thread(pthread_create(&threads[i], NULL, increment, NULL));
    for (int i = 0; i < THREADS; ++i)
        check_thread(pthread_join(threads[i], NULL));
    printf("Final counter: %d (expected %d)\n", counter, THREADS * INCREMENTS);
    check_thread(pthread_mutex_destroy(&lock));
    return counter != THREADS * INCREMENTS;
}
