#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_framework.h"

// ================== �����㷨��װ���� ==================
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

// ==================�㷨�б� ==================
const SortAlgorithm algorithms[] = {
    {1, "��������", insertSort},
    {2, "�鲢����", MergeSortWrapper},
    {3, "��������(�ݹ顪���)", QuickSortRecursionWrapper}, 
    {4, "��������(�ݹ顪����ȡ��)", QuickSortMedianWrapper},
    {5, "��������(�ǵݹ�)", QuickSort},
    {6, "��������", CountSortWrapper},
    {7, "������������", RadixCountSort},
};
const int ALGO_COUNT = sizeof(algorithms) / sizeof(algorithms[0]);

// ================== ��������ʵ�� ==================
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

// ================== ���Ժ���ʵ�� ==================
void runLargeDataTest(int size, SortAlgorithm* selected, int count) {
    char filename[50];
    sprintf(filename, "large_data_%d.txt", size);
    GenerateData(size, filename);

    int actualSize;
    int* data = LoadData(filename, &actualSize);
    if (!data || actualSize != size) {
        printf("���ݼ���ʧ��: %s\n", filename);
        return;
    }

    printf("\n=== ���Խ�� (��������%d) ===\n", size);
    for (int i = 0; i < count; i++) {
        int* arr = copyArray(data, size);
        clock_t start = clock();
        selected[i].func(arr, size);
        clock_t end = clock();
        printf("%-20s: %.3fs | %s\n",
            selected[i].name,
            (double)(end - start) / CLOCKS_PER_SEC,
            isSorted(arr, size) ? "��Ч" : "��Ч");
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
        printf("���ݼ���ʧ��: %s\n", filename);
        return;
    }

    printf("\n=== ���Խ�� (��������%d*%d) ===\n", TEST_SIZE, ITERATIONS);
    for (int i = 0; i < count; i++) {
        clock_t total = 0;
        for (int j = 0; j < ITERATIONS; j++) {
            int* arr = copyArray(data, TEST_SIZE);
            clock_t start = clock();
            selected[i].func(arr, TEST_SIZE);
            total += clock() - start;
            free(arr);
        }
        printf("%-20s: %.3fs | ��Ч\n",
            selected[i].name,
            (double)total / CLOCKS_PER_SEC);
    }
    free(data);
}
