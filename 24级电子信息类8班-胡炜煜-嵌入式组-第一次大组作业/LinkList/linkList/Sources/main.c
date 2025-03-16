#include<stdio.h>
#include<stdlib.h>
#include"linkedList.h"


// ��������
void clear_screen() {
    system("cls || clear");
}

//��ӡ����
void printNode(ElemType e) {
    printf("%d -> ", e);
}



int main(void)
{
    int choice;
    LinkedList L= NULL;

    while (1)
    {   
        clear_screen();
        printf("\n********************************             ********************************\n");
        printf("*         1.��ʼ������         *             *         2.�����½ڵ�         *\n");
        printf("*         3.��ת����           *             *         4.�������           *\n");
        printf("*         5.�м�����           * 11.�˳�ϵͳ *         6.�ж��Ƿ�ɻ�       *\n");
        printf("*         7.ɾ���ڵ�           *             *         8.��ż��ת           *\n");
        printf("*         9.��ѯ����           *             *         10.����ѭ������      *\n");
        printf("********************************             ********************************\n");
        /*****************************************��ӡҳ��***********************************************/

        int num,data,position, list_length;
        printf("�������Ӧ����(1-11): ");
        scanf_s("%d", &choice);
        if (choice < 1 || choice > 11) 
        {
            printf("��Чѡ����������롣\n");
            continue;
        }

        switch (choice)
        {
        case 1://��ʼ������
            DestroyList(&L);
            if (InitList(&L) == SUCCESS)
            {
                
                while (1) {
                    printf("������Ҫ�����Ľڵ���(1-20): ");
                    if (scanf_s("%d", &num) != 1) // �������������
                    { 
                        printf("�������������������\n");
                        while (getchar() != '\n'); // ������뻺����
                        continue;
                    }
                    if (num >= 1 && num <= 20) {
                        break; // ������ȷ���˳�ѭ��
                    }
                    else {
                        printf("������󣬽ڵ���������1��20֮�䡣\n");
                    }
                }

                LNode* tail = L;
                for (int i = 1; i <= num; i++) 
                {
                    
                    printf("�������%d���ڵ������(���Ȳ���10) :",i); 
                    scanf_s("%d", &data);
                    LNode* Node = (LNode*)malloc(sizeof(LNode));

                    if (Node == NULL) 
                    {
                      return ERROR;
                    }
                      Node->data = data;
                      Node->next = NULL;  
                      tail->next = Node;
                      tail = Node;
                }
                printf("��ʼ����ɡ�\n");
                system("pause");
            }
            else
            {
                printf("ERROR\n");
            }
            break;
        case 2:
            if (L == NULL)  // ��������Ƿ��ѳ�ʼ��
            {
                printf("��������δ��ʼ��������ѡ��ѡ��1��������\n");
                system("pause");
                break;
            }
            printf("�������ݣ�\n");//��ӡ
            TraverseList(L, printNode);
            printf("NULL\n");

            printf("������Ҫ�����λ�ã�");
            scanf_s("%d", &position);
            printf("������Ҫ��������ݣ�");
            scanf_s("%d", &data);

            LNode* p = L;   // ��֤������Ч��
            list_length = 0;
            while (p->next != NULL) 
            {
                list_length++;
                p = p->next;
            }
            if (position < 0 || position > list_length+1) 
            {
                printf("������Чλ�ã���ǰ������Ϊ%d��\n", list_length);
                system("pause");
                break;
            }

            p = L;    // ��λ����λ��
            for (int i = 0; i < position-1; i++) 
            {
                p = p->next;
                if (p == NULL) {
                    printf("��������ṹ�쳣��\n");
                    DestroyList(&L);
                    system("pause");
                    return ERROR;
                }
            }

             LNode* q = (LNode*)malloc(sizeof(LNode));  // �����½ڵ�
             if (q == NULL)
             {
                 return ERROR;
             }
            q->data = data;
            q->next = NULL;
           
            if (InsertList(p, q) == SUCCESS)  // ִ�в������
            {
                printf("����ɹ����½ڵ�����ӵ�%d��λ��\n", position);
            }
            else {
                printf("���󣺲���ʧ�ܣ�\n");
                free(q);
            }
            system("pause");
            break;
        case 3:
            if (L == NULL)  // ��������Ƿ��ѳ�ʼ��
            {
                printf("��������δ��ʼ��������ѡ��ѡ��1��������\n");
                system("pause");
                break;
            }

            if (ReverseList(&L) == SUCCESS) {  
                printf("����ת�ɹ���\n");
            }
            else {
                printf("��������ͷ�ڵ���Ч����תʧ�ܣ�\n");
            }
            system("pause");
            break;
        case 4:
            if (L == NULL)  // ��������Ƿ��ѳ�ʼ��
            {
                printf("��������δ��ʼ��������ѡ��ѡ��1��������\n");
                system("pause");
                break;
            }
            printf("�������ݣ�\n");
            TraverseList(L, printNode);
            printf("NULL\n");
            system("pause");
            break;
        
        case 7:
            if (L == NULL) // ��������Ƿ��ѳ�ʼ��
            {
                printf("��������δ��ʼ�������ȴ�������\n");
                system("pause");
                break;
            }
            
            printf("�������ݣ�\n");//��ӡ
            TraverseList(L, printNode);
            printf("NULL\n");

            printf("������Ҫɾ�������ݣ�");
            scanf_s("%d", &data);
            printf("���������ݵ�λ��(�ڼ���)��");
            scanf_s("%d", &position);

            list_length = 0;//����Ϸ���У��
            p = L->next;
            while (p->next != NULL)
            {
                list_length++;
                p = p->next;
            }
            if (position < 0 || position > list_length+1)
            {
                printf("������Чλ�ã���ǰ������Ϊ%d��\n", list_length);
                system("pause");
                break;
            }
            
            LNode* pre = L;  // ��λǰ���ڵ�
            int i;
            for (i = 1; pre != NULL && i < position; i++) 
            {
                pre = pre->next;
            }
            
            ElemType deleted_value;  // ִ��ɾ������
            if (pre != NULL && pre->next != NULL) 
            {
                if (pre->next->data == data) // ��֤����һ����
                {
                    if (DeleteList(pre, &deleted_value) == SUCCESS) 
                    {
                        printf("ɾ���ɹ�����ǰ����\n");
                        TraverseList(L, printNode);
                        printf("NULL\n");
                    }
                    else 
                    {
                        printf("ɾ��ʧ�ܣ�\n");
                    }
                }
                else 
                {
                    printf("ɾ��ʧ�ܣ�λ�� %d �����ݲ��� %d\n", position, data);
                }
            }
            else 
            {
                printf("ɾ��ʧ�ܣ�λ�� %d ��Ч\n", position);
            }
            system("pause");
            break;

        case 11:
            if (L != NULL) 
            {
                DestroyList(&L); 
            }
            printf("�˳�����\n");
            system("pause");   
            return 0;           
        }


    }
    return 0;
}    
                
                    
            
            
            

  
  
            
           






