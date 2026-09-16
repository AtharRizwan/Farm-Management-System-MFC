// EmployeeDlg.cpp : implementation file
//

#include "pch.h"
#include "Farm Management System MFC.h"
#include "EmployeeDlg.h"
#include "employee.h"
#include <stdexcept>


// EmployeeDlg dialog

IMPLEMENT_DYNAMIC(EmployeeDlg, ThemedDialog)

EmployeeDlg::EmployeeDlg(CWnd* pParent /*=nullptr*/)
	: ThemedDialog(IDD_EMPLOYEES, pParent)
{
}

void EmployeeDlg::DoDataExchange(CDataExchange* pDX)
{
	ThemedDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EMPLOYEE_LIST, m_list);
}


BEGIN_MESSAGE_MAP(EmployeeDlg, ThemedDialog)
	ON_BN_CLICKED(IDC_EMP_ADD, &EmployeeDlg::OnAdd)
	ON_BN_CLICKED(IDC_EMP_UPDATE, &EmployeeDlg::OnUpdate)
	ON_BN_CLICKED(IDC_EMP_REMOVE, &EmployeeDlg::OnRemove)
	ON_BN_CLICKED(IDC_EMP_CLEAR, &EmployeeDlg::OnClear)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EMPLOYEE_LIST, &EmployeeDlg::OnSelectionChanged)
END_MESSAGE_MAP()


// EmployeeDlg message handlers

BOOL EmployeeDlg::OnInitDialog()
{
	ThemedDialog::OnInitDialog();

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	CRect rect;
	m_list.GetClientRect(&rect);
	int width = rect.Width();
	m_list.InsertColumn(0, _T("Name"), LVCFMT_LEFT, width * 50 / 100);
	m_list.InsertColumn(1, _T("Age"), LVCFMT_RIGHT, width * 15 / 100);
	m_list.InsertColumn(2, _T("Salary (Rs.)"), LVCFMT_RIGHT, width - width * 50 / 100 - width * 15 / 100);

	SendDlgItemMessage(IDC_EMP_NAME, EM_SETLIMITTEXT, 60);
	SendDlgItemMessage(IDC_EMP_AGE, EM_SETLIMITTEXT, 3);
	SendDlgItemMessage(IDC_EMP_SALARY, EM_SETLIMITTEXT, 15);

	FillList();
	GotoDlgCtrl(GetDlgItem(IDC_EMP_NAME));
	return FALSE;  // focus was set to a control
}

void EmployeeDlg::FillList(int select)
{
	const std::vector<Employee>& employees = GetFarm().employees;
	m_list.SetRedraw(FALSE);
	m_list.DeleteAllItems();
	for (int i = 0; i < static_cast<int>(employees.size()); ++i)
	{
		const Employee& employee = employees[i];
		m_list.InsertItem(i, ToCString(employee.getName()));
		CString age;
		age.Format(_T("%d"), employee.getAge());
		m_list.SetItemText(i, 1, age);
		m_list.SetItemText(i, 2, FormatMoney(employee.getSalary()));
	}
	if (select >= 0 && select < m_list.GetItemCount())
	{
		m_list.SetItemState(select, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_list.EnsureVisible(select, FALSE);
	}
	m_list.SetRedraw(TRUE);
	m_list.Invalidate();

	CString count;
	count.Format(employees.size() == 1 ? _T("%d employee") : _T("%d employees"), static_cast<int>(employees.size()));
	SetDlgItemText(IDC_SUBTITLE, count);
	UpdateButtons();
}

int EmployeeDlg::SelectedIndex() const
{
	return m_list.GetNextItem(-1, LVNI_SELECTED);
}

void EmployeeDlg::UpdateButtons()
{
	bool selected = SelectedIndex() >= 0;
	GetDlgItem(IDC_EMP_UPDATE)->EnableWindow(selected);
	GetDlgItem(IDC_EMP_REMOVE)->EnableWindow(selected);
}

void EmployeeDlg::OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW change = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	if (!(change->uChanged & LVIF_STATE) || ((change->uNewState ^ change->uOldState) & LVIS_SELECTED) == 0)
	{
		return;
	}
	int index = SelectedIndex();
	const std::vector<Employee>& employees = GetFarm().employees;
	if (index >= 0 && index < static_cast<int>(employees.size()))
	{
		const Employee& employee = employees[index];
		SetDlgItemText(IDC_EMP_NAME, ToCString(employee.getName()));
		SetDlgItemInt(IDC_EMP_AGE, employee.getAge(), FALSE);
		SetDlgItemText(IDC_EMP_SALARY, FormatNumber(employee.getSalary(), 2));
		ShowError(_T(""));
	}
	UpdateButtons();
}

bool EmployeeDlg::ReadForm(std::vector<Employee>& employees, int index)
{
	CString name;
	GetDlgItemText(IDC_EMP_NAME, name);
	name.Trim();
	int age = 0;
	double salary = 0;
	CString error;
	UINT badControl = 0;
	if (name.IsEmpty())
	{
		error = _T("Name is required.");
		badControl = IDC_EMP_NAME;
	}
	else if (!ReadInt(*this, IDC_EMP_AGE, _T("Age"), age, error))
	{
		badControl = IDC_EMP_AGE;
	}
	else if (!ReadDouble(*this, IDC_EMP_SALARY, _T("Salary"), salary, error))
	{
		badControl = IDC_EMP_SALARY;
	}
	else
	{
		try
		{
			Employee employee(ToUtf8(name), age, salary);
			if (index < 0)
			{
				employees.push_back(employee);
			}
			else
			{
				employees[index] = employee;
			}
			return true;
		}
		catch (const std::invalid_argument& e)
		{
			error = ToCString(e.what()) + _T(".");
			badControl = IDC_EMP_AGE;
		}
	}
	ShowError(error);
	GotoDlgCtrl(GetDlgItem(badControl));
	return false;
}

bool EmployeeDlg::Save(const std::vector<Employee>& employees)
{
	FarmData& farm = GetFarm();
	std::vector<Employee> previous = farm.employees;
	farm.employees = employees;
	try
	{
		farm.saveEmployees();
		return true;
	}
	catch (const std::exception& e)
	{
		farm.employees = previous;
		AfxMessageBox(_T("The employee list could not be saved.\n\n") + ToCString(e.what()), MB_ICONERROR);
		return false;
	}
}

void EmployeeDlg::OnAdd()
{
	std::vector<Employee> employees = GetFarm().employees;
	if (ReadForm(employees, -1) && Save(employees))
	{
		FillList(static_cast<int>(employees.size()) - 1);
		OnClear();
	}
}

void EmployeeDlg::OnUpdate()
{
	int index = SelectedIndex();
	if (index < 0)
	{
		return;
	}
	std::vector<Employee> employees = GetFarm().employees;
	if (ReadForm(employees, index) && Save(employees))
	{
		FillList(index);
	}
}

void EmployeeDlg::OnRemove()
{
	int index = SelectedIndex();
	std::vector<Employee> employees = GetFarm().employees;
	if (index < 0 || index >= static_cast<int>(employees.size()))
	{
		return;
	}
	CString question;
	question.Format(_T("Remove %s from the employee list?"), (LPCTSTR)ToCString(employees[index].getName()));
	if (AfxMessageBox(question, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
	{
		return;
	}
	employees.erase(employees.begin() + index);
	if (Save(employees))
	{
		FillList();
		OnClear();
	}
}

void EmployeeDlg::OnClear()
{
	m_list.SetItemState(-1, 0, LVIS_SELECTED);
	SetDlgItemText(IDC_EMP_NAME, _T(""));
	SetDlgItemText(IDC_EMP_AGE, _T(""));
	SetDlgItemText(IDC_EMP_SALARY, _T(""));
	ShowError(_T(""));
	UpdateButtons();
	GotoDlgCtrl(GetDlgItem(IDC_EMP_NAME));
}

// The Enter key adds a new employee, or updates the selected one
void EmployeeDlg::OnOK()
{
	if (SelectedIndex() >= 0)
	{
		OnUpdate();
	}
	else
	{
		OnAdd();
	}
}
