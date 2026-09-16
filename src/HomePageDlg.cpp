// HomePageDlg.cpp : implementation file
//

#include "pch.h"
#include "Farm Management System MFC.h"
#include "HomePageDlg.h"
#include "CropsDlg.h"
#include "EmployeeDlg.h"


// HomePageDlg dialog

IMPLEMENT_DYNAMIC(HomePageDlg, ThemedDialog)

HomePageDlg::HomePageDlg(CWnd* pParent /*=nullptr*/)
	: ThemedDialog(IDD_HOME, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(HomePageDlg, ThemedDialog)
	ON_BN_CLICKED(IDC_OPEN_CROPS, &HomePageDlg::OnOpenCrops)
	ON_BN_CLICKED(IDC_OPEN_EMPLOYEES, &HomePageDlg::OnOpenEmployees)
	ON_BN_CLICKED(IDC_LOGOUT, &HomePageDlg::OnLogout)
END_MESSAGE_MAP()

BOOL HomePageDlg::OnInitDialog()
{
	ThemedDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	for (UINT id : { IDC_HOME_HEADER_CROP, IDC_HOME_HEADER_STAGE, IDC_HOME_HEADER_STORED, IDC_HOME_REVENUE, IDC_HOME_EMPLOYEES })
	{
		UseBoldFont(GetDlgItem(id));
	}
	UpdateSummary();
	return TRUE;
}

// Formats tonnes with thousands separators, e.g. "50,000 t" or "12.5 t"
static CString FormatTonnes(double value)
{
	CString plain = FormatNumber(value);
	int point = plain.Find(_T('.'));
	NUMBERFMT format{};
	format.NumDigits = point >= 0 ? plain.GetLength() - point - 1 : 0;
	format.LeadingZero = 1;
	format.Grouping = 3;
	format.lpDecimalSep = const_cast<LPWSTR>(L".");
	format.lpThousandSep = const_cast<LPWSTR>(L",");
	format.NegativeOrder = 1;
	WCHAR buffer[64];
	if (GetNumberFormatW(LOCALE_INVARIANT, 0, plain, &format, buffer, _countof(buffer)) > 0)
	{
		plain = buffer;
	}
	return plain + _T(" t");
}

void HomePageDlg::UpdateSummary()
{
	const FarmData& farm = GetFarm();
	const Harvest& harvest = farm.harvest;
	struct Row
	{
		const Crop& crop;
		UINT statusId;
		UINT storedId;
		double stored;
	};
	const Row rows[] = {
		{ farm.wheat, IDC_HOME_WHEAT_STATUS, IDC_HOME_WHEAT_STORED, harvest.getAmountWheat() },
		{ farm.corn, IDC_HOME_CORN_STATUS, IDC_HOME_CORN_STORED, harvest.getAmountCorn() },
		{ farm.rice, IDC_HOME_RICE_STATUS, IDC_HOME_RICE_STORED, harvest.getAmountRice() },
	};
	for (const Row& row : rows)
	{
		SetDlgItemText(row.statusId, ToCString(row.crop.getGrowthStatus()));
		SetDlgItemText(row.storedId, FormatTonnes(row.stored));
	}
	SetDlgItemText(IDC_HOME_REVENUE, _T("Rs. ") + FormatMoney(harvest.getRevenue()));
	CString employees;
	employees.Format(_T("%d"), static_cast<int>(farm.employees.size()));
	SetDlgItemText(IDC_HOME_EMPLOYEES, employees);
}

// There is no OK button, so the Enter key must not close the window
void HomePageDlg::OnOK()
{
}

void HomePageDlg::OnOpenCrops()
{
	CropsDlg dlg(this);
	dlg.DoModal();
	UpdateSummary();
}

void HomePageDlg::OnOpenEmployees()
{
	EmployeeDlg dlg(this);
	dlg.DoModal();
	UpdateSummary();
}

void HomePageDlg::OnLogout()
{
	EndDialog(IDC_LOGOUT);
}
