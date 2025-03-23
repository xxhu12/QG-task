/* ====================== main.c ====================== */
#include "Calculator.h"
#include <stdio.h>

int main() {
    char expr[256];  // 输入缓冲区

    // 用户输入处理
    printf("请输入四则运算式（支持小数和负数）:\n");
    if (!fgets(expr, sizeof(expr), stdin)) {
        fprintf(stderr, "错误: 输入读取失败\n");
        return 1;
    }
    expr[strcspn(expr, "\n")] = '\0';  // 去除换行符

    // 表达式转换
    PostfixElement* postfix = NULL;
    Status status = InfixToPostfix(expr, &postfix);

    /* 错误处理分支 */
    if (status != CALC_SUCCESS) {
        FreePostfixList(postfix);
        // 根据错误类型显示不同信息
        switch (status) {
        case CALC_ERROR_MEMORY:
            fprintf(stderr, "错误: 内存分配失败\n"); break;
        case CALC_ERROR_INVALID_EXPR:
            fprintf(stderr, "错误: 表达式语法错误\n"); break;
        default:
            fprintf(stderr, "未知错误 (%d)\n", status);
        }
        return 1;
    }

    // 表达式求值
    double result;
    status = EvaluatePostfix(postfix, &result);
    FreePostfixList(postfix);  // 无论成功与否都释放内存

    /* 求值结果处理 */
    if (status != CALC_SUCCESS) {
        // 根据错误类型显示不同信息
        switch (status) {
        case CALC_ERROR_DIV_ZERO:
            fprintf(stderr, "错误: 除零操作\n"); break;
        default:
            fprintf(stderr, "计算错误 (%d)\n", status);
        }
        return 1;
    }

    // 格式化输出结果（自动判断整数/小数）
    if (fabs(result - round(result)) < 1e-10) {
        printf("计算结果: %.0f\n", result);
    }
    else {
        printf("计算结果: %.6f\n", result);
    }

    system("pause");

    return 0;
}
