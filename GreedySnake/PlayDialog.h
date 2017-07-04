#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CGreedySnakeDlg;
// CPlayDialog �Ի���

class CPlayDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPlayDialog)

public:
	CPlayDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayDialog();

// �Ի�������
	enum { IDD = IDD_PLAY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool ifcontinue; 
	//���б���
	virtual BOOL OnInitDialog();
	void SetParameter(struct parameter para);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedDrawButton();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void DrawSnake();
	void DrawGame();
	void DrawLine();
	void Redraw();
	void MakeFood();
	void MakeObstacle();
	bool JudgeDead();	
	void SaveFile(ofstream *file);
	void ReadFile();
	void SetColor(int r, int g, int b);
	void GetSquare(CPoint point);
	bool JudgeSquare(int x, int y);
	void DrawTempBody();
	void MakeTempBody();
private:
	struct node *snakehead, snakefood, *snaketail, *snakeobstacle;
	struct parameter snakepara;
	Direction snakedir;
	double fx, fy;
	int pause;
	struct node former, temp, mouse;
	int m_totalvalue;
	int m_length;
	CButton m_drawsnake;
	CDC *pDC;
public:
	int m_rownumber;
};