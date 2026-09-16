#pragma once
#include "afxdialogex.h"


// HomePageDlg dialog

class HomePageDlg : public CDialog
{
	CBrush m_backgroundBrush;
	virtual BOOL OnInitDialog();
	DECLARE_DYNAMIC(HomePageDlg)

public:
	HomePageDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~HomePageDlg();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtncrops();
	afx_msg void OnBnClickedbtnemployeedlg();
};
