#ifndef DISK_LAB_H
#define DISK_LAB_H
#include <stdio.h>
#include <stdlib.h>
enum DiskPolicy { FCFS, SCAN, CSCAN };

static int compare_cylinder(const void *left, const void *right) {
    int a = *(const int *)left, b = *(const int *)right;
    return (a > b) - (a < b);
}
static void move_head(int *head, int target, long long *movement, const char *label) {
    long long delta = (long long)target - *head;
    *movement += delta < 0 ? -delta : delta;
    *head = target;
    printf(" -> %d%s", target, label);
}
static int disk_demo(enum DiskPolicy policy) {
    enum { MAX_REQUESTS = 200 };
    int queue[MAX_REQUESTS], cylinders, n, head, direction = 1;
    printf("Cylinder count, request count (1-200), and initial head: ");
    if (scanf("%d%d%d", &cylinders, &n, &head) != 3 || cylinders < 1 ||
        n < 1 || n > MAX_REQUESTS || head < 0 || head >= cylinders) return 1;
    printf("Enter %d requests in range 0-%d: ", n, cylinders - 1);
    for (int i = 0; i < n; ++i)
        if (scanf("%d", &queue[i]) != 1 || queue[i] < 0 || queue[i] >= cylinders)
            return 1;
    if (policy != FCFS) {
        printf("Direction (0 lower, 1 higher): ");
        if (scanf("%d", &direction) != 1 || (direction != 0 && direction != 1)) return 1;
    }
    long long movement = 0;
    printf("Head path: %d", head);
    if (policy == FCFS) {
        for (int i = 0; i < n; ++i) move_head(&head, queue[i], &movement, "");
    } else {
        qsort(queue, n, sizeof *queue, compare_cylinder);
        int lower = 0;
        while (lower < n && queue[lower] < head) ++lower;
        int upper = lower;
        while (upper < n && queue[upper] == head) {
            move_head(&head, queue[upper], &movement, "");
            ++upper;
        }
        int step = direction ? 1 : -1;
        int forward = direction ? upper : lower - 1;
        int end = direction ? n : -1;
        for (int i = forward; i != end; i += step)
            move_head(&head, queue[i], &movement, "");
        int pending = direction ? lower : n - upper;
        if (pending) {
            int edge = direction ? cylinders - 1 : 0;
            if (head != edge) move_head(&head, edge, &movement, " (end)");
            if (policy == CSCAN) {
                move_head(&head, direction ? 0 : cylinders - 1, &movement, " (wrap)");
                int restart = direction ? 0 : n - 1;
                for (int i = restart, j = 0; j < pending; i += step, ++j)
                    move_head(&head, queue[i], &movement, "");
            } else {
                int restart = direction ? lower - 1 : upper;
                for (int i = restart, j = 0; j < pending; i -= step, ++j)
                    move_head(&head, queue[i], &movement, "");
            }
        }
    }
    printf("\nTotal head movement: %lld cylinders\n", movement);
    return 0;
}
#endif
