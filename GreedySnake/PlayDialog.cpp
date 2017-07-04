// PlayDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GreedySnake.h"
#include "PlayDialog.h"
#include "afxdialogex.h"
#include "GreedySnakeDlg.h"
#include <time.h>
const double sidex = 15;
const double sidey = 15;
const double width = 360;
const double height = 360;
// CPlayDialog 对话框

IMPLEMENT_DYNAMIC(CPlayDialog, CDialogEx)

CPlayDialog::CPlayDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlayDialog::IDD, pParent)
	, m_totalvalue(0)
	, m_length(3)
	, m_rownumber(0)
{

}

CPlayDialog::~CPlayDialog()
{
}

void CPlayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRAW_BUTTON, m_drawsnake);
	DDX_Text(pDX, IDC_TOTAL_STATIC, m_totalvalue);
	DDX_Text(pDX, IDC_LENGTH_STATIC, m_length);
	DDX_Text(pDX, IDC_ROWNUM, m_rownumber);
}


BOOL CPlayDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	srand((unsigned)time(NULL)); 
	pDC = GetDC();
	fx = width/snakepara.rownum;
	fy = height/snakepara.rownum;
	pause = 1;
	snakedir = UP;

	if(!ifcontinue)
	{
		if(!snakepara.ifdraw)
		{
			int i = snakepara.rownum / 2;
			m_drawsnake.EnableWindow(FALSE);
			m_drawsnake.ShowWindow(FALSE);
			snakehead = NULL;
			snakehead = insert(snakehead,i,i-1,1);
			snakehead = insert(snakehead,i,i,2);
			snakehead = insert(snakehead,i,i+1,3);
			snaketail = getnode(snakehead,3);
		}
		else
		{
			snakehead = NULL;
			former.x = -1;
			m_drawsnake.EnableWindow(TRUE);
			m_drawsnake.ShowWindow(TRUE);
		}
		snakeobstacle = NULL;
		if(snakepara.ifobstacle)
		{
			while(getsize(snakeobstacle) < snakepara.rownum-2)
				MakeObstacle();
		}
		MakeFood();
	}
	else	
	{
		m_drawsnake.EnableWindow(FALSE);
		m_drawsnake.ShowWindow(FALSE);
	}
	m_rownumber = snakepara.rownum;
	m_totalvalue = snakepara.rownum * snakepara.rownum - getsize(snakeobstacle);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常: OCX 属性页应返回 FALSE
}


BEGIN_MESSAGE_MAP(CPlayDialog, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DRAW_BUTTON, &CPlayDialog::OnBnClickedDrawButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CPlayDialog 消息处理程序


void CPlayDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawGame();
	if(!snakepara.ifdraw)
		DrawSnake();
	DrawLine();
}


void CPlayDialog::SetParameter(struct parameter para)
{
	snakepara.rownum = para.rownum;
	snakepara.speed = para.speed;
	snakepara.ifobstacle = para.ifobstacle;
	snakepara.ifline = para.ifline;
	snakepara.ifdraw = para.ifdraw;
}


void CPlayDialog::DrawSnake()
{
	snakedir = JudgeDirection(snakehead); 
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


void CPlayDialog::DrawGame()
{	
	SetColor(255,255,255);
	pDC->Rectangle(sidex,sidey,sidex+width,sidey+height);
	
	SetColor(0,255,0);
	pDC->Ellipse(sidex+snakefood.x*fx,sidey+snakefood.y*fy,sidex+(snakefood.x+1)*fx,sidey+(snakefood.y+1)*fy);

	if(snakepara.ifobstacle)
	{
		SetColor(0,0,0);
		struct node *p = snakeobstacle;
		while(p)
		{
			pDC->Rectangle(sidex+p->x*fx,sidey+p->y*fy,sidex+(p->x+1)*fx,sidey+(p->y+1)*fy);
			p = p->next;
		}
	}
}


void CPlayDialog::DrawLine()
{
	if(snakepara.ifline)
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
}


//不弹出对话框键盘
BOOL CPlayDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
    {
		if(!snakepara.ifdraw)
		{
			UINT key = pMsg->wParam;
			if(snakedir != DEAD)
			{
				switch(key)
				{
				case(VK_LEFT):
					if(snakedir != RIGHT)
						snakedir = LEFT;
					break;
				case(VK_RIGHT):
					if(snakedir != LEFT)
						snakedir = RIGHT;
					break;
				case(VK_UP):
					if(snakedir != DOWN)
						snakedir = UP;
					break;
				case(VK_DOWN):
					if(snakedir != UP)
						snakedir = DOWN;
					break;
				case(VK_SPACE):
					pause++;
					KillTimer(1);
					break;
				}
				if(pause == 0 || pause == 1)
				{
					if(snakedir + key != 40 && key >= VK_LEFT && key <= VK_DOWN)
					{		
						KillTimer(1);
						SetTimer(1,2000/snakepara.speed,NULL);
						Redraw();
						pause = 0;
					}
				}
				else
				{
					pause = 0;
					SetTimer(1,2000/snakepara.speed,NULL);
				}
			}
		}
		if(snakedir != DEAD)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPlayDialog::Redraw()
{
	struct node *last = snaketail;
	snakehead = move(snakehead, &snaketail, snakedir);
	if(!JudgeDead())
	{
		if(snakehead->x == snakefood.x && snakehead->y == snakefood.y)
		{
			snaketail = insertlast(snaketail, last);
			m_length++;
			UpdateData(FALSE);
			if(m_length+snakepara.rownum < snakepara.rownum * snakepara.rownum+2)
				MakeFood();
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
				free(last);
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


void CPlayDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
	Redraw();
}


void CPlayDialog::OnDestroy()
{
	KillTimer(1);
	ReleaseDC(pDC);
	CGreedySnakeDlg *parent = (CGreedySnakeDlg*)GetParent();
	if(snakepara.ifdraw || snakedir == DEAD)
	{
		remove("UserData.sna");
		parent->m_continuebutton.EnableWindow(FALSE);
	}
	else
	{	
		ofstream file("UserData.sna", ios_base::out|ios_base::binary);
		SaveFile(&file);
		file.close();
		parent->m_continuebutton.EnableWindow(TRUE);
	}
	clear(snakehead);
	clear(snakeobstacle);
	snakehead = NULL;
	snakeobstacle = NULL;
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}

 
bool CPlayDialog::JudgeDead()
{
	if(snakehead->x == -1 || snakehead->x == snakepara.rownum 
		|| snakehead->y == -1 || snakehead->y ==snakepara.rownum)
		return true;
	struct node *p = snakehead, *q = snakeobstacle;
	while(q && ((p->x != q->x)||(p->y != q->y)))
		q = q->next;
	if(q)
		return true;
	else
	{
		p = p->next;
		while((p->x != snakehead->x)||(p->y != snakehead->y))
		{
			p = p->next;
			if (!p)
				return false;
		}
		return true;
	}
}


void CPlayDialog::MakeFood()
{
	int result = rand() % (snakepara.rownum * snakepara.rownum);
	snakefood.x = result / snakepara.rownum;
	snakefood.y = result % snakepara.rownum;
	snakefood.next = NULL;
	if (!(judgecomp(snakehead, snakefood.x, snakefood.y)) ||
		!(judgecomp(snakeobstacle, snakefood.x, snakefood.y)))
		MakeFood();
}


void CPlayDialog::MakeObstacle()
{
	int effnum = snakepara.rownum-2;
	int result = rand() % (effnum * effnum);
	int x = result / effnum +1;
	int y = result % effnum +1;
	if(judgecomp(snakehead, x, y) && judgecomp(snakeobstacle, x, y))
		snakeobstacle = insert(snakeobstacle, x, y, 1);
	else
		MakeObstacle();
}


void CPlayDialog::SaveFile(ofstream *file)
{
	file->clear();
	struct node *p=snakehead;
	(*file)<<m_length<<"\r\n";
	while(p)
	{
		(*file)<<p->x<<' '<<p->y<<"\r\n";
		p = p->next;
	}
	(*file)<<snakefood.x<<' '<<snakefood.y<<"\r\n";
	(*file)<<snakepara.rownum<<"\r\n";
	(*file)<<snakepara.speed<<"\r\n";
	if(snakepara.ifline)
		(*file)<<1<<"\r\n";
	else
		(*file)<<0<<"\r\n";
	if(snakepara.ifobstacle)
	{
		(*file)<<getsize(snakeobstacle)<<"\r\n";
		p = snakeobstacle;
		while(p)
		{
			(*file)<<p->x<<' '<<p->y<<"\r\n";
			p = p->next;
		} 
	}
	else
		(*file)<<0<<"\r\n";
}


void CPlayDialog::ReadFile()
{
	ifstream file("UserData.sna", ios_base::in|ios_base::binary);
	snakehead = NULL;
	snakeobstacle = NULL;
	int x, y, temp;
	file>>temp;
	m_length = 0;
	while(temp)
	{
		file>>x>>y;
		snakehead = insert(snakehead, x, y, m_length+1);
		m_length++;
		temp--;
	}
	snaketail = getnode(snakehead, m_length);
	file>>snakefood.x>>snakefood.y>>snakepara.rownum>>snakepara.speed;
	snakepara.ifdraw = false;
	file>>temp;
	if(temp)
		snakepara.ifline = true;
	else
		snakepara.ifline = false;
	file>>temp;
	if(!temp)
	{
		snakepara.ifobstacle = false;
		snakeobstacle = NULL;
	}
	else
	{
		snakepara.ifobstacle = true;
		while(temp)
		{
			file>>x>>y;
			snakeobstacle = insert(snakeobstacle, x, y, getsize(snakeobstacle)+1);
			temp--;
		}
	}
}


void CPlayDialog::OnBnClickedDrawButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CString name;
	if(getsize(snakehead) < 2)
	{
		AfxMessageBox(_T("还没画好玩个毛线！"));
	}
	else
	{
		snakepara.ifdraw = false;
		snakedir = JudgeDirection(snakehead);
		snaketail = getnode(snakehead, m_length);
		m_drawsnake.EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->SetFocus();
	}
}


void CPlayDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(snakepara.ifdraw)
	{
		if (JudgeSquare(former.x, former.y))
		{
			SetColor(255,255,255);
			if(!snakehead)
			{
				pDC->Rectangle(sidex+former.x*fx,sidey+former.y*fy,
					sidex+(former.x+1)*fx,sidey+(former.y+1)*fy);
			}
			else
				DrawTempBody();
			DrawLine();
		}
		GetSquare(point);
		if (JudgeSquare(mouse.x, mouse.y))
		{
			if(!snakehead)
			{
				SetColor(255,0,0);
				pDC->Ellipse(sidex+mouse.x*fx,sidey+mouse.y*fy,
					sidex+(mouse.x+1)*fx,sidey+(mouse.y+1)*fy);
			}
			else
			{
				SetColor(255,255,0);
				DrawTempBody();
			}
			DrawLine();
			former = mouse;
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CPlayDialog::DrawTempBody()
{
	int a;
	if(mouse.x == temp.x)
	{
		a = mouse.y - temp.y;
		if(a > 0)
		{
			for(int i=1; i<=a; i++)	
			{
				if(!JudgeSquare(temp.x, temp.y+i))
					break;
				pDC->Rectangle(sidex+temp.x*fx,sidey+(temp.y+i)*fy,
						sidex+(temp.x+1)*fx,sidey+(temp.y+i+1)*fy);
			}
		}
		else
		{
			for(int i=-1; i>=a; i--)
			{
				if(!JudgeSquare(temp.x, temp.y+i))
					break;
				pDC->Rectangle(sidex+temp.x*fx,sidey+(temp.y+i)*fy,
						sidex+(temp.x+1)*fx,sidey+(temp.y+i+1)*fy);
			}		
				
		}
	}
	if(mouse.y == temp.y)
	{
		a = mouse.x - temp.x;
		if(a > 0)
		{
			for(int i=1; i<=a; i++)
			{
				if(!JudgeSquare(temp.x+i, temp.y))
					break;
				pDC->Rectangle(sidex+(temp.x+i)*fx,sidey+temp.y*fy,
					sidex+(temp.x+i+1)*fx,sidey+(temp.y+1)*fy);
			}
		}
		else
		{
			for(int i=-1; i>=a; i--)
			{
				if(!JudgeSquare(temp.x+i, temp.y))
					break;
				pDC->Rectangle(sidex+(temp.x+i)*fx,sidey+temp.y*fy,
						sidex+(temp.x+i+1)*fx,sidey+(temp.y+1)*fy);

			}		
		}	
	}
	DrawLine();
}


void CPlayDialog::MakeTempBody()
{
	int a,i;
	if(mouse.x == temp.x)
	{
		a = mouse.y - temp.y;
		if(a > 0)
		{
			for(i=1; i<=a; i++)	
			{
				if(!JudgeSquare(temp.x, temp.y+i))
					break;
				snakehead = insert(snakehead, temp.x, temp.y+i, m_length+1);
				m_length++;
			}
			temp.y = temp.y+i-1;
		}
		else
		{
			for(i=-1; i>=a; i--)
			{
				if(!JudgeSquare(temp.x, temp.y+i))
					break;
				snakehead = insert(snakehead, temp.x, temp.y+i, m_length+1);
				m_length++;
			}		
			temp.y = temp.y+i+1;	
		}
	}
	if(mouse.y == temp.y)
	{
		a = mouse.x - temp.x;
		if(a > 0)
		{
			for(i=1; i<=a; i++)
			{
				if(!JudgeSquare(temp.x+i, temp.y))
					break;
				snakehead = insert(snakehead, temp.x+i, temp.y, m_length+1);
				m_length++;
			}
			temp.x = temp.x+i-1;
		}
		else
		{
			for(i=-1; i>=a; i--)
			{
				if(!JudgeSquare(temp.x+i, temp.y))
					break;
				snakehead = insert(snakehead, temp.x+i, temp.y, m_length+1);
				m_length++;
			}		
			temp.x = temp.x+i+1;
		}	
	}
}


void CPlayDialog::GetSquare(CPoint point)
{
	mouse.x = -1;
	mouse.y = -1;
	mouse.next = NULL; 
	for(int i=0;i<snakepara.rownum;i++)
    {
        for(int j=0;j<snakepara.rownum;j++)
        {
			if((point.x >= sidex+fx*i) && (point.x < sidex+fx*(i+1))
				&&(point.y >= sidey+fy*j) && (point.y < sidey+fy*(j+1)))
            {
				mouse.x = i;
				mouse.y = j;
				mouse.next = NULL;
				return;
            }
        }
    }
}


bool CPlayDialog::JudgeSquare(int x, int y)
{
	if ((x != -1) && judgecomp(snakehead, x, y) && judgecomp(snakeobstacle, x, y)
			&& (!(x == snakefood.x&&y == snakefood.y)))
		return true;
	else
		return false;
}


void  CPlayDialog::SetColor(int r, int g, int b)
{
	pDC->SetDCBrushColor(RGB(r,g,b));
	pDC->SelectStockObject(DC_BRUSH);
	pDC->SetDCPenColor(RGB(r,g,b));
	pDC->SelectStockObject(DC_PEN);
}


void CPlayDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(snakepara.ifdraw)
	{
		if(JudgeSquare(mouse.x, mouse.y))
        {
			if(!snakehead)
			{
				SetColor(255,0,0);
				pDC->Ellipse(sidex+mouse.x*fx,sidey+mouse.y*fy,
						sidex+(mouse.x+1)*fx,sidey+(mouse.y+1)*fy);
				snakehead = insert(snakehead, mouse.x, mouse.y, 1);
				m_length = 1;
				temp = mouse;
			}
			else
			{
				SetColor(255,255,0);
				DrawTempBody();
				MakeTempBody();
			}
        }
		UpdateData(FALSE);
    }
	CDialogEx::OnLButtonDown(nFlags, point);
}
