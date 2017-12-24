#pragma once


// Alert dialog

class Alert : public CDialogEx
{
	DECLARE_DYNAMIC(Alert)

public:
	Alert(CWnd* pParent = NULL);   // standard constructor
	virtual ~Alert();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
