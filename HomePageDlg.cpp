// HomePageDlg.cpp : implementation file
//

#include "pch.h"
#include "Farm Management System MFC.h"
#include "afxdialogex.h"
#include "HomePageDlg.h"
#include "CropsDlg.h"


// HomePageDlg dialog

IMPLEMENT_DYNAMIC(HomePageDlg, CDialog)

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
END_MESSAGE_MAP()


// HomePageDlg message handlers


void HomePageDlg::OnBnClickedbtncrops()
{
	// TODO: Add your control notification handler code here
	CropsDlg Dlg;
	Dlg.DoModal();
}
