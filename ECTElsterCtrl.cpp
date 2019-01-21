// ECTElsterCtrl.cpp : Implementation of the CECTElsterCtrl ActiveX Control class.
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
#include "FormularCtrl.h"
#include "ECTElsterCtrl.h"
#include "ECTElsterPropPage.h"
#include "AboutDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CECTElsterCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CECTElsterCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CECTElsterCtrl, COleControl)
	DISP_FUNCTION_ID(CECTElsterCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CECTElsterCtrl, "Init", dispidInit, Init, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CECTElsterCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CECTElsterCtrl, 1)
	PROPPAGEID(CECTElsterPropPage::guid)
END_PROPPAGEIDS(CECTElsterCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CECTElsterCtrl, "ECTELSTER.ECTElsterCtrl.1",
	0xdf841939, 0x5148, 0x4e3a, 0x8d, 0x6e, 0xdd, 0x99, 0xde, 0x4, 0x97, 0x76)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CECTElsterCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DECTElster =
		{ 0xEE4BCF66, 0x82D9, 0x4103, { 0x8C, 0xA4, 0x97, 0x82, 0xB1, 0xA2, 0x8D, 0xA } };
const IID BASED_CODE IID_DECTElsterEvents =
		{ 0xC66F423E, 0x538C, 0x4120, { 0xA4, 0xE6, 0x5C, 0x41, 0x34, 0x5B, 0x3D, 0xAE } };



// Control type information

static const DWORD BASED_CODE _dwECTElsterOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CECTElsterCtrl, IDS_ECTELSTER, _dwECTElsterOleMisc)



// CECTElsterCtrl::CECTElsterCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CECTElsterCtrl

BOOL CECTElsterCtrl::CECTElsterCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ECTELSTER,
			IDB_ECTELSTER,
			afxRegApartmentThreading,
			_dwECTElsterOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CECTElsterCtrl::CECTElsterCtrl - Constructor

CECTElsterCtrl::CECTElsterCtrl()
: m_dokID(0)
{
	InitializeIIDs(&IID_DECTElster, &IID_DECTElsterEvents);
	m_pElsterDlg = NULL;
}



// CECTElsterCtrl::~CECTElsterCtrl - Destructor

CECTElsterCtrl::~CECTElsterCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CECTElsterCtrl::OnDraw - Drawing function

void CECTElsterCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	if (m_pElsterDlg && m_pElsterDlg->m_hWnd) m_pElsterDlg->MoveWindow(rcBounds, TRUE);
}



// CECTElsterCtrl::DoPropExchange - Persistence support

void CECTElsterCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CECTElsterCtrl::OnResetState - Reset control to default state

void CECTElsterCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CECTElsterCtrl::AboutBox - Display an "About" box to the user

void CECTElsterCtrl::AboutBox()
{
	extern CECTElsterApp theApp;
	CAboutDlg dlgAbout(this);
	dlgAbout.m_Version = theApp.GetVersion();
	dlgAbout.DoModal();
}

// Wird von EC&T aufgerufen, um das Dokument-Handle zu übergeben

void CECTElsterCtrl::Init(LONG ID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_dokID = ID;

	m_pElsterDlg = new CElsterDlg ( this );
	m_pElsterDlg->m_dokID = m_dokID;
	HRESULT nResult = m_pElsterDlg->Create(IDD_ELSTERDLG, this);
	if (!nResult) 
		AfxMessageBox(_T("Dialog Fenster konnte nicht erzeugt werden."));
	else 
		m_pElsterDlg->ShowWindow(SW_SHOW);

}

void CECTElsterCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	if (m_pElsterDlg) delete m_pElsterDlg;
}
