#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc > 2) { fprintf(stderr, "Usage: %s [new-file]\n", argv[0]); return 1; }
    const char *path = argc == 2 ? argv[1] : "ex26_demo.txt";
    FILE *file = fopen(path, "wbx");
    if (!file) { perror(path); return 1; }
    int failed = fputs("File created.\n", file) == EOF;
    if (fclose(file)) failed = 1;

    if (!failed && (file = fopen(path, "ab")) != NULL) {
        failed = fputs("Data appended.\n", file) == EOF;
        if (fclose(file)) failed = 1;
    } else failed = 1;

    if (!failed && (file = fopen(path, "rb")) != NULL) {
        for (int ch; (ch = fgetc(file)) != EOF;) putchar(ch);
        failed = ferror(file);
        if (fclose(file)) failed = 1;
    } else failed = 1;
    if (remove(path)) { perror("remove"); return 1; }
    puts("Demo file deleted.");
    return failed != 0;
}
