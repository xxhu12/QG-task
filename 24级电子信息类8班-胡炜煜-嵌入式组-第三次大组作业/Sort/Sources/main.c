#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <ctype.h>
#include "test_framework.h"

// 最大选择数量定义
#define MAX_SELECTIONS 10

void showMainMenu() {
    system("cls || clear");
    printf("=== 排序算法测试平台 ===\n");
    printf("1. 选择数据规模\n");
    printf("2. 退出\n");
    printf("输入选择: ");
}

void showSizeMenu() {
    system("cls || clear");
    printf("选择数据规模：\n");
    printf("1. 小数据\n");
    printf("2. 大数据\n");
    printf("输入选择: ");
}

void showAlgorithmMenu() {
    system("cls || clear");
    printf("选择排序算法（多选，空格分隔）：\n");
    for (int i = 0; i < ALGO_COUNT; i++)
        printf("%d. %s\n", algorithms[i].id, algorithms[i].name);
    printf("输入选择（1-%d，例：1 3 5）: ", ALGO_COUNT);
}

int main() {
    srand((unsigned int)time(NULL));
    int mainChoice, sizeChoice;

    while (1) {
        // 处理主菜单选择
        int result;
        do {
            showMainMenu();
            result = scanf("%d", &mainChoice);
            if (result != 1) {
                while (getchar() != '\n');
                printf("无效输入，请输入数字 (1 或 2): ");
            }
            else if (mainChoice < 1 || mainChoice > 2) {
                printf("无效选择，请重新输入。\n");
                result = 0;
            }
        } while (result != 1);

        if (mainChoice == 2) break;

        if (mainChoice == 1) {
            // 处理数据规模选择
            do {
                showSizeMenu();
                result = scanf("%d", &sizeChoice);
                if (result != 1) {
                    while (getchar() != '\n');
                    printf("无效输入，请输入数字 (1 或 2): ");
                }
                else if (sizeChoice < 1 || sizeChoice > 2) {
                    printf("无效选择，请重新输入。\n");
                    result = 0;
                }
            } while (result != 1);

            // 清除输入缓冲区
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            // 算法选择处理
            showAlgorithmMenu();
            char input[256];
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("输入读取失败。\n");
                continue;
            }

            // 增强的输入解析
            int selected[MAX_SELECTIONS] = { 0 };
            int count = 0;
            char* token = strtok(input, " \n");  // 关键修改1：包含换行符

            while (token != NULL && count < MAX_SELECTIONS) {
                // 关键修改2：验证数字输入
                int valid = 1;
                for (int i = 0; token[i] != '\0'; i++) {
                    if (!isdigit(token[i])) {
                        valid = 0;
                        break;
                    }
                }

                if (valid) {
                    int id = atoi(token);
                    if (id >= 1 && id <= ALGO_COUNT) {
                        selected[count++] = id - 1;
                    }
                    else {
                        printf("忽略无效选项: %d\n", id);
                    }
                }
                else {
                    printf("忽略非数字输入: %s\n", token);
                }
                token = strtok(NULL, " \n");  // 关键修改3：统一分隔符
            }

            // 验证至少选择了一个算法
            if (count == 0) {
                printf("错误：未选择任何有效算法！\n");
                printf("\n按回车键继续...");
                while (getchar() != '\n');
                continue;
            }

            // 生成算法列表
            SortAlgorithm chosenAlgos[MAX_SELECTIONS];
            for (int i = 0; i < count; i++) {
                if (selected[i] < ALGO_COUNT) {
                    chosenAlgos[i] = algorithms[selected[i]];
                }
                else {
                    printf("发现无效索引: %d\n", selected[i]);
                    count = i;  // 截断到有效位置
                    break;
                }
            }

            // 执行测试
            if (sizeChoice == 1) {
                runSmallDataTest(chosenAlgos, count);
            }
            else {
                const int sizes[] = { 10000, 50000, 200000 };
                for (int i = 0; i < 3; i++) {
                    runLargeDataTest(sizes[i], chosenAlgos, count);
                }
            }
        }

        printf("\n按回车键继续...");
        while (getchar() != '\n');  // 清除缓冲区
    }
    return 0;
}
