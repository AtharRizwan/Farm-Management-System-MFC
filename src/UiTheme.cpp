#include "pch.h"
#include <cwchar>
#include <sstream>
#include "resource.h"
#include "UiTheme.h"

IMPLEMENT_DYNAMIC(ThemedDialog, CDialogEx)

BEGIN_MESSAGE_MAP(ThemedDialog, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

ThemedDialog::ThemedDialog(UINT nIDTemplate, CWnd* pParent)
	: CDialogEx(nIDTemplate, pParent)
{
	m_backgroundBrush.CreateSolidBrush(Theme::Background);
}

BOOL ThemedDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(Theme::Background);

	LOGFONT lf{};
	GetFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_boldFont.CreateFontIndirect(&lf);
	lf.lfHeight = lf.lfHeight * 8 / 5;
	lf.lfWeight = FW_SEMIBOLD;
	m_titleFont.CreateFontIndirect(&lf);

	if (CWnd* title = GetDlgItem(IDC_TITLE))
	{
		title->SetFont(&m_titleFont);
	}
	if (CWnd* error = GetDlgItem(IDC_ERROR))
	{
		error->SetFont(&m_boldFont);
		error->ShowWindow(SW_HIDE);
	}
	return TRUE;
}

HBRUSH ThemedDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor != CTLCOLOR_STATIC && nCtlColor != CTLCOLOR_DLG)
	{
		return hbr;
	}

	TCHAR className[32] = {};
	::GetClassName(pWnd->GetSafeHwnd(), className, _countof(className));
	bool isLabel = _tcsicmp(className, _T("Static")) == 0;
	bool isButton = _tcsicmp(className, _T("Button")) == 0;
	if (nCtlColor == CTLCOLOR_DLG || isLabel || isButton)
	{
		pDC->SetBkMode(TRANSPARENT);
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_TITLE:
			pDC->SetTextColor(Theme::Accent);
			break;
		case IDC_ERROR:
			pDC->SetTextColor(Theme::Error);
			break;
		default:
			if (isButton && (pWnd->GetStyle() & BS_TYPEMASK) == BS_GROUPBOX)
			{
				pDC->SetTextColor(Theme::Accent);
			}
			break;
		}
		return m_backgroundBrush;
	}
	return hbr;
}

void ThemedDialog::ShowError(const CString& message)
{
	if (CWnd* error = GetDlgItem(IDC_ERROR))
	{
		error->SetWindowText(message);
		error->ShowWindow(message.IsEmpty() ? SW_HIDE : SW_SHOW);
	}
	if (!message.IsEmpty())
	{
		MessageBeep(MB_ICONWARNING);
	}
}

void ThemedDialog::UseBoldFont(CWnd* control)
{
	control->SetFont(&m_boldFont);
}

CString ToCString(const std::string& utf8)
{
	return CString(CA2W(utf8.c_str(), CP_UTF8));
}

std::string ToUtf8(const CString& text)
{
	return std::string(CW2A(text, CP_UTF8));
}

CString FormatNumber(double value, int maxDecimals)
{
	CString text;
	text.Format(_T("%.*f"), maxDecimals, value);
	if (text.Find(_T('.')) >= 0)
	{
		text.TrimRight(_T('0'));
		text.TrimRight(_T('.'));
	}
	if (text == _T("-0"))
	{
		text = _T("0");
	}
	return text;
}

CString FormatMoney(double value)
{
	CString plain;
	plain.Format(_T("%.2f"), value);
	NUMBERFMT format{};
	format.NumDigits = 2;
	format.LeadingZero = 1;
	format.Grouping = 3;
	format.lpDecimalSep = const_cast<LPWSTR>(L".");
	format.lpThousandSep = const_cast<LPWSTR>(L",");
	format.NegativeOrder = 1;
	WCHAR buffer[64];
	if (GetNumberFormatW(LOCALE_INVARIANT, 0, plain, &format, buffer, _countof(buffer)) > 0)
	{
		return CString(buffer);
	}
	return plain;
}

bool ReadDouble(CWnd& dialog, UINT controlId, const CString& label, double& value, CString& error)
{
	CString text;
	dialog.GetDlgItemText(controlId, text);
	text.Trim();
	if (text.IsEmpty())
	{
		error = label + _T(" is required.");
		return false;
	}
	wchar_t* end = nullptr;
	double parsed = wcstod(text, &end);
	if (end == nullptr || *end != L'\0' || !_finite(parsed))
	{
		error = label + _T(" must be a number.");
		return false;
	}
	if (parsed < 0)
	{
		error = label + _T(" cannot be negative.");
		return false;
	}
	value = parsed;
	return true;
}

bool ReadInt(CWnd& dialog, UINT controlId, const CString& label, int& value, CString& error)
{
	CString text;
	dialog.GetDlgItemText(controlId, text);
	text.Trim();
	if (text.IsEmpty())
	{
		error = label + _T(" is required.");
		return false;
	}
	wchar_t* end = nullptr;
	errno = 0;
	long parsed = wcstol(text, &end, 10);
	if (end == nullptr || *end != L'\0')
	{
		error = label + _T(" must be a whole number.");
		return false;
	}
	if (errno == ERANGE || parsed > INT_MAX || parsed < INT_MIN)
	{
		error = label + _T(" is too large.");
		return false;
	}
	if (parsed < 0)
	{
		error = label + _T(" cannot be negative.");
		return false;
	}
	value = static_cast<int>(parsed);
	return true;
}

bool ParseDate(const std::string& text, SYSTEMTIME& date)
{
	int month = 0;
	int day = 0;
	int year = 0;
	char slash1 = 0;
	char slash2 = 0;
	std::istringstream in(text);
	if (!(in >> month >> slash1 >> day >> slash2 >> year) || slash1 != '/' || slash2 != '/')
	{
		return false;
	}
	if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1753 || year > 9999)
	{
		return false;
	}
	SYSTEMTIME result{};
	result.wYear = static_cast<WORD>(year);
	result.wMonth = static_cast<WORD>(month);
	result.wDay = static_cast<WORD>(day);
	// Rejects dates such as 2/30
	FILETIME check;
	if (!SystemTimeToFileTime(&result, &check))
	{
		return false;
	}
	date = result;
	return true;
}

std::string FormatDate(const SYSTEMTIME& date)
{
	return std::to_string(date.wMonth) + "/" + std::to_string(date.wDay) + "/" + std::to_string(date.wYear);
}

void ShowDate(CDateTimeCtrl& picker, const std::string& text)
{
	SYSTEMTIME date{};
	if (ParseDate(text, date))
	{
		picker.SetTime(&date);
	}
	else
	{
		// Wine ignores GDT_NONE without a SYSTEMTIME pointer
		picker.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, reinterpret_cast<LPARAM>(&date));
	}
}

std::string ReadDate(CDateTimeCtrl& picker)
{
	SYSTEMTIME date{};
	if (picker.SendMessage(DTM_GETSYSTEMTIME, 0, reinterpret_cast<LPARAM>(&date)) != GDT_VALID)
	{
		return std::string();
	}
	return FormatDate(date);
}
