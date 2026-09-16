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
	UpdateSummary();
	return TRUE;
}

void HomePageDlg::UpdateSummary()
{
	const FarmData& farm = GetFarm();
	CString growing;
	for (const Crop* crop : { static_cast<const Crop*>(&farm.wheat), static_cast<const Crop*>(&farm.corn), static_cast<const Crop*>(&farm.rice) })
	{
		if (!growing.IsEmpty())
		{
			growing += _T(", ");
		}
		growing += ToCString(crop->getName() + ": " + crop->getGrowthStatus());
	}
	const Harvest& harvest = farm.harvest;
	CString summary;
	summary.Format(_T("Crops:  %s\n")
		_T("In storage:  %s t wheat, %s t corn, %s t rice\n")
		_T("Total revenue:  Rs. %s\n")
		_T("Employees:  %d"),
		(LPCTSTR)growing,
		(LPCTSTR)FormatNumber(harvest.getAmountWheat()),
		(LPCTSTR)FormatNumber(harvest.getAmountCorn()),
		(LPCTSTR)FormatNumber(harvest.getAmountRice()),
		(LPCTSTR)FormatMoney(harvest.getRevenue()),
		static_cast<int>(farm.employees.size()));
	SetDlgItemText(IDC_SUMMARY, summary);
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
