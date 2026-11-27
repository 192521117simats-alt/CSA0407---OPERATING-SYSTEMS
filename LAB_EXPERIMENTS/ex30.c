#include "thread_lab.h"

struct Context { pthread_t main_id; int result; };
static void *worker(void *arg) {
    struct Context *context = arg;
    pthread_t self = pthread_self();
    printf("pthread_equal(worker, worker): %s\n", pthread_equal(self, self) ? "true" : "false");
    printf("pthread_equal(worker, main): %s\n",
           pthread_equal(self, context->main_id) ? "true" : "false");
    context->result = 42;
    puts("Worker calls pthread_exit.");
    pthread_exit(&context->result);
    return NULL;
}
int main(void) {
    struct Context context = {.main_id = pthread_self()};
    pthread_t thread;
    void *result;
    check_thread(pthread_create(&thread, NULL, worker, &context));
    check_thread(pthread_join(thread, &result));
    printf("pthread_join returned worker result: %d\n", *(int *)result);
    return 0;
}
