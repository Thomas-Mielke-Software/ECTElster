// ECTElsterCtrl.h : Declaration of the CECTElsterCtrl ActiveX Control class.
// CECTElsterCtrl : See ECTElsterCtrl.cpp for implementation.
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

#pragma once
#include "elsterdlg.h"

class CECTElsterCtrl : public COleControl
{
	DECLARE_DYNCREATE(CECTElsterCtrl)

// Constructor
public:
	CECTElsterCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CECTElsterCtrl();

	DECLARE_OLECREATE_EX(CECTElsterCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CECTElsterCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CECTElsterCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CECTElsterCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidInit = 1L
	};
protected:
	void Init(LONG ID);
public:
	long m_dokID; // enthält das Dokument-Handle, um auf die EC&T-ActiveX-Objekte zugreifen zu können
	afx_msg void OnDestroy();
	// Pointer auf den Haupt-Dialog
	CElsterDlg *m_pElsterDlg;
};

