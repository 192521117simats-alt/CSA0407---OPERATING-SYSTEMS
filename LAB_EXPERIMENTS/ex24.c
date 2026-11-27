#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    const char *path = "ex24_demo.txt";
    const char message[] = "UNIX file management\n";
    char buffer[sizeof message];
    int fd = open(path, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) { perror(path); return 1; }
    int failed = 0;
    if (write(fd, message, strlen(message)) != (ssize_t)strlen(message) ||
        lseek(fd, 0, SEEK_SET) == -1) {
        fputs("Write or seek failed.\n", stderr);
        failed = 1;
    } else {
        ssize_t count = read(fd, buffer, sizeof buffer - 1);
        if (count == -1) { perror("read"); failed = 1; }
        else { buffer[count] = '\0'; printf("Read: %s", buffer); }
    }
    if (close(fd) == -1) { perror("close"); failed = 1; }
    if (unlink(path) == -1) { perror("unlink"); failed = 1; }
    else puts("Demo file closed and removed.");
    return failed;
}
