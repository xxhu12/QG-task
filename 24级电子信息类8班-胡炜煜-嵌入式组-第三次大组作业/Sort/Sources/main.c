#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <ctype.h>
#include "test_framework.h"

// ���ѡ����������
#define MAX_SELECTIONS 10

void showMainMenu() {
    system("cls || clear");
    printf("=== �����㷨����ƽ̨ ===\n");
    printf("1. ѡ�����ݹ�ģ\n");
    printf("2. �˳�\n");
    printf("����ѡ��: ");
}

void showSizeMenu() {
    system("cls || clear");
    printf("ѡ�����ݹ�ģ��\n");
    printf("1. С����\n");
    printf("2. ������\n");
    printf("����ѡ��: ");
}

void showAlgorithmMenu() {
    system("cls || clear");
    printf("ѡ�������㷨����ѡ���ո�ָ�����\n");
    for (int i = 0; i < ALGO_COUNT; i++)
        printf("%d. %s\n", algorithms[i].id, algorithms[i].name);
    printf("����ѡ��1-%d������1 3 5��: ", ALGO_COUNT);
}

int main() {
    srand((unsigned int)time(NULL));
    int mainChoice, sizeChoice;

    while (1) {
        // �������˵�ѡ��
        int result;
        do {
            showMainMenu();
            result = scanf("%d", &mainChoice);
            if (result != 1) {
                while (getchar() != '\n');
                printf("��Ч���룬���������� (1 �� 2): ");
            }
            else if (mainChoice < 1 || mainChoice > 2) {
                printf("��Чѡ�����������롣\n");
                result = 0;
            }
        } while (result != 1);

        if (mainChoice == 2) break;

        if (mainChoice == 1) {
            // �������ݹ�ģѡ��
            do {
                showSizeMenu();
                result = scanf("%d", &sizeChoice);
                if (result != 1) {
                    while (getchar() != '\n');
                    printf("��Ч���룬���������� (1 �� 2): ");
                }
                else if (sizeChoice < 1 || sizeChoice > 2) {
                    printf("��Чѡ�����������롣\n");
                    result = 0;
                }
            } while (result != 1);

            // ������뻺����
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            // �㷨ѡ����
            showAlgorithmMenu();
            char input[256];
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("�����ȡʧ�ܡ�\n");
                continue;
            }

            // ��ǿ���������
            int selected[MAX_SELECTIONS] = { 0 };
            int count = 0;
            char* token = strtok(input, " \n");  // �ؼ��޸�1���������з�

            while (token != NULL && count < MAX_SELECTIONS) {
                // �ؼ��޸�2����֤��������
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
                        printf("������Чѡ��: %d\n", id);
                    }
                }
                else {
                    printf("���Է���������: %s\n", token);
                }
                token = strtok(NULL, " \n");  // �ؼ��޸�3��ͳһ�ָ���
            }

            // ��֤����ѡ����һ���㷨
            if (count == 0) {
                printf("����δѡ���κ���Ч�㷨��\n");
                printf("\n���س�������...");
                while (getchar() != '\n');
                continue;
            }

            // �����㷨�б�
            SortAlgorithm chosenAlgos[MAX_SELECTIONS];
            for (int i = 0; i < count; i++) {
                if (selected[i] < ALGO_COUNT) {
                    chosenAlgos[i] = algorithms[selected[i]];
                }
                else {
                    printf("������Ч����: %d\n", selected[i]);
                    count = i;  // �ضϵ���Чλ��
                    break;
                }
            }

            // ִ�в���
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

        printf("\n���س�������...");
        while (getchar() != '\n');  // ���������
    }
    return 0;
}
