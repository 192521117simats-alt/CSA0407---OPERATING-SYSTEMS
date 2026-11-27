#ifndef ALLOCATION_LAB_H
#define ALLOCATION_LAB_H
#include <stdio.h>
enum Allocation { SEQUENTIAL, INDEXED, LINKED };

static int allocation_demo(enum Allocation kind) {
    enum { MAX_BLOCKS = 1000 };
    int disk, files, used[MAX_BLOCKS] = {0}, data[MAX_BLOCKS], next[MAX_BLOCKS];
    printf("Disk blocks (1-1000) and number of files: ");
    if (scanf("%d%d", &disk, &files) != 2 || disk < 1 ||
        disk > MAX_BLOCKS || files < 1 || files > disk) return 1;
    printf("Block numbers range from 0 to %d.\n", disk - 1);
    for (int f = 1; f <= files; ++f) {
        int count, index = -1, start = 0, chosen[MAX_BLOCKS] = {0};
        printf("File %d: enter number of data blocks: ", f);
        if (scanf("%d", &count) != 1 || count < 1 || count > disk) return 1;
        if (kind == SEQUENTIAL) {
            printf("Starting block: ");
            if (scanf("%d", &start) != 1) return 1;
            if (start < 0 || start > disk - count) {
                puts("Allocation rejected: range is outside disk.");
                continue;
            }
        }
        int valid = 1;
        if (kind == INDEXED) {
            printf("Index block: ");
            if (scanf("%d", &index) != 1) return 1;
            if (index < 0 || index >= disk || used[index]) valid = 0;
            else chosen[index] = 1;
        }
        if (kind != SEQUENTIAL) printf("Enter %d data block numbers: ", count);
        for (int j = 0; j < count; ++j) {
            int block = start + j;
            if (kind != SEQUENTIAL && scanf("%d", &block) != 1) return 1;
            data[j] = block;
            if (block < 0 || block >= disk || used[block] || chosen[block]) valid = 0;
            else chosen[block] = 1;
        }
        if (!valid) { puts("Allocation rejected: invalid, repeated or occupied block."); continue; }
        for (int j = 0; j < count; ++j) used[data[j]] = 1;
        if (kind == INDEXED) {
            used[index] = 1;
            printf("Index block %d\n", index);
            for (int j = 0; j < count; ++j) printf("  Entry %d -> block %d\n", j, data[j]);
        } else if (kind == LINKED) {
            for (int j = 0; j < count; ++j)
                next[data[j]] = j + 1 < count ? data[j + 1] : -1;
            printf("First block: %d | Last block: %d\nChain:", data[0], data[count - 1]);
            for (int block = data[0]; block != -1; block = next[block]) printf(" %d ->", block);
            puts(" END");
        } else {
            printf("Start: %d | Length: %d\nSequential read:", start, count);
            for (int j = 0; j < count; ++j) printf(" %d", data[j]);
            putchar('\n');
        }
    }
    return 0;
}
#endif
