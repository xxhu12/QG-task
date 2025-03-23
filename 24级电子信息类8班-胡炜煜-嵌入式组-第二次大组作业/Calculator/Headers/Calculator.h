/* ====================== Calculator.h ====================== */
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/* 错误状态码枚举 */
typedef enum Status {
    CALC_SUCCESS,            // 操作成功
    CALC_ERROR_INVALID_EXPR, // 无效表达式
    CALC_ERROR_MEMORY,       // 内存分配失败
    CALC_ERROR_DIV_ZERO,     // 除零错误
    CALC_ERROR_EMPTY_STACK   // 空栈操作
} Status;

/* 链栈节点结构 */
typedef struct StackNode {
    double data;               // 支持浮点数
    struct StackNode* next;
} StackNode;

/* 链栈控制结构 */
typedef struct {
    StackNode* top;          // 栈顶指针
    size_t size;             // 栈元素计数器
} LinkStack;

/* 栈操作接口声明 */
Status StackInit(LinkStack* stack);                     // 栈初始化
Status StackDestroy(LinkStack* stack);                  // 栈销毁
Status StackPush(LinkStack* stack, double data);        // 元素入栈
Status StackPop(LinkStack* stack, double* data);        // 元素出栈
Status StackTop(const LinkStack* stack, double* data);  // 查看栈顶
int StackIsEmpty(const LinkStack* stack);               // 判空检查

/* 后缀表达式元素类型标识 */
typedef enum {
    ELEMENT_OPERAND,        // 操作数类型
    ELEMENT_OPERATOR        // 运算符类型
} PostfixElementType;

/* 后缀表达式链表节点结构 */
typedef struct PostfixElement {
    PostfixElementType type;   // 元素类型标识
    union {                    
        double operand;        // 操作数值（浮点数）
        char operator;         // 运算符符号
    } value;
    struct PostfixElement* next;  // 指向下一节点的指针
} PostfixElement;

/* 核心功能接口 */
Status InfixToPostfix(const char* expr, PostfixElement** postfixList);  // 中缀转后缀
Status EvaluatePostfix(const PostfixElement* postfixList, double* result);  // 后缀表达式求值
void FreePostfixList(PostfixElement* head);  // 释放链表内存

#endif

