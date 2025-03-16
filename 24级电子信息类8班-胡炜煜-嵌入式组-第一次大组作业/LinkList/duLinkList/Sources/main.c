#include<stdio.h>
#include<stdlib.h>
#include"duLinkedList.h"


// 清屏函数
void clear_screen() {
    system("cls || clear");
}

//打印函数
void printNode(ElemType e) {
    printf("%d -> ", e);
}

int main(void)
{
    int choice;
    DuLinkedList L = NULL;

    while (1)
    {
        clear_screen();
        printf("\n********************************             ********************************\n");
        printf("*         1.初始化链表         *             *         2.插入新节点         *\n");
        printf("*         3.反转链表           *             *         4.输出链表           *\n");
        printf("*         5.中间链表           * 11.退出系统 *         6.判断是否成环       *\n");
        printf("*         7.删除节点           *             *         8.奇偶反转           *\n");
        printf("*         9.查询链表           *             *         10.构造循环链表      *\n");
        printf("********************************             ********************************\n");
        /*****************************************打印页面***********************************************/

        int num, data, position, list_length;
        printf("请输入对应数字(1-11): ");
        scanf_s("%d", &choice);
        if (choice < 1 || choice > 11)
        {
            printf("无效选项，请重新输入。\n");
            continue;
        }

        switch (choice)
        {
        case 1://初始化链表
            DestroyList_DuL(&L);
            if (InitList_DuL(&L) == SUCCESS)
            {

                while (1) {
                    printf("请输入要创建的节点数(1-20): ");
                    if (scanf_s("%d", &num) != 1) // 处理非数字输入
                    {
                        printf("输入错误，请输入整数。\n");
                        while (getchar() != '\n'); // 清除输入缓冲区
                        continue;
                    }
                    if (num >= 1 && num <= 20) {
                        break; // 输入正确，退出循环
                    }
                    else {
                        printf("输入错误，节点数必须在1到20之间。\n");
                    }
                }

                DuLNode* tail = L;
                for (int i = 1; i <= num; i++)
                {

                    printf("请输入第%d个节点的数据(长度不超10) :", i);
                    scanf_s("%d", &data);
                    DuLNode* Node = (DuLNode*)malloc(sizeof(DuLNode));

                    if (Node == NULL)
                    {
                        return ERROR;
                    }
                    Node->data = data;
                    Node->prior = tail;
                    Node->next = NULL;
                    tail->next = Node;
                    tail = Node;
                }
                printf("初始化完成。\n");
                system("pause");
            }
            else
            {
                printf("ERROR\n");
            }
            break;
        case 2:
            if (L == NULL)  // 检查链表是否已初始化
            {
                printf("错误：链表未初始化，请先选择选项1创建链表！\n");
                system("pause");
                break;
            }

            printf("链表内容：\n");
            TraverseList_DuL(L, printNode);
            printf("NULL\n");

            printf("请输入要插入的位置：");
            scanf_s("%d", &position);
            printf("请输入要插入的数据：");
            scanf_s("%d", &data);

            DuLNode* p = L;   // 验证输入有效性
            list_length = 0;
            while (p->next != NULL)
            {
                list_length++;
                p = p->next;
            }
            if (position < 0 || position > list_length+1)
            {
                printf("错误：无效位置，当前链表长度为%d！\n", list_length);
                system("pause");
                break;
            }

            p = L;    // 定位插入位置
            for (int i = 0; i < position-1; i++)
            {
                p = p->next;
                if (p == NULL) {
                    printf("错误：链表结构异常！\n");
                    DestroyList_DuL(&L);
                    system("pause");
                    return ERROR;
                }
            }

            DuLNode* q = (DuLNode*)malloc(sizeof(DuLNode));  // 创建新节点
            if (q == NULL)
            {
                return ERROR;
            }
            q->data = data;
            q->next = NULL;
            q->prior = NULL;

            if (InsertAfterList_DuL(p, q) == SUCCESS)  // 执行插入操作
            {
                printf("插入成功！新节点已添加到%d号位置\n", position);
            }
            else {
                printf("错误：插入失败！\n");
                free(q);
            }
            break;
        case 3:
            if (L == NULL)  // 检查链表是否已初始化
            {
                printf("错误：链表未初始化，请先选择选项1创建链表！\n");
                system("pause");
                break;
            }
            
            if (L->next == NULL || L->next->next == NULL) // 如果链表为空或只有一个节点，无需反转
            {
                printf("链表为空或只有一个节点，无需反转！\n");
                system("pause");
                break;
            }
            
            DuLNode* first = L->next;
            // 找到原链表的最后一个节点
            DuLNode* last = first;
            while (last->next != NULL) 
            {
                last = last->next;
            }
            
            DuLNode* current = first;
            DuLNode* temp = NULL;
            while (current != NULL) // 交换prior和next
            {
                temp = current->prior;
                current->prior = current->next;
                current->next = temp;
                current = current->prior;
            }
            // 头尾交换
            L->next = last;
            last->prior = L;
            first->next = NULL;

            printf("链表反转完成！\n");
            TraverseList_DuL(L, printNode);
            printf("NULL\n");
            system("pause");
            break;

        case 4:
            if (L == NULL)  // 检查链表是否已初始化
            {
                printf("错误：链表未初始化，请先选择选项1创建链表！\n");
                system("pause");
                break;
            }
            printf("链表内容：\n");
            TraverseList_DuL(L, printNode);
            printf("NULL\n");
            system("pause");
            break;

        case 7:
            if (L == NULL) // 检查链表是否已初始化
            {
                printf("错误：链表未初始化，请先创建链表！\n");
                system("pause");
                break;
            }

            printf("链表内容：\n");//打印
            TraverseList_DuL(L, printNode);
            printf("NULL\n");

            printf("请输入要删除的数据：");
            scanf_s("%d", &data);
            printf("请输入数据的位置(第几个)：");
            scanf_s("%d", &position);

            list_length = 0;//输入合法性校验
            p = L->next;
            while (p->next != NULL)
            {
                list_length++;
                p = p->next;
            }
            if (position < 0 || position > list_length+1)
            {
                printf("错误：无效位置，当前链表长度为%d！\n", list_length);
                system("pause");
                break;
            }

            DuLNode* pre = L;  // 定位前驱节点
            int i;
            for (i = 1; pre != NULL && i < position; i++)
            {
                pre = pre->next;
            }

            ElemType deleted_value;  // 执行删除操作
            if (pre != NULL && pre->next != NULL)
            {
                if (pre->next->data == data) // 验证数据一致性
                {
                    if (DeleteList_DuL(pre, &deleted_value) == SUCCESS)
                    {
                        printf("删除成功！当前链表：\n");
                        TraverseList_DuL(L, printNode);
                        printf("NULL\n");
                    }
                    else
                    {
                        printf("删除失败！\n");
                    }
                }
                else
                {
                    printf("删除失败！位置 %d 的数据不是 %d\n", position, data);
                }
            }
            else
            {
                printf("删除失败！位置 %d 无效\n", position);
            }
            system("pause");
            break;

        case 11:
            if (L != NULL)
            {
                DestroyList_DuL(&L);
            }
            printf("退出程序！\n");
            system("pause");
            return 0;
        }


    }
    
	return 0;
}
