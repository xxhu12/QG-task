#include<stdio.h>
#include<stdlib.h>
#include"linkedList.h"

//L为头节点，传入二级指针
Status InitList(LinkedList* L)
{
    *L = (LinkedList)malloc(sizeof(LNode));
    if (*L == NULL) 
    {
        return ERROR;//必要的空指针判断
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
    LinkedList temp;//借用局部指针作为”垃圾桶“
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
        return ERROR;//必要的空指针判断
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
/* void是函数的返回类型。
   (*visit) 表示 visit 是一个指针。
   (ElemType e)是函数的参数类型列表。*/
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
    LNode* q = (*L)->next;   // q 指向第一个数据节点
    while (q) {
        LNode* r = q->next; // 暂存后继节点
        q->next = p;        // 反转当前节点的指针
        p = q;              // p 前移
        q = r;              // q 前移
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
