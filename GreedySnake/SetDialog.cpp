// SetDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GreedySnake.h"
#include "SetDialog.h"
#include "afxdialogex.h"
#include "GreedySnakeDlg.h"

// CSetDialog 对话框

IMPLEMENT_DYNAMIC(CSetDialog, CDialogEx)

CSetDialog::CSetDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetDialog::IDD, pParent)
	, m_ifdraw(FALSE)
{	
	
}

CSetDialog::~CSetDialog()
{
}

void CSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_rownum);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Radio(pDX, IDC_RADIO1, m_ifobstacle);
	DDX_Radio(pDX, IDC_RADIO3, m_ifline);
	DDX_Radio(pDX, IDC_RADIO5, m_ifdraw);
}


BEGIN_MESSAGE_MAP(CSetDialog, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CSetDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetDialog 消息处理程序


BOOL CSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	parent = (CGreedySnakeDlg*)GetParent();
	m_slider.SetRange(4,20);
	m_slider.SetTicFreq(2);
	m_rownum = parent->para.rownum;
	m_slider.SetPos(parent->para.speed);
	m_ifobstacle = !parent->para.ifobstacle;
	m_ifline = !parent->para.ifline;
	m_ifdraw = !parent->para.ifdraw;
	UpdateData(FALSE);
	return TRUE;
	// 异常: OCX 属性页应返回 FALSE
}


void CSetDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSetDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_rownum < 10 || m_rownum > 25)
	{
		AfxMessageBox(_T("行列数的范围为10-25！"));
		return;
	}
	parent->para.rownum = m_rownum;
	parent->para.speed = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
	parent->para.ifobstacle = !(m_ifobstacle);
	parent->para.ifline = !(m_ifline);
	parent->para.ifdraw = !(m_ifdraw);
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
