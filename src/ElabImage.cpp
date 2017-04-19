// ElabImage.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ElabImage.h"
#include "ElabImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CElabImageApp

BEGIN_MESSAGE_MAP(CElabImageApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CElabImageApp construction

CElabImageApp::CElabImageApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CElabImageApp object

CElabImageApp theApp;


// CElabImageApp initialization

BOOL CElabImageApp::InitInstance()
{
	CWinApp::InitInstance();


	CElabImageDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
