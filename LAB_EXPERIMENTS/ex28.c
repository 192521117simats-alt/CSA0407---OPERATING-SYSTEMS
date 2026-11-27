#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) { fprintf(stderr, "Usage: %s <literal-pattern> <file>\n", argv[0]); return 2; }
    FILE *file = fopen(argv[2], "r");
    if (!file) { perror(argv[2]); return 2; }
    size_t capacity = 256, length = 0;
    char *line = malloc(capacity);
    int result = 1;
    if (!line) { fclose(file); return 2; }
    for (;;) {
        int ch = fgetc(file);
        if (ch != EOF) {
            if (length == capacity - 1) {
                char *larger = capacity <= (size_t)-1 / 2 ? realloc(line, capacity * 2) : NULL;
                if (!larger) { result = 2; break; }
                line = larger;
                capacity *= 2;
            }
            line[length++] = (char)ch;
        }
        if (ch == '\n' || (ch == EOF && length)) {
            line[length] = '\0';
            if (strstr(line, argv[1])) {
                fputs(line, stdout);
                if (line[length - 1] != '\n') putchar('\n');
                result = 0;
            }
            length = 0;
        }
        if (ch == EOF) break;
    }
    if (ferror(file) || ferror(stdout)) result = 2;
    free(line);
    if (fclose(file)) result = 2;
    return result;
}
