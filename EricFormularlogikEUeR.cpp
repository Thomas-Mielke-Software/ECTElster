// EricFormularlogikEUeR.cpp : implementation file
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

#include "stdafx.h"
#include "ECTElster.h"
#include "EricFormularlogikEUeR.h"

IMPLEMENT_DYNAMIC(CEricFormularlogikEUeR, CEricFormularlogik)

// TODO: E6000602 Rechtsform in <Allg> als Enumeration!!
// TODO: E6000603 Einkunftsart: Land- und Forstwirtschaft = 1, Gewerbebetrieb = 2, Selbst�ndige Arbeit = 3
// TODO: E6000604 Enumeration - 1 [Steuerpflichtige Person/Ehemann/Person A (Ehegatte A/Lebenspartner[in] A)/Gesellschaft/K�rperschaft] 2[Ehefrau / Person B(Ehegatte B / Lebenspartner[in] B)] - 3[Beide Ehegatten / Lebenspartner[innen]]
// TODO: E6000019 Wurden im Kalenderjahr / Wirtschaftsjahr Grundst�cke / grundst�cksgleiche Rechte entnommen oder ver�u�ert ? (Ja = 1, Nein = 2)
CString CEricFormularlogikEUeR::GetDatenteil()
{
    XNode xmlDatenTeil;  // xml-Ausgabe f�r ERiC (root node hier <DatenTeil>)
    xmlDatenTeil.name = "DatenTeil";
    LPXNode pNutzdatenblockNode = ZuXmlBaumHinzufuegen(&xmlDatenTeil, "Nutzdatenblock", "");

    // NutzdatenHeader    
    LPXNode pNutzdatenHeaderNode = ZuXmlBaumHinzufuegen(pNutzdatenblockNode, "NutzdatenHeader", "");
    pNutzdatenHeaderNode->AppendAttr("version", "11");
    ZuXmlBaumHinzufuegen(pNutzdatenHeaderNode, "NutzdatenTicket", m_Jetzt.Format(_T("%Y%m%d%H%M%S")));
    LPXNode pEmpfaengerNode = ZuXmlBaumHinzufuegen(pNutzdatenHeaderNode, "Empfaenger", XMLEscape(m_EmpfaengerFinanzamt));
    pEmpfaengerNode->AppendAttr("id", "F");
    LPXNode pHerstellerNode = ZuXmlBaumHinzufuegen(pNutzdatenHeaderNode, "Hersteller", "");
    ZuXmlBaumHinzufuegen(pHerstellerNode, "ProduktName", "EasyCash&amp;Tax Elster Plugin");
    ZuXmlBaumHinzufuegen(pHerstellerNode, "ProduktVersion", m_AppVersion);
    ZuXmlBaumHinzufuegen(pNutzdatenHeaderNode, "DatenLieferant", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("vorname"))
        + _T(" ") + m_pEinstellungCtrl->HoleEinstellung(_T("name")) + _T("; ") + m_EmailAdresse));

    // Nutzdaten
    LPXNode pNutzdatenNode = ZuXmlBaumHinzufuegen(pNutzdatenblockNode, "Nutzdaten", "");
    LPXNode pE77 = ZuXmlBaumHinzufuegen(pNutzdatenNode, "E77", "");
    pE77->AppendAttr("xmlns", "http://finkonsens.de/elster/elstererklaerung/euer/e77/v" + m_Jahr);
    pE77->AppendAttr("version", m_Jahr);

    // <Allg>-Feldwerte in Nutzdaten erg�nzen, wo Datenbezug nicht �ber das .eca m�glich
    LPXNode pAllg = ZuXmlBaumHinzufuegen(pE77, "EUER/Allg", "");
    ZuXmlBaumHinzufuegen(pAllg, "E6000016", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("vorname")) + _T(" ") + m_pEinstellungCtrl->HoleEinstellung(_T("name"))));
    ZuXmlBaumHinzufuegen(pAllg, "E6000023", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("strasse"))));
    ZuXmlBaumHinzufuegen(pAllg, "E6000024", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("plz"))));
    ZuXmlBaumHinzufuegen(pAllg, "E6000025", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("ort"))));
    ZuXmlBaumHinzufuegen(pAllg, "E6000017", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("unternehmensart1"))));
    ZuXmlBaumHinzufuegen(pAllg, "E6000602", "140");
    ZuXmlBaumHinzufuegen(pAllg, "E6000603", "3");
    ZuXmlBaumHinzufuegen(pAllg, "E6000604", "1");
    ZuXmlBaumHinzufuegen(pAllg, "E6000019", "2");

    // .eca-Formular -> ERiC-XML
    ZuXmlBaumHinzufuegen(pE77, "EUER/BEin", ""); /// Reihenfolge vordefinieren
    ZuXmlBaumHinzufuegen(pE77, "EUER/BAus", ""); //
    WerteAusEcaFormularGenerieren(pE77);  // <EUER> etc.

    return xmlDatenTeil.GetXML();  
}

CString CEricFormularlogikEUeR::GetLoginfo()
{
	return _T(" / E�R-Gewinn: ") + m_GewinnMerken + _T(" / Jahr: ") + m_Jahr;
}

void CEricFormularlogikEUeR::UebertragungAbschliessen()
{

}

void CEricFormularlogikEUeR::WerteAusEcaFormularGenerieren(LPXNode pXmlOut)
{
    // Formulardefinitionsdatei in xmldoc laden
    XDoc xmldoc;
    xmldoc.LoadFile(m_FormularDateipfad);

    LPXNode xmlIn = xmldoc.GetRoot();  // XML-Input von der .eca-Datei

    if (xmlIn)
    {
        LPXNode felder = NULL;
        felder = xmlIn->Find("felder");
        if (felder)
        {
            for (const auto& feld : felder->GetChilds())
            {
                int nID = 0;
                CString csElsterFeldname;
                CString csTyp;
                CString csFeldname;
                CString csFeldwert;
                nID = atoi(feld->GetAttrValue("id"));
                csFeldwert = m_pFormularCtrl->HoleFeldwertUeberID(nID);
                if (csFeldwert.IsEmpty()) continue;  // keine XML-Elemente f�r leere Feldwerte bauen
                csFeldname = feld->GetChildValue(_T("name"));
                csTyp = feld->GetAttrValue("typ");
                csElsterFeldname = feld->GetAttrValue("elsterfeldname");
                ZuXmlBaumHinzufuegen(pXmlOut, csElsterFeldname, csFeldwert);
            }
        }
    }
}

LPXNode CEricFormularlogikEUeR::ZuXmlBaumHinzufuegen(LPXNode pEricXml, const CString &csElsterFeldname, const CString &csFeldwert)
{
    // Elster-Feldnamen auseinanderpfl�cken und in vector packen
    int nTokenPos = 0;
    std::vector<CString> ElsterFeldnamenselemente;
    CString csElem;  // jeweiliges Element im durch '/' getrennten 'Pfad'
    while ((csElem = csElsterFeldname.Tokenize(_T("/"), nTokenPos)) != "")
        ElsterFeldnamenselemente.push_back(csElem);

    // gem�� Elster-Feldnamen in XML-Baumstruktur herabsteigen und ggf neu einsortieren (absteigend)
    LPXNode node = pEricXml;
    for (const auto& csEbene : ElsterFeldnamenselemente)
    {
        LPXNode foundNode = node->Find(csEbene);
        if (foundNode)          // existiert Pfadebene bereits im Baum?
            node = foundNode;   // dann einfach herabsteigen
        else
        {                       // ansonsten auf dieser Ebene einsortieren
            // XNodes nodes = node->GetChilds();
            // node->insert
            // (
            //     std::upper_bound(nodes.begin(), nodes.end(), ),
            //     item
            // );

            node = node->AppendChild(csEbene);  // in neu angeh�ngte Ebene herabsteigen
        }
    }

    if (node != pEricXml)  // wenn Reise durch den Baum erfolgreich, dann Wert eintragen
        node->value = csFeldwert;

    return node;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	LISTE AUFBAUEN
//
void CEricFormularlogikEUeR::UpdateListe(CString& csFormularDateipfad, CString (&m_pListeInhalt)[500][5], BOOL bNurSpaltenbreitenAnpassen)
{
}