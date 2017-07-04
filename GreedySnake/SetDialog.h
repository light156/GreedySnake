#pragma once
#include "afxcmn.h"

class CGreedySnakeDlg;
// CSetDialog 对话框

class CSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDialog)

public:
	CSetDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDialog();

// 对话框数据
	enum { IDD = IDD_SET_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT m_rownum;
	CSliderCtrl m_slider;
	CGreedySnakeDlg *parent;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
	BOOL m_ifobstacle;
	BOOL m_ifline;
	BOOL m_ifdraw;
};
