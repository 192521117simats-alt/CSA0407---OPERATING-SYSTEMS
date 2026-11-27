#ifndef MEMORY_FIT_H
#define MEMORY_FIT_H
#include <stdio.h>
#include <string.h>
enum Fit { FIRST = 1, BEST, WORST };
enum { LIMIT = 100 };

static int read_sizes(const char *label, int sizes[]) {
    int count;
    printf("Number of %s (1-100): ", label);
    if (scanf("%d", &count) != 1 || count < 1 || count > LIMIT) return 0;
    printf("Enter %d positive sizes: ", count);
    for (int i = 0; i < count; ++i)
        if (scanf("%d", &sizes[i]) != 1 || sizes[i] <= 0) return 0;
    return count;
}
static int memory_demo(enum Fit fit) {
    int blocks[LIMIT], processes[LIMIT], remaining[LIMIT];
    int n = read_sizes("blocks", blocks);
    int m = n ? read_sizes("processes", processes) : 0;
    if (!m) { fputs("Invalid sizes.\n", stderr); return 1; }
    for (int policy = FIRST; policy <= WORST; ++policy) {
        if (fit && policy != (int)fit) continue;
        memcpy(remaining, blocks, (size_t)n * sizeof *blocks);
        const char *names[] = {"First Fit", "Best Fit", "Worst Fit"};
        printf("\n%s\nProcess\tSize\tBlock\n", names[policy - 1]);
        for (int i = 0; i < m; ++i) {
            int chosen = -1;
            for (int j = 0; j < n; ++j) {
                if (remaining[j] < processes[i]) continue;
                if (chosen < 0 ||
                    (policy == BEST && remaining[j] < remaining[chosen]) ||
                    (policy == WORST && remaining[j] > remaining[chosen]))
                    chosen = j;
                if (policy == FIRST) break;
            }
            printf("%d\t%d\t", i + 1, processes[i]);
            if (chosen < 0) puts("Not allocated");
            else {
                remaining[chosen] -= processes[i];
                printf("%d\n", chosen + 1);
            }
        }
        printf("Free space by block:");
        for (int j = 0; j < n; ++j) printf(" %d", remaining[j]);
        putchar('\n');
    }
    return 0;
}
#endif
