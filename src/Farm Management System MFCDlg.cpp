// Farm Management System MFCDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Farm Management System MFC.h"
#include "Farm Management System MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	// The app has a single built-in account
	const LPCTSTR kUsername = _T("admin");
	const LPCTSTR kPassword = _T("admin");
}


// CAboutDlg dialog used for App About

class CAboutDlg : public ThemedDialog
{
public:
	CAboutDlg() : ThemedDialog(IDD_ABOUTBOX, nullptr) {}

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
};


// CFarmManagementSystemMFCDlg dialog

CFarmManagementSystemMFCDlg::CFarmManagementSystemMFCDlg(CWnd* pParent /*=nullptr*/)
	: ThemedDialog(IDD_LOGIN, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CFarmManagementSystemMFCDlg, ThemedDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CFarmManagementSystemMFCDlg message handlers

BOOL CFarmManagementSystemMFCDlg::OnInitDialog()
{
	ThemedDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		CString strAboutMenu;
		if (strAboutMenu.LoadString(IDS_ABOUTBOX) && !strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SendDlgItemMessage(IDC_USERNAME, EM_SETLIMITTEXT, 64);
	SendDlgItemMessage(IDC_PASSWORD, EM_SETLIMITTEXT, 64);
	GotoDlgCtrl(GetDlgItem(IDC_USERNAME));
	return FALSE;  // focus was set to a control
}

void CFarmManagementSystemMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		ThemedDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFarmManagementSystemMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		ThemedDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFarmManagementSystemMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Sign In button and the Enter key
void CFarmManagementSystemMFCDlg::OnOK()
{
	CString username;
	CString password;
	GetDlgItemText(IDC_USERNAME, username);
	GetDlgItemText(IDC_PASSWORD, password);
	username.Trim();

	if (username.IsEmpty() || password.IsEmpty())
	{
		ShowError(_T("Enter your username and password."));
		GotoDlgCtrl(GetDlgItem(username.IsEmpty() ? IDC_USERNAME : IDC_PASSWORD));
		return;
	}
	if (username != kUsername || password != kPassword)
	{
		ShowError(_T("Incorrect username or password."));
		SetDlgItemText(IDC_PASSWORD, _T(""));
		GotoDlgCtrl(GetDlgItem(IDC_PASSWORD));
		return;
	}
	ThemedDialog::OnOK();
}
