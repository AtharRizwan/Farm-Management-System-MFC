#pragma once
#include "afxdialogex.h"


// MedicineDlg dialog

class MedicineDlg : public CDialog
{
	DECLARE_DYNAMIC(MedicineDlg)

public:
	MedicineDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MedicineDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
