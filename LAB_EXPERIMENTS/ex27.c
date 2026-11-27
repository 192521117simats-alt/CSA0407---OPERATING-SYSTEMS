#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    const char *path = argc > 1 ? argv[1] : ".";
    DIR *dir = opendir(path);
    if (!dir) { perror(path); return 1; }
    int failed = 0;
    for (;;) {
        errno = 0;
        struct dirent *entry = readdir(dir);
        if (!entry) {
            if (errno) { perror("readdir"); failed = 1; }
            break;
        }
        if (entry->d_name[0] != '.') puts(entry->d_name);
    }
    if (closedir(dir)) { perror("closedir"); failed = 1; }
    return failed;
}
