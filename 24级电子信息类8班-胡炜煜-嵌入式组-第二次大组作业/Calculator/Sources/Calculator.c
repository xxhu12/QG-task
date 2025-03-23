/* ====================== Calculator.c ====================== */
#include "Calculator.h"

// ================= ջģ��ʵ�� =================

/**
 * ��ʼ��ջ
 * @param stack Ҫ��ʼ����ջָ��
 * @return ״̬�룺�ɹ�����CALC_SUCCESS��ջΪ�շ��ش���
 */
Status StackInit(LinkStack* stack) {
    if (!stack) return CALC_ERROR_EMPTY_STACK;
    stack->top = NULL;
    stack->size = 0;
    return CALC_SUCCESS;
}

/**
 * ����ջ���ͷ����нڵ��ڴ�
 * @param stack Ŀ��ջָ��
 * @return ״̬�룺�ɹ�����CALC_SUCCESS��ջΪ�շ��ش���
 */
Status StackDestroy(LinkStack* stack) {
    if (!stack) return CALC_ERROR_EMPTY_STACK;
    // ѭ����������Ԫ��ֱ��ջ��
    while (!StackIsEmpty(stack)) {
        double temp;
        StackPop(stack, &temp);
    }
    return CALC_SUCCESS;
}

/**
 * Ԫ����ջ
 * @param stack Ŀ��ջָ��
 * @param data Ҫ��ջ������
 * @return ״̬�룺�ɹ�����CALC_SUCCESS��ջΪ�ջ��ڴ����ʧ�ܷ��ش���
 */
Status StackPush(LinkStack* stack, double data) {
    if (!stack) return CALC_ERROR_EMPTY_STACK;
    // �����½ڵ�
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) return CALC_ERROR_MEMORY;
    // �½ڵ����ջ��
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
    return CALC_SUCCESS;
}

/**
 * Ԫ�س�ջ
 * @param stack Ŀ��ջָ��
 * @param data ���ڽ��ճ�ջ���ݵ�ָ��
 * @return ״̬�룺�ɹ�����CALC_SUCCESS��ջΪ�շ��ش���
 */
Status StackPop(LinkStack* stack, double* data) {
    if (!stack || StackIsEmpty(stack)) return CALC_ERROR_EMPTY_STACK;
    // ȡ��ջ���ڵ㲢����ջ�ṹ
    StackNode* temp = stack->top;
    *data = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;
    return CALC_SUCCESS;
}

/**
 * ��ȡջ��Ԫ�أ���������
 * @param stack Ŀ��ջָ��
 * @param data ���ڽ���ջ�����ݵ�ָ��
 * @return ״̬�룺�ɹ�����CALC_SUCCESS��ջΪ�շ��ش���
 */
Status StackTop(const LinkStack* stack, double* data) {
    if (!stack || StackIsEmpty(stack)) return CALC_ERROR_EMPTY_STACK;
    *data = stack->top->data;
    return CALC_SUCCESS;
}

/**
 * ���ջ�Ƿ�Ϊ��
 * @param stack Ŀ��ջָ��
 * @return 1Ϊ�գ�0�ǿ�
 */
int StackIsEmpty(const LinkStack* stack) {
    return stack->top == NULL;
}

// ================= ���ʽ����ģ�� =================

/* �ж��ַ��Ƿ�Ϊ��������������ţ� */
static int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

/* ��ȡ��������ȼ� */
static int get_priority(char op) {
    switch (op) {
    case '+': case '-': return 1;  // �Ӽ����ȼ����
    case '*': case '/': return 2;  // �˳����ȼ��ϸ�
    case '(':           return 0;  // ���������⴦�����ȼ����
    default:            return -1; // ��Ч�����
    }
}

/**
 * ����׺���ʽת��Ϊ��׺���ʽ
 * @param expr �������׺���ʽ�ַ���
 * @param postfixList ����ĺ�׺���ʽ����ͷָ��
 * @return ״̬�룺�ɹ�����CALC_SUCCESS�����ʽ��Ч���ڴ���󷵻���Ӧ������
 */
Status InfixToPostfix(const char* expr, PostfixElement** postfixList) {
    if (!expr || !postfixList) return CALC_ERROR_INVALID_EXPR;
    *postfixList = NULL; // ��ʼ���������
    PostfixElement* tail = NULL; // ����βָ��

    LinkStack opStack; // �����ջ
    Status status = StackInit(&opStack);
    if (status != CALC_SUCCESS) return status;

    size_t len = strlen(expr);
    for (size_t i = 0; i < len; ) {
        // �����հ��ַ�
        if (isspace(expr[i])) { i++; continue; }

        /* �������֣�����������С���� */
        if ((expr[i] == '-' && (i == 0 || expr[i - 1] == '(')) || isdigit(expr[i])) {
            int is_negative = 0; // �������
            double num = 0.0;
            int decimal_places = 0; // С��λ��

            // ������
            if (expr[i] == '-') {
                is_negative = 1;
                i++;
            }

            // ������������
            while (i < len && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }

            // ����С������
            if (i < len && expr[i] == '.') {
                i++;
                while (i < len && isdigit(expr[i])) {
                    num = num * 10 + (expr[i] - '0');
                    decimal_places++; // ͳ��С��λ��
                    i++;
                }
                // ����С������ʵ��ֵ
                if (decimal_places > 0) {
                    num /= pow(10, decimal_places);
                }
            }

            // Ӧ�ø���
            if (is_negative) num = -num;

            // �����������ڵ㲢��������
            PostfixElement* node = (PostfixElement*)malloc(sizeof(PostfixElement));
            if (!node) goto memory_error;
            node->type = ELEMENT_OPERAND;
            node->value.operand = num;
            node->next = NULL;

            if (!*postfixList) { // ����Ϊ��ʱ��ʼ��
                *postfixList = node;
                tail = node;
            }
            else { // ����׷�ӵ�����β��
                tail->next = node;
                tail = node;
            }
        }
        /* ���������� */
        else if (expr[i] == '(') {
            StackPush(&opStack, '('); // ������ֱ����ջ
            i++;
        }
        /* ���������� */
        else if (expr[i] == ')') {
            double top_op;
            // ��������ջ��Ԫ��ֱ������������
            while (StackTop(&opStack, &top_op) == CALC_SUCCESS && (char)top_op != '(') {
                // ����������ڵ㲢��������
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
            // ջ��˵�����Ų�ƥ��
            if (StackIsEmpty(&opStack)) {
                status = CALC_ERROR_INVALID_EXPR;
                goto cleanup;
            }
            StackPop(&opStack, &top_op); // ����������
            i++;
        }
        /* ��������� */
        else if (is_operator(expr[i])) {
            char current_op = expr[i];
            // �Ƚϵ�ǰ�������ջ������������ȼ�
            while (!StackIsEmpty(&opStack)) {
                double top_op;
                StackTop(&opStack, &top_op);
                // ջ�����������������ȼ������ڵ�ǰ�����ʱ������ջ��
                if ((char)top_op != '(' &&
                    get_priority((char)top_op) >= get_priority(current_op)) {
                    // ����������ڵ㲢��������
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
            StackPush(&opStack, current_op); // ��ǰ�������ջ
            i++;
        }
        /* �Ƿ��ַ����� */
        else {
            status = CALC_ERROR_INVALID_EXPR;
            goto cleanup;
        }
    }

    /* ����ջ��ʣ�������� */
    while (!StackIsEmpty(&opStack)) {
        double op;
        StackPop(&opStack, &op);
        // ʣ��������˵�����Ų�ƥ��
        if ((char)op == '(') {
            status = CALC_ERROR_INVALID_EXPR;
            goto cleanup;
        }

        // ����������ڵ㲢��������
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

memory_error: // �ڴ����ʧ�ܴ���
    StackDestroy(&opStack);
    FreePostfixList(*postfixList); // �ͷ��ѷ��������ڴ�
    return CALC_ERROR_MEMORY;
}

/**
 * �����׺���ʽ��ֵ
 * @param postfixList ��׺���ʽ����
 * @param result ���ڽ��ռ�������ָ��
 * @return ״̬�룺�ɹ�����CALC_SUCCESS�����ʽ��Ч�������󷵻���Ӧ������
 */
Status EvaluatePostfix(const PostfixElement* postfixList, double* result) {
    if (!postfixList || !result) return CALC_ERROR_INVALID_EXPR;

    LinkStack operandStack; // ������ջ
    Status status = StackInit(&operandStack);
    if (status != CALC_SUCCESS) return status;

    const PostfixElement* current = postfixList;
    while (current) {
        if (current->type == ELEMENT_OPERAND) {
            // ��������ջ
            StackPush(&operandStack, current->value.operand);
        }
        else {
            double b, a;
            // �����Ҳ��������������
            if (StackPop(&operandStack, &b) != CALC_SUCCESS ||
                StackPop(&operandStack, &a) != CALC_SUCCESS) {
                StackDestroy(&operandStack);
                return CALC_ERROR_INVALID_EXPR; // ���������㣬���ʽ��Ч
            }

            // ������������м���
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
                // ���������
                if (fabs(b) < 1e-10) {
                    StackDestroy(&operandStack);
                    return CALC_ERROR_DIV_ZERO;
                }
                StackPush(&operandStack, a / b);
                break;
            default:
                StackDestroy(&operandStack);
                return CALC_ERROR_INVALID_EXPR; // ��Ч�����
            }
        }
        current = current->next;
    }

    // ����ջ��Ӧֻʣһ��Ԫ�أ������
    if (StackIsEmpty(&operandStack)) {
        StackDestroy(&operandStack);
        return CALC_ERROR_INVALID_EXPR;
    }

    StackPop(&operandStack, result);
    StackDestroy(&operandStack);
    return CALC_SUCCESS;
}

/**
 * �ͷź�׺���ʽ�����ڴ�
 * @param head ����ͷָ��
 */
void FreePostfixList(PostfixElement* head) {
    while (head) {
        PostfixElement* temp = head;
        head = head->next;
        free(temp);
    }
}
