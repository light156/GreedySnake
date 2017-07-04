
// GreedySnakeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GreedySnake.h"
#include "GreedySnakeDlg.h"
#include "afxdialogex.h"
#include "PlayDialog.h"
#include "SetDialog.h"
#include "AutoDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGreedySnakeDlg 对话框



CGreedySnakeDlg::CGreedySnakeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGreedySnakeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGreedySnakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTINUE_BUTTON, m_continuebutton);
}

BEGIN_MESSAGE_MAP(CGreedySnakeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, &CGreedySnakeDlg::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_CONTINUE_BUTTON, &CGreedySnakeDlg::OnBnClickedContinueButton)
	ON_BN_CLICKED(IDC_SET_BUTTON, &CGreedySnakeDlg::OnBnClickedSetButton)
	ON_BN_CLICKED(IDC_AUTO_BUTTON, &CGreedySnakeDlg::OnBnClickedAutoButton)
END_MESSAGE_MAP()


// CGreedySnakeDlg 消息处理程序

BOOL CGreedySnakeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	para.rownum = 20;
	para.speed = 12;
	para.ifobstacle = false;
	para.ifline = true;
	para.ifdraw = true;
	ifstream file("UserData.sna", ios_base::in|ios_base::binary);
	if(!file)
		m_continuebutton.EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CGreedySnakeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
		
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGreedySnakeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		 CPaintDC dc(this);   
         CRect rect;   
         GetClientRect(&rect);   
         CDC dcMem;   
         dcMem.CreateCompatibleDC(&dc);   
         CBitmap bmpBackground;   
         bmpBackground.LoadBitmap(IDB_BITMAP1);  
         BITMAP bitmap;   
         bmpBackground.GetBitmap(&bitmap);   
         CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);  
		 dc.SetStretchBltMode(HALFTONE);
         dc.StretchBlt(0,0,rect.Width()-120,rect.Height(),&dcMem,20,0,   
         bitmap.bmWidth-100,bitmap.bmHeight,SRCCOPY);   
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGreedySnakeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGreedySnakeDlg::OnBnClickedStartButton()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;			    // 用于保存DoModal函数的返回值   

    CPlayDialog dlg;			// 构造对话框类CTipDlg的实例
	dlg.ifcontinue = FALSE;
	dlg.SetParameter(para);
    nRes = dlg.DoModal();		// 弹出对话框   
    if (IDCANCEL == nRes)    
        return; 
}


void CGreedySnakeDlg::OnBnClickedContinueButton()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;			    // 用于保存DoModal函数的返回值   

    CPlayDialog dlg;			// 构造对话框类CTipDlg的实例 
	dlg.ifcontinue = TRUE;
	dlg.ReadFile();
    nRes = dlg.DoModal();		// 弹出对话框   
    if (IDCANCEL == nRes)    
        return;
}


void CGreedySnakeDlg::OnBnClickedSetButton()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;			    // 用于保存DoModal函数的返回值   
    CSetDialog dlg;			// 构造对话框类CTipDlg的实例   
    nRes = dlg.DoModal();		// 弹出对话框   
    if (IDCANCEL == nRes)    
        return; 
}


void CAboutDlg::OnPaint()
{
	CDC *pDC = GetDC();
	CPaintDC dc(this);   
    CRect rect;   
    GetClientRect(&rect);      
    pDC->CreateCompatibleDC(&dc);   
    CBitmap bmpBackground;   
    bmpBackground.LoadBitmap(IDB_BITMAP1);  
    BITMAP bitmap;   
    bmpBackground.GetBitmap(&bitmap);   
    CBitmap *pbmpOld=pDC->SelectObject(&bmpBackground);   
    dc.StretchBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,   
    bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);   
}


void CGreedySnakeDlg::OnBnClickedAutoButton()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;			    // 用于保存DoModal函数的返回值   

    CAutoDialog dlg;			// 构造对话框类CTipDlg的实例
    nRes = dlg.DoModal();		// 弹出对话框   
    if (IDCANCEL == nRes)    
        return;
}
