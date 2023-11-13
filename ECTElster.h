// ECTElster.h : main header file for ECTElster.ocx
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

#if !defined( __AFXCTL_H__ )
#error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols


// CECTElsterApp : See ECTElster.cpp for implementation.

class CECTElsterApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
	CString & GetVersion(void);
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

// auskommentieren, um echte Daten senden zu können:
//#define TESTVERBINDUNG
//!!!!!!!!!!!! und nicht vergessen: bei einem neuen Jahr das Copyright-Datum hochsetzen!!!
#if defined(NDEBUG)
#if defined(TESTVERBINDUNG)
#pragma message("")
#pragma message("    ________________________________________________________________")
#pragma message("    ACHTUNG: TESTVERBINDUNG IST NOCH EINGESCHALTET IN RELEASE-BUILD!")
#pragma message("    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^")
#endif
#endif

// globale Hilfsfunktionen
extern CString XMLEscape(CString StringZumEscapen);
extern void ERiC(BOOL bNurValidieren);
extern void Ansi2Utf8(CString ansiText, CStringA& utf8Text);
extern void Utf8toAnsi(CStringA utf8Text, CString& ansiText);
extern int RegSearchReplace(CString& string, LPCTSTR sSearchExp, LPCTSTR sReplaceExp, CStringArray& csaReplaceCount);	// Helfer für reguläre Ausdrücke
extern void PrintString(CString Dokumentname, CString Text);
