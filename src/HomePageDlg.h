#pragma once
#include "UiTheme.h"


// Home page. DoModal returns IDC_LOGOUT when the user logs out.
class HomePageDlg : public ThemedDialog
{
	DECLARE_DYNAMIC(HomePageDlg)

public:
	HomePageDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOME };
#endif

protected:
	BOOL OnInitDialog() override;
	void OnOK() override;
	DECLARE_MESSAGE_MAP()

	afx_msg void OnOpenCrops();
	afx_msg void OnOpenEmployees();
	afx_msg void OnLogout();

private:
	void UpdateSummary();
	HICON m_hIcon;
};
