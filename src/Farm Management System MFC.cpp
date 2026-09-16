// Farm Management System MFC.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "Farm Management System MFC.h"
#include "Farm Management System MFCDlg.h"
#include "HomePageDlg.h"
#include "UiTheme.h"
#include <stdexcept>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFarmManagementSystemMFCApp

BEGIN_MESSAGE_MAP(CFarmManagementSystemMFCApp, CWinApp)
END_MESSAGE_MAP()


// CFarmManagementSystemMFCApp construction

CFarmManagementSystemMFCApp::CFarmManagementSystemMFCApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


// The one and only CFarmManagementSystemMFCApp object

CFarmManagementSystemMFCApp theApp;

FarmData& GetFarm()
{
	return theApp.m_farm;
}

namespace
{
	// Shows a dialog as the app's main window. MFC posts WM_QUIT when the main
	// window is destroyed; that message is removed so the next dialog can run.
	INT_PTR RunMainDialog(CDialog& dlg)
	{
		AfxGetApp()->m_pMainWnd = &dlg;
		INT_PTR result = dlg.DoModal();
		AfxGetApp()->m_pMainWnd = nullptr;
		MSG msg;
		while (::PeekMessage(&msg, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE))
		{
		}
		return result;
	}
}


// CFarmManagementSystemMFCApp initialization

BOOL CFarmManagementSystemMFCApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES | ICC_DATE_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("Farm Management System"));

	// The data files are opened by relative path, so a missing file usually
	// means the app was started from the wrong folder. Missing files in data/
	// are first copied from defaults/.
	try
	{
		m_farm.loadAll();
	}
	catch (const std::exception& e)
	{
		CString message;
		message.Format(_T("The farm data could not be loaded.\n\n%s\n\n")
			_T("If a file is missing, start the program from the folder that contains the \"defaults\" folder."),
			(LPCTSTR)ToCString(e.what()));
		AfxMessageBox(message, MB_ICONERROR);
		return FALSE;
	}

	// Show the login screen, then the home page. Logging out returns to the login screen.
	for (;;)
	{
		CFarmManagementSystemMFCDlg login;
		if (RunMainDialog(login) != IDOK)
		{
			break;
		}
		HomePageDlg home;
		if (RunMainDialog(home) != IDC_LOGOUT)
		{
			break;
		}
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
