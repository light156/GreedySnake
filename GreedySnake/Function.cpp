#include "Function.h"
#include "stdafx.h"

int getsize(struct node *head)
{
	if(head == NULL)
	{
		return 0;
	}
	struct node *p = head;
	int size = 0;
	while(p != NULL)
	{
		size++;
		p = p->next;
	}
	return size;
}


struct node *getnode(struct node *head, int pos)
{
	if((pos > (getsize(head)+1))||(pos < 1))
		return NULL;
	struct node *p = head;
	int i = pos-1;
	while(i != 0)
	{
		p = p->next;
		i--;
	}
	return p;
}


struct node *insert(struct node *head, int x, int y, int pos)
{
	if((pos > (getsize(head)+1))||(pos < 1))
		return head;
	struct node *p = head, *q=head, *s = (struct node*)malloc(sizeof(struct node));
	s->x = x;
	s->y = y;
	s->former = NULL;
	if(pos == 1)
	{
		s->next = head;
		if(head)
			head->former = s;
		return s;
	}
	else 
	{
		int i = pos-1;
		while(i != 0)
		{
			q = p;
			p = p->next;
			i--;
		}
		q->next = s;
		s->next = p;
		if(p)
			p->former = s;
		s->former = q;
		return head;
	}
}


struct node *erase(struct node *head, int pos) 
{
	if((pos > getsize(head))||(pos < 1))
		return NULL;
	struct node *p = head, *q=head;
	if(pos == 1)
	{
		head = head->next;
		if(head)
			head->former = NULL;
		return head;
	}
	else
	{
		int i = pos-1;
		while(i != 0)
		{
			q = p;
			p = p->next;
			i--;
		}
		q->next = p->next;
		if(p->next)
			(p->next)->former = q;
		return head;
	}
}


struct node *insertlast(struct node *last,struct node *p)
{
	last->next = p;
	p->next = NULL;
	p->former = last;
	return p;
}


struct node *eraselast(struct node *last)
{
	struct node *p = last->former;
	p->next = NULL;
	return p;
}


struct node *move(struct node *head, struct node **last, Direction dir)
{
	int _x[4] = {0, -1, 0, 1}, x;
	int _y[4] = {-1, 0, 1, 0}, y;
	if(head == NULL)
	{
		return NULL;
	}
	x = head->x + _x[dir];
	y = head->y + _y[dir];
	head = insert(head, x, y, 1);
	*last = eraselast(*last);
	return head;
} 


//判断食物是否在蛇身上，若在则返回false，若不在则返回true
bool judgecomp(struct node *head, int x, int y)
{
	if(head == NULL)
	{
		return true;
	}
	struct node *p = head;
	while(p->x!=x || p->y!=y)
	{
		p = p->next;
		if(!p)
			return true;
	}
	return false;
}


Direction JudgeDirection(struct node *head)
{
	struct node *p = head->next;
	if(head->x == p->x)
	{
		if(head->y == p->y-1)
			return UP;
		else
			return DOWN;
	}
	else
	{
		if(head->x == p->x-1)
			return LEFT;
		else
			return RIGHT;
	}
}


void clear(struct node *head)
{
	struct node *p = head, *q;
	while(p)
	{
		q = p;
		p = p->next;
		free(q);
	}
}


struct node *copy(struct node *head, struct node *copyhead)
{
	clear(copyhead);
	struct node *p = head, *q = (struct node*)malloc(sizeof(struct node)), *r;
	q->x = p->x;
	q->y = p->y;
	q->former = NULL;
	copyhead = q;
	while(p->next)
	{
		p = p->next;
		r = (struct node*)malloc(sizeof(struct node));
		r->x = p->x;
		r->y = p->y;
		q->next = r;
		r->former = q;
		q = r;
	}
	q->next = NULL;
	return copyhead;
}