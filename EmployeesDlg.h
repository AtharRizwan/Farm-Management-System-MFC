#pragma once
#include "afxdialogex.h"


// EmployeesDlg dialog

class EmployeesDlg : public CDialog
{
	DECLARE_DYNAMIC(EmployeesDlg)

public:
	EmployeesDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EmployeesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
