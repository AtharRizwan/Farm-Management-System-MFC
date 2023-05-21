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
#include <atlstr.h>
using namespace std;

Corn corn;
Wheat wheat;
Rice rice;
Harvest harvest;
CString output;
CString input;

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
	DDX_Control(pDX, dtpWheatPlanting, SetWheatPlantingDate);
	DDX_Control(pDX, dtpWheatHarvesting, setHarvestingDate);
}


BEGIN_MESSAGE_MAP(CropsDlg, CDialog)
	ON_BN_CLICKED(btnProcess, &CropsDlg::OnBnClickedbtnprocess)
	ON_BN_CLICKED(btnAdvanceStatusWheat, &CropsDlg::OnBnClickedbtnadvancestatuswheat)
	ON_NOTIFY(DTN_DATETIMECHANGE, dtpWheatPlanting, &CropsDlg::OnDtnDatetimechangedtpwheatplanting)
	ON_NOTIFY(DTN_DATETIMECHANGE, dtpWheatHarvesting, &CropsDlg::OnDtnDatetimechangedtpwheatharvesting)
END_MESSAGE_MAP()



// CropsDlg message handlers


BOOL CropsDlg::OnInitDialog()
{
	// Call the base class implementation
	BOOL bResult = CDialog::OnInitDialog();

	// Perform your desired task here
		output = wheat.getWheatType().c_str();
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
		output = to_string(wheat.calculateYield()).c_str();
		SetDlgItemText(txtYieldWheat, output);
	// Return the result
	return bResult;
}

void CropsDlg::OnBnClickedbtnprocess()
{
	// TODO: Add your control notification handler code here
	try
	{
		GetDlgItemText(txtWheatType, input);
		CT2A pszConvertedString(input);
		std::string type(pszConvertedString);
		wheat.setWheatType(type);
		GetDlgItemText(txtWheatFieldSize, input);
		wheat.setFieldSize(_ttoi(input));
		GetDlgItemText(txtWheatQuantity, input);
		wheat.setQuantity(_ttof(input));
		GetDlgItemText(txtWheatPrice, input);
		wheat.setPrice(_ttof(input));
		GetDlgItemText(txtHeadWeight, input);
		wheat.setHeadWeight(_ttof(input));
		GetDlgItemText(txtHeadsYard, input);
		wheat.setHeadsPerYard(_ttoi(input));
		AfxMessageBox(L"Saved Successfully!");
	}
	catch (...)
	{
		AfxMessageBox(L"Error");
	}
}


void CropsDlg::OnBnClickedbtnadvancestatuswheat()
{
	// TODO: Add your control notification handler code here
	wheat.advanceStatus();
	output = wheat.getGrowthStatus().c_str();
	SetDlgItemText(txtWheatStatus, output);
}


void CropsDlg::OnDtnDatetimechangedtpwheatplanting(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTime plantingDate;
	SetWheatPlantingDate.GetTime(plantingDate);
	string pDate;
	pDate = to_string(plantingDate.GetMonth()) + "/" + to_string(plantingDate.GetDay()) + "/" + to_string(plantingDate.GetYear());
	wheat.setPlantingDate(pDate);
	output = wheat.getPlantingDate().c_str();
	SetDlgItemText(txtWheatPDate, output);
	*pResult = 0;
}


void CropsDlg::OnDtnDatetimechangedtpwheatharvesting(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTime harvestingDate;
	setHarvestingDate.GetTime(harvestingDate);
	string hDate;
	hDate = to_string(harvestingDate.GetMonth()) + "/" + to_string(harvestingDate.GetDay()) + "/" + to_string(harvestingDate.GetYear());
	wheat.setHarvestingDate(hDate);
	output = wheat.getHarvestingDate().c_str();
	SetDlgItemText(txtWheatHDate, output);
	*pResult = 0;
}
