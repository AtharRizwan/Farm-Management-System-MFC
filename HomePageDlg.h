#pragma once
#include "afxdialogex.h"


// HomePageDlg dialog

class HomePageDlg : public CDialog
{
	DECLARE_DYNAMIC(HomePageDlg)

public:
	HomePageDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~HomePageDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtncrops();
	afx_msg void OnBnClickedBtnanimals();
	afx_msg void OnBnClickedBtnanimals2();
	afx_msg void OnBnClickedBtnanimals3();
};
