#ifndef THREAD_LAB_H
#define THREAD_LAB_H
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void check_thread(int error) {
    if (error) { fprintf(stderr, "%s\n", strerror(error)); exit(EXIT_FAILURE); }
}
#ifdef USE_SEMAPHORES
#include <semaphore.h>
static inline void check_sem(int result) {
    if (result == -1) { perror("semaphore"); exit(EXIT_FAILURE); }
}
static inline void take_sem(sem_t *sem) {
    int result;
    do { result = sem_wait(sem); } while (result == -1 && errno == EINTR);
    check_sem(result);
}
#endif
#endif
