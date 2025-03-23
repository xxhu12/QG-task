/* ====================== Calculator.c ====================== */
#include "Calculator.h"

// ================= 栈模块实现 =================

/**
 * 初始化栈
 * @param stack 要初始化的栈指针
 * @return 状态码：成功返回CALC_SUCCESS，栈为空返回错误
 */
Status StackInit(LinkStack* stack) {
    if (!stack) return CALC_ERROR_EMPTY_STACK;
    stack->top = NULL;
    stack->size = 0;
    return CALC_SUCCESS;
}

/**
 * 销毁栈，释放所有节点内存
 * @param stack 目标栈指针
 * @return 状态码：成功返回CALC_SUCCESS，栈为空返回错误
 */
Status StackDestroy(LinkStack* stack) {
    if (!stack) return CALC_ERROR_EMPTY_STACK;
    // 循环弹出所有元素直至栈空
    while (!StackIsEmpty(stack)) {
        double temp;
        StackPop(stack, &temp);
    }
    return CALC_SUCCESS;
}

/**
 * 元素入栈
 * @param stack 目标栈指针
 * @param data 要入栈的数据
 * @return 状态码：成功返回CALC_SUCCESS，栈为空或内存分配失败返回错误
 */
Status StackPush(LinkStack* stack, double data) {
    if (!stack) return CALC_ERROR_EMPTY_STACK;
    // 创建新节点
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) return CALC_ERROR_MEMORY;
    // 新节点插入栈顶
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
    return CALC_SUCCESS;
}

/**
 * 元素出栈
 * @param stack 目标栈指针
 * @param data 用于接收出栈数据的指针
 * @return 状态码：成功返回CALC_SUCCESS，栈为空返回错误
 */
Status StackPop(LinkStack* stack, double* data) {
    if (!stack || StackIsEmpty(stack)) return CALC_ERROR_EMPTY_STACK;
    // 取出栈顶节点并更新栈结构
    StackNode* temp = stack->top;
    *data = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;
    return CALC_SUCCESS;
}

/**
 * 获取栈顶元素（不弹出）
 * @param stack 目标栈指针
 * @param data 用于接收栈顶数据的指针
 * @return 状态码：成功返回CALC_SUCCESS，栈为空返回错误
 */
Status StackTop(const LinkStack* stack, double* data) {
    if (!stack || StackIsEmpty(stack)) return CALC_ERROR_EMPTY_STACK;
    *data = stack->top->data;
    return CALC_SUCCESS;
}

/**
 * 检查栈是否为空
 * @param stack 目标栈指针
 * @return 1为空，0非空
 */
int StackIsEmpty(const LinkStack* stack) {
    return stack->top == NULL;
}

// ================= 表达式处理模块 =================

/* 判断字符是否为运算符（包含括号） */
static int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

/* 获取运算符优先级 */
static int get_priority(char op) {
    switch (op) {
    case '+': case '-': return 1;  // 加减优先级最低
    case '*': case '/': return 2;  // 乘除优先级较高
    case '(':           return 0;  // 左括号特殊处理，优先级最低
    default:            return -1; // 无效运算符
    }
}

/**
 * 将中缀表达式转换为后缀表达式
 * @param expr 输入的中缀表达式字符串
 * @param postfixList 输出的后缀表达式链表头指针
 * @return 状态码：成功返回CALC_SUCCESS，表达式无效或内存错误返回相应错误码
 */
Status InfixToPostfix(const char* expr, PostfixElement** postfixList) {
    if (!expr || !postfixList) return CALC_ERROR_INVALID_EXPR;
    *postfixList = NULL; // 初始化输出链表
    PostfixElement* tail = NULL; // 链表尾指针

    LinkStack opStack; // 运算符栈
    Status status = StackInit(&opStack);
    if (status != CALC_SUCCESS) return status;

    size_t len = strlen(expr);
    for (size_t i = 0; i < len; ) {
        // 跳过空白字符
        if (isspace(expr[i])) { i++; continue; }

        /* 处理数字（包括负数和小数） */
        if ((expr[i] == '-' && (i == 0 || expr[i - 1] == '(')) || isdigit(expr[i])) {
            int is_negative = 0; // 负数标记
            double num = 0.0;
            int decimal_places = 0; // 小数位数

            // 处理负号
            if (expr[i] == '-') {
                is_negative = 1;
                i++;
            }

            // 解析整数部分
            while (i < len && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }

            // 解析小数部分
            if (i < len && expr[i] == '.') {
                i++;
                while (i < len && isdigit(expr[i])) {
                    num = num * 10 + (expr[i] - '0');
                    decimal_places++; // 统计小数位数
                    i++;
                }
                // 计算小数部分实际值
                if (decimal_places > 0) {
                    num /= pow(10, decimal_places);
                }
            }

            // 应用负号
            if (is_negative) num = -num;

            // 创建操作数节点并加入链表
            PostfixElement* node = (PostfixElement*)malloc(sizeof(PostfixElement));
            if (!node) goto memory_error;
            node->type = ELEMENT_OPERAND;
            node->value.operand = num;
            node->next = NULL;

            if (!*postfixList) { // 链表为空时初始化
                *postfixList = node;
                tail = node;
            }
            else { // 否则追加到链表尾部
                tail->next = node;
                tail = node;
            }
        }
        /* 处理左括号 */
        else if (expr[i] == '(') {
            StackPush(&opStack, '('); // 左括号直接入栈
            i++;
        }
        /* 处理右括号 */
        else if (expr[i] == ')') {
            double top_op;
            // 持续弹出栈顶元素直到遇到左括号
            while (StackTop(&opStack, &top_op) == CALC_SUCCESS && (char)top_op != '(') {
                // 创建运算符节点并加入链表
                PostfixElement* node = (PostfixElement*)malloc(sizeof(PostfixElement));
                if (!node) goto memory_error;
                node->type = ELEMENT_OPERATOR;
                node->value.operator = (char)top_op;
                node->next = NULL;

                if (!*postfixList) {
                    *postfixList = node;
                    tail = node;
                }
                else {
                    tail->next = node;
                    tail = node;
                }
                StackPop(&opStack, &top_op);
            }
            // 栈空说明括号不匹配
            if (StackIsEmpty(&opStack)) {
                status = CALC_ERROR_INVALID_EXPR;
                goto cleanup;
            }
            StackPop(&opStack, &top_op); // 弹出左括号
            i++;
        }
        /* 处理运算符 */
        else if (is_operator(expr[i])) {
            char current_op = expr[i];
            // 比较当前运算符与栈顶运算符的优先级
            while (!StackIsEmpty(&opStack)) {
                double top_op;
                StackTop(&opStack, &top_op);
                // 栈顶不是左括号且优先级不低于当前运算符时，弹出栈顶
                if ((char)top_op != '(' &&
                    get_priority((char)top_op) >= get_priority(current_op)) {
                    // 创建运算符节点并加入链表
                    PostfixElement* node = (PostfixElement*)malloc(sizeof(PostfixElement));
                    if (!node) goto memory_error;
                    node->type = ELEMENT_OPERATOR;
                    node->value.operator = (char)top_op;
                    node->next = NULL;

                    if (!*postfixList) {
                        *postfixList = node;
                        tail = node;
                    }
                    else {
                        tail->next = node;
                        tail = node;
                    }
                    StackPop(&opStack, &top_op);
                }
                else {
                    break;
                }
            }
            StackPush(&opStack, current_op); // 当前运算符入栈
            i++;
        }
        /* 非法字符处理 */
        else {
            status = CALC_ERROR_INVALID_EXPR;
            goto cleanup;
        }
    }

    /* 处理栈中剩余的运算符 */
    while (!StackIsEmpty(&opStack)) {
        double op;
        StackPop(&opStack, &op);
        // 剩余左括号说明括号不匹配
        if ((char)op == '(') {
            status = CALC_ERROR_INVALID_EXPR;
            goto cleanup;
        }

        // 创建运算符节点并加入链表
        PostfixElement* node = (PostfixElement*)malloc(sizeof(PostfixElement));
        if (!node) goto memory_error;
        node->type = ELEMENT_OPERATOR;
        node->value.operator = (char)op;
        node->next = NULL;

        if (!*postfixList) {
            *postfixList = node;
            tail = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
    }

cleanup:
    StackDestroy(&opStack);
    return status;

memory_error: // 内存分配失败处理
    StackDestroy(&opStack);
    FreePostfixList(*postfixList); // 释放已分配链表内存
    return CALC_ERROR_MEMORY;
}

/**
 * 计算后缀表达式的值
 * @param postfixList 后缀表达式链表
 * @param result 用于接收计算结果的指针
 * @return 状态码：成功返回CALC_SUCCESS，表达式无效或除零错误返回相应错误码
 */
Status EvaluatePostfix(const PostfixElement* postfixList, double* result) {
    if (!postfixList || !result) return CALC_ERROR_INVALID_EXPR;

    LinkStack operandStack; // 操作数栈
    Status status = StackInit(&operandStack);
    if (status != CALC_SUCCESS) return status;

    const PostfixElement* current = postfixList;
    while (current) {
        if (current->type == ELEMENT_OPERAND) {
            // 操作数入栈
            StackPush(&operandStack, current->value.operand);
        }
        else {
            double b, a;
            // 弹出右操作数和左操作数
            if (StackPop(&operandStack, &b) != CALC_SUCCESS ||
                StackPop(&operandStack, &a) != CALC_SUCCESS) {
                StackDestroy(&operandStack);
                return CALC_ERROR_INVALID_EXPR; // 操作数不足，表达式无效
            }

            // 根据运算符进行计算
            switch (current->value.operator) {
            case '+':
                StackPush(&operandStack, a + b);
                break;
            case '-':
                StackPush(&operandStack, a - b);
                break;
            case '*':
                StackPush(&operandStack, a * b);
                break;
            case '/':
                // 检查除零错误
                if (fabs(b) < 1e-10) {
                    StackDestroy(&operandStack);
                    return CALC_ERROR_DIV_ZERO;
                }
                StackPush(&operandStack, a / b);
                break;
            default:
                StackDestroy(&operandStack);
                return CALC_ERROR_INVALID_EXPR; // 无效运算符
            }
        }
        current = current->next;
    }

    // 最终栈中应只剩一个元素（结果）
    if (StackIsEmpty(&operandStack)) {
        StackDestroy(&operandStack);
        return CALC_ERROR_INVALID_EXPR;
    }

    StackPop(&operandStack, result);
    StackDestroy(&operandStack);
    return CALC_SUCCESS;
}

/**
 * 释放后缀表达式链表内存
 * @param head 链表头指针
 */
void FreePostfixList(PostfixElement* head) {
    while (head) {
        PostfixElement* temp = head;
        head = head->next;
        free(temp);
    }
}
