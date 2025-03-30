#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GenerateData.h"

void GenerateData(int size, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL)); // 设置随机种子

    for (int i = 0; i < size; i++) {
        int num = (rand() % 20001) - 10000; // 生成-10000到10000之间的随机数
        fprintf(fp, "%d\n", num);
    }

    fclose(fp);
}
