
// GreedySnakeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

// CGreedySnakeDlg �Ի���
class CGreedySnakeDlg : public CDialogEx
{
// ����
public:
	CGreedySnakeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GREEDYSNAKE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedContinueButton();
	afx_msg void OnBnClickedSetButton();
	
	struct parameter para;
	CButton m_continuebutton;
	afx_msg void OnBnClickedAutoButton();
};
