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

}

CropsDlg::~CropsDlg()
{
}

void CropsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CropsDlg, CDialog)
	ON_BN_CLICKED(btnProcess, &CropsDlg::OnBnClickedbtnprocess)
END_MESSAGE_MAP()



// CropsDlg message handlers


BOOL CropsDlg::OnInitDialog()
{
	// Call the base class implementation
	BOOL bResult = CDialog::OnInitDialog();

	// Perform your desired task here
	try
	{
		Corn corn;
		Wheat wheat;
		Rice rice;
		Harvest harvest;
		CString output(wheat.getWheatType().c_str());
		SetDlgItemText(txtWheatType, output);
		output = to_string(wheat.getFieldSize()).c_str();
		SetDlgItemText(txtWheatFieldSize, output);
		output = to_string(wheat.getQuantity()).c_str();
		SetDlgItemText(txtWheatQuantity, output);
		output = to_string(wheat.getPrice()).c_str();
		SetDlgItemText(txtWheatPrice, output);
		output = wheat.getGrowthStatus().c_str();
		SetDlgItemText(txtWheatStatus, output);
		output = wheat.getPlantingDate().c_str();
		SetDlgItemText(txtWheatPDate, output);
		output = wheat.getHarvestingDate().c_str();
		SetDlgItemText(txtWheatHDate, output);
		output = to_string(wheat.getHeadWeight()).c_str();
		SetDlgItemText(txtHeadWeight, output);
		output = to_string(wheat.getHeadsPerYard()).c_str();
		SetDlgItemText(txtHeadsYard, output);
	}
	catch (...)
	{

	}
	// Return the result
	return bResult;
}

void CropsDlg::OnBnClickedbtnprocess()
{
	// TODO: Add your control notification handler code here
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
