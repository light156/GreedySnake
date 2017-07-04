// AutoDialog.cpp : 实现文件

#include "stdafx.h"
#include "GreedySnake.h"
#include "AutoDialog.h"
#include "afxdialogex.h"
#include <stack>
#include <queue>
#include <math.h>
/*
#include <io.h>  
#include <fcntl.h>  
#include <iostream>
*/

stack <Direction> dirs;
queue <Direction> copydirs;
const double sidex = 15;
const double sidey = 15;
const double width = 360;
const double height = 360;
int sss,deadline = 0;
int _x[4] = {0, -1, 0, 1};
int _y[4] = {-1, 0, 1, 0};
// CAutoDialog 对话框

IMPLEMENT_DYNAMIC(CAutoDialog, CDialogEx)

CAutoDialog::CAutoDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoDialog::IDD, pParent)
	, m_length(0)
	, m_totalvalue(0)
{

}

CAutoDialog::~CAutoDialog()
{
}

void CAutoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LENGTH_A, m_length);
	DDX_Text(pDX, IDC_TOTAL_A, m_totalvalue);
	DDX_Control(pDX, IDC_SLIDER_A, m_slider);
	DDX_Control(pDX, IDC_SPIN2, m_spin);
	DDX_Text(pDX, IDC_ROWNUMBER, m_rownumber);
}


BEGIN_MESSAGE_MAP(CAutoDialog, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CAutoDialog::OnDeltaposSpin2)
END_MESSAGE_MAP()


// CAutoDialog 消息处理程序


void CAutoDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	speed = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_A))->GetPos();
	KillTimer(1);
	if(pause % 2 == 0)
		SetTimer(1,2000/speed,NULL);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CAutoDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	srand((unsigned)time(NULL)); 
//	InitConsole();
	pDC = GetDC();
	m_slider.SetRange(10,200);
	m_slider.SetTicFreq(10);
	m_slider.SetPos((speed=100));
	m_rownumber = 15;
	SetBegin();
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CAutoDialog::SetBegin()
{
	fx = width/m_rownumber;
	fy = height/m_rownumber;

	int i = m_rownumber / 2;
	clear(snakehead);
	snakehead = NULL;
	snakehead = insert(snakehead,i,i-1,1);
	snakehead = insert(snakehead,i,i,2);
	snakehead = insert(snakehead,i,i+1,3);
	snaketail = getnode(snakehead,3);
	snakedir = UP;
	m_length = 3;
	pause = 1;
	flag = false;
	m_totalvalue = m_rownumber * m_rownumber;
	MakeFood();
	sss = m_totalvalue;
	UpdateData(FALSE);
	DrawGame();
	DrawSnake();
	DrawLine();
	if(map)
	{
		for(int i=0;i<m_rownumber-1;i++)
			delete[] map[i];
		delete[] map;
	}
	map = new Map*[m_rownumber];
	for(int i=0;i<m_rownumber;i++)
		map[i] = new Map[m_rownumber];
}
/*
void CAutoDialog::InitConsole()  
{  
    int nRet= 0;  
    FILE* fp;  
    AllocConsole();  
    nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);  
    fp = _fdopen(nRet, "w");  
    *stdout = *fp;  
    setvbuf(stdout, NULL, _IONBF, 0);  
}  
*/
void CAutoDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting	
	DrawGame();
	DrawSnake();
	DrawLine();
}


void CAutoDialog::DrawSnake()
{
	struct node *p = snakehead;
	SetColor(255,0,0);
	pDC->Ellipse(sidex+p->x*fx,sidey+p->y*fy,sidex+(p->x+1)*fx,sidey+(p->y+1)*fy);
	p=p->next;

	SetColor(255,255,0);
	while(p)
	{
		pDC->Rectangle(sidex+p->x*fx,sidey+p->y*fy,sidex+(p->x+1)*fx,sidey+(p->y+1)*fy);
		p=p->next;
	}
	UpdateData(FALSE);
}


void CAutoDialog::DrawGame()
{	
	SetColor(255,255,255);
	pDC->Rectangle(sidex,sidey,sidex+width,sidey+height);
	
	SetColor(0,255,0);
	pDC->Ellipse(sidex+snakefood.x*fx,sidey+snakefood.y*fy,sidex+(snakefood.x+1)*fx,sidey+(snakefood.y+1)*fy);
}


void CAutoDialog::DrawLine()
{
	pDC->SetDCPenColor(RGB(105,105,105));
	pDC->SelectStockObject(DC_PEN);
	for(int i=0;i<m_rownumber;i++)
	{
		pDC->MoveTo(sidex, sidey+fy*i);
		pDC->LineTo(sidex+width, sidey+fy*i);
		pDC->MoveTo(sidex+fx*i, sidey);
		pDC->LineTo(sidex+fx*i, sidey+height);
	}
	pDC->MoveTo(sidex, sidey+height);
	pDC->LineTo(sidex+width, sidey+height);
	pDC->MoveTo(sidex+width, sidey);
	pDC->LineTo(sidex+width, sidey+height);
}


//不弹出对话框键盘 
BOOL CAutoDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
    {
		UINT key = pMsg->wParam;
		if(snakedir != DEAD)
		{
			if(key == VK_SPACE)
			{
				pause++;
				m_spin.ShowWindow(FALSE);
			}
			if(pause % 2 == 0)
				SetTimer(1,2000/speed,NULL);
			else
				KillTimer(1);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAutoDialog::Redraw()
{
	last = snaketail;
	snakehead = move(snakehead, &snaketail, snakedir);
	deadline++;
	if(!JudgeDead())
	{
		if(snakehead->x == snakefood.x && snakehead->y == snakefood.y)
		{
			snaketail = insertlast(snaketail, last);
			m_length++;
			UpdateData(FALSE);
			if(m_length < m_totalvalue)
			{
				MakeFood();
				deadline = 0;
			}
			else
			{
				SetColor(255,0,0);
				struct node *p=snakehead;
				pDC->Ellipse(sidex+p->x*fx,sidey+p->y*fy,sidex+(p->x+1)*fx,sidey+(p->y+1)*fy);
				SetColor(255,255,0);
				p=p->next;
				pDC->Rectangle(sidex+p->x*fx,sidey+p->y*fy,sidex+(p->x+1)*fx,sidey+(p->y+1)*fy);
				DrawLine();
				KillTimer(1);
				AfxMessageBox(_T("恭喜你！"));
				snakedir = DEAD;
				m_slider.EnableWindow(FALSE); 
				return;
			}
			SetColor(0,255,0);
			pDC->Ellipse(sidex+snakefood.x*fx,sidey+snakefood.y*fy,
				sidex+(snakefood.x+1)*fx,sidey+(snakefood.y+1)*fy);
		}
		else
		{
			SetColor(255,255,255);
			pDC->Rectangle(sidex+last->x*fx,sidey+last->y*fy,
				sidex+(last->x+1)*fx,sidey+(last->y+1)*fy);
			free(last);
		}
		SetColor(255,0,0);
		struct node *p=snakehead;
		pDC->Ellipse(sidex+p->x*fx,sidey+p->y*fy,sidex+(p->x+1)*fx,sidey+(p->y+1)*fy);
		SetColor(255,255,0);
		p=p->next;
		pDC->Rectangle(sidex+p->x*fx,sidey+p->y*fy,sidex+(p->x+1)*fx,sidey+(p->y+1)*fy);
		DrawLine();
	}
	else
	{
		KillTimer(1);
		AfxMessageBox(_T("非常遗憾！"));
		snakedir = DEAD;
	}
}


void CAutoDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!flag) 
	{
		SetMap();
		if(AStar(food))
		{
			if(m_length+1 == m_totalvalue)
			{
				snakedir = dirs.top();
				Redraw();
				return;
			}
			copyhead = copy(snakehead, copyhead);
			while(!dirs.empty())
			{
				snakedir = dirs.top();
				copydirs.push(snakedir);
				dirs.pop();
				if(!dirs.empty())
				{
					last = snaketail;
					snakehead = move(snakehead, &snaketail, snakedir);
					free(last);
				}
			}
			snakehead = insert(snakehead, food.x, food.y, 1);    //这时已经移动完的蛇
			SetMap();
			snakehead = copy(copyhead, snakehead);
			snaketail = getnode(snakehead, m_length);
			if(AStar(tail))
				flag = true;
		}
		if(!flag)
		{
			while(!copydirs.empty())
				copydirs.pop();
			GetFurthest();
			while(!dirs.empty())
				dirs.pop();
			Redraw();
		}
	}
	if(flag)
	{
		snakedir = copydirs.front(); 
		copydirs.pop();
		Redraw();
		flag = false;
		while(!dirs.empty())
			dirs.pop();
		while(!copydirs.empty())
			copydirs.pop();
	}
}


void CAutoDialog::OnDestroy()
{
	KillTimer(1);
	for(int i=0;i<m_rownumber;i++)
		delete[] map[i];
	delete[] map;
	clear(snakehead);
	clear(copyhead);
	ReleaseDC(pDC);
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}

bool CAutoDialog::JudgeDead()
{
	if(snakehead->x == -1 || snakehead->x == m_rownumber
		|| snakehead->y == -1 || snakehead->y ==m_rownumber)
		return true;
	struct node *p = snakehead->next;
	while((p->x != snakehead->x)||(p->y != snakehead->y))
	{
		p = p->next;
		if (!p)
			return false;
	}
	return true;
}

void CAutoDialog::MakeFood()
{
	int result = rand() % m_totalvalue;
	snakefood.x = result / m_rownumber;
	snakefood.y = result % m_rownumber;
	snakefood.next = NULL;
	if (!(judgecomp(snakehead, snakefood.x, snakefood.y)))
		MakeFood();
}

void  CAutoDialog::SetColor(int r, int g, int b)
{
	pDC->SetDCBrushColor(RGB(r,g,b));
	pDC->SelectStockObject(DC_BRUSH);
	pDC->SetDCPenColor(RGB(r,g,b));
	pDC->SelectStockObject(DC_PEN);
}

bool CAutoDialog::JudgeIn(int x,int y)
{
	if(x>=0 && x<m_rownumber && y>=0 && y<m_rownumber)
		return true;
	return false;
}

int CAutoDialog::Manhattan(int x, int y, int goalx, int goaly)     //加权曼哈顿  
{   
	int distance,dx,dy;
	dx = abs(x-goalx);
	dy = abs(y-goaly);
	if(dx>dy)
		distance = 10*dx+6*dy;
	else
		distance = 6*dx+10*dy;
	return distance;
}


void CAutoDialog::SetMap()
{
	for(int i=0;i<m_rownumber;i++)
	{
		for(int j=0;j<m_rownumber;j++)
		{
			map[i][j].x=j;
			map[i][j].y=i;
			map[i][j].visit=0;
			map[i][j].pre=NULL;
			map[i][j].ch = 'O';
		}
	}
	struct node *p = snakehead;
 	map[p->y][p->x].gx=0;
	map[p->y][p->x].visit=1;
	map[p->y][p->x].ch='S';
	start=map[p->y][p->x];
	p=p->next;
	while(p->next)
	{
		map[p->y][p->x].ch = 'X';
		p=p->next;
	}
	map[p->y][p->x].ch = 'E';
	tail = map[p->y][p->x];

	map[snakefood.y][snakefood.x].ch = 'F';
	food = map[snakefood.y][snakefood.x];
}


bool operator<(Map a,Map b)
{
	return a.gx+a.hx>b.gx+b.hx;
}


bool CAutoDialog::AStar(struct Map goal)
{
	Direction dir;
	priority_queue <Map> q;
	q.push(start);
	Map now;
	int xx,yy;
	while(!q.empty())
	{
		now=q.top();
		q.pop();
		if(now.x == goal.x && now.y == goal.y)
		{
			Map *p=map[goal.y][goal.x].pre, *r=&goal;
			while(p)
			{
				if(p->x == r->x)
				{
					if(p->y == r->y-1)
						dir = DOWN;
					else
						dir = UP;
				}
				else
				{
					if(p->x == r->x-1)
						dir = RIGHT;
					else
						dir = LEFT;
				}
				dirs.push(dir);

				r=p;
				p=p->pre;
			}
			return true;
		}
		for(int i=0;i<4;i++)
		{
			xx=now.x+_x[i];
			yy=now.y+_y[i];
			if(!JudgeIn(xx,yy) || map[yy][xx].ch=='X' || map[yy][xx].visit==1)
				 continue;
			map[yy][xx].pre=&map[now.y][now.x];
			map[yy][xx].visit=1;
			map[yy][xx].hx=Manhattan(xx,yy,goal.x,goal.y);//选择想用的h(x)
			map[yy][xx].gx=now.gx+1;

			q.push(map[yy][xx]);
		}
	}
	return false;
}


void CAutoDialog::GetFurthest()
{  
	int xx,yy,max=0,temp,seconddir=-1;
	for(int i=0;i<4;i++)
	{
		xx=snakehead->x+_x[i];
		yy=snakehead->y+_y[i];
		last = snaketail;
		snakehead = move(snakehead, &snaketail, (Direction)i);
		struct node *head = snakehead;
		if(JudgeDead()) 
		{
			snakehead = erase(snakehead,1);
			snaketail = insertlast(snaketail, last);
			free(head);
			continue;
		}
		SetMap();
		if(!AStar(tail))
		{
			while(!dirs.empty())
				dirs.pop();
			snakehead = erase(snakehead,1);
			snaketail = insertlast(snaketail, last);
			free(head);
			continue;
		}
		snakehead = erase(snakehead,1);
		snaketail = insertlast(snaketail, last);
		free(head);
		temp = Manhattan(xx, yy, snakefood.x, snakefood.y);
		if(temp>=max)
		{
			if(max!=0)
				seconddir = snakedir;
			max = temp;
			snakedir = (Direction)i;
		}
	}
	if(deadline > m_totalvalue)
	{
		if(seconddir != -1)
		{
			snakedir = (Direction)seconddir;
			deadline = 0;
		}
	}
}

void CAutoDialog::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(pNMUpDown-> iDelta == -1) // 如果此值为1 , 说明点击了Spin的往下箭头 
	{
		if(m_rownumber < 25)
		{
			m_rownumber++;	
			SetBegin();
		}
	}
	else if(pNMUpDown-> iDelta == 1)
	{
		if(m_rownumber > 10)
		{
			m_rownumber--;
			SetBegin();
		}
	}
	UpdateData(FALSE);
}
