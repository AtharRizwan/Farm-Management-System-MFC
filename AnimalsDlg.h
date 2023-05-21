#pragma once
#include "afxdialogex.h"


// AnimalsDlg dialog

class AnimalsDlg : public CDialog
{
	DECLARE_DYNAMIC(AnimalsDlg)

public:
	AnimalsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AnimalsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
