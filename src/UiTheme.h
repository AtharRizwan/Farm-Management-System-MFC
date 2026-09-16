#pragma once
#include <string>
#include "afxdialogex.h"

// Colors shared by all dialogs
namespace Theme
{
	const COLORREF Background = RGB(232, 243, 237);
	const COLORREF Accent = RGB(38, 104, 74);
	const COLORREF Error = RGB(185, 28, 28);
}

// Base class for the app's dialogs. It paints the themed background,
// draws labels and group boxes without a gray box around them, and styles
// the controls with the shared IDs IDC_TITLE, IDC_SUBTITLE and IDC_ERROR.
class ThemedDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ThemedDialog)

public:
	ThemedDialog(UINT nIDTemplate, CWnd* pParent);

protected:
	BOOL OnInitDialog() override;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

	// Shows a message in the IDC_ERROR label, or hides it when empty
	void ShowError(const CString& message);
	// Draws the control's text in the dialog's bold font
	void UseBoldFont(CWnd* control);

private:
	CBrush m_backgroundBrush;
	CFont m_titleFont;
	CFont m_boldFont;
};

// Text conversion between the UTF-8 model and the UI
CString ToCString(const std::string& utf8);
std::string ToUtf8(const CString& text);

// Formats a number with up to the given number of decimals, without trailing zeros
CString FormatNumber(double value, int maxDecimals = 3);
// Formats a number with thousands separators and two decimals, for display only
CString FormatMoney(double value);

// Read a number from an edit control. On failure they return false and set
// error to a message that names the field.
bool ReadDouble(CWnd& dialog, UINT controlId, const CString& label, double& value, CString& error);
bool ReadInt(CWnd& dialog, UINT controlId, const CString& label, int& value, CString& error);

// Converts between the stored "M/D/YYYY" dates and SYSTEMTIME.
// ParseDate returns false for "Not set" and for text that is not a date.
bool ParseDate(const std::string& text, SYSTEMTIME& date);
std::string FormatDate(const SYSTEMTIME& date);

// Shows the model's date in a date picker that has the DTS_SHOWNONE style
void ShowDate(CDateTimeCtrl& picker, const std::string& text);
// Returns the picker's date, or an empty string when it is unchecked
std::string ReadDate(CDateTimeCtrl& picker);
