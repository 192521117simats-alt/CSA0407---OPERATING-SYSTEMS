#ifndef PAGING_LAB_H
#define PAGING_LAB_H
#include <stdio.h>
enum PagePolicy { FIFO, LRU, OPTIMAL };

static int paging_demo(enum PagePolicy policy) {
    enum { MAX_FRAMES = 32, MAX_REFERENCES = 200 };
    int pages[MAX_REFERENCES], frames[MAX_FRAMES], last_use[MAX_FRAMES];
    int n, capacity, used = 0, next = 0, faults = 0;
    printf("Frame count (1-32) and reference count (1-200): ");
    if (scanf("%d%d", &capacity, &n) != 2 || capacity < 1 ||
        capacity > MAX_FRAMES || n < 1 || n > MAX_REFERENCES) return 1;
    printf("Enter the page reference string: ");
    for (int i = 0; i < n; ++i)
        if (scanf("%d", &pages[i]) != 1 || pages[i] < 0) return 1;

    for (int i = 0; i < n; ++i) {
        int slot = 0;
        while (slot < used && frames[slot] != pages[i]) ++slot;
        int hit = slot < used;
        if (!hit) {
            ++faults;
            if (used < capacity) slot = used++;
            else if (policy == FIFO) {
                slot = next;
                next = (next + 1) % capacity;
            } else if (policy == LRU) {
                slot = 0;
                for (int j = 1; j < capacity; ++j)
                    if (last_use[j] < last_use[slot]) slot = j;
            } else {
                int farthest = -1;
                for (int j = 0; j < capacity; ++j) {
                    int future = i + 1;
                    while (future < n && pages[future] != frames[j]) ++future;
                    if (future > farthest) { farthest = future; slot = j; }
                }
            }
            frames[slot] = pages[i];
        }
        last_use[slot] = i;
        printf("%d |", pages[i]);
        for (int j = 0; j < capacity; ++j) {
            if (j < used) printf(" %d", frames[j]);
            else printf(" -");
        }
        printf(" | %s\n", hit ? "hit" : "fault");
    }
    printf("Page faults: %d\nPage hits: %d\n", faults, n - faults);
    return 0;
}
#endif
