// CropsDlg.cpp : implementation file
//

#include "pch.h"
#include "Farm Management System MFC.h"
#include "afxdialogex.h"
#include "CropsDlg.h"
#include "Crop.h"
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

// CropsDlg dialog

IMPLEMENT_DYNAMIC(CropsDlg, CDialog)

CropsDlg::CropsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG2, pParent)
{
	try
	{
		Crop* corn = new Corn;
		Crop* wheat = new Wheat;
		Crop* rice = new Rice;
		Harvest harvest;
		CString output(wheat->getName().c_str());
		SetDlgItemText(txtWheatType, output);
		output = to_string(wheat->getFieldSize()).c_str();
		SetDlgItemText(txtWheatFieldSize, output);
	}
	catch (...)
	{

	}
}

CropsDlg::~CropsDlg()
{
}

void CropsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CropsDlg, CDialog)
END_MESSAGE_MAP()


// CropsDlg message handlers
