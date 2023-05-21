// HomePageDlg.cpp : implementation file
//

#include "pch.h"
#include "Farm Management System MFC.h"
#include "afxdialogex.h"
#include "HomePageDlg.h"
#include "CropsDlg.h"


// HomePageDlg dialog

IMPLEMENT_DYNAMIC(HomePageDlg, CDialog)

HBRUSH HomePageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Check if the control is the dialog itself
	if (pWnd == this) {
		// Set the background color of the dialog
		pDC->SetBkColor(RGB(104, 166, 145));  // Set your desired background color here
		hbr = m_backgroundBrush;
	}

	return hbr;
}

BOOL HomePageDlg::OnInitDialog()
{
	// Call the base class implementation
	BOOL bResult = CDialog::OnInitDialog();

	// Perform your desired task here
	m_backgroundBrush.CreateSolidBrush(RGB(104, 166, 145));

	// Return the result
	return bResult;
}

HomePageDlg::HomePageDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

HomePageDlg::~HomePageDlg()
{
}

void HomePageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HomePageDlg, CDialog)
	ON_BN_CLICKED(btnCrops, &HomePageDlg::OnBnClickedbtncrops)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// HomePageDlg message handlers


void HomePageDlg::OnBnClickedbtncrops()
{
	// TODO: Add your control notification handler code here
	CropsDlg Dlg;
	Dlg.DoModal();
}
