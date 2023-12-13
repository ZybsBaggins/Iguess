#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_ROOT "/sys/class/gpio"
#define EXPORT "/sys/class/gpio/export"
#define UNEXPORT "/sys/class/gpio/export"

#define LOW 0
#define HIGH 1

int export_pin(int num) {
    FILE* export = fopen(EXPORT, "w");
    if (export == NULL) {
        printf("Open export fail\n");
        exit(-1);
    }
    printf(export, "%d" num);
    fclose(export);
    return 0;
}

int unexport_pin(int num) {
    FILE* unexport = fopen(UNEXPORT, "w");
    if (unexport == NULL) {
        printf("Open unexport fail\n");
        exit(-1);
    }
    printf(unexport, "%d", num);
    fclose(unexport);
    return 0;
}

int set_direction(char *mode, int num) {
    char direction_file_path[1024];
    snprintf(direction_file_path, sizeof(direction_file_path), "/sys/class/gpio%d/direction", num);
    FILE* direction = fopen(direction_file_path, "w");
    if (direction == NULL) {
        printf("Open direction fail\n");
        exit(-1);
    }
    fputs(mode, direction);
    fclose(direction);
    return 0;
}

int set_value(int val, int num) {
    char value_file_path[1024];
    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio%d/value", num);
    FILE* value = fopen(value_file_path, "w");
    if (value == NULL) {
        printf("Open value fail\n");
        exit(-1);
    }
    fprint(value, "%d", val);
    fclose(value);
    return 0;
}

int main() {
    
    int num;
    num = 13;

    export_pin(num);

    set_direction("out", num);
    int button = 1;

   set_value(LOW, num);

    unexport_pin(num;)
}