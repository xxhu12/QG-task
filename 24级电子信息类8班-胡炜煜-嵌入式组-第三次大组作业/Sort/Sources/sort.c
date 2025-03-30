#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

// 插入排序阈值
#define INSERTION_THRESHOLD 15

// ================================================================================================

// 插入排序
void insertSort(int* a, int n) 
{
    for (int i = 1; i < n; i++) //遍历未排序元素
    {
        int key = a[i];//当前待插入元素的下标
        if (key >= a[i - 1]) //提前判断是否需要插入,若已经大于已排序的末尾，则无需后续操作，减少不必要的移动
        {
            continue;
        }
        int j; 
        for (j = i - 1; j >= 0 && a[j] > key; j--) //将比 key 大的元素后移，腾出插入位置
        {
            a[j + 1] = a[j];
        }
        a[j + 1] = key; //插入
    }
}

// ================================================================================================

// 归并排序（合并两个有序子数组 a[begin..mid] 和 a[mid+1..end]）
void MergeArray(int* a, int begin, int mid, int end, int* temp) 
{
    int i = begin, j = mid + 1, k = 0;
    while (i <= mid && j <= end) 
    {
        temp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];//比较元素并存入临时数组 temp
    }
    // 剩余元素拷贝到temp
    if (i <= mid) memcpy(temp + k, a + i, (mid - i + 1) * sizeof(int));
    if (j <= end) memcpy(temp + k, a + j, (end - j + 1) * sizeof(int));
    //复制回原数组
    int total = k + (mid - i + 1) + (end - j + 1);
    memcpy(a + begin, temp, total * sizeof(int));
}

// ================================================================================================

//归并排序
void MergeSort(int* a, int begin, int end, int* temp) 
{
    if (begin < end)//只有当子数组有至少 2 个元素时，才进行排序操作 
    {
        if (end - begin + 1 <= INSERTION_THRESHOLD) // 小数组使用插入排序
        {
            insertSort(a + begin, end - begin + 1); 
            return;
        }
        int mid = begin + (end - begin) / 2;//拆分数组
        //递归排序子数组
        MergeSort(a, begin, mid, temp);
        MergeSort(a, mid + 1, end, temp);
        if (a[mid] > a[mid + 1]) //条件合并
        {
            MergeArray(a, begin, mid, end, temp);
        }
    }
}

// ================================================================================================

//快速排序（递归版）（随机优化）
void QuickSort_Recursion(int* a, int begin, int end) 
{
    if (begin >= end) return;//递归终止条件

    // ================== 随机基准优化 ==================
    //生成 [begin, end] 范围内的随机索引
    int random_index = begin + rand() % (end - begin + 1);

    //将随机元素交换到末尾位置
    int temp = a[end];
    a[end] = a[random_index];
    a[random_index] = temp;

    // 将交换后的末尾元素 a[end] 作为基准值 pivot
    int pivot = a[end]; 
    // ================================================

    int i = begin - 1;//分区，根据基准值 pivot划分左右
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
    //基准归位
    int pivot_index = i + 1;//计算基准的正确位置
    temp = a[pivot_index];
    a[pivot_index] = a[end];
    a[end] = temp;
    //递归分割
    QuickSort_Recursion(a, begin, pivot_index - 1);
    QuickSort_Recursion(a, pivot_index + 1, end);
}

// ================================================================================================
// 快速排序（三数取中优化）
int Partition(int* a, int begin, int end) 
{
    int mid = begin + (end - begin) / 2;

    // 三数取中，将中间值交换到end位置
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
    // 将中间值交换到end作为枢轴
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

    // 1. 寻找数组的最小值
    int min = a[0];
    for (int i = 1; i < size; i++) 
    {
        if (a[i] < min) 
            min = a[i];
    }

    // 2. 计算实际需要的值域范围
    int range = max - min + 1;

    // 3. 创建计数数组
    int* count = (int*)calloc(range, sizeof(int));
    if (count == NULL) 
    {
        return;
    }

    // 4. 统计元素出现次数（负数安全映射）
    for (int i = 0; i < size; i++) 
    {
        // 隐含风险：若 a[i] > max 会导致越界
        count[a[i] - min]++; // 值域偏移映射
    }

    // 5. 重构排序数组
    int index = 0;
    for (int i = 0; i < range; i++) 
    {
        while (count[i] > 0) 
        {
            a[index++] = i + min; // 逆向偏移恢复原始值
            count[i]--;
        }
    }

    free(count);
}


// ================================================================================================
// 基数计数排序
void RadixCountSort(int* a, int size) 
{
    if (size <= 1) return;

    // 处理负数，计算偏移量
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

    // 计算最大值的位数,决定基数排序的轮次
    int digits = 0;
    int temp = max_val;
    do 
    {
        digits++;
        temp /= 10;
    } while (temp != 0);

    int* output = (int*)malloc(size * sizeof(int));
    if (!output) return; // 内存分配失败

    int exp = 1;
    for (int d = 0; d < digits; d++, exp *= 10) 
    {
        int count[10] = { 0 };

        // 统计当前位的出现次数
        for (int i = 0; i < size; i++) 
        {
            int digit = (a[i] / exp) % 10;
            count[digit]++;
        }

        // 计算前缀和
        for (int i = 1; i < 10; i++) 
        {
            count[i] += count[i - 1];
        }

        // 逆序填充output数组
        for (int i = size - 1; i >= 0; i--)
        {
            int digit = (a[i] / exp) % 10;
            output[count[digit] - 1] = a[i];
            count[digit]--;
        }

        // 拷贝回原数组
        memcpy(a, output, size * sizeof(int));
    }

    free(output);

    // 恢复原始值
    if (offset != 0) 
    {
        for (int i = 0; i < size; i++) 
        {
            a[i] -= offset;
        }
    }
}

// ================================================================================================
// 快速排序（非递归版）
// 栈结构定义（用于模拟递归）
typedef struct {
    int* data;      // 栈数据存储
    int capacity;   // 栈容量
    int top;        // 栈顶指针
} Stack;

void stackInit(Stack* s, int initialCapacity) {
    s->data = (int*)malloc(initialCapacity * sizeof(int));
    s->capacity = initialCapacity;
    s->top = 0;
}

void stackPush(Stack* s, int value) {
    // 栈满时自动扩容为原来的两倍
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
// 三数取中划分函数
int partition(int* a, int low, int high) {
    // 三数取中法选择基准
    int mid = low + (high - low) / 2;
    if (a[low] > a[mid]) { int t = a[low]; a[low] = a[mid]; a[mid] = t; }
    if (a[mid] > a[high]) { int t = a[mid]; a[mid] = a[high]; a[high] = t; }
    if (a[low] > a[mid]) { int t = a[low]; a[low] = a[mid]; a[mid] = t; }

    // 将中值交换到末尾作为基准
    int pivot = a[mid];
    a[mid] = a[high];
    a[high] = pivot;

    // 标准划分过程
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
// 非递归快速排序（优化后）
void QuickSort(int* a, int size) {
    if (size <= 1) return;

    Stack stack;
    stackInit(&stack, size / 2);  // 初始容量设为 size/2 减少扩容次数

    stackPush(&stack, 0);
    stackPush(&stack, size - 1);

    while (stack.top > 0) {
        int high = stackPop(&stack);
        int low = stackPop(&stack);

        // 小范围使用插入排序优化
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

        // 直接调用 partition，内部已通过三数取中选择基准
        int pivot_pos = partition(a, low, high);

        // 优先处理较小分区（减少栈深度）
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





