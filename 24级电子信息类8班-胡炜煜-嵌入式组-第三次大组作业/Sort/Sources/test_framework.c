#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_framework.h"

// ================== 排序算法包装函数 ==================
void MergeSortWrapper(int* a, int n) {
    int* temp = (int*)malloc(n * sizeof(int));
    if (temp) {
        MergeSort(a, 0, n - 1, temp);
        free(temp);
    }
}

void QuickSortRecursionWrapper(int* a, int n) {
    QuickSort_Recursion(a, 0, n - 1);
}
void QuickSortMedian(int* a, int begin, int end) {
    if (begin >= end) return;
    int pivot_pos = Partition(a, begin, end);
    QuickSortMedian(a, begin, pivot_pos - 1);
    QuickSortMedian(a, pivot_pos + 1, end);
}

void QuickSortMedianWrapper(int* a, int n) {
    QuickSortMedian(a, 0, n - 1);
}

void CountSortWrapper(int* a, int size) {
    if (size <= 0) return;
    int max = a[0];
    for (int i = 1; i < size; i++)
        if (a[i] > max) max = a[i];
    CountSort(a, size, max);
}

// ==================算法列表 ==================
const SortAlgorithm algorithms[] = {
    {1, "插入排序", insertSort},
    {2, "归并排序", MergeSortWrapper},
    {3, "快速排序(递归—随机)", QuickSortRecursionWrapper}, 
    {4, "快速排序(递归—三数取中)", QuickSortMedianWrapper},
    {5, "快速排序(非递归)", QuickSort},
    {6, "计数排序", CountSortWrapper},
    {7, "基数计数排序", RadixCountSort},
};
const int ALGO_COUNT = sizeof(algorithms) / sizeof(algorithms[0]);

// ================== 辅助函数实现 ==================
int* copyArray(const int* src, int size) {
    int* dest = (int*)malloc(size * sizeof(int));
    if (dest) memcpy(dest, src, size * sizeof(int));
    return dest;
}

int isSorted(const int* a, int size) {
    for (int i = 1; i < size; i++)
        if (a[i] < a[i - 1]) return 0;
    return 1;
}

// ================== 测试函数实现 ==================
void runLargeDataTest(int size, SortAlgorithm* selected, int count) {
    char filename[50];
    sprintf(filename, "large_data_%d.txt", size);
    GenerateData(size, filename);

    int actualSize;
    int* data = LoadData(filename, &actualSize);
    if (!data || actualSize != size) {
        printf("数据加载失败: %s\n", filename);
        return;
    }

    printf("\n=== 测试结果 (数据量：%d) ===\n", size);
    for (int i = 0; i < count; i++) {
        int* arr = copyArray(data, size);
        clock_t start = clock();
        selected[i].func(arr, size);
        clock_t end = clock();
        printf("%-20s: %.3fs | %s\n",
            selected[i].name,
            (double)(end - start) / CLOCKS_PER_SEC,
            isSorted(arr, size) ? "有效" : "无效");
        free(arr);
    }
    free(data);
}

void runSmallDataTest(SortAlgorithm* selected, int count) {
    const int TEST_SIZE = 100;
    const int ITERATIONS = 100000;
    char filename[] = "small_data.txt";
    GenerateData(TEST_SIZE, filename);

    int actualSize;
    int* data = LoadData(filename, &actualSize);
    if (!data || actualSize != TEST_SIZE) {
        printf("数据加载失败: %s\n", filename);
        return;
    }

    printf("\n=== 测试结果 (数据量：%d*%d) ===\n", TEST_SIZE, ITERATIONS);
    for (int i = 0; i < count; i++) {
        clock_t total = 0;
        for (int j = 0; j < ITERATIONS; j++) {
            int* arr = copyArray(data, TEST_SIZE);
            clock_t start = clock();
            selected[i].func(arr, TEST_SIZE);
            total += clock() - start;
            free(arr);
        }
        printf("%-20s: %.3fs | 有效\n",
            selected[i].name,
            (double)total / CLOCKS_PER_SEC);
    }
    free(data);
}
