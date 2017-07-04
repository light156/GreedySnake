#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAutoDialog 对话框

class CAutoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoDialog)

public:
	CAutoDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoDialog();

// 对话框数据
	enum { IDD = IDD_AUTO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);	
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	void SetBegin();
	void DrawSnake();
	void DrawGame();
	void DrawLine();
	void Redraw();
	void MakeFood();
	bool JudgeDead();
	void SetColor(int r, int g, int b);

	bool JudgeIn(int x, int y);
	int Manhattan(int x, int y, int goalx, int goaly);
	void SetMap();
	bool AStar(struct Map goal);
	void GetFurthest();

//	void InitConsole();
private:
	struct node *snakehead, *copyhead, snakefood, *snaketail, *last;
	Direction snakedir;
	int m_rownumber, speed, pause;
	int m_totalvalue, m_length;

	CSliderCtrl m_slider;
	CDC *pDC;
	double fx, fy;

	struct Map **map, start, tail, food; 
	bool flag;
	CSpinButtonCtrl m_spin;
};

bool operator<(Map a,Map b);