#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct Employee { int id; char name[64]; double salary; };

int main(int argc, char *argv[]) {
    const char *path = argc > 1 ? argv[1] : "ex16_employees.dat";
    FILE *file = fopen(path, "r+b");
    if (!file && errno == ENOENT) file = fopen(path, "w+b");
    if (!file) { perror(path); return 1; }
    int result = 0;
    for (;;) {
        int action, id;
        puts("0 Exit | 1 Add | 2 Display | 3 Update | 4 Delete");
        if (scanf("%d", &action) != 1 || !action) break;
        if (action < 1 || action > 4) { puts("Invalid choice."); continue; }
        printf("Employee ID (1-100000): ");
        if (scanf("%d", &id) != 1 || id < 1 || id > 100000) { result = 1; break; }
        long offset = (long)(id - 1) * (long)sizeof(struct Employee);
        struct Employee employee = {0};
        if (fseek(file, offset, SEEK_SET)) { result = 1; break; }
        int exists = fread(&employee, sizeof employee, 1, file) == 1 && employee.id == id;
        if (ferror(file)) { result = 1; break; }
        if (action == 1 && exists) { puts("Employee already exists."); continue; }
        if (action != 1 && !exists) { puts("Employee not found."); continue; }
        if (action == 2) {
            printf("%d %s %.2f\n", employee.id, employee.name, employee.salary);
            continue;
        }
        employee = (struct Employee){.id = action == 4 ? 0 : id};
        if (action != 4) {
            printf("Enter name and salary on separate lines: ");
            if (scanf(" %63[^\n]", employee.name) != 1 ||
                scanf("%lf", &employee.salary) != 1 || employee.salary < 0) {
                result = 1;
                break;
            }
        }
        if (fseek(file, offset, SEEK_SET) ||
            fwrite(&employee, sizeof employee, 1, file) != 1 || fflush(file)) {
            result = 1;
            break;
        }
        puts(action == 4 ? "Employee deleted." : "Employee saved.");
    }
    if (fclose(file)) result = 1;
    if (result) fputs("Employee operation failed.\n", stderr);
    return result;
}
