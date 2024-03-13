// EricFormularlogikUStVA.h : Declaration of the CElsterDlg property page class.
// CEricFormularlogikUStVA : See EricFormularlogikUStVA.cpp for implementation.
//
// Copyright (C) 2023  Thomas Mielke
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
#include "EricFormularlogik.h"

class CEricFormularlogikUStVA :
    public CEricFormularlogik
{
    DECLARE_DYNAMIC(CEricFormularlogikUStVA)
    virtual CString GetDatenteil();
    virtual CString GetLoginfo();
    virtual void UebertragungAbschliessen();
    virtual void UpdateListe(CString& csMomentanerFormularAnzeigename, CString& csJahr, CString& csFormularDateipfad, CString& csBetrieb, CString (&m_pListeInhalt)[500][6], CQuickList* pListe, BOOL bNurSpaltenbreitenAnpassen = FALSE);
    virtual CString GetVerfahren() { return (CString)_T("ElsterAnmeldung"); };
    virtual CString GetDatenart() { return (CString)_T("UStVA"); };
    virtual CString GetVersandbestaetigungPrefix() { return (CString)_T("UStVA") + m_Zeitraum + _T("_"); };

private:
    CString m_FeldWert83Merken;
};

