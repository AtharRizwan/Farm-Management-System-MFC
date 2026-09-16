#pragma once
#include <functional>
#include "UiTheme.h"

struct FarmData;
class Crop;


// Crops & Harvest dialog
class CropsDlg : public ThemedDialog
{
	DECLARE_DYNAMIC(CropsDlg)

public:
	CropsDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CROPS };
#endif

	// Control IDs of the fields that every crop has
	struct CropIds
	{
		UINT variety, fieldSize, quantity, price, status, planted, harvested, yield;
	};

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;
	void OnOK() override;
	void OnCancel() override;
	DECLARE_MESSAGE_MAP()

	afx_msg void OnFieldChanged(UINT id);
	afx_msg void OnDateChanged(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdvanceWheat();
	afx_msg void OnAdvanceCorn();
	afx_msg void OnAdvanceRice();
	afx_msg void OnFieldLogWheat();
	afx_msg void OnFieldLogCorn();
	afx_msg void OnFieldLogRice();
	afx_msg void OnHarvest();
	afx_msg void OnSell();
	afx_msg void OnNewSeason();

private:
	// Index of a crop: 0 = wheat, 1 = corn, 2 = rice
	static Crop& CropAt(FarmData& farm, int index);

	// Shows the saved data in the controls
	void LoadControls();
	void UpdateYieldPreview();

	// Reads every field into farm. On failure it returns false and
	// remembers the message and the control that needs fixing.
	bool ReadForm(FarmData& farm);
	bool ReadCommon(Crop& crop, int index);
	bool ReadWheat(FarmData& farm);
	bool ReadCorn(FarmData& farm);
	bool ReadRice(FarmData& farm);
	bool ReadNumber(UINT id, const CString& label, double& value);
	bool ReadWhole(UINT id, const CString& label, int& value);
	bool Apply(UINT id, const CString& label, const std::function<void()>& setter);
	bool Fail(UINT id, const CString& message);
	void ShowStatus(const CString& error);
	void ShowFormError();

	// Reads the form, runs the action on the result and saves everything.
	// The action returns false to cancel. Returns true if the data was saved.
	bool Commit(const std::function<bool(FarmData&)>& action);

	void AdvanceStatus(int index);
	void OpenFieldLog(int index);

	CDateTimeCtrl m_planted[3];
	CDateTimeCtrl m_harvested[3];
	bool m_loading = false;
	bool m_dirty = false;
	CString m_error;
	UINT m_errorControl = 0;
};
