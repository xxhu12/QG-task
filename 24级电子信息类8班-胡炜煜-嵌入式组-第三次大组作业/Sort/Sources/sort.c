#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

// ����������ֵ
#define INSERTION_THRESHOLD 15

// ================================================================================================

// ��������
void insertSort(int* a, int n) 
{
    for (int i = 1; i < n; i++) //����δ����Ԫ��
    {
        int key = a[i];//��ǰ������Ԫ�ص��±�
        if (key >= a[i - 1]) //��ǰ�ж��Ƿ���Ҫ����,���Ѿ������������ĩβ��������������������ٲ���Ҫ���ƶ�
        {
            continue;
        }
        int j; 
        for (j = i - 1; j >= 0 && a[j] > key; j--) //���� key ���Ԫ�غ��ƣ��ڳ�����λ��
        {
            a[j + 1] = a[j];
        }
        a[j + 1] = key; //����
    }
}

// ================================================================================================

// �鲢���򣨺ϲ��������������� a[begin..mid] �� a[mid+1..end]��
void MergeArray(int* a, int begin, int mid, int end, int* temp) 
{
    int i = begin, j = mid + 1, k = 0;
    while (i <= mid && j <= end) 
    {
        temp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];//�Ƚ�Ԫ�ز�������ʱ���� temp
    }
    // ʣ��Ԫ�ؿ�����temp
    if (i <= mid) memcpy(temp + k, a + i, (mid - i + 1) * sizeof(int));
    if (j <= end) memcpy(temp + k, a + j, (end - j + 1) * sizeof(int));
    //���ƻ�ԭ����
    int total = k + (mid - i + 1) + (end - j + 1);
    memcpy(a + begin, temp, total * sizeof(int));
}

// ================================================================================================

//�鲢����
void MergeSort(int* a, int begin, int end, int* temp) 
{
    if (begin < end)//ֻ�е������������� 2 ��Ԫ��ʱ���Ž���������� 
    {
        if (end - begin + 1 <= INSERTION_THRESHOLD) // С����ʹ�ò�������
        {
            insertSort(a + begin, end - begin + 1); 
            return;
        }
        int mid = begin + (end - begin) / 2;//�������
        //�ݹ�����������
        MergeSort(a, begin, mid, temp);
        MergeSort(a, mid + 1, end, temp);
        if (a[mid] > a[mid + 1]) //�����ϲ�
        {
            MergeArray(a, begin, mid, end, temp);
        }
    }
}

// ================================================================================================

//�������򣨵ݹ�棩������Ż���
void QuickSort_Recursion(int* a, int begin, int end) 
{
    if (begin >= end) return;//�ݹ���ֹ����

    // ================== �����׼�Ż� ==================
    //���� [begin, end] ��Χ�ڵ��������
    int random_index = begin + rand() % (end - begin + 1);

    //�����Ԫ�ؽ�����ĩβλ��
    int temp = a[end];
    a[end] = a[random_index];
    a[random_index] = temp;

    // ���������ĩβԪ�� a[end] ��Ϊ��׼ֵ pivot
    int pivot = a[end]; 
    // ================================================

    int i = begin - 1;//���������ݻ�׼ֵ pivot��������
    for (int j = begin; j < end; j++) 
    {
        if (a[j] <= pivot)
        {
            i++;
            int swap_tmp = a[i];
            a[i] = a[j];
            a[j] = swap_tmp;
        }
    }
    //��׼��λ
    int pivot_index = i + 1;//�����׼����ȷλ��
    temp = a[pivot_index];
    a[pivot_index] = a[end];
    a[end] = temp;
    //�ݹ�ָ�
    QuickSort_Recursion(a, begin, pivot_index - 1);
    QuickSort_Recursion(a, pivot_index + 1, end);
}

// ================================================================================================
// ������������ȡ���Ż���
int Partition(int* a, int begin, int end) 
{
    int mid = begin + (end - begin) / 2;

    // ����ȡ�У����м�ֵ������endλ��
    if (a[begin] > a[mid]) 
    {
        int temp = a[begin];
        a[begin] = a[mid];
        a[mid] = temp;
    }
    if (a[mid] > a[end]) 
    {
        int temp = a[mid];
        a[mid] = a[end];
        a[end] = temp;
    }
    if (a[begin] > a[mid]) 
    {
        int temp = a[begin];
        a[begin] = a[mid];
        a[mid] = temp;
    }
    // ���м�ֵ������end��Ϊ����
    int temp = a[mid];
    a[mid] = a[end];
    a[end] = temp;

    int pivot = a[end];
    int i = begin - 1;
    for (int j = begin; j < end; j++) 
    {
        if (a[j] <= pivot) 
        {
            i++;
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
  
    temp = a[i + 1];
    a[i + 1] = a[end];
    a[end] = temp;

    return i + 1;
}


// ================================================================================================

void CountSort(int* a, int size, int max) 
{
    if (size <= 0) return;

    // 1. Ѱ���������Сֵ
    int min = a[0];
    for (int i = 1; i < size; i++) 
    {
        if (a[i] < min) 
            min = a[i];
    }

    // 2. ����ʵ����Ҫ��ֵ��Χ
    int range = max - min + 1;

    // 3. ������������
    int* count = (int*)calloc(range, sizeof(int));
    if (count == NULL) 
    {
        return;
    }

    // 4. ͳ��Ԫ�س��ִ�����������ȫӳ�䣩
    for (int i = 0; i < size; i++) 
    {
        // �������գ��� a[i] > max �ᵼ��Խ��
        count[a[i] - min]++; // ֵ��ƫ��ӳ��
    }

    // 5. �ع���������
    int index = 0;
    for (int i = 0; i < range; i++) 
    {
        while (count[i] > 0) 
        {
            a[index++] = i + min; // ����ƫ�ƻָ�ԭʼֵ
            count[i]--;
        }
    }

    free(count);
}


// ================================================================================================
// ������������
void RadixCountSort(int* a, int size) 
{
    if (size <= 1) return;

    // ������������ƫ����
    int max_val = a[0], min_val = a[0];
    for (int i = 1; i < size; i++) 
    {
        if (a[i] > max_val) max_val = a[i];
        if (a[i] < min_val) min_val = a[i];
    }

    int offset = 0;
    if (min_val < 0) 
    {
        offset = -min_val;
        max_val += offset; 
        for (int i = 0; i < size; i++) {
            a[i] += offset;
        }
    }

    // �������ֵ��λ��,��������������ִ�
    int digits = 0;
    int temp = max_val;
    do 
    {
        digits++;
        temp /= 10;
    } while (temp != 0);

    int* output = (int*)malloc(size * sizeof(int));
    if (!output) return; // �ڴ����ʧ��

    int exp = 1;
    for (int d = 0; d < digits; d++, exp *= 10) 
    {
        int count[10] = { 0 };

        // ͳ�Ƶ�ǰλ�ĳ��ִ���
        for (int i = 0; i < size; i++) 
        {
            int digit = (a[i] / exp) % 10;
            count[digit]++;
        }

        // ����ǰ׺��
        for (int i = 1; i < 10; i++) 
        {
            count[i] += count[i - 1];
        }

        // �������output����
        for (int i = size - 1; i >= 0; i--)
        {
            int digit = (a[i] / exp) % 10;
            output[count[digit] - 1] = a[i];
            count[digit]--;
        }

        // ������ԭ����
        memcpy(a, output, size * sizeof(int));
    }

    free(output);

    // �ָ�ԭʼֵ
    if (offset != 0) 
    {
        for (int i = 0; i < size; i++) 
        {
            a[i] -= offset;
        }
    }
}

// ================================================================================================
// �������򣨷ǵݹ�棩
// ջ�ṹ���壨����ģ��ݹ飩
typedef struct {
    int* data;      // ջ���ݴ洢
    int capacity;   // ջ����
    int top;        // ջ��ָ��
} Stack;

void stackInit(Stack* s, int initialCapacity) {
    s->data = (int*)malloc(initialCapacity * sizeof(int));
    s->capacity = initialCapacity;
    s->top = 0;
}

void stackPush(Stack* s, int value) {
    // ջ��ʱ�Զ�����Ϊԭ��������
    if (s->top >= s->capacity) {
        s->capacity *= 2;
        s->data = (int*)realloc(s->data, s->capacity * sizeof(int));
    }
    s->data[s->top++] = value;
}

int stackPop(Stack* s) {
    return s->data[--s->top];
}

void stackFree(Stack* s) {
    free(s->data);
}

// ====================================================================
// ����ȡ�л��ֺ���
int partition(int* a, int low, int high) {
    // ����ȡ�з�ѡ���׼
    int mid = low + (high - low) / 2;
    if (a[low] > a[mid]) { int t = a[low]; a[low] = a[mid]; a[mid] = t; }
    if (a[mid] > a[high]) { int t = a[mid]; a[mid] = a[high]; a[high] = t; }
    if (a[low] > a[mid]) { int t = a[low]; a[low] = a[mid]; a[mid] = t; }

    // ����ֵ������ĩβ��Ϊ��׼
    int pivot = a[mid];
    a[mid] = a[high];
    a[high] = pivot;

    // ��׼���ֹ���
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (a[j] <= pivot) {
            i++;
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    int temp = a[i + 1];
    a[i + 1] = a[high];
    a[high] = temp;
    return i + 1;
}

// ====================================================================
// �ǵݹ���������Ż���
void QuickSort(int* a, int size) {
    if (size <= 1) return;

    Stack stack;
    stackInit(&stack, size / 2);  // ��ʼ������Ϊ size/2 �������ݴ���

    stackPush(&stack, 0);
    stackPush(&stack, size - 1);

    while (stack.top > 0) {
        int high = stackPop(&stack);
        int low = stackPop(&stack);

        // С��Χʹ�ò��������Ż�
        if (high - low + 1 <= INSERTION_THRESHOLD) {
            for (int i = low + 1; i <= high; i++) {
                int key = a[i];
                int j = i - 1;
                while (j >= low && a[j] > key) {
                    a[j + 1] = a[j];
                    j--;
                }
                a[j + 1] = key;
            }
            continue;
        }

        // ֱ�ӵ��� partition���ڲ���ͨ������ȡ��ѡ���׼
        int pivot_pos = partition(a, low, high);

        // ���ȴ����С����������ջ��ȣ�
        if (pivot_pos - low < high - pivot_pos) {
            if (pivot_pos + 1 < high) {
                stackPush(&stack, pivot_pos + 1);
                stackPush(&stack, high);
            }
            if (low < pivot_pos - 1) {
                stackPush(&stack, low);
                stackPush(&stack, pivot_pos - 1);
            }
        }
        else {
            if (low < pivot_pos - 1) {
                stackPush(&stack, low);
                stackPush(&stack, pivot_pos - 1);
            }
            if (pivot_pos + 1 < high) {
                stackPush(&stack, pivot_pos + 1);
                stackPush(&stack, high);
            }
        }
    }

    stackFree(&stack);
}
// ================================================================================================





