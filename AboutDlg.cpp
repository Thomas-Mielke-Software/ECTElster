// AboutDlg.cpp : implementation file
//
// Copyright (C) 2019  Thomas Mielke
// 
// Diese Bibliothek ist freie Software; Sie dürfen sie unter den Bedingungen
// der GNU Lesser General Public License, wie von der Free Software Foundation 
// veröffentlicht, weiterverteilen und/oder modifizieren; entweder gemäß 
// Version 2.1 der Lizenz oder (nach Ihrer Option) jeder späteren Version.
//
// Diese Bibliothek wird in der Hoffnung weiterverbreitet, daß sie nützlich 
// sein wird, jedoch OHNE IRGENDEINE GARANTIE, auch ohne die implizierte 
// Garantie der MARKTREIFE oder der VERWENDBARKEIT FÜR EINEN BESTIMMTEN ZWECK.
// Mehr Details finden Sie in der GNU Lesser General Public License.
//
// Sie sollten eine Kopie der GNU Lesser General Public License zusammen mit 
// dieser Bibliothek erhalten haben; falls nicht, schreiben Sie an die Free 
// Software Foundation, Inc., 51 Franklin St, 5th Floor, Boston, MA 02110, USA. 

#include "stdafx.h"
#include "ECTElster.h"
#include "AboutDlg.h"


// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
	, m_Version(_T(""))
{

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VERSION, m_Version);
	DDX_Control(pDX, IDC_GITHUB, m_GitHub);
	DDX_Control(pDX, IDC_ECTFORUM, m_EctForum);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAboutDlg message handlers


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_GitHub.SetURL(_T("https://github.com/Thomas-Mielke-Software/ECTElster/issues"));
	m_GitHub.SetTooltip(_T("Auf GitHub kann man sich z.B. mit einem Microsoft-Konto einloggen."));
	m_GitHub.SizeToContent();

	m_EctForum.SetURL(_T("https://easyct.de/forum/viewforum.php?forum_id=11"));
	m_EctForum.SetTooltip(_T("Ein vorheriger Login bzw. eine Registrierung ist nötig, um im EC&&T-Forum schreiben zu können."));
	m_EctForum.SizeToContent();

	return TRUE;
}
