#pragma once
#include <vector>
#include "UiTheme.h"

class Employee;

// Employees dialog: list, add, update and remove employees
class EmployeeDlg : public ThemedDialog
{
	DECLARE_DYNAMIC(EmployeeDlg)

public:
	EmployeeDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMPLOYEES };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;
	void OnOK() override;
	DECLARE_MESSAGE_MAP()

	afx_msg void OnAdd();
	afx_msg void OnUpdate();
	afx_msg void OnRemove();
	afx_msg void OnClear();
	afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);

private:
	void FillList(int select = -1);
	int SelectedIndex() const;
	void UpdateButtons();
	// Builds an employee from the form, or shows an error and returns false
	bool ReadForm(std::vector<Employee>& employees, int index);
	// Saves the new list; on success it replaces the app's list
	bool Save(const std::vector<Employee>& employees);

	CListCtrl m_list;
};
