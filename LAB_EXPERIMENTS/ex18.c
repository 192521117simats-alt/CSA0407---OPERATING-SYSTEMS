#define USE_SEMAPHORES
#include "thread_lab.h"
enum { CAPACITY = 3, ITEMS = 10 };
static int buffer[CAPACITY];
static sem_t empty, full;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void *produce(void *arg) {
    (void)arg;
    for (int i = 0; i < ITEMS; ++i) {
        take_sem(&empty);
        check_thread(pthread_mutex_lock(&lock));
        buffer[i % CAPACITY] = i + 1;
        printf("Produced %d\n", i + 1);
        check_thread(pthread_mutex_unlock(&lock));
        check_sem(sem_post(&full));
    }
    return NULL;
}
static void *consume(void *arg) {
    (void)arg;
    for (int i = 0; i < ITEMS; ++i) {
        take_sem(&full);
        check_thread(pthread_mutex_lock(&lock));
        printf("Consumed %d\n", buffer[i % CAPACITY]);
        check_thread(pthread_mutex_unlock(&lock));
        check_sem(sem_post(&empty));
    }
    return NULL;
}
int main(void) {
    pthread_t producer, consumer;
    check_sem(sem_init(&empty, 0, CAPACITY));
    check_sem(sem_init(&full, 0, 0));
    check_thread(pthread_create(&producer, NULL, produce, NULL));
    check_thread(pthread_create(&consumer, NULL, consume, NULL));
    check_thread(pthread_join(producer, NULL));
    check_thread(pthread_join(consumer, NULL));
    check_sem(sem_destroy(&empty));
    check_sem(sem_destroy(&full));
    check_thread(pthread_mutex_destroy(&lock));
    return 0;
}
