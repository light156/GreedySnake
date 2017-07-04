#pragma once
#include <malloc.h>
#include <fstream>
using namespace std;

struct node
{
	int x;
	int y;
	struct node* next;
	struct node* former;
};

struct parameter
{
	int rownum;
	int speed;
	bool ifline;
	bool ifobstacle;
	bool ifdraw;
};

struct Map
{
	char ch;	   //把障碍物设为了x
	int x,y;
	double hx,gx;  
	bool visit;    //是否被访问过
	Map *pre;
};

enum Direction{UP, LEFT, DOWN, RIGHT, DEAD};

int getsize(struct node *head);
struct node *getnode(struct node *head, int pos);
struct node *insert(struct node *head, int x, int y, int pos);
struct node *erase(struct node *head, int pos);
struct node *insertlast(struct node *last,struct node *p);
struct node *eraselast(struct node *last);
struct node *move(struct node *head, struct node **last, Direction dir);
struct node *copy(struct node *head, struct node *copyhead);
bool judgecomp(struct node *head, int x, int y);
Direction JudgeDirection(struct node *head);
void clear(struct node *head);