// Alert.cpp : implementation file
//

#include "stdafx.h"
#include "CAD_V1.h"
#include "Alert.h"
#include "afxdialogex.h"


// Alert dialog

IMPLEMENT_DYNAMIC(Alert, CDialogEx)

Alert::Alert(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

Alert::~Alert()
{
}

void Alert::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Alert, CDialogEx)
	ON_BN_CLICKED(IDOK, &Alert::OnBnClickedOk)
END_MESSAGE_MAP()


// Alert message handlers


void Alert::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
