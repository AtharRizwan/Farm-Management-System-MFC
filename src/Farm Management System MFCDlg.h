
// Farm Management System MFCDlg.h : header file
//

#pragma once


// CFarmManagementSystemMFCDlg dialog
class CFarmManagementSystemMFCDlg : public CDialogEx
{
// Construction
public:
	CFarmManagementSystemMFCDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FARM_MANAGEMENT_SYSTEM_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangePassword();
	afx_msg void OnEnChangetxtusername();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedbtnlogin();
	afx_msg void OnEnChangetxtpassword();
};
