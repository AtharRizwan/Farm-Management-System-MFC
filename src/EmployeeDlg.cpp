// EmployeeDlg.cpp : implementation file
//

#include "pch.h"
#include "Farm Management System MFC.h"
#include "afxdialogex.h"
#include "EmployeeDlg.h"
#include "employee.h"
#include <vector>
#include <fstream>
#include <string>
using namespace std;

std::vector <Employee> employees;
// Load all employees into this vector


// EmployeeDlg dialog

IMPLEMENT_DYNAMIC(EmployeeDlg, CDialog)


HBRUSH EmployeeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Check if the control is the dialog itself
	if (pWnd == this) {
		// Set the background color of the dialog
		pDC->SetBkColor(RGB(104, 166, 145));  // Set your desired background color here
		hbr = m_backgroundBrush;
	}

	return hbr;
}


BOOL EmployeeDlg::OnInitDialog()
{
	// Call the base class implementation
	BOOL bResult = CDialog::OnInitDialog();
	string buffer;
	std::fstream file;
	// Perform your desired task here
	m_backgroundBrush.CreateSolidBrush(RGB(104, 166, 145));
	try
	{
		file.open("data/Employee.txt", std::ios::in | std::ios::binary);
		if (!file.is_open())
		{
			throw "Unable to open file";
		}
		// Load employee data
		while (file)
		{
			std::string name;
			int age;
			double salary;
			file >> name;
			file >> buffer;
			age = stoi(buffer);
			file >> buffer;
			salary = stod(buffer);
			buffer.clear();
			Employee employee(name, age, salary);
			employees.push_back(employee);

			CStringA dataStringA(employee.getName().c_str());
			CString dataString(dataStringA);

			dataString.AppendFormat(_T(", Age: %d, Salary: %.2f"), employee.getAge(), employee.getSalary());
			EmpData.AddString(dataString); // Add the data string to the list box
		}
		file.close();
	}
	catch (...)
	{
		AfxMessageBox(L"Opened!");
	}

	
	// Return the result
	return bResult;
}

EmployeeDlg::EmployeeDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG3, pParent)
{

}

EmployeeDlg::~EmployeeDlg()
{
}

void EmployeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, listEmpData, EmpData);
}


BEGIN_MESSAGE_MAP(EmployeeDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(btnAddEmployee, &EmployeeDlg::OnBnClickedbtnaddemployee)
	ON_BN_CLICKED(btnAddEmployee2, &EmployeeDlg::OnBnClickedbtnaddemployee2)
END_MESSAGE_MAP()


// EmployeeDlg message handlers


void EmployeeDlg::OnBnClickedbtnaddemployee()
{
	// TODO: Add your control notification handler code here
	string buffer;
	std::fstream file;
	try
	{
		file.open("data/Employee.txt", std::ios::app);
		if (!file.is_open())
		{
			throw "Unable to open file";
		}
		// Load employee data
		while (file)
		{
			std::string name;
			int age;
			double salary;
			file >> name;
			file >> buffer;
			age = stoi(buffer);
			file >> buffer;
			salary = stod(buffer);
			Employee employee(name, age, salary);
			employees.push_back(employee);
		}
		file.close();
	}
	catch (...)
	{
		AfxMessageBox(L"Error!");
	}
}


void EmployeeDlg::OnBnClickedbtnaddemployee2()
{
	CString input;
	string name;
	int age;
	double salary;
	try
	{
		// TODO: Add your control notification handler code here
		GetDlgItemText(txtName, input);
		CT2A pszConvertedStringWheat(input);
		std::string name_(pszConvertedStringWheat);
		name = name_;
		GetDlgItemText(txtAge, input);
		age = (_ttoi(input));
		GetDlgItemText(txtSalary, input);
		salary = (_ttof(input));
		Employee employee(name, age, salary);
		employees.push_back(employee);
		string buffer;
		std::fstream file;

		file.open("data/Employee.txt", std::ios::app);
		if (!file.is_open())
		{
			throw "Unable to open file";
		}
		// Add file to employee data
		file << employee.getName() + " " + to_string(employee.getAge()) + " " + to_string(employee.getSalary()) << endl;
		file.close();

		file.open("data/Employee.txt", std::ios::in);
		if (!file.is_open())
		{
			throw "Unable to open file";
		}
		// Load employee data
			CStringA dataStringA(employee.getName().c_str());
			CString dataString(dataStringA);

			dataString.AppendFormat(_T(", Age: %d, Salary: %.2f"), employee.getAge(), employee.getSalary());
			EmpData.AddString(dataString); // Add the data string to the list box
		file.close();
		AfxMessageBox(L"Added Successfully!");
	}
	catch (...)
	{
		AfxMessageBox(L"Error!");
	}



}
