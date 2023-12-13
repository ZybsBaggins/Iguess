#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_ROOT "/sys/class/gpio"
#define EXPORT GPIO_ROOT "/export"
#define UNEXPORT GPIO_ROOT "/unexport"

#define LOW 0
#define HIGH 1

int export_pin(int num) {
    FILE* export = fopen(EXPORT, "w");
    if (export == NULL) {
        printf("Open export fail\n");
        exit(-1);
    }
    fprintf(export, "%d", num);
    fclose(export);
    return 0;
}

int unexport_pin(int num) {
    FILE* unexport = fopen(UNEXPORT, "w");
    if (unexport == NULL) {
        printf("Open unexport fail\n");
        exit(-1);
    }
    fprintf(unexport, "%d", num);
    fclose(unexport);
    return 0;
}

int set_direction(char *mode, int num) {
    char direction_file_path[1024];
    snprintf(direction_file_path, sizeof(direction_file_path), "/sys/class/gpio/gpio%d/direction", num);
    FILE* direction = fopen(direction_file_path, "w");
    if (direction == NULL) {
        printf("Open direction fail\n");
        exit(-1);
    }
    fprintf(direction, "%s", mode);
    fclose(direction);
    return 0;
}

int set_value(int val, int num) {
    char value_file_path[1024];
    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", num);
    FILE* value = fopen(value_file_path, "w");
    if (value == NULL) {
        printf("Open value fail\n");
        exit(-1);
    }
    fprintf(value, "%d", val);
    fclose(value);
    return 0;
}

int main() {
    
    int num;
    num = 13;

    export_pin(num);

    set_direction("out", num); // Change direction to "out"
    int button = 1;

    set_value(HIGH, num);

    unexport_pin(num);

    return 0;
}
