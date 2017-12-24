
// CAD_V1.h : main header file for the CAD_V1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <CONIO.H> 
// CCAD_V1App:
// See CAD_V1.cpp for the implementation of this class
//

class CCAD_V1App : public CWinApp
{
public:
	CCAD_V1App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCAD_V1App theApp;
