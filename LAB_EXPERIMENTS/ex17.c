#include <stdio.h>
enum { MAX = 20 };
int main(void) {
    int n, m, allocation[MAX][MAX], need[MAX][MAX], done[MAX] = {0}, order[MAX];
    long long work[MAX];
    printf("Number of processes and resource types (1-20 each): ");
    if (scanf("%d%d", &n, &m) != 2 || n < 1 || n > MAX || m < 1 || m > MAX)
        return 1;
    puts("Enter Allocation matrix:");
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (scanf("%d", &allocation[i][j]) != 1 || allocation[i][j] < 0) return 1;
    puts("Enter Maximum matrix:");
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            int maximum;
            if (scanf("%d", &maximum) != 1 || maximum < allocation[i][j]) return 1;
            need[i][j] = maximum - allocation[i][j];
        }
    puts("Enter Available vector:");
    for (int j = 0; j < m; ++j)
        if (scanf("%lld", &work[j]) != 1 || work[j] < 0 || work[j] > 2147483647LL)
            return 1;
    int completed = 0;
    while (completed < n) {
        int before = completed;
        for (int i = 0; i < n; ++i) {
            if (done[i]) continue;
            int j = 0;
            while (j < m && need[i][j] <= work[j]) ++j;
            if (j != m) continue;
            for (j = 0; j < m; ++j) work[j] += allocation[i][j];
            done[i] = 1;
            order[completed++] = i;
        }
        if (before == completed) { puts("Unsafe state. No safe sequence."); return 0; }
    }
    printf("Safe sequence:");
    for (int i = 0; i < n; ++i) printf(" P%d", order[i]);
    putchar('\n');
    return 0;
}
