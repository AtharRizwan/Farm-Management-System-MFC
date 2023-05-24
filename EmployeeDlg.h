#pragma once
#include "afxdialogex.h"
#include <vector>

// EmployeeDlg dialog

class EmployeeDlg : public CDialog
{
	DECLARE_DYNAMIC(EmployeeDlg);
		CBrush m_backgroundBrush;
public:
	virtual BOOL OnInitDialog();
	EmployeeDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EmployeeDlg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnaddemployee();
	CListBox EmpData;
	afx_msg void OnBnClickedbtnaddemployee2();
};
