#ifndef DIRECTORY_LAB_H
#define DIRECTORY_LAB_H
#include <stdio.h>
#include <string.h>
enum { DIR_MAX = 16, FILE_MAX = 32, NAME_MAX_LEN = 31 };
struct Directory {
    char name[NAME_MAX_LEN + 1];
    char files[FILE_MAX][NAME_MAX_LEN + 1];
    int count;
};

static int directory_demo(int two_levels) {
    struct Directory dirs[DIR_MAX] = {{.name = "root"}};
    int count = two_levels ? 0 : 1;
    for (;;) {
        puts("\n0 Exit | 1 Add file | 2 Find file | 3 Remove file | 4 List");
        if (two_levels) puts("5 Add user directory");
        int action;
        if (scanf("%d", &action) != 1) return feof(stdin) ? 0 : 1;
        if (!action) return 0;
        if (action == 4) {
            for (int i = 0; i < count; ++i) {
                printf("%s:", dirs[i].name);
                for (int j = 0; j < dirs[i].count; ++j)
                    printf(" %s", dirs[i].files[j]);
                putchar('\n');
            }
            continue;
        }
        if (action < 1 || action > 5 || (action == 5 && !two_levels)) {
            puts("Invalid choice.");
            continue;
        }
        int index = 0;
        if (two_levels) {
            char user[NAME_MAX_LEN + 1];
            printf("User directory: ");
            if (scanf("%31s", user) != 1) return 1;
            while (index < count && strcmp(dirs[index].name, user)) ++index;
            if (action == 5) {
                if (index < count) puts("Directory already exists.");
                else if (count == DIR_MAX) puts("Directory table is full.");
                else strcpy(dirs[count++].name, user);
                continue;
            }
            if (index == count) { puts("Directory not found."); continue; }
        }
        struct Directory *dir = &dirs[index];
        char file[NAME_MAX_LEN + 1];
        printf("File name: ");
        if (scanf("%31s", file) != 1) return 1;
        int i = 0;
        while (i < dir->count && strcmp(dir->files[i], file)) ++i;
        if (action == 1) {
            if (i < dir->count) puts("File already exists.");
            else if (dir->count == FILE_MAX) puts("Directory is full.");
            else { strcpy(dir->files[dir->count++], file); puts("File added."); }
        } else if (i == dir->count) puts("File not found.");
        else if (action == 2) printf("Found %s/%s\n", dir->name, file);
        else {
            --dir->count;
            if (i != dir->count) strcpy(dir->files[i], dir->files[dir->count]);
            puts("File removed.");
        }
    }
}
#endif
