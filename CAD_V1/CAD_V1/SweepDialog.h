#pragma once


// SweepDialog dialog

class SweepDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SweepDialog)

public:
	SweepDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SweepDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	GLdouble sweep_vx;
	GLdouble sweep_vy;
	GLdouble sweep_vz;
};
