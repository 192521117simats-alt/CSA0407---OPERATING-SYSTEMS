#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    const char *path = argc > 1 ? argv[1] : "ex25.c";
    const char *directory = argc > 2 ? argv[2] : ".";
    int fd = open(path, O_RDONLY);
    if (fd == -1) { perror(path); return 1; }
    int flags = fcntl(fd, F_GETFL);
    off_t end = lseek(fd, 0, SEEK_END);
    struct stat info;
    int status = stat(path, &info);
    int closed = close(fd);
    if (flags == -1 || end == (off_t)-1 || status == -1 || closed == -1) {
        fputs("fcntl, lseek, stat or close failed.\n", stderr);
        return 1;
    }
    printf("fcntl flags: %#x\nlseek end: %jd\nstat size: %jd bytes\n",
           (unsigned)flags, (intmax_t)end, (intmax_t)info.st_size);
    DIR *dir = opendir(directory);
    if (!dir) { perror(directory); return 1; }
    int failed = 0;
    for (;;) {
        errno = 0;
        struct dirent *entry = readdir(dir);
        if (!entry) {
            if (errno) { perror("readdir"); failed = 1; }
            break;
        }
        puts(entry->d_name);
    }
    if (closedir(dir) == -1) { perror("closedir"); failed = 1; }
    return failed;
}
