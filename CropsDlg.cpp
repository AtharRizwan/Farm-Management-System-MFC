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
	DDX_Control(pDX, dtpWheatHarvesting, setWheatHarvestingDate);
	DDX_Control(pDX, dtpCornPlanting, SetCornPlanting);
	DDX_Control(pDX, dtpCornHarvesting, SetCornHarvesting);
}


BEGIN_MESSAGE_MAP(CropsDlg, CDialog)
	ON_BN_CLICKED(btnProcess, &CropsDlg::OnBnClickedbtnprocess)
	ON_BN_CLICKED(btnAdvanceStatusWheat, &CropsDlg::OnBnClickedbtnadvancestatuswheat)
	ON_NOTIFY(DTN_DATETIMECHANGE, dtpWheatPlanting, &CropsDlg::OnDtnDatetimechangedtpwheatplanting)
	ON_NOTIFY(DTN_DATETIMECHANGE, dtpWheatHarvesting, &CropsDlg::OnDtnDatetimechangedtpwheatharvesting)
	ON_BN_CLICKED(btnAdvanceStatusCorn, &CropsDlg::OnBnClickedbtnadvancestatuscorn)
	ON_NOTIFY(DTN_DATETIMECHANGE, dtpCornPlanting, &CropsDlg::OnDtnDatetimechangedtpcornplanting)
	ON_NOTIFY(DTN_DATETIMECHANGE, dtpCornHarvesting, &CropsDlg::OnDtnDatetimechangedtpcornharvesting)
END_MESSAGE_MAP()



// CropsDlg message handlers


BOOL CropsDlg::OnInitDialog()
{
	// Call the base class implementation
	BOOL bResult = CDialog::OnInitDialog();

	// Perform your desired task here
	// Wheat load data
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
		// Corn load data
		output = corn.getCornType().c_str();
		SetDlgItemText(txtCornType, output);
		output = to_string(corn.getFieldSize()).c_str();
		SetDlgItemText(txtCornFieldSize, output);
		output = to_string(corn.getQuantity()).c_str();
		SetDlgItemText(txtCornQuantity, output);
		output = to_string(corn.getPrice()).c_str();
		SetDlgItemText(txtCornPrice, output);
		output = corn.getGrowthStatus().c_str();
		SetDlgItemText(txtCornStatus, output);
		output = corn.getPlantingDate().c_str();
		SetDlgItemText(txtCornPDate, output);
		output = corn.getHarvestingDate().c_str();
		SetDlgItemText(txtCornHDate, output);
		output = to_string(corn.getEarWeight()).c_str();
		SetDlgItemText(txtEarWeight, output);
		output = to_string(corn.getKernalsPerEar()).c_str();
		SetDlgItemText(txtKernalsEar, output);
		output = to_string(corn.getEarsPerAcre()).c_str();
		SetDlgItemText(txtEarsAcre, output);
		output = to_string(corn.getShrinkage()).c_str();
		SetDlgItemText(txtShrinkage, output);
		output = to_string(corn.calculateYield()).c_str();
		SetDlgItemText(txtYieldCorn, output);
	// Return the result
	return bResult;
}

void CropsDlg::OnBnClickedbtnprocess()
{
	// TODO: Add your control notification handler code here
	// Wheat save data
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
		output = to_string(wheat.calculateYield()).c_str();
		SetDlgItemText(txtYieldWheat, output);
	}
	catch (...)
	{
		AfxMessageBox(L"Error");
	}
	// Corn save data
	try
	{
		GetDlgItemText(txtCornType, input);
		CT2A pszConvertedString(input);
		std::string type(pszConvertedString);
		corn.setCornType(type);
		GetDlgItemText(txtCornFieldSize, input);
		corn.setFieldSize(_ttoi(input));
		GetDlgItemText(txtCornQuantity, input);
		corn.setQuantity(_ttof(input));
		GetDlgItemText(txtCornPrice, input);
		corn.setPrice(_ttof(input));
		GetDlgItemText(txtEarWeight, input);
		corn.setEarWeight(_ttof(input));
		GetDlgItemText(txtEarsAcre, input);
		corn.setEarsPerAcre(_ttoi(input));
		GetDlgItemText(txtKernalsEar, input);
		corn.setKernalsPerEar(_ttoi(input));
		GetDlgItemText(txtShrinkage, input);
		corn.setShrinkage(_ttoi(input));
		output = to_string(corn.calculateYield()).c_str();
		SetDlgItemText(txtYieldCorn, output);
	}
	catch (...)
	{
		AfxMessageBox(L"Error");
	}
	AfxMessageBox(L"Saved Successfully!");
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
	setWheatHarvestingDate.GetTime(harvestingDate);
	string hDate;
	hDate = to_string(harvestingDate.GetMonth()) + "/" + to_string(harvestingDate.GetDay()) + "/" + to_string(harvestingDate.GetYear());
	wheat.setHarvestingDate(hDate);
	output = wheat.getHarvestingDate().c_str();
	SetDlgItemText(txtWheatHDate, output);
	*pResult = 0;
}


void CropsDlg::OnBnClickedbtnadvancestatuscorn()
{
	// TODO: Add your control notification handler code here
	corn.advanceStatus();
	output = corn.getGrowthStatus().c_str();
	SetDlgItemText(txtCornStatus, output);
}


void CropsDlg::OnDtnDatetimechangedtpcornplanting(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTime plantingDate;
	SetCornPlanting.GetTime(plantingDate);
	string pDate;
	pDate = to_string(plantingDate.GetMonth()) + "/" + to_string(plantingDate.GetDay()) + "/" + to_string(plantingDate.GetYear());
	corn.setPlantingDate(pDate);
	output = corn.getPlantingDate().c_str();
	SetDlgItemText(txtCornPDate, output);
	*pResult = 0;
}


void CropsDlg::OnDtnDatetimechangedtpcornharvesting(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTime harvestingDate;
	SetCornHarvesting.GetTime(harvestingDate);
	string hDate;
	hDate = to_string(harvestingDate.GetMonth()) + "/" + to_string(harvestingDate.GetDay()) + "/" + to_string(harvestingDate.GetYear());
	corn.setHarvestingDate(hDate);
	output = corn.getHarvestingDate().c_str();
	SetDlgItemText(txtCornHDate, output);
	*pResult = 0;
}
