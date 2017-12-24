// SweepDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CAD_V1.h"
#include "SweepDialog.h"
#include "afxdialogex.h"


// SweepDialog dialog

IMPLEMENT_DYNAMIC(SweepDialog, CDialogEx)

SweepDialog::SweepDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	sweep_vx = 0;
	sweep_vy = 0;
	sweep_vz = 0;
}

SweepDialog::~SweepDialog()
{
}

void SweepDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, sweep_vx);
	DDX_Text(pDX, IDC_EDIT2, sweep_vy);
	DDX_Text(pDX, IDC_EDIT3, sweep_vz);
}


BEGIN_MESSAGE_MAP(SweepDialog, CDialogEx)
END_MESSAGE_MAP()


// SweepDialog message handlers
