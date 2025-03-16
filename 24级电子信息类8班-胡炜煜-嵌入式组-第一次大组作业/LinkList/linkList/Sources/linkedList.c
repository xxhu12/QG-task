#include<stdio.h>
#include<stdlib.h>
#include"linkedList.h"

//LΪͷ�ڵ㣬�������ָ��
Status InitList(LinkedList* L)
{
    *L = (LinkedList)malloc(sizeof(LNode));
    if (*L == NULL) 
    {
        return ERROR;//��Ҫ�Ŀ�ָ���ж�
    }
    (*L)->next = NULL;
    return SUCCESS;
}

void DestroyList(LinkedList* L)
{
    if (L == NULL || *L == NULL) 
    {
        return;
    }
    LinkedList temp;//���þֲ�ָ����Ϊ������Ͱ��
    while (*L != NULL) 
    {
        temp = *L;
        *L = (*L)->next;
        free(temp);
    }
    *L = NULL;
}

Status InsertList(LNode* p, LNode* q)
{
    if (p == NULL || q == NULL)
    {
        return ERROR;//��Ҫ�Ŀ�ָ���ж�
    }
    q->next = p->next;
    p->next = q;
    return SUCCESS;
}

Status DeleteList(LNode* p, ElemType* e)
{
    if (p == NULL || p->next == NULL)
    {
        return ERROR;
    }
    LNode* temp;
    temp = p->next;
    *e = temp->data;
    p->next = temp->next;
    free(temp);
    return SUCCESS;
}
/* void�Ǻ����ķ������͡�
   (*visit) ��ʾ visit ��һ��ָ�롣
   (ElemType e)�Ǻ����Ĳ��������б�*/
void TraverseList(LinkedList L, void (*visit)(ElemType e))
{
    LNode* p = L->next;
    while (p != NULL) 
    {
        visit(p->data);
        p = p->next;
    }
}

Status SearchList(LinkedList L, ElemType e)
{
    LNode* p = L->next;
    while (p != NULL) 
    {
        if (p->data == e) 
        {
            return SUCCESS;
        }
        p = p->next;
    }
    return ERROR;
}

Status ReverseList(LinkedList* L)
{
    if (*L == NULL) 
    { 
        return ERROR;
    }

    LNode* p = NULL;
    LNode* q = (*L)->next;   // q ָ���һ�����ݽڵ�
    while (q) {
        LNode* r = q->next; // �ݴ��̽ڵ�
        q->next = p;        // ��ת��ǰ�ڵ��ָ��
        p = q;              // p ǰ��
        q = r;              // q ǰ��
    }
    (*L)->next = p;            
    return SUCCESS;
}





Status IsLoopList(LinkedList L)
{

}

LNode* ReverseEvenList(LinkedList* L)
{

}

LNode* FindMidNode(LinkedList* L)
{

}
