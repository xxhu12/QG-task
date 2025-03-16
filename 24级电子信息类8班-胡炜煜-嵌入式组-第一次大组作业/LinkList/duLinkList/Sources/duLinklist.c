#include<stdio.h>
#include<stdlib.h>
#include"duLinkedList.h"


Status InitList_DuL(DuLinkedList* L)
{
	*L = (DuLinkedList)malloc(sizeof(DuLNode));
	if (*L == NULL)
	{
		return ERROR;
	}
	(*L)->prior = NULL;
	(*L)->next = NULL;
	return SUCCESS;
}


void DestroyList_DuL(DuLinkedList* L)
{
	if (L == NULL || *L == NULL)
		return;

	DuLinkedList current = *L;
	while (current != NULL)
	{
		DuLinkedList temp = current;
		current = current->next;
		free(temp);
	}
	*L = NULL;
}

Status InsertBeforeList_DuL(DuLNode* p, DuLNode* q)
{
	if (p == NULL || q == NULL)
	{
		return ERROR;
	}

	q->prior = p->prior;
	q->next = p;

	if (p->prior != NULL)
	{
	p->prior->next = q;
	p->prior = q;
    }
	
	return SUCCESS;
}

Status InsertAfterList_DuL(DuLNode* p, DuLNode* q)
{
	if (p == NULL || q == NULL)
	{
		return ERROR;
	}
	q->next = p->next;
	q->prior = p;

	if (p->next != NULL)
	{
		p->next->prior = q;
		p->next = q;
	}
	return SUCCESS;
}

Status DeleteList_DuL(DuLNode* p, ElemType* e)
{
	if (p == NULL)
	{
		return ERROR;
	}
	if (p->prior != NULL)
	{
		p->prior->next = p->next;
	}
	if (p->next != NULL)
	{
		p->next->prior = p->prior;
	}
	*e = p->data;
	free(p);

	return SUCCESS;
}

void TraverseList_DuL(DuLinkedList L, void (*visit)(ElemType e))
{
	DuLNode* p = L->next;
	while (p != NULL)
	{
		visit(p->data);
		p = p->next;
	}
}		

