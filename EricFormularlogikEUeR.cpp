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
#include <vector> 
#include <algorithm>
#include "ECTElster.h"
#include "EricFormularlogikEUeR.h"

IMPLEMENT_DYNAMIC(CEricFormularlogikEUeR, CEricFormularlogik)

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	HILFSFUNKTION F�R DAS LESEN DES .ECA-FORMULARS
//
LPXNode CEricFormularlogikEUeR::ZuXmlBaumHinzufuegen(LPXNode pEricXml, const CString& csElsterFeldname, const CString& csFeldwert)
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
		if (foundNode)							// existiert Pfadebene bereits im Baum?
			node = foundNode;					// dann einfach herabsteigen
		else
		{                       
			node = node->AppendChild(csEbene);  // sonst in neu angeh�ngte Ebene herabsteigen
		}
	}
	if (node != pEricXml)  // wenn Reise durch den Baum erfolgreich, dann Wert eintragen
		node->value = csFeldwert;

	return node;
}

bool CEricFormularlogikEUeR::CompareInterval(Formularfeld f1, Formularfeld f2)
{
	return (f1.seite * 10000 + f1.vertikal < f2.seite * 10000 + f2.vertikal);
}

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
    ZuXmlBaumHinzufuegen(pAllg, "E6000602", m_csRechtsform);
	ZuXmlBaumHinzufuegen(pAllg, "E6000603", m_csEinkunftsart);
	ZuXmlBaumHinzufuegen(pAllg, "E6000604", m_csBetriebsinhaber);
	ZuXmlBaumHinzufuegen(pAllg, "E6000019", m_csGrundstuecksveraeusserungen);

    // .eca-Formular -> ERiC-XML
    ZuXmlBaumHinzufuegen(pE77, "EUER/BEin", ""); /// Reihenfolge vordefinieren
    ZuXmlBaumHinzufuegen(pE77, "EUER/BAus", ""); //
	std::vector<Formularfeld> dummyFeldarray(0);
	std::vector<Formularabschnitt> dummyAbschnittarray(0);
    WerteAusEcaFormularGenerieren(pE77, dummyFeldarray, dummyAbschnittarray, FLAG_GEN_XML);  // <EUER> etc.

	// "Vorsatz"
	LPXNode pVorsatz = ZuXmlBaumHinzufuegen(pE77, "Vorsatz", "");
	ZuXmlBaumHinzufuegen(pVorsatz, "Unterfallart", "77");
    ZuXmlBaumHinzufuegen(pVorsatz, "Vorgang", "01");
	ZuXmlBaumHinzufuegen(pVorsatz, "StNr", m_Bundesfinanzamtsnummer);
	ZuXmlBaumHinzufuegen(pVorsatz, "Zeitraum", m_Jahr);								// >2021</Zeitraum>
	ZuXmlBaumHinzufuegen(pVorsatz, "AbsName	", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("vorname")) + _T(" ") + m_pEinstellungCtrl->HoleEinstellung(_T("name"))));
	ZuXmlBaumHinzufuegen(pVorsatz, "AbsStr", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("strasse"))));
	ZuXmlBaumHinzufuegen(pVorsatz, "AbsPlz", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("plz"))));
	ZuXmlBaumHinzufuegen(pVorsatz, "AbsOrt", XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("ort"))));
	ZuXmlBaumHinzufuegen(pVorsatz, "Copyright", "2023 Thomas Mielke (GNU LGPL v2.1)");
	ZuXmlBaumHinzufuegen(pVorsatz, "OrdNrArt", "S");
	ZuXmlBaumHinzufuegen(pVorsatz, "Rueckuebermittlung/Bescheid", "2");  // TODO: Rueckuebermittlung/Bescheid Checkbox
    return xmlDatenTeil.GetXML();  
}

CString CEricFormularlogikEUeR::GetLoginfo()
{
	return _T(" / E�R-Gewinn: ") + m_GewinnMerken + _T(" / Jahr: ") + m_Jahr;
}

void CEricFormularlogikEUeR::UebertragungAbschliessen()
{

}

void CEricFormularlogikEUeR::WerteAusEcaFormularGenerieren(LPXNode pXmlOut, std::vector<Formularfeld> &feldarray, std::vector<Formularabschnitt>& abschnittarray, int flagsGen)
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
			if (flagsGen & FLAG_GEN_STDVECTOR) feldarray.reserve(felder->GetChildCount());
            for (const auto& feld : felder->GetChilds())
            {
                int nID = 0;
                CString csFeldname;
				CString csTyp;
				CString csElsterFeldname;
                CString csFeldwert;
                nID = atoi(feld->GetAttrValue("id"));
                csFeldwert = m_pFormularCtrl->HoleFeldwertUeberID(nID);
                if (csFeldwert.IsEmpty()) continue;  // keine XML-Elemente f�r leere Feldwerte bauen
                csFeldname = feld->GetChildValue(_T("name"));
                csTyp = feld->GetAttrValue("typ");
                csElsterFeldname = feld->GetAttrValue("elsterfeldname");
				if (flagsGen & FLAG_GEN_XML)
					ZuXmlBaumHinzufuegen(pXmlOut, csElsterFeldname, csFeldwert);
				if (flagsGen & FLAG_GEN_STDVECTOR)
				{
					int seite = atoi(feld->GetAttrValue("seite"));
					int horizontal = atoi(feld->GetAttrValue("horizontal"));
					int vertikal = atoi(feld->GetAttrValue("vertikal"));
					Formularfeld HinzuzufuegendesFeld(
						nID,
						csFeldname,
						csTyp,
						seite,
						horizontal,
						vertikal,
						csElsterFeldname,
						csFeldwert
					);
					feldarray.push_back(HinzuzufuegendesFeld);
				}
            }
        }

		LPXNode abschnitte = NULL;
		abschnitte = xmlIn->Find("abschnitte");
		if (abschnitte && (flagsGen & FLAG_GEN_ABSCHNITTE))
		{
			abschnittarray.reserve(abschnitte->GetChildCount());
			for (const auto& abschnitt : abschnitte->GetChilds())
			{
				CString csAbschnittname = abschnitt->GetAttrValue(_T("name"));
				int seite = atoi(abschnitt->GetAttrValue("seite"));
				int vertikal = atoi(abschnitt->GetAttrValue("vertikal"));
				Formularabschnitt HinzuzufuegendesFeld(
					csAbschnittname,
					seite,
					vertikal
				);
				abschnittarray.push_back(HinzuzufuegendesFeld);
			}
		}		
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	LISTE AUFBAUEN
//
void CEricFormularlogikEUeR::UpdateListe(CString& csFormularDateipfad, CString(&ListeInhalt)[500][6], CQuickList* pListe, BOOL bNurSpaltenbreitenAnpassen)
{
	m_pListe = pListe;
	m_FormularDateipfad = csFormularDateipfad;

	// ggf. alte Liste l�schen
	while (m_pListe->DeleteColumn(0));

	// wieviel Platz haben wir?
	RECT r;
	m_pListe->GetWindowRect(&r);
	int ListeBreite = r.right - r.left - 21;

	// Spaltenbreiten bestimmen
#pragma warning(push)
#pragma warning(disable:6001)
	static _TCHAR* Spaltentitel[] = { _T("Beschreibung"), _T("Feld-Nr."), _T("Nicht abz."), _T("Feld-Nr."), _T("Abziehbar") };
	static int AnzahlSpalten = sizeof(Spaltentitel) / sizeof(Spaltentitel[0]);
	int Spaltenbreite[sizeof(Spaltentitel) / sizeof(Spaltentitel[0])];
	int Spaltenzaehler;
	for (Spaltenzaehler = 0; Spaltenzaehler < AnzahlSpalten; Spaltenzaehler++)
		Spaltenbreite[Spaltenzaehler] = m_pListe->GetStringWidth(Spaltentitel[Spaltenzaehler]) * 4 / 3;
	Spaltenbreite[2] = Spaltenbreite[4] = max(Spaltenbreite[2], Spaltenbreite[4]); // Spalte 2 und 4 sollen gleich breit sein
	Spaltenbreite[0] = ListeBreite - Spaltenbreite[1] - Spaltenbreite[2] - Spaltenbreite[3] - Spaltenbreite[4];	// Beschreibungstext bekommt war �brig ist...
	if (Spaltenbreite[0] < Spaltenbreite[1] + Spaltenbreite[2])	// ... es sei denn, es ist zu wenig
		Spaltenbreite[0] = Spaltenbreite[1] + Spaltenbreite[2];
#pragma warning(pop)

	// Listenheader aufbauen
	m_pListe->SetExtendedStyle(m_pListe->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	for (Spaltenzaehler = 0; Spaltenzaehler < AnzahlSpalten; Spaltenzaehler++)
		m_pListe->InsertColumn(Spaltenzaehler, Spaltentitel[Spaltenzaehler], LVCFMT_LEFT, Spaltenbreite[Spaltenzaehler], -1);

	if (bNurSpaltenbreitenAnpassen) return;	// --> hier schon raus, wenn sich der Inhalt nicht ge�ndert hat

	// Formular-Dateipfad holen
	if (!m_FormularDateipfad.GetLength())
	{
		AfxMessageBox(_T("Ich kann den Dateipfad des Formulars nicht ermitteln. Bitte �berpr�fen, ob die entsprechenden UStVA-Formulare f�r das aktuelle Jahr installiert sind. Die sind �blicherweise in der aktuellen Version des EC&T-Hauptprogramms enthalten oder sind zum Entpacken ins Programmverzeichnis in Form eines Zip-Archivs auf www.easyct.de unter Downloads -> Formular-Archiv verf�gbar. Die Formulare f�r das neue Jahr werden �blicher Weise im Laufe des Januars �ber ein Hauptprogramm-Update verf�gbar."));
		return;
	}

	// Formular vorbereiten
	m_pFormularCtrl->WaehleFormular(m_FormularDateipfad);

	// Formularlayout aus .eca-Datei einlesen
	std::vector<Formularfeld> Formularfelder(0);
	std::vector<Formularabschnitt> Abschnitte(0);
	WerteAusEcaFormularGenerieren(NULL, Formularfelder, Abschnitte, FLAG_GEN_STDVECTOR | FLAG_GEN_ABSCHNITTE);
	sort(Formularfelder.begin(), Formularfelder.end(), CEricFormularlogikEUeR::CompareInterval);

	// Formularlayout in Liste �bertragen
	int Zeile, Spalte;
	CString FeldWert83Merken;
	BOOL bSternchenHinweisAnzeigen = FALSE;
	for (Zeile = 0; Zeile < sizeof(ListeInhalt) / sizeof(ListeInhalt[0]); Zeile++)	// Listenspeicher initialisieren
		for (Spalte = 0; Spalte < sizeof(ListeInhalt[0]) / sizeof(ListeInhalt[0][0]); Spalte++)
			ListeInhalt[Zeile][Spalte] = _T("");
	Zeile = 0;
	auto abschnitt = begin(Abschnitte);
	for (const auto& feld : Formularfelder)
	{
		if (!feld.elsterFeldname.IsEmpty())
		{
			// Muss noch eine Abschnitts�berschift davor?
			auto letzteVorhergehendeAbschnittsueberschrift = end(Abschnitte);
			while (abschnitt != end(Abschnitte)
				&& abschnitt->seite * 10000 + abschnitt->vertikal < feld.seite * 10000 + feld.vertikal)
			{
				letzteVorhergehendeAbschnittsueberschrift = abschnitt++;
			}
			if (letzteVorhergehendeAbschnittsueberschrift != end(Abschnitte))
			{
				Zeile++;
				ListeInhalt[Zeile++][0] = letzteVorhergehendeAbschnittsueberschrift->name;
			}

			// Feld in Liste eintragen
			ListeInhalt[Zeile][0] = "    " + feld.name;
			CString FeldIdAlsString;
			FeldIdAlsString.Format(_T("%d"), feld.id);
			int nAbziehbarOffset = 0;
			if (feld.horizontal > 700) nAbziehbarOffset = 2;
			ListeInhalt[Zeile][1 + nAbziehbarOffset] = FeldIdAlsString;	// Feld ID (KZ) in 2. Spalte eintragen
			ListeInhalt[Zeile][2 + nAbziehbarOffset] = m_pFormularCtrl->HoleFeldwertUeberID(feld.id);
			ListeInhalt[Zeile][5] = feld.elsterFeldname;
			Zeile++;
		}
	}
#ifdef TESTVERBINDUNG
	++Zeile;
	ListeInhalt[++Zeile][0] = _T("ACHTUNG: Das installierte OCX ist nur eine Testversion und baut nur eine Testverbindung auf.");
	ListeInhalt[++Zeile][0] = _T("               Es werden keine Daten als reale Vorg�nge behandelt.");
#endif

	m_pListe->SetItemCount(Zeile);
	m_pListe->RedrawItems(0, Zeile - 1);
	m_pListe->InvalidateRect(NULL, FALSE);
}