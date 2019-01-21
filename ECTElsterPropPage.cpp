// ECTElsterPropPage.cpp : Implementation of the CECTElsterPropPage property page class.

#include "stdafx.h"
#include "ECTElster.h"
#include "ECTElsterPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CECTElsterPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CECTElsterPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CECTElsterPropPage, "ECTELSTER.ECTElsterPropPage.1",
	0xa58f19a0, 0xfedb, 0x4f7c, 0x8e, 0xc0, 0x24, 0x8b, 0xa2, 0xf5, 0x5a, 0x47)



// CECTElsterPropPage::CECTElsterPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CECTElsterPropPage

BOOL CECTElsterPropPage::CECTElsterPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ECTELSTER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CECTElsterPropPage::CECTElsterPropPage - Constructor

CECTElsterPropPage::CECTElsterPropPage() :
	COlePropertyPage(IDD, IDS_ECTELSTER_PPG_CAPTION)
{
}



// CECTElsterPropPage::DoDataExchange - Moves data between page and properties

void CECTElsterPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CECTElsterPropPage message handlers
