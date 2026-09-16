
// Farm Management System MFCDlg.h : header file
//

#pragma once
#include "UiTheme.h"


// Login dialog. DoModal returns IDOK after a successful sign-in.
class CFarmManagementSystemMFCDlg : public ThemedDialog
{
// Construction
public:
	CFarmManagementSystemMFCDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	BOOL OnInitDialog() override;
	void OnOK() override;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
