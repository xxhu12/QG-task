#ifndef DATA_LOADER_H
#define DATA_LOADER_H

/**
 * @brief 从文件中加载数据
 * @param filename 输入文件名
 * @param size 输出数据量
 * @return 动态分配的整数数组指针
 */
int* LoadData(const char* filename, int* size);

#endif
