#include <stdio.h>

int main(void) {
    enum { CAPACITY = 10 };
    int queue[CAPACITY], head = 0, tail = 0, full = 0, next_item = 1;
    for (;;) {
        int action;
        puts("1 Produce | 2 Consume | 3 Exit");
        if (scanf("%d", &action) != 1) return feof(stdin) ? 0 : 1;
        if (action == 3) return 0;
        if (action == 1) {
            if (full == CAPACITY) { puts("Producer waits: buffer full."); continue; }
            queue[tail] = next_item++;
            printf("Produced %d\n", queue[tail]);
            tail = (tail + 1) % CAPACITY;
            ++full;
        } else if (action == 2) {
            if (!full) { puts("Consumer waits: buffer empty."); continue; }
            printf("Consumed %d\n", queue[head]);
            head = (head + 1) % CAPACITY;
            --full;
        } else { puts("Invalid choice."); continue; }
        printf("Full slots: %d | Empty slots: %d\n", full, CAPACITY - full);
    }
}
