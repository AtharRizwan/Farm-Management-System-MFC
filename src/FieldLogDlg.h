#pragma once
#include "UiTheme.h"

class Crop;
class Harvest;


// Records watering, fertilization, pests or disease for one crop.
// DoModal returns IDOK after an entry was recorded.
class FieldLogDlg : public ThemedDialog
{
public:
	FieldLogDlg(const Crop& crop, Harvest& storage, CWnd* pParent = nullptr);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIELDLOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;
	void OnOK() override;
	DECLARE_MESSAGE_MAP()

	afx_msg void OnActionChanged();

private:
	enum Action { Water, Fertilize, Pest, Disease };
	Action SelectedAction();

	const Crop& m_crop;
	Harvest& m_storage;
	CComboBox m_action;
	CDateTimeCtrl m_date;
};
