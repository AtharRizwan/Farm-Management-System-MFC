// FieldLogDlg.cpp : implementation file
//

#include "pch.h"
#include "resource.h"
#include "Crop.h"
#include "FieldLogDlg.h"
#include <stdexcept>

FieldLogDlg::FieldLogDlg(const Crop& crop, Harvest& storage, CWnd* pParent /*=nullptr*/)
	: ThemedDialog(IDD_FIELDLOG, pParent), m_crop(crop), m_storage(storage)
{
}

void FieldLogDlg::DoDataExchange(CDataExchange* pDX)
{
	ThemedDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_ACTION, m_action);
	DDX_Control(pDX, IDC_LOG_DATE, m_date);
}

BEGIN_MESSAGE_MAP(FieldLogDlg, ThemedDialog)
	ON_CBN_SELCHANGE(IDC_LOG_ACTION, &FieldLogDlg::OnActionChanged)
END_MESSAGE_MAP()

BOOL FieldLogDlg::OnInitDialog()
{
	ThemedDialog::OnInitDialog();

	CString name = ToCString(m_crop.getName());
	SetWindowText(_T("Field Log - ") + name);
	SetDlgItemText(IDC_TITLE, _T("Field Log: ") + name);

	// Items are added in the order of the Action enum
	m_action.AddString(_T("Water the crop"));
	m_action.AddString(_T("Apply fertilizer"));
	m_action.AddString(_T("Record a pest infestation"));
	m_action.AddString(_T("Record a disease outbreak"));
	// A matured crop no longer needs water or fertilizer
	bool growing = !m_crop.isMature() && !m_crop.isHarvested();
	m_action.SetCurSel(growing ? Water : Pest);

	m_date.SetFormat(_T("yyyy-MM-dd"));
	SendDlgItemMessage(IDC_LOG_DESCRIPTION, EM_SETLIMITTEXT, 200);
	OnActionChanged();
	return TRUE;
}

FieldLogDlg::Action FieldLogDlg::SelectedAction()
{
	int selection = m_action.GetCurSel();
	return selection < 0 ? Water : static_cast<Action>(selection);
}

void FieldLogDlg::OnActionChanged()
{
	Action action = SelectedAction();
	bool usesAmount = action == Water || action == Fertilize;

	CString unit;
	CString stock;
	if (action == Water)
	{
		unit = _T("litres");
		stock = _T("In storage: ") + FormatNumber(m_storage.getWater()) + _T(" litres");
	}
	else if (action == Fertilize)
	{
		unit = _T("kg");
		stock = _T("In storage: ") + FormatNumber(m_storage.getFertilizer()) + _T(" kg");
	}
	SetDlgItemText(IDC_LOG_UNIT, unit);
	SetDlgItemText(IDC_LOG_STOCK, stock);

	GetDlgItem(IDC_LOG_AMOUNT_LABEL)->EnableWindow(usesAmount);
	GetDlgItem(IDC_LOG_AMOUNT)->EnableWindow(usesAmount);
	GetDlgItem(IDC_LOG_DESCRIPTION_LABEL)->EnableWindow(!usesAmount);
	GetDlgItem(IDC_LOG_DESCRIPTION)->EnableWindow(!usesAmount);
	ShowError(_T(""));
}

// Record button and the Enter key
void FieldLogDlg::OnOK()
{
	Action action = SelectedAction();
	SYSTEMTIME date{};
	m_date.GetTime(&date);
	std::string when = FormatDate(date);

	try
	{
		if (action == Water || action == Fertilize)
		{
			double amount = 0;
			CString error;
			if (!ReadDouble(*this, IDC_LOG_AMOUNT, _T("Amount"), amount, error))
			{
				ShowError(error);
				GotoDlgCtrl(GetDlgItem(IDC_LOG_AMOUNT));
				return;
			}
			if (action == Water)
			{
				m_crop.recordCropWatering(m_storage, when, amount);
			}
			else
			{
				m_crop.recordFertilization(m_storage, when, amount);
			}
		}
		else
		{
			CString description;
			GetDlgItemText(IDC_LOG_DESCRIPTION, description);
			description.Trim();
			if (action == Pest)
			{
				m_crop.recordPestInfestation(when, ToUtf8(description));
			}
			else
			{
				m_crop.recordDiseaseOutbreak(when, ToUtf8(description));
			}
		}
	}
	catch (const std::invalid_argument& e)
	{
		ShowError(ToCString(e.what()) + _T("."));
		return;
	}
	catch (const std::exception& e)
	{
		AfxMessageBox(_T("The entry could not be recorded.\n\n") + ToCString(e.what()), MB_ICONERROR);
		return;
	}
	ThemedDialog::OnOK();
}
