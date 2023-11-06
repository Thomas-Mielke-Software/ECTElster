// EricFormularlogikEUeR.h : Declaration of the CElsterDlg property page class.
// CEricFormularlogikEUeR : See EricFormularlogikEUeR.cpp for implementation.
//
// Copyright (C) 2023  Thomas Mielke
// 
// Diese Bibliothek ist freie Software; Sie d�rfen sie unter den Bedingungen
// der GNU Lesser General Public License, wie von der Free Software Foundation 
// ver�ffentlicht, weiterverteilen und/oder modifizieren; entweder gem�� 
// Version 2.1 der Lizenz oder (nach Ihrer Option) jeder sp�teren Version.
//
// Diese Bibliothek wird in der Hoffnung weiterverbreitet, da� sie n�tzlich 
// sein wird, jedoch OHNE IRGENDEINE GARANTIE, auch ohne die implizierte 
// Garantie der MARKTREIFE oder der VERWENDBARKEIT F�R EINEN BESTIMMTEN ZWECK.
// Mehr Details finden Sie in der GNU Lesser General Public License.
//
// Sie sollten eine Kopie der GNU Lesser General Public License zusammen mit 
// dieser Bibliothek erhalten haben; falls nicht, schreiben Sie an die Free 
// Software Foundation, Inc., 51 Franklin St, 5th Floor, Boston, MA 02110, USA. 

#pragma once
#include "XMLite.h"  // https://www.codeproject.com/Articles/3426/XMLite-simple-XML-parser
#include "EricFormularlogik.h"

class CEricFormularlogikEUeR :
    public CEricFormularlogik
{
    DECLARE_DYNAMIC(CEricFormularlogikEUeR)
    virtual CString GetDatenteil();
    virtual CString GetLoginfo();
    virtual void UebertragungAbschliessen();
    virtual void UpdateListe(CString& csFormularDateipfad, CString (&m_pListeInhalt)[500][5], BOOL bNurSpaltenbreitenAnpassen = FALSE);
    virtual CString GetVerfahren() { return (CString)_T("ElsterErklaerung"); };
    virtual CString GetDatenart() { return (CString)_T("EUER"); };

private:
    CString m_GewinnMerken;

    // interne Hilfsfunktion
    void WerteAusEcaFormularGenerieren(LPXNode pXmlOut);
    LPXNode ZuXmlBaumHinzufuegen(LPXNode pEricXml, const CString &csElsterFeldname, const CString &csFeldwert);
};