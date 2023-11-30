// EricFormularlogikEUeR.h : Declaration of the CElsterDlg property page class.
// CEricFormularlogikEUeR : See EricFormularlogikEUeR.cpp for implementation.
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
#include "XMLite.h"  // https://www.codeproject.com/Articles/3426/XMLite-simple-XML-parser
#include "EricFormularlogik.h"

// CEricFormularlogikEUeR::WerteAusEcaFormularGenerieren oder CEricFormularlogikEUeR::Aveur, jeweils letzter Parameter
#define FLAG_GEN_XML        0x1     // Ausgabe als ERiC-XML
#define FLAG_GEN_INTERN     0x2     // Ausgabe in internem Format (std::vector oder Listen-Array)
#define FLAG_GEN_ABSCHNITTE 0x4     // generiert Abschnittsüberschriften

class Formularfeld
{
public:
    int id;
    CString name;
    CString typ;
    int seite;
    int horizontal;
    int vertikal;
    CString elsterFeldname;
    CString wert;

    Formularfeld() {}
    Formularfeld(
        int nID,
        CString csName,
        CString csTyp,
        int nSeite,
        int nHorizontal,
        int nVertikal,
        CString csElsterFeldname,
        CString csWert
    ) :
        id(nID),
        name(csName),
        typ(csTyp),
        seite(nSeite),
        horizontal(nHorizontal),
        vertikal(nVertikal),
        elsterFeldname(csElsterFeldname),
        wert(csWert) 
    {
    }
};


class Formularabschnitt
{
public:
    CString name;
    int seite;
    int vertikal;

    Formularabschnitt() {}
    Formularabschnitt(
        CString csName,
        int nSeite,
        int nVertikal
    ) :
        name(csName),
        seite(nSeite),
        vertikal(nVertikal)
    {
    }
};

class CEricFormularlogikEUeR :
    public CEricFormularlogik
{
    DECLARE_DYNAMIC(CEricFormularlogikEUeR)
    virtual CString GetDatenteil();
    virtual CString GetLoginfo();
    virtual void UebertragungAbschliessen();
    virtual void UpdateListe(CString& csFormularDateipfad, CString& csBetrieb, CString (&m_pListeInhalt)[500][6], CQuickList* pListe, BOOL bNurSpaltenbreitenAnpassen = FALSE);
    virtual CString GetVerfahren() { return (CString)_T("ElsterErklaerung"); };
    virtual CString GetDatenart() { return (CString)_T("EUER"); };
    virtual CString GetVersandbestaetigungPrefix() { 
        CString csTempBetrieb = m_csBetrieb;
        csTempBetrieb.Replace(_T(" "), _T("_"));
        return (CString)_T("EÜR") + m_Jahr + _T("_") + csTempBetrieb +(CString)_T("_"); };

private:
    CString m_GewinnMerken;

    // interne Hilfsfunktion
    BOOL WerteAusEcaFormularGenerieren(LPXNode pXmlOut, std::vector<Formularfeld> &felder, std::vector<Formularabschnitt>& abschnittarray, int flagsGen);
    LPXNode ZuXmlBaumHinzufuegen(LPXNode pEricXml, const CString &csElsterFeldname, const CString &csFeldwert, BOOL bRecycleBestehendenNode = TRUE);
    static bool CompareInterval(Formularfeld f1, Formularfeld f2);  // Hilfsfunktion für vector-sort
    void AveuerGenerieren(CString(&ListeInhalt)[500][6], int& Zeile, LPXNode xml, int flagsGen);
};