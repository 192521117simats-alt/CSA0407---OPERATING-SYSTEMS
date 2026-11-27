#define _POSIX_C_SOURCE 200809L
#include <grp.h>
#include <inttypes.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static int show_permissions(int fd) {
    struct stat info;
    if (fstat(fd, &info) == -1) { perror("fstat"); return 1; }
    const mode_t bits[] = { S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                            S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH };
    const char symbols[] = "rwx";
    char permissions[10];
    for (int i = 0; i < 9; ++i)
        permissions[i] = info.st_mode & bits[i] ? symbols[i % 3] : '-';
    permissions[9] = '\0';
    struct passwd *owner = getpwuid(info.st_uid);
    struct group *group = getgrgid(info.st_gid);
    printf("Mode %04o | owner %.3s | group %.3s | others %.3s\n",
           (unsigned)(info.st_mode & 0777), permissions, permissions + 3, permissions + 6);
    printf("Owner: %s (%ju) | Group: %s (%ju)\n",
           owner ? owner->pw_name : "unknown", (uintmax_t)info.st_uid,
           group ? group->gr_name : "unknown", (uintmax_t)info.st_gid);
    return 0;
}
int main(void) {
    char path[] = "/tmp/oslab_ex40_XXXXXX";
    int fd = mkstemp(path);
    if (fd == -1) { perror("mkstemp"); return 1; }
    printf("Effective UID: %ju (%s)\n", (uintmax_t)geteuid(),
           geteuid() == 0 ? "root" : "non-root account");
    puts("r = read, w = write, x = execute. Owner, group and others have separate bits.");
    puts("Initial permissions:");
    int failed = show_permissions(fd);
    if (fchmod(fd, 0754) == -1) { perror("fchmod"); failed = 1; }
    else { puts("After setting mode 0754:"); failed |= show_permissions(fd); }
    if (close(fd) == -1) { perror("close"); failed = 1; }
    if (unlink(path) == -1) { perror("unlink"); failed = 1; }
    return failed;
}
