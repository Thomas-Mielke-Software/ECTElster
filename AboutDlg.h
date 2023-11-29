#pragma once


// CAboutDlg dialog

#include "afxlinkctrl.h"

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX_ECTELSTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Version;
	CMFCLinkCtrl m_GitHub;
	CMFCLinkCtrl m_EctForum;
	virtual BOOL OnInitDialog();
};
