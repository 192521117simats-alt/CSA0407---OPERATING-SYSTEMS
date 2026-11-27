#define USE_SEMAPHORES
#include "thread_lab.h"
static sem_t count_lock, resource;
static int readers, value;

static void *read_value(void *arg) {
    int id = *(int *)arg;
    take_sem(&count_lock);
    if (++readers == 1) take_sem(&resource);
    check_sem(sem_post(&count_lock));

    printf("Reader %d read %d\n", id, value);

    take_sem(&count_lock);
    if (--readers == 0) check_sem(sem_post(&resource));
    check_sem(sem_post(&count_lock));
    return NULL;
}
static void *write_value(void *arg) {
    int id = *(int *)arg;
    take_sem(&resource);
    printf("Writer %d wrote %d\n", id, ++value);
    check_sem(sem_post(&resource));
    return NULL;
}
int main(void) {
    pthread_t threads[5];
    int ids[] = {1, 1, 2, 2, 3};
    check_sem(sem_init(&count_lock, 0, 1));
    check_sem(sem_init(&resource, 0, 1));
    for (int i = 0; i < 5; ++i)
        check_thread(pthread_create(&threads[i], NULL,
                     i % 2 ? write_value : read_value, &ids[i]));
    for (int i = 0; i < 5; ++i) check_thread(pthread_join(threads[i], NULL));
    check_sem(sem_destroy(&resource));
    check_sem(sem_destroy(&count_lock));
    return 0;
}
