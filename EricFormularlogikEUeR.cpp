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
#include <vector> 
#include <algorithm>
#include "ECTElster.h"
#include "EricFormularlogikEUeR.h"

IMPLEMENT_DYNAMIC(CEricFormularlogikEUeR, CEricFormularlogik)

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	HILFSFUNKTION FÜR DAS LESEN DES .ECA-FORMULARS
//
LPXNode CEricFormularlogikEUeR::ZuXmlBaumHinzufuegen(LPXNode pEricXml, const CString& csElsterFeldname, const CString& csFeldwert, BOOL bRecycleBestehendenNode)
{
	// Elster-Feldnamen auseinanderpflücken und in vector packen
	int nTokenPos = 0;
	std::vector<CString> ElsterFeldnamenselemente;
	CString csElem;  // jeweiliges Element im durch '/' getrennten 'Pfad'
	while ((csElem = csElsterFeldname.Tokenize(_T("/"), nTokenPos)) != "")
		ElsterFeldnamenselemente.push_back(csElem);

	// gemäß Elster-Feldnamen in XML-Baumstruktur herabsteigen und ggf neu einsortieren (absteigend)
	LPXNode node = pEricXml;
	for (int i = 0; i < ElsterFeldnamenselemente.size(); i++)
	{
		LPXNode foundNode = NULL;
		if (!bRecycleBestehendenNode && i == ElsterFeldnamenselemente.size() - 1)	// Sonderfall: forciertes anlegen gleichnamiger Nodes für XML-Array gefordert?
			node = node->AppendChild(ElsterFeldnamenselemente[i]);  
		else
		{
			for (auto const& child : node->GetChilds())
				if (child->name == ElsterFeldnamenselemente[i])		// node->Find(ElsterFeldnamenselemente[i]) würde rekursiv durchsuchen :/
				{
					foundNode = child;
					break;
				}
			if (foundNode)											// existiert Pfadebene bereits im Baum?
				node = foundNode;									// dann einfach herabsteigen
			else
				node = node->AppendChild(ElsterFeldnamenselemente[i]);  // sonst in neu angehängte Ebene herabsteigen
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
    XNode xmlDatenTeil;  // xml-Ausgabe für ERiC (root node hier <DatenTeil>)
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

    // <Allg>-Feldwerte in Nutzdaten ergänzen, wo Datenbezug nicht über das .eca möglich
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

	// Anlage AVEÜR generieren
	CString dummyListeInhalt[500][6];
	int dummyZeile;
	AveuerGenerieren(dummyListeInhalt, dummyZeile, pE77, FLAG_GEN_XML);

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
	ZuXmlBaumHinzufuegen(pVorsatz, "Copyright", _T("Copyleft 2023 Thomas Mielke Softwareentwicklung"));
	ZuXmlBaumHinzufuegen(pVorsatz, "OrdNrArt", "S");
	ZuXmlBaumHinzufuegen(pVorsatz, "Rueckuebermittlung/Bescheid", "2");  // TODO: Rueckuebermittlung/Bescheid Checkbox

    return xmlDatenTeil.GetXML();  
}

CString CEricFormularlogikEUeR::GetLoginfo()
{
	return _T(" / EÜR-Gewinn: ") + m_GewinnMerken + _T(" / Jahr: ") + m_Jahr;
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
			if (flagsGen & FLAG_GEN_INTERN) feldarray.reserve(felder->GetChildCount());
            for (const auto& feld : felder->GetChilds())
            {
                int nID = 0;
                CString csFeldname;
				CString csTyp;
				CString csElsterFeldname;
                CString csFeldwert;
                nID = atoi(feld->GetAttrValue("id"));
                csFeldwert = m_pFormularCtrl->HoleFeldwertUeberID(nID);
                if (csFeldwert.IsEmpty()) continue;  // keine XML-Elemente für leere Feldwerte bauen
                csFeldname = feld->GetChildValue(_T("name"));
                csTyp = feld->GetAttrValue("typ");
                csElsterFeldname = feld->GetAttrValue("elsterfeldname");
				if (flagsGen & FLAG_GEN_XML)
					ZuXmlBaumHinzufuegen(pXmlOut, csElsterFeldname, csFeldwert);
				if (flagsGen & FLAG_GEN_INTERN)
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
void CEricFormularlogikEUeR::UpdateListe(CString& csFormularDateipfad, CString& csBetrieb, CString(&ListeInhalt)[500][6], CQuickList* pListe, BOOL bNurSpaltenbreitenAnpassen)
{
	m_pListe = pListe;
	m_FormularDateipfad = csFormularDateipfad;

	// ggf. alte Liste löschen
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
	Spaltenbreite[0] = ListeBreite - Spaltenbreite[1] - Spaltenbreite[2] - Spaltenbreite[3] - Spaltenbreite[4];	// Beschreibungstext bekommt war übrig ist...
	if (Spaltenbreite[0] < Spaltenbreite[1] + Spaltenbreite[2])	// ... es sei denn, es ist zu wenig
		Spaltenbreite[0] = Spaltenbreite[1] + Spaltenbreite[2];
#pragma warning(pop)

	// Listenheader aufbauen
	m_pListe->SetExtendedStyle(m_pListe->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	for (Spaltenzaehler = 0; Spaltenzaehler < AnzahlSpalten; Spaltenzaehler++)
		m_pListe->InsertColumn(Spaltenzaehler, Spaltentitel[Spaltenzaehler], LVCFMT_LEFT, Spaltenbreite[Spaltenzaehler], -1);

	if (bNurSpaltenbreitenAnpassen) return;	// --> hier schon raus, wenn sich der Inhalt nicht geändert hat

	// Formular-Dateipfad holen
	if (!m_FormularDateipfad.GetLength())
	{
		AfxMessageBox(_T("Ich kann den Dateipfad des Formulars nicht ermitteln. Bitte überprüfen, ob die entsprechenden UStVA-Formulare für das aktuelle Jahr installiert sind. Die sind üblicherweise in der aktuellen Version des EC&T-Hauptprogramms enthalten oder sind zum Entpacken ins Programmverzeichnis in Form eines Zip-Archivs auf www.easyct.de unter Downloads -> Formular-Archiv verfügbar. Die Formulare für das neue Jahr werden üblicher Weise im Laufe des Januars über ein Hauptprogramm-Update verfügbar."));
		return;
	}

	// Formular vorbereiten
	TRY
	{
		m_pFormularCtrl->WaehleFormularUndBetrieb(m_FormularDateipfad, csBetrieb);
		m_csBetrieb = csBetrieb;
	}
	CATCH(COleException, e)
	{
		CString csMsg, csOrigMsg;
		e->GetErrorMessage(csOrigMsg.GetBuffer(200), 200);
		csMsg.Format("Achtung: Damit für jeden Betrieb eine eigene EÜR übertragen werden kann, wird EasyCash&Tax v2.48 oder höher benötigt. Die angezeigten Werte sind für alle Betriebe zusammengenommen, was wahrscheinlich nicht gewollt ist. Bitte mit dem Plugin-Manager updaten. Ursprüngliche Fehlermeldung: %s", (LPCTSTR)csOrigMsg);
		AfxMessageBox(csMsg);
		m_pFormularCtrl->WaehleFormular(m_FormularDateipfad);
	}
	END_CATCH

	// Formularlayout aus .eca-Datei einlesen
	std::vector<Formularfeld> Formularfelder(0);
	std::vector<Formularabschnitt> Abschnitte(0);
	WerteAusEcaFormularGenerieren(NULL, Formularfelder, Abschnitte, FLAG_GEN_INTERN | FLAG_GEN_ABSCHNITTE);
	sort(Formularfelder.begin(), Formularfelder.end(), CEricFormularlogikEUeR::CompareInterval);

	// Formularlayout in Liste übertragen
	int Zeile, Spalte;
	CString FeldWert83Merken;
	BOOL bSternchenHinweisAnzeigen = FALSE;
	for (Zeile = 0; Zeile < sizeof(ListeInhalt) / sizeof(ListeInhalt[0]); Zeile++)	// Listenspeicher initialisieren
		for (Spalte = 0; Spalte < sizeof(ListeInhalt[0]) / sizeof(ListeInhalt[0][0]); Spalte++)
			ListeInhalt[Zeile][Spalte] = _T("");
	Zeile = 0;


	Zeile++;
	ListeInhalt[Zeile++][0] = "ACHTUNG: Dies ist eine frühe Preview-Version der EÜR-Übertragung mittels Elster-Schnittstelle, die noch Fehler enthalten kann.";
	ListeInhalt[Zeile++][0] = "      Es empfiehlt sich die Werte auf dem Versandbestätigungs-PDF, das nach dem Senden erstellt wird, noch einmal genau zu prüfen.";
	ListeInhalt[Zeile++][0] = "      Sollten Probleme auftauchen, bitte eine der Kontaktoptionen nutzen (Infoknopf unten rechts).";
	ListeInhalt[Zeile++][0] = "      Ansonsten gibt es keinen Grund, die Funktion nicht auszuprobieren: Im schlimmsten Fall muss eine korrigierte EÜR, wie gehabt, über das Elster-Onlineportal erstellt werden.";
	Zeile++;

	ListeInhalt[Zeile++][0] = "ANLAGE EÜR " + m_Jahr + (!m_csBetrieb.IsEmpty() ? (CString)" für " + m_csBetrieb : "");
	auto abschnitt = begin(Abschnitte);
	for (const auto& feld : Formularfelder)
	{
		if (!feld.elsterFeldname.IsEmpty())
		{
			// Muss noch eine Abschnittsüberschift davor?
			auto letzteVorhergehendeAbschnittsueberschrift = end(Abschnitte);
			while (abschnitt != end(Abschnitte)
				&& abschnitt->seite * 10000 + abschnitt->vertikal < feld.seite * 10000 + feld.vertikal)
			{
				letzteVorhergehendeAbschnittsueberschrift = abschnitt++;
			}
			if (letzteVorhergehendeAbschnittsueberschrift != end(Abschnitte))
			{
				Zeile++;
				ListeInhalt[Zeile++][0] = "  " + letzteVorhergehendeAbschnittsueberschrift->name;
			}

			// Feld in Liste eintragen
			ListeInhalt[Zeile][0] = "      " + feld.name;
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

	AveuerGenerieren(ListeInhalt, Zeile, NULL, FLAG_GEN_INTERN);

#ifdef TESTVERBINDUNG
	++Zeile;
	ListeInhalt[++Zeile][0] = _T("ACHTUNG: Das installierte OCX ist nur eine Testversion und baut nur eine Testverbindung auf.");
	ListeInhalt[++Zeile][0] = _T("               Es werden keine Daten als reale Vorgänge behandelt.");
#endif

	m_pListe->SetItemCount(Zeile);
	m_pListe->RedrawItems(0, Zeile - 1);
	m_pListe->InvalidateRect(NULL, FALSE);
}

#define LANGID_DEUTSCH (MAKELCID(MAKELANGID(0x07, 0x01), SORT_DEFAULT))

// je nach flagsGen Anlage AVEÜR in Liste aufführen oder eric-kompatibles XML generieren
void CEricFormularlogikEUeR::AveuerGenerieren(CString(&ListeInhalt)[500][6], int& Zeile, LPXNode xml, int flagsGen)
{
	CString ausgaben_posten_name[100];
	int ausgaben_posten_feldzuweisungen[100];  // dem EÜR-Formular zugewiesenes feld für das Ausgabenkonto
	BOOL ausgaben_posten_buchungen[100];
	BOOL ausgaben_posten_buchungen_unzugewiesen = FALSE;
	std::vector<int> kontenMitFeldzuweisungen;

	if (flagsGen & FLAG_GEN_XML && !xml) return;  // xml muss bei FLAG_GEN_XML angegeben werden

	// Ausgabenkonten bzw. -felder aufbereiten
	for (int i = 0; i < 100; i++)
	{
		CString csKey, csValue;
		csKey.Format(_T("[AusgabenRechnungsposten]%02d"), i);
		ausgaben_posten_name[i] = m_pEinstellungCtrl->HoleEinstellung(csKey);
		ausgaben_posten_buchungen[i] = FALSE;

		csKey.Format(_T("[AusgabenFeldzuweisungen]%02d"), i);
		csValue = m_pEinstellungCtrl->HoleEinstellung(csKey);
		csKey = "|E/Ü-Rechnung=";
		int nPos = csValue.Find(csKey);
		if (nPos > 2)  // erst hinter "ECT" suchen
			kontenMitFeldzuweisungen.push_back(ausgaben_posten_feldzuweisungen[i] = atoi(csValue.Mid(nPos + csKey.GetLength())));
		else
			ausgaben_posten_feldzuweisungen[i] = 0;
	}

	for (auto const& konto : kontenMitFeldzuweisungen)
	{
		;
	}

	CStringArray csaaKonten[20];  // ein paar Arrays für die AfA-Gruppen (werden referenziert)
	struct afaGruppeType
	{
		int nFeldkennzeichen;				// EC&T-FeldID
		CString csGruppenbezeichnung;
		CString csBezeichnungAfaSum;

		CString elsterKontextEinz;			// XML-"Pfad" für Einzelbeträge
		CString elsterBezeichnungEinz;
		CString elsterAnschaffungsdatumEinz;
		CString elsterAnschaffungskostenEinz;
		CString elsterBuchwertBeginnEinz;
		CString elsterAfaEinz;
		CString elsterAbgaengeEinz;
		CString elsterBuchwertEndeEinz;

		CString elsterKontextSum;			// XML-"Pfad" für Summenbeträge
		CString elsterAnschaffungskostenSum;
		CString elsterBuchwertBeginnSum;
		CString elsterAfaSum;
		CString elsterAbgaengeSum;
		CString elsterBuchwertEndeSum;
		
		CString elsterKontextAfaGesamtSum;	// "Obergruppe" -- z.B. Grundstueck(/GesamtSum) für Gebäude, Grund&Boden, Andere
		CString elsterAfaGesamtSum;

		CStringArray& csaKonten;		// Konten, die mit diesem Feld assoziiert sind, bilden eine AfA-Gruppe (Referenz auf csaaKonten[])
		std::vector<long> buchungen;	// zugehörige Buchungs-IDs
	} afaGruppen[] =
	{   // Grunstücke/grundstücksgl.Rechte  Bezeich.    AnschDatum  AnschKosten Bw.Beginn   AfA         Abgänge     Bw.Ende    

		// TODO: Grundstueck/Grund_u_Boden -- ist ja keine wirkliche AfA; wie man nicht-abzuschreibende Anlagegegenstäde behandeln kann muss im Hauptprogramm gelöst werden
		
		// Grundstueck/Gebaeude
		1136, "Gebäude", "Summe AfA aller Grundstücke/grundstücksgleichen Rechte (Übertrag in Zeile 29 der Anlage EÜR)",
		"Grundstueck/Gebaeude/Einz",		"E6007031", "",         "E6007032", "E6007034", "E6007038", "E6007040", "E6007062",
		"Grundstueck/Gebaeude/Sum",			                        "E6007033", "E6007035", "E6007039", "E6007041", "E6007043",
		"Grundstueck/GesamtSum",                                                            "E6007071",
		csaaKonten[0], std::vector<long>(),

		// Grundstueck/Andere
		1136, "Andere (z. B. grundstücksgleiche Rechte)", "Summe AfA aller Grundstücke/grundstücksgleichen Rechte (Übertrag in Zeile 29 der Anlage EÜR)", 
		"Grundstueck/Andere/Einz",			"E6007051", "",	       "E6007052", "E6007054", "E6007058", "E6007060", "E6007062",
		"Grundstueck/Andere/Sum",			                       "E6007053", "E6007055", "E6007059", "E6007061", "E6007063", 
		"Grundstueck/GesamtSum",                                                           "E6007071",
		csaaKonten[0], std::vector<long>(),


		// Arbeitszimmer                   Bezeich.    AnschDatum  AnschKosten Bw.Beginn   AfA         Abgänge     Bw.Ende

		// TODO: Arbeitszimmer Anteil Grund und Boden -- ist auch keine wirkliche AfA; wie man nicht-abzuschreibende Anlagegegenstäde behandeln kann muss im Hauptprogramm gelöst werden

		// Arbeitszimmer Anteil Gebäude
		1172, "Arbeitszimmer Anteil Gebäude", "Summe AfA (zu erfassen in Zeile 70 der Anlage EÜR)",
		"Arbeitszimmer/Anteil_Gebauede/Einz", "E6007131", "",      "E6007132", "E6007134", "E6007138", "E6007140", "E6007142",
		"Arbeitszimmer/Anteil_Gebauede/Sum",                       "E6007133", "E6007135", "E6007139", "E6007141", "E6007143",
		"",                                                                                "",
		csaaKonten[0], std::vector<long>(),


		// Immaterielle_WG
		1131, "Immaterielle Wirtschaftsgüter", "Summe AfA (Übertrag in Zeile 30 der Anlage EÜR)",
		"Immaterielle_WG/Einz",			    "E6007244", "",	       "E6007245", "E6007246", "E6007248", "E6007249", "E6007250",
		"Immaterielle_WG/Sum",			                           "E6007224", "E6007225", "E6007227", "E6007228", "E6007229",
		"",																			       "",
		csaaKonten[1], std::vector<long>(),


		// Bewegliche_WG					Bezeich.    AnschDatum  AnschKosten Bw.Beginn   AfA         Abgänge     Bw.Ende      

		// Bewegliche_WG/KFZ
		1130, "Kraftfahrzeuge", "Summe AfA aller beweglichen Wirtschaftsgüter (Übertrag in Zeile 31 der Anlage EÜR)",
		"Bewegliche_WG/KFZ/Einz",		    "E6007311", "E6007326", "E6007312", "E6007314", "E6007320", "E6007322", "E6007324",
		"Bewegliche_WG/KFZ/Sum",                                    "E6007313", "E6007315", "E6007321", "E6007323", "E6007325",
		"Bewegliche_WG/GesamtSum",                                                          "E6007372",
		csaaKonten[2], std::vector<long>(),

		// TODO: Bewegliche_WG/Buero
		1130, "Büroausstattung", "Summe AfA aller beweglichen Wirtschaftsgüter (Übertrag in Zeile 31 der Anlage EÜR)",
		"Bewegliche_WG/Buero/Einz",		    "E6007331", "E6007346", "E6007332", "E6007334", "E6007340", "E6007342", "E6007344",
		"Bewegliche_WG/Buero/Sum",                                  "E6007333", "E6007335", "E6007341", "E6007343", "E6007345",
		"Bewegliche_WG/GesamtSum",                                                          "E6007372",
		csaaKonten[3], std::vector<long>(),

		// Bewegliche_WG/Andere
		1130, "Andere bewegliche Wirtschaftsgüter", "Summe AfA aller beweglichen Wirtschaftsgüter (Übertrag in Zeile 31 der Anlage EÜR)",
		"Bewegliche_WG/Andere/Einz",		"E6007351", "E6007366", "E6007352", "E6007354", "E6007360", "E6007362", "E6007364", 
		"Bewegliche_WG/Andere/Sum",                                 "E6007353", "E6007355", "E6007361", "E6007363", "E6007365",
		"Bewegliche_WG/GesamtSum",                                                          "E6007372",
		csaaKonten[4], std::vector<long>(),


		// TODO: §7b
		// TODO: §7g

		// Sammelposten_VZ					Bezeich.    AnschDatum  AnschKosten Bw.Beginn   AfA         Abgänge     Bw.Ende 

		// Sammelposten aktuelles Jahr
		1137, "Sammelposten", "Gesamtsumme Auflösungsbeträge (Übertrag in Zeile 44 der Anlage EÜR)",
		"Sammelposten_VZ/Einz",		        "E6007381", "",         "E6007384", "",         "E6007385", "",         "E6007386", 
		"Sammelposten_VZ/Sum",                                      "E6007393", "",         "E6007394", "",         "E6007395",
		"GesamtSum",														                "E6017399",
		csaaKonten[5], std::vector<long>(),
		
		// Sammelposten Vorjahr
		1137, "Sammelposten", "Gesamtsumme Auflösungsbeträge (Übertrag in Zeile 44 der Anlage EÜR)",
		"",		                           "",          "",         "dummy",    "",         "dummy", "",            "dummy",
		"Sammelposten_VZm1",                                        "E6017391", "E6017392", "E6017393", "",         "E6017394",
		"GesamtSum",														                "E6017399",
		csaaKonten[6], std::vector<long>(),

		// Sammelposten vor zwei Jahren
		1137, "Sammelposten", "Gesamtsumme Auflösungsbeträge (Übertrag in Zeile 44 der Anlage EÜR)",
		"",		                           "",          "",         "dummy",    "",         "dummy", "",            "dummy",
		"Sammelposten_VZm2",                                        "E6017395", "E6017396", "E6017397", "",         "E6017398",
		"GesamtSum",														                "E6017399",
		csaaKonten[7], std::vector<long>(),

		// Sammelposten vor drei Jahren
		1137, "Sammelposten", "Gesamtsumme Auflösungsbeträge (Übertrag in Zeile 44 der Anlage EÜR)",
		"",		                           "",          "",         "dummy",    "",         "dummy", "",            "dummy",
		"Sammelposten_VZm3",                                        "E6017400", "E6017401", "E6017402", "",         "E6017403",
		"GesamtSum",														                "E6017399",
		csaaKonten[8], std::vector<long>(),

		// Sammelposten vor vier Jahren
		1137, "Sammelposten", "Gesamtsumme Auflösungsbeträge (Übertrag in Zeile 44 der Anlage EÜR)",
		"",		                           "",          "",         "dummy",    "",         "dummy", "",            "dummy",
		"Sammelposten_VZm4",                                        "E6017404", "E6017405", "E6017406", "",         "",
		"GesamtSum",														                "E6017399",
		csaaKonten[9], std::vector<long>(),


		0, /* Ende-Marker */ "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", csaaKonten[5], std::vector<long>()
	};

	// alle Ausgabenbuchungen (id) durchgehen und bei AfAs in eine passende AfA-Gruppe eingliedern
	long id;
	for (id = m_pDokumentCtrl->FindeErsteAusgabenBuchung(); id != 0; id = m_pDokumentCtrl->FindeNaechsteBuchung(id))
	{
		m_pBuchungCtrl->SetID(id);

		CString csUrspruenglichesAnschaffungsdatum = m_pBuchungCtrl->HoleBenutzerdefWert("EasyCash", "UrspruenglichesAnschaffungsdatum");
		CString csUrspruenglichesKonto = m_pBuchungCtrl->HoleBenutzerdefWert("EasyCash", "UrspruenglichesKonto");
		if ((m_pBuchungCtrl->GetAbschreibungJahre() > 1 || !csUrspruenglichesAnschaffungsdatum.IsEmpty())				 // AfA oder Abgang?
			&& (m_csBetrieb.IsEmpty() || m_pBuchungCtrl->GetBetrieb() == m_csBetrieb))
		{
			for (int i = 0; i < 100 && !ausgaben_posten_name[i].IsEmpty(); i++)											 // Ausgaben-Konten durchgehen
				if ((m_pBuchungCtrl->GetAbschreibungJahre() > 1 && ausgaben_posten_name[i] == m_pBuchungCtrl->GetKonto())
					|| (m_pBuchungCtrl->GetAbschreibungJahre() <= 1 && ausgaben_posten_name[i] == csUrspruenglichesKonto))	 // passt es zu der Buchung?
				{
					int g; // AfA-GruppenIterator
					int nLetzteGruppeMitPassendemFeldkennzeichen = -1;
					for (g = 0; afaGruppen[g].nFeldkennzeichen; g++)														  // haben wir eine AfA-Gruppe,
					{																										  // bei dem das Konto der Buchung
						if (ausgaben_posten_feldzuweisungen[i] == afaGruppen[g].nFeldkennzeichen)							  // mit einem AfA-Feld verknüpft ist
						{
							nLetzteGruppeMitPassendemFeldkennzeichen = g;  // möglichen fallback merken
							if (afaGruppen[g].csGruppenbezeichnung == "Sammelposten")
							{
								if (m_pBuchungCtrl->GetAbschreibungNr() == 1 &&											  // und, wenn es sich nicht um den
									afaGruppen[g].elsterKontextSum == "Sammelposten_VZ/Sum")								  // Sonderfall der Sammelposten handelt,
								{
									break;	// Sammelposten des aktuellen Jahres
								}
								CString csVorjahressammelposten;
								csVorjahressammelposten.Format("Sammelposten_VZm%d", m_pBuchungCtrl->GetAbschreibungNr() - 1);
								if (m_pBuchungCtrl->GetAbschreibungNr() <= 5 &&
									afaGruppen[g].elsterKontextSum == csVorjahressammelposten)
								{
									break;	// Sammelposten des 2. bis 5. Jahres -> "Sammelposten_VZm1" bis "Sammelposten_VZm4"
								}
								continue;  // ansonsten einfach weiter nach dem richtigen Sammelposten-Kontext suchen
							}
							else if (ausgaben_posten_name[i].GetLength() >= afaGruppen[g].csGruppenbezeichnung.GetLength() && // wo das rechte Ende Kontonames  
								ausgaben_posten_name[i].Right(afaGruppen[g].csGruppenbezeichnung.GetLength()).MakeLower()	  // der Gruppenbezeichnung entspricht?
								== afaGruppen[g].csGruppenbezeichnung.MakeLower())
							{
								break;
							}
						}
					}
					if (!afaGruppen[g].nFeldkennzeichen)																 	 // nicht? schade. dann letzte Gruppe
						g = nLetzteGruppeMitPassendemFeldkennzeichen;													 	 // mit passendem Feldkennzeichen benutzen

					if (g >= 0)	
					{
						BOOL bKontoSchonEnthalten = FALSE;
						for (int konto = 0; konto < afaGruppen[g].csaKonten.GetCount(); konto++)
						{
							if (afaGruppen[g].csaKonten[konto] == m_pBuchungCtrl->GetKonto())
							{
								bKontoSchonEnthalten = TRUE;
								break;
							}
						}
						if (!bKontoSchonEnthalten)										// Konto in die AfA-Gruppe aufnehmen, 
							afaGruppen[g].csaKonten.Add(m_pBuchungCtrl->GetKonto());	// wenn noch nicht geschehen
						afaGruppen[g].buchungen.push_back(id);		// zu Buchungen in der passenden Gruppe hinzufügen
					}
					else if (flagsGen & FLAG_GEN_INTERN)
					{
						Zeile++;
						ListeInhalt[Zeile++][0] = (CString)"Hinweis: " + (m_pBuchungCtrl->GetAbschreibungJahre() > 1 ? "Die AfA '" : "Der AfA-Abgang '")
							+ m_pBuchungCtrl->GetBeschreibung() + "' ist auf das Konto '" + m_pBuchungCtrl->GetKonto()
							+ "' gebucht, das keinem der offiziellen AfA-Feldern der EÜR zugewiesen ist. "
							+ "Bitte das unter Einstellungen->E/Ü-Konten ändern.";
							// TODO: Liste möglicher Kontennahmen-FeldID-Kombinationen anhängen
					}

					break;  // sehr schön: AfA- bzw. Abgangs-Buchung ist untergebracht; Ausgaben-Konten-Loop verlassen und nächste Buchung untersuchen
				}
		}
	}

	LPXNode xmlAveur = NULL;
	BOOL bUeberschriftSchonGedruckt = FALSE;
	COleCurrency cyAfaGesamtSum;

	// für jede AfA-Gruppe ...
	for (int g = 0; afaGruppen[g].nFeldkennzeichen; g++)
	{
		COleCurrency cyAnschaffungskostenSum;
		COleCurrency cyBuchwertBeginnSum;
		COleCurrency cyAfaSum;
		COleCurrency cyAbgaengeSum;
		COleCurrency cyBuchwertEndeSum;

		if (afaGruppen[g].buchungen.size() != 0)
		{
			// ggf. Überschrift "ANLAGE AVEÜR" in Liste
			if (!bUeberschriftSchonGedruckt)
			{
				if (flagsGen & FLAG_GEN_INTERN)
				{
					Zeile++;
					ListeInhalt[Zeile++][0] = "ANLAGE AVEÜR " + m_Jahr + (!m_csBetrieb.IsEmpty() ? (CString)" für " + m_csBetrieb : "");
				}
				if (flagsGen & FLAG_GEN_XML)
					xmlAveur = ZuXmlBaumHinzufuegen(xml, "AVEUER", "");

				bUeberschriftSchonGedruckt = TRUE;
			}

			// Überschrift für AfA-Gruppe
			if (flagsGen & FLAG_GEN_INTERN)
			{
				Zeile++;
				if (afaGruppen[g].csGruppenbezeichnung == "Sammelposten")
					ListeInhalt[Zeile++][0].Format("  %s %d", afaGruppen[g].csGruppenbezeichnung, m_pDokumentCtrl->GetJahr() - atoi(afaGruppen[g].elsterKontextSum.Right(1)));
				else
					ListeInhalt[Zeile++][0] = "  " + afaGruppen[g].csGruppenbezeichnung;

			}

			// ... Buchungen der Gruppe durchgehen
			for (auto const& id : afaGruppen[g].buchungen)
			{
				m_pBuchungCtrl->SetID(id);

				CString csUrspruenglichesAnschaffungsdatum = m_pBuchungCtrl->HoleBenutzerdefWert("EasyCash", "UrspruenglichesAnschaffungsdatum");

				BOOL bIstAbgang = FALSE;

				// Datum
				if (csUrspruenglichesAnschaffungsdatum != "")
				{
					bIstAbgang = TRUE;
				}
				else
				{
					auto Datum = COleDateTime(m_pBuchungCtrl->GetDatum());
					sprintf(csUrspruenglichesAnschaffungsdatum.GetBuffer(100), "%02d.%02d.%04d", Datum.GetDay(), Datum.GetMonth(), Datum.GetYear() - m_pBuchungCtrl->GetAbschreibungNr() + 1);
				}

				// --- Einz-Kontext ---

				LPXNode xmlKontextEinz;
				if (flagsGen & FLAG_GEN_XML)
					xmlKontextEinz = ZuXmlBaumHinzufuegen(xmlAveur, afaGruppen[g].elsterKontextEinz, "", FALSE);  // FALSE = es werden auf der untersten Ebene mehrere Nodes des gleichen Namens angelegt

				// E6007351 - Bezeichnung
				if (!afaGruppen[g].elsterBezeichnungEinz.IsEmpty())
				{
					if (flagsGen & FLAG_GEN_INTERN)
					{
						Zeile++;
						ListeInhalt[Zeile][0] = "    " + m_pBuchungCtrl->GetBeschreibung();
						ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterBezeichnungEinz;
					}
					if (flagsGen & FLAG_GEN_XML)
					{
						ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterBezeichnungEinz, m_pBuchungCtrl->GetBeschreibung());
					}
				}

				// Anschaffungs-/Herstellungsdatum E6007366
				if (!afaGruppen[g].elsterAnschaffungsdatumEinz.IsEmpty())
				{
					if (flagsGen & FLAG_GEN_INTERN)
					{
						ListeInhalt[Zeile][0] = "      Anschaffungs-/Herstellungsdatum";
						ListeInhalt[Zeile][4] = (LPCTSTR)csUrspruenglichesAnschaffungsdatum;
						ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterAnschaffungsdatumEinz;
					}
					if (flagsGen & FLAG_GEN_XML)
						ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterAnschaffungsdatumEinz, (LPCTSTR)csUrspruenglichesAnschaffungsdatum);
				}

				if (bIstAbgang)
				{
					if (afaGruppen[g].csGruppenbezeichnung == "Sammelposten" && flagsGen & FLAG_GEN_INTERN)
						ListeInhalt[Zeile++][0] = "Hinweis: Abgänge von Sammelposten wie '" + m_pBuchungCtrl->GetBeschreibung() + "' sind nicht vorgesehen.";

					// Anschaffungskosten E6007352
					CString csUrspruenglicherNettobetrag = m_pBuchungCtrl->HoleBenutzerdefWert("EasyCash", "UrspruenglicherNettobetrag").TrimLeft();
					COleCurrency cyUrspruenglicherNettobetrag;
					cyAnschaffungskostenSum += cyUrspruenglicherNettobetrag;
					if (cyUrspruenglicherNettobetrag.ParseCurrency(csUrspruenglicherNettobetrag, 0, LANGID_DEUTSCH))
						cyAnschaffungskostenSum += cyUrspruenglicherNettobetrag;
					else
						ListeInhalt[Zeile++][0] = "Fehler: Konvertieren des gemerkten urspruenglichen Nettobetrags des AfA-Abgangs schlug fehl. Irgendwas stimmt hier nicht. Bitte den Fehler melden.";
					if (!afaGruppen[g].elsterKontextEinz.IsEmpty())  // nur darstellen/verarbeiten, wenn es einen Elster-Kontext gibt
					{
						if (flagsGen & FLAG_GEN_INTERN)
						{
							ListeInhalt[Zeile][0] = "      Anschaffungs-/Herstellungskosten/Einlagewert";
							ListeInhalt[Zeile][4] = csUrspruenglicherNettobetrag;
							ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterAnschaffungskostenEinz;
						}
						if (flagsGen & FLAG_GEN_XML)
							ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterAnschaffungskostenEinz, csUrspruenglicherNettobetrag);
					}

					// Buchwert zu Beginn des Gewinnermittlungszeitraums E6007354
					CString csUrspruenglicherRestwert = m_pBuchungCtrl->HoleBenutzerdefWert("EasyCash", "UrspruenglicherRestwert").TrimLeft();
					COleCurrency cyUrspruenglicherRestwert;
					if (cyUrspruenglicherRestwert.ParseCurrency(csUrspruenglicherRestwert, 0, LANGID_DEUTSCH))
						cyBuchwertBeginnSum += cyUrspruenglicherRestwert;
					else
						ListeInhalt[Zeile++][0] = "Fehler: Konvertieren des gemerkten urspruenglichen Restwert des AfA-Abgangs schlug fehl. Irgendwas stimmt hier nicht. Bitte den Fehler melden.";
					if (!afaGruppen[g].elsterKontextEinz.IsEmpty())  // nur darstellen/verarbeiten, wenn es einen Elster-Kontext gibt
					{
						if (flagsGen & FLAG_GEN_INTERN)
						{
							ListeInhalt[Zeile][0] = "      Buchwert zu Beginn des Gewinnermittlungszeitraums";
							ListeInhalt[Zeile][4] = csUrspruenglicherRestwert;
							ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterBuchwertBeginnEinz;
						}
						if (flagsGen & FLAG_GEN_XML)
							ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterBuchwertBeginnEinz, csUrspruenglicherRestwert);
					}

					// E6007356 - Zugänge
					// TODO: implementieren in EC&T-Hauptprogramm

					// E6007358 - Sonderabschreibung nach § 7g Absatz 5 und 6 EStG
					// TODO: implementieren in EC&T-Hauptprogramm

					// Abgänge E6007362
					COleCurrency cyAbgaenge = (CURRENCY)m_pBuchungCtrl->HoleNetto();
					cyAbgaengeSum += cyAbgaenge;
					if (!afaGruppen[g].elsterKontextEinz.IsEmpty())  // nur darstellen/verarbeiten, wenn es einen Elster-Kontext gibt
					{
						CString csAbgaenge = FormatCy2d(cyAbgaenge, 0UL, LANGID_DEUTSCH);
						if (flagsGen & FLAG_GEN_INTERN)
						{
							ListeInhalt[Zeile][0] = "      Abgänge";
							ListeInhalt[Zeile][4] = csAbgaenge;
							ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterAbgaengeEinz;
						}
						if (flagsGen & FLAG_GEN_XML)
							ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterAbgaengeEinz, csAbgaenge);
					}

					// Buchwert am Ende des Gewinnermittlungszeitraums E6007364
					if (!afaGruppen[g].elsterKontextEinz.IsEmpty())  // nur darstellen/verarbeiten, wenn es einen Elster-Kontext gibt
					{
						if (flagsGen & FLAG_GEN_INTERN)
						{
							ListeInhalt[Zeile][0] = "      Buchwert am Ende des Gewinnermittlungszeitraums";
							ListeInhalt[Zeile][4] = "0,00";  // EC&T kann im Moment nur Totalabgänge verarbeiten (Ausscheiden des Anlagenguts)
							ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterBuchwertEndeEinz;
						}
						if (flagsGen & FLAG_GEN_XML)
							ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterBuchwertEndeEinz, "0,00");
					}
				}
				else
				{
					// Anschaffungskosten E6007352
					COleCurrency csAnschaffungskosten = m_pBuchungCtrl->HoleNetto();
					cyAnschaffungskostenSum += csAnschaffungskosten;
					if (!afaGruppen[g].elsterKontextEinz.IsEmpty())  // nur darstellen/verarbeiten, wenn es einen Elster-Kontext gibt
					{
						CString csUrspruenglicherRestwert = FormatCy2d(csAnschaffungskosten, 0UL, LANGID_DEUTSCH);
						if (flagsGen & FLAG_GEN_INTERN)
						{
							ListeInhalt[Zeile][0] = "      Anschaffungs-/Herstellungskosten/Einlagewert";
							ListeInhalt[Zeile][4] = csUrspruenglicherRestwert;
							ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterAnschaffungskostenEinz;
						}
						if (flagsGen & FLAG_GEN_XML)
							ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterAnschaffungskostenEinz, csUrspruenglicherRestwert);
					}

					// Buchwert zu Beginn des Gewinnermittlungszeitraums E6007354
					COleCurrency cyAbschreibungRestwert = m_pBuchungCtrl->GetAbschreibungRestwert();
					cyBuchwertBeginnSum += cyAbschreibungRestwert;
					if (!afaGruppen[g].elsterKontextEinz.IsEmpty())  // nur darstellen/verarbeiten, wenn es einen Elster-Kontext gibt
					{
						CString csAbschreibungRestwert = FormatCy2d(cyAbschreibungRestwert, 0UL, LANGID_DEUTSCH);
						if (flagsGen & FLAG_GEN_INTERN)
						{
							ListeInhalt[Zeile][0] = "      Buchwert zu Beginn des Gewinnermittlungszeitraums";
							ListeInhalt[Zeile][4] = csAbschreibungRestwert;
							ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterBuchwertBeginnEinz;
						}
						if (flagsGen & FLAG_GEN_XML)
							ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterBuchwertBeginnEinz, csAbschreibungRestwert);
					}

					// E6007356 - Zugänge
					// TODO: implementieren in EC&T-Hauptprogramm

					// E6007358 - Sonderabschreibung nach § 7g Absatz 5 und 6 EStG
					// TODO: implementieren in EC&T-Hauptprogramm

					// AfAs E6007360
					COleCurrency cyAfA = m_pBuchungCtrl->HoleBuchungsjahrNetto(m_pDokumentCtrl->GetID());
					cyAfaSum += cyAfA;
					CString csAfA = FormatCy2d(cyAfA, 0UL, LANGID_DEUTSCH);
					if (!afaGruppen[g].elsterKontextEinz.IsEmpty())  // nur darstellen/verarbeiten, wenn es einen Elster-Kontext gibt
					{
						if (flagsGen & FLAG_GEN_INTERN)
						{
							ListeInhalt[Zeile][0] = "      AfA";
							ListeInhalt[Zeile][4] = csAfA;
							ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterAfaEinz;
						}
						if (flagsGen & FLAG_GEN_XML)
							ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterAfaEinz, csAfA);
					}

					// Buchwert am Ende des Gewinnermittlungszeitraums E6007364
					COleCurrency cyBuchwertAmEndeDesGewinnermittlungszeitraums = (CURRENCY)(cyAbschreibungRestwert - cyAfA);
					cyBuchwertEndeSum += cyBuchwertAmEndeDesGewinnermittlungszeitraums;
					if (!afaGruppen[g].elsterKontextEinz.IsEmpty())  // nur darstellen/verarbeiten, wenn es einen Elster-Kontext gibt
					{
						CString csBuchwertAmEndeDesGewinnermittlungszeitraums = FormatCy2d(cyBuchwertAmEndeDesGewinnermittlungszeitraums, 0UL, LANGID_DEUTSCH);
						if (flagsGen & FLAG_GEN_INTERN)
						{
							ListeInhalt[Zeile][0] = "      Buchwert am Ende des Gewinnermittlungszeitraums";
							ListeInhalt[Zeile][4] = csBuchwertAmEndeDesGewinnermittlungszeitraums;
							ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextEinz + "/" + afaGruppen[g].elsterBuchwertEndeEinz;
						}
						if (flagsGen & FLAG_GEN_XML)
							ZuXmlBaumHinzufuegen(xmlKontextEinz, afaGruppen[g].elsterBuchwertEndeEinz, csBuchwertAmEndeDesGewinnermittlungszeitraums);
					}

					// Sanity-Checks für Sammelposten
					if (afaGruppen[g].csGruppenbezeichnung == "Sammelposten" && flagsGen & FLAG_GEN_INTERN)
					{
						ListeInhalt[Zeile++][0].Format("      %s im %dten Jahr mit einer Auflösungsrate von %s €", (LPCTSTR)m_pBuchungCtrl->GetBeschreibung(), m_pBuchungCtrl->GetAbschreibungNr(), (LPCTSTR)csAfA);
						if (cyAnschaffungskostenSum < COleCurrency(250, 0) || cyAnschaffungskostenSum >= COleCurrency(1000, 0))
							ListeInhalt[Zeile++][0] = "Hinweis: Pool-AfA-Sammelposten wie '" + m_pBuchungCtrl->GetBeschreibung() + "' dürfen nur einen Anschaffungswert von min. 250 € bis max. 999,99 € haben.";
						if (m_pBuchungCtrl->GetAbschreibungGenauigkeit() != 0)
							ListeInhalt[Zeile++][0] = "Hinweis: Pool-AfA-Sammelposten wie '" + m_pBuchungCtrl->GetBeschreibung() + "' müssen mit ganzjährlicher Abschreibungsgenauigkeit gebucht werden.";
						if (m_pBuchungCtrl->GetAbschreibungJahre() != 5)
							ListeInhalt[Zeile++][0] = "Hinweis: Pool-AfA-Sammelposten wie '" + m_pBuchungCtrl->GetBeschreibung() + "' müssen über genau fünf Jahre abgeschrieben werden.";
					}
				}
			}
		}

		if (cyAnschaffungskostenSum != COleCurrency(0, 0) ||
			cyBuchwertBeginnSum != COleCurrency(0, 0) ||
			cyAfaSum != COleCurrency(0, 0) ||
			cyAbgaengeSum != COleCurrency(0, 0) ||
			cyBuchwertEndeSum != COleCurrency(0, 0) ||
			cyBuchwertBeginnSum != COleCurrency(0, 0) ||
			cyBuchwertBeginnSum != COleCurrency(0, 0) ||
			cyBuchwertBeginnSum != COleCurrency(0, 0))
		{
			if (flagsGen & FLAG_GEN_INTERN)
				Zeile++;

				LPXNode xmlKontextSum = NULL;
			if (flagsGen & FLAG_GEN_XML)
				xmlKontextSum = ZuXmlBaumHinzufuegen(xmlAveur, afaGruppen[g].elsterKontextSum, "");

			// Buchwert zu Beginn des Gewinnermittlungszeitraums Summe
			CString csAnschaffungskostenSum = FormatCy2d(cyAnschaffungskostenSum, 0UL, LANGID_DEUTSCH);
			if (flagsGen & FLAG_GEN_INTERN)
			{
				ListeInhalt[Zeile][0] = "    Summe Anschaffungs-/Herstellungskosten/Einlagewert";
				ListeInhalt[Zeile][4] = csAnschaffungskostenSum;
				ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextSum + "/" + afaGruppen[g].elsterAnschaffungskostenSum;
			}
			if (flagsGen & FLAG_GEN_XML)
				ZuXmlBaumHinzufuegen(xmlKontextSum, afaGruppen[g].elsterAnschaffungskostenSum, csAnschaffungskostenSum);

			// Buchwert zu Beginn des Gewinnermittlungszeitraums Summe
			if (cyBuchwertBeginnSum != COleCurrency(0, 0))
			{
				CString csBuchwertBeginn = FormatCy2d(cyBuchwertBeginnSum, 0UL, LANGID_DEUTSCH);
				if (flagsGen & FLAG_GEN_INTERN)
				{
					ListeInhalt[Zeile][0] = "    Summe der Buchwerte zu Beginn des Gewinnermittlungszeitraums";
					ListeInhalt[Zeile][4] = csBuchwertBeginn;
					ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextSum + "/" + afaGruppen[g].elsterBuchwertBeginnSum;
				}
				if (flagsGen & FLAG_GEN_XML)
					ZuXmlBaumHinzufuegen(xmlKontextSum, afaGruppen[g].elsterBuchwertBeginnSum, csBuchwertBeginn);
			}

			// E6007356 - Zugänge
			// TODO: implementieren in EC&T-Hauptprogramm

			// E6007358 - Sonderabschreibung nach § 7g Absatz 5 und 6 EStG
			// TODO: implementieren in EC&T-Hauptprogramm

			// AfAs Summe
			if (cyAfaSum != COleCurrency(0, 0))
			{
				cyAfaGesamtSum += cyAfaSum;  // AfA-Obergruppen-Gesamtsumme mitführen
				CString csAfA = FormatCy2d(cyAfaSum, 0UL, LANGID_DEUTSCH);
				if (flagsGen & FLAG_GEN_INTERN)
				{
					ListeInhalt[Zeile][0] = "    Summe der AfAs";
					ListeInhalt[Zeile][4] = csAfA;
					ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextSum + "/" + afaGruppen[g].elsterAfaSum;
				}
				if (flagsGen & FLAG_GEN_XML)
					ZuXmlBaumHinzufuegen(xmlKontextSum, afaGruppen[g].elsterAfaSum, csAfA);
			}

			// Abgänge Summe
			if (cyAbgaengeSum != COleCurrency(0, 0))
			{
				CString csAbgaenge = FormatCy2d(cyAbgaengeSum, 0UL, LANGID_DEUTSCH);
				if (flagsGen & FLAG_GEN_INTERN)
				{
					ListeInhalt[Zeile][0] = "    Summe der Abgänge";
					ListeInhalt[Zeile][4] = csAbgaenge;
					ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextSum + "/" + afaGruppen[g].elsterAbgaengeSum;
				}
				if (flagsGen & FLAG_GEN_XML)
					ZuXmlBaumHinzufuegen(xmlKontextSum, afaGruppen[g].elsterAbgaengeSum, csAbgaenge);
			}

			// Buchwert am Ende des Gewinnermittlungszeitraums Summe
			CString csBuchwertEnde = FormatCy2d(cyBuchwertEndeSum, 0UL, LANGID_DEUTSCH);
			if (flagsGen & FLAG_GEN_INTERN)
			{
				ListeInhalt[Zeile][0] = "    Summe der Buchwerte am Ende des Gewinnermittlungszeitraums";
				ListeInhalt[Zeile][4] = csBuchwertEnde;
				ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextSum + "/" + afaGruppen[g].elsterBuchwertEndeSum;
			}
			if (flagsGen & FLAG_GEN_XML)
				ZuXmlBaumHinzufuegen(xmlKontextSum, afaGruppen[g].elsterBuchwertEndeSum, csBuchwertEnde);
		}

		// AfAs Gesamtsumme über mehrere Untergruppen (Obergruppenwechsel)
		if (afaGruppen[g + 1].elsterKontextAfaGesamtSum != afaGruppen[g].elsterKontextAfaGesamtSum)
		{
			if (cyAfaGesamtSum != COleCurrency(0, 0) && !afaGruppen[g].elsterKontextAfaGesamtSum.IsEmpty())
			{
				CString csAfaGesamtSum = FormatCy2d(cyAfaGesamtSum, 0UL, LANGID_DEUTSCH);
				if (flagsGen & FLAG_GEN_INTERN)
				{
					ListeInhalt[++Zeile][0] = "  " + afaGruppen[g].csBezeichnungAfaSum;
					ListeInhalt[Zeile][4] = csAfaGesamtSum;
					ListeInhalt[Zeile++][5] = "AVEUER/" + afaGruppen[g].elsterKontextSum + "/" + afaGruppen[g].elsterAfaGesamtSum;
				}
				if (flagsGen & FLAG_GEN_XML)
				{
					/*if (afaGruppen[g].csGruppenbezeichnung == "Sammelposten")
					{
						LPXNode xmlKontextAfaGesamtSum = ZuXmlBaumHinzufuegen(xmlAveur, 
							afaGruppen[g].elsterKontextAfaGesamtSum + _T("/") + afaGruppen[g].elsterAfaGesamtSum, csAfaGesamtSum);
					}
					else*/
					{
						LPXNode xmlKontextAfaGesamtSum = ZuXmlBaumHinzufuegen(xmlAveur, afaGruppen[g].elsterKontextAfaGesamtSum, "");
						ZuXmlBaumHinzufuegen(xmlKontextAfaGesamtSum, afaGruppen[g].elsterAfaGesamtSum, csAfaGesamtSum);
					}
				}
			}

			cyAfaGesamtSum.SetCurrency(0, 0);
		}
	}
}
