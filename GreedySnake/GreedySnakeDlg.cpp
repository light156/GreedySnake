
// GreedySnakeDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CGreedySnakeDlg �Ի���



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


// CGreedySnakeDlg ��Ϣ�������

BOOL CGreedySnakeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	para.rownum = 20;
	para.speed = 12;
	para.ifobstacle = false;
	para.ifline = true;
	para.ifdraw = true;
	ifstream file("UserData.sna", ios_base::in|ios_base::binary);
	if(!file)
		m_continuebutton.EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGreedySnakeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGreedySnakeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGreedySnakeDlg::OnBnClickedStartButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;			    // ���ڱ���DoModal�����ķ���ֵ   

    CPlayDialog dlg;			// ����Ի�����CTipDlg��ʵ��
	dlg.ifcontinue = FALSE;
	dlg.SetParameter(para);
    nRes = dlg.DoModal();		// �����Ի���   
    if (IDCANCEL == nRes)    
        return; 
}


void CGreedySnakeDlg::OnBnClickedContinueButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;			    // ���ڱ���DoModal�����ķ���ֵ   

    CPlayDialog dlg;			// ����Ի�����CTipDlg��ʵ�� 
	dlg.ifcontinue = TRUE;
	dlg.ReadFile();
    nRes = dlg.DoModal();		// �����Ի���   
    if (IDCANCEL == nRes)    
        return;
}


void CGreedySnakeDlg::OnBnClickedSetButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;			    // ���ڱ���DoModal�����ķ���ֵ   
    CSetDialog dlg;			// ����Ի�����CTipDlg��ʵ��   
    nRes = dlg.DoModal();		// �����Ի���   
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;			    // ���ڱ���DoModal�����ķ���ֵ   

    CAutoDialog dlg;			// ����Ի�����CTipDlg��ʵ��
    nRes = dlg.DoModal();		// �����Ի���   
    if (IDCANCEL == nRes)    
        return;
}
