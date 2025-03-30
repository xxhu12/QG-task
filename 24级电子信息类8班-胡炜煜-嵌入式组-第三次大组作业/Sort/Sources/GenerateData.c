#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "DataLoader.h"

int* LoadData(const char* filename, int* size) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Failed to open file");
        *size = 0;
        return NULL;
    }

    int capacity = 1024;
    int* data = (int*)malloc(capacity * sizeof(int));
    if (data == NULL) {
        fclose(fp);
        *size = 0;
        return NULL;
    }

    int count = 0;
    int num;
    while (fscanf(fp, "%d", &num) == 1) {
        if (count >= capacity) {
            capacity *= 2;
            int* temp = (int*)realloc(data, capacity * sizeof(int));
            if (!temp) {
                free(data);
                fclose(fp);
                *size = 0;
                return NULL;
            }
            data = temp;
        }
        data[count++] = num;
    }

    fclose(fp);

    // 调整数组到实际大小
    int* result = (int*)realloc(data, count * sizeof(int));
    if (!result) {
        free(data);
        *size = 0;
        return NULL;
    }

    *size = count;
    return result;
}
