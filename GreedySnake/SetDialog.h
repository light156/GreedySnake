#pragma once
#include "afxcmn.h"

class CGreedySnakeDlg;
// CSetDialog �Ի���

class CSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDialog)

public:
	CSetDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetDialog();

// �Ի�������
	enum { IDD = IDD_SET_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
