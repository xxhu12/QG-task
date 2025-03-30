#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include "sort.h"
#include "GenerateData.h"
#include "DataLoader.h"

typedef struct {
    int id;
    const char* name;
    void (*func)(int*, int);
} SortAlgorithm;

extern const SortAlgorithm algorithms[];
extern const int ALGO_COUNT;

// 通用辅助函数
int* copyArray(const int* src, int size);
int isSorted(const int* a, int size);

// 测试函数
void runLargeDataTest(int size, SortAlgorithm* selected, int count);
void runSmallDataTest(SortAlgorithm* selected, int count);

#endif

