/* ====================== Calculator.h ====================== */
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/* ����״̬��ö�� */
typedef enum Status {
    CALC_SUCCESS,            // �����ɹ�
    CALC_ERROR_INVALID_EXPR, // ��Ч���ʽ
    CALC_ERROR_MEMORY,       // �ڴ����ʧ��
    CALC_ERROR_DIV_ZERO,     // �������
    CALC_ERROR_EMPTY_STACK   // ��ջ����
} Status;

/* ��ջ�ڵ�ṹ */
typedef struct StackNode {
    double data;               // ֧�ָ�����
    struct StackNode* next;
} StackNode;

/* ��ջ���ƽṹ */
typedef struct {
    StackNode* top;          // ջ��ָ��
    size_t size;             // ջԪ�ؼ�����
} LinkStack;

/* ջ�����ӿ����� */
Status StackInit(LinkStack* stack);                     // ջ��ʼ��
Status StackDestroy(LinkStack* stack);                  // ջ����
Status StackPush(LinkStack* stack, double data);        // Ԫ����ջ
Status StackPop(LinkStack* stack, double* data);        // Ԫ�س�ջ
Status StackTop(const LinkStack* stack, double* data);  // �鿴ջ��
int StackIsEmpty(const LinkStack* stack);               // �пռ��

/* ��׺���ʽԪ�����ͱ�ʶ */
typedef enum {
    ELEMENT_OPERAND,        // ����������
    ELEMENT_OPERATOR        // ���������
} PostfixElementType;

/* ��׺���ʽ����ڵ�ṹ */
typedef struct PostfixElement {
    PostfixElementType type;   // Ԫ�����ͱ�ʶ
    union {                    
        double operand;        // ������ֵ����������
        char operator;         // ���������
    } value;
    struct PostfixElement* next;  // ָ����һ�ڵ��ָ��
} PostfixElement;

/* ���Ĺ��ܽӿ� */
Status InfixToPostfix(const char* expr, PostfixElement** postfixList);  // ��׺ת��׺
Status EvaluatePostfix(const PostfixElement* postfixList, double* result);  // ��׺���ʽ��ֵ
void FreePostfixList(PostfixElement* head);  // �ͷ������ڴ�

#endif

