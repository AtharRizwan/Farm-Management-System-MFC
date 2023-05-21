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
};
