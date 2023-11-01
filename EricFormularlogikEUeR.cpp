// EricFormularlogikEUeR.cpp : implementation file
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

#include "stdafx.h"
#include "ECTElster.h"
#include "EricFormularlogikEUeR.h"
#include "XMLite.h"  // https://www.codeproject.com/Articles/3426/XMLite-simple-XML-parser

// TODO: E6000602 Rechtsform in <Allg> als Enumeration!!
// TODO: E6000603 Einkunftsart: Land- und Forstwirtschaft = 1, Gewerbebetrieb = 2, Selbständige Arbeit = 3
// TODO: E6000604 Enumeration - 1 [Steuerpflichtige Person/Ehemann/Person A (Ehegatte A/Lebenspartner[in] A)/Gesellschaft/Körperschaft] 2[Ehefrau / Person B(Ehegatte B / Lebenspartner[in] B)] - 3[Beide Ehegatten / Lebenspartner[innen]]
// TODO: E6000019 Wurden im Kalenderjahr / Wirtschaftsjahr Grundstücke / grundstücksgleiche Rechte entnommen oder veräußert ? (Ja = 1, Nein = 2)
CString CEricFormularlogikEUeR::GetDatenteil()
{
	CString DatenTeil =
		_T("<DatenTeil>\r\n\
<Nutzdatenblock>\r\n\
<NutzdatenHeader version=\"11\">\r\n\
<NutzdatenTicket>") + m_Jetzt.Format(_T("%Y%m%d%H%M%S")) + _T("</NutzdatenTicket>\r\n\
<Empfaenger id=\"F\">") + XMLEscape(m_EmpfaengerFinanzamt) + _T("</Empfaenger>\r\n\
<Hersteller>\r\n\
<ProduktName>EasyCash&amp;Tax Elster Plugin</ProduktName>\r\n\
<ProduktVersion>") + XMLEscape(m_AppVersion) + _T("</ProduktVersion>\r\n\
</Hersteller>\r\n\
<DatenLieferant>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("vorname"))
	+ _T(" ") + m_pEinstellungCtrl->HoleEinstellung(_T("name")) + _T("; ") + m_EmailAdresse) + _T("</DatenLieferant>\r\n\
</NutzdatenHeader>\r\n\
<Nutzdaten>\r\n\
<E77 xmlns=\"http://finkonsens.de/elster/elstererklaerung/euer/e77/v") + m_Jahr + _T("\" version=\"") + m_Jahr + _T("\">\r\n\
<EUER>\r\n\
	<Allg>\r\n\
		<E6000016>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("vorname")) + _T(" ") + m_pEinstellungCtrl->HoleEinstellung(_T("name"))) + _T("</E6000016>\r\n\
		<E6000023>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("strasse"))) + _T("</E6000023>\r\n\
        <E6000024>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("plz"))) + _T("</E6000024> \r\n\
        <E6000025>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("ort"))) + _T("</E6000025>\r\n\
        <E6000017>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("unternehmensart1"))) + _T("</E6000017>\r\n\
        <E6000602>140</E6000602> \r\n\
        <E6000603>3</E6000603> \r\n\
        <E6000604>1</E6000604> \r\n\
        <E6000019>2</E6000019> \r\n\
    </Allg>\r\n"
+ Summe()
 +
    /*"	<BAus>\r\n\
        <Ware_Rohstoff_Hilfsstoff>\r\n\
            <Sum>\r\n\
            <E6001601>500,00</E6001601>\r\n\
            </Sum>\r\n\
        </Ware_Rohstoff_Hilfsstoff>\r\n\
	</BAus>\r\n\
    <Ermittlung_Gewinn>\r\n\
        <Uebertrag>\r\n\
            <E6005501>500,00</E6005501>\r\n\
            <E6005601>500,00</E6005601>\r\n\
        </Uebertrag>\r\n\
        <Korrektur_GuV>\r\n\
            <E6006801>0,00</E6006801>\r\n\
        </Korrektur_GuV>\r\n\
        <Stpfl_GuV>\r\n\
            <E6007002>500,00</E6007002>\r\n\
            <E6007202>500,00</E6007202>\r\n\
        </Stpfl_GuV>\r\n\
    </Ermittlung_Gewinn>\r\n\
</EUER>\r\n\*/
"</E77>\r\n\
</Nutzdaten>\r\n\
</Nutzdatenblock>\r\n\
</DatenTeil>");
	return DatenTeil;
}

CString CEricFormularlogikEUeR::GetLoginfo()
{
	return _T(" / EÜR-Gewinn: ") + m_GewinnMerken + _T(" / Jahr: ") + m_Jahr;
}

void CEricFormularlogikEUeR::UebertragungAbschliessen()
{

}

CString CEricFormularlogikEUeR::Summe()
{
    // Formulardefinitionsdatei in xmldoc laden
    XDoc xmldoc;
    xmldoc.LoadFile(m_FormularDateipfad);

    LPXNode xmlIn = xmldoc.GetRoot();  // XML-Input von der .eca-Datei
    XNode xmlOut;  // xml-Ausgabe für ERiC (root node)

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
                if (csFeldwert.IsEmpty()) continue;  // keine XML-Elemente für leere Feldwerte bauen
                csFeldname = feld->GetChildValue(_T("name"));
                csTyp = feld->GetAttrValue("typ");
                csElsterFeldname = feld->GetAttrValue("elsterfeldname");

                // Elster-Feldnamen auseinanderpflücken und in vector packen
                int nTokenPos = 0;
                std::vector<CString> ElsterFeldnamenselemente;
                CString csElem;  // jeweiliges Element im durch '/' getrennten 'Pfad'
                while ((csElem = csElsterFeldname.Tokenize(_T("/"), nTokenPos)) != "")
                    ElsterFeldnamenselemente.push_back(csElem);

                // gemäß Elster-Feldnamen in XML-Baumstruktur herabsteigen und ggf neu einsortieren (absteigend)
                LPXNode node = &xmlOut;
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

                        node = node->AppendChild(csEbene);  // in neu angehängte Ebene herabsteigen
                    }
                }

                if (node != &xmlOut)  // wenn Reise durch den Baum erfolgreich, dann Wert eintragen
                    node->value = csFeldwert;
            }
        }
    }
    return xmlOut.GetXML();
}