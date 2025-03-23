/* ====================== main.c ====================== */
#include "Calculator.h"
#include <stdio.h>

int main() {
    char expr[256];  // ���뻺����

    // �û����봦��
    printf("��������������ʽ��֧��С���͸�����:\n");
    if (!fgets(expr, sizeof(expr), stdin)) {
        fprintf(stderr, "����: �����ȡʧ��\n");
        return 1;
    }
    expr[strcspn(expr, "\n")] = '\0';  // ȥ�����з�

    // ���ʽת��
    PostfixElement* postfix = NULL;
    Status status = InfixToPostfix(expr, &postfix);

    /* �������֧ */
    if (status != CALC_SUCCESS) {
        FreePostfixList(postfix);
        // ���ݴ���������ʾ��ͬ��Ϣ
        switch (status) {
        case CALC_ERROR_MEMORY:
            fprintf(stderr, "����: �ڴ����ʧ��\n"); break;
        case CALC_ERROR_INVALID_EXPR:
            fprintf(stderr, "����: ���ʽ�﷨����\n"); break;
        default:
            fprintf(stderr, "δ֪���� (%d)\n", status);
        }
        return 1;
    }

    // ���ʽ��ֵ
    double result;
    status = EvaluatePostfix(postfix, &result);
    FreePostfixList(postfix);  // ���۳ɹ�����ͷ��ڴ�

    /* ��ֵ������� */
    if (status != CALC_SUCCESS) {
        // ���ݴ���������ʾ��ͬ��Ϣ
        switch (status) {
        case CALC_ERROR_DIV_ZERO:
            fprintf(stderr, "����: �������\n"); break;
        default:
            fprintf(stderr, "������� (%d)\n", status);
        }
        return 1;
    }

    // ��ʽ�����������Զ��ж�����/С����
    if (fabs(result - round(result)) < 1e-10) {
        printf("������: %.0f\n", result);
    }
    else {
        printf("������: %.6f\n", result);
    }

    system("pause");

    return 0;
}
