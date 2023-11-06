// EricFormularlogikUStVA.cpp : implementation file
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
#include "EricFormularlogikUStVA.h"

IMPLEMENT_DYNAMIC(CEricFormularlogikUStVA, CEricFormularlogik)

CString CEricFormularlogikUStVA::GetDatenteil()
{
	CString DatenTeil =
		_T("<DatenTeil> \
<Nutzdatenblock> \
<NutzdatenHeader version=\"11\"> \
<NutzdatenTicket>") + m_Jetzt.Format(_T("%Y%m%d%H%M%S")) + _T("</NutzdatenTicket> \
<Empfaenger id=\"F\">") + XMLEscape(m_EmpfaengerFinanzamt) + _T("</Empfaenger> \
<Hersteller> \
<ProduktName>EasyCash&amp;Tax Elster Plugin</ProduktName> \
<ProduktVersion>") + XMLEscape(m_AppVersion) + _T("</ProduktVersion> \
</Hersteller> \
<DatenLieferant>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("vorname")) + _T(" ") + m_pEinstellungCtrl->HoleEinstellung(_T("name")) + _T("; ")
	+ m_EmailAdresse) + "</DatenLieferant> \
</NutzdatenHeader> \
<Nutzdaten> \
<Anmeldungssteuern " + (atoi(m_Jahr) < 2021 ? _T("art = \"UStVA\"") : _T("xmlns=\"http://finkonsens.de/elster/elsteranmeldung/ustva/v") + m_Jahr + _T("\"")) + _T(" version=\"") + m_Jahr + (atoi(m_Jahr) < 2021 ? _T("01") : _T("")) + _T("\"> \
<Erstellungsdatum>") + m_Jetzt.Format(_T("%Y%m%d")) + _T("</Erstellungsdatum> \
<DatenLieferant> \
<Name>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("vorname")) + _T(" ") + m_pEinstellungCtrl->HoleEinstellung(_T("name"))) + _T("</Name> \
<Strasse>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("strasse"))) + _T("</Strasse> \
<PLZ>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("plz"))) + _T("</PLZ> \
<Ort>") + XMLEscape(m_pEinstellungCtrl->HoleEinstellung(_T("ort"))) + _T("</Ort> \
") + (CString)(m_Telefon.IsEmpty() ? _T("") : _T("<Telefon>") + XMLEscape(m_Telefon) + _T("</Telefon>")) + _T(" \
") + (CString)(m_EmailAdresse.IsEmpty() ? _T("") : _T("<Email>") + XMLEscape(m_EmailAdresse) + _T("</Email>")) + _T(" \
</DatenLieferant> \
<Steuerfall> \
<Umsatzsteuervoranmeldung> \
<Jahr>") + m_Jahr + _T("</Jahr> \
<Zeitraum>") + m_Zeitraum + _T("</Zeitraum> \
<Steuernummer>") + XMLEscape(m_Bundesfinanzamtsnummer) + _T("</Steuernummer> \
<Kz09>") + XMLEscape(m_HerstellerID) + _T("</Kz09> \
") + (CString)(m_KorrigierteAnmeldung ? _T("<Kz10>1</Kz10>") : _T("")) + _T(" \
") + (CString)(m_BelegeWerdenNachgereicht ? _T("<Kz22>1</Kz22>") : _T("")) + _T(" \
§§§Kz21Platzhalter§§§\
") + (CString)(m_EinzugsermaechtigungWiderrufen ? _T("<Kz26>1</Kz26>") : _T("")) + _T(" \
") + (CString)(m_VerrechnungDesErstattungsanspruchs ? _T("<Kz29>1</Kz29>") : _T(""));

	// weitere Kennziffern aus der Liste lesen
	int Zeile;
	CString Feldwerte[100];
	for (Zeile = 0; Zeile < m_pListe->GetItemCount(); Zeile++)
	{
		int Spalte;
		for (Spalte = 1; Spalte < 4; Spalte += 2)
		{
			CString FeldID = m_pListe->GetItemText(Zeile, Spalte);
			CString FeldWert = m_pListe->GetItemText(Zeile, Spalte + 1);
#ifdef TESTVERBINDUNG
			if (Spalte == 3 && FeldWert == _T("TEST!"))
				FeldWert = _T("");
#endif
			if (FeldID.GetLength() && FeldWert.GetLength())
			{
				if (atoi(FeldID) == 83) m_FeldWert83Merken = FeldWert;
				FeldWert.Replace(_T(','), _T('.'));
				Feldwerte[atoi(FeldID)] = FeldWert;
			}
		}
	}

	// Feldwerte in richtige Reihenfolge bringen
	// Reihenfolge muss dem Schema entsprechen: (Jahr?,Zeitraum?,Steuernummer?,Kz09?,Kz10?,Kz21?,Kz22?,Kz23?,Kz23_Begruendung?,Kz26?,Kz29?,Kz35?,Kz36?,Kz37?,Kz39?,Kz41?,Kz42?,Kz43?,Kz44?,Kz45?,Kz46?,Kz47?,Kz48?,Kz49?,Kz50?,Kz59?,Kz60?,Kz61?,Kz62?,Kz63?,Kz64?,Kz65?,Kz66?,Kz67?,Kz69?,Kz73?,Kz74?,Kz76?,Kz77?,Kz80?,Kz81?,Kz83?,Kz84?,Kz85?,Kz86?,Kz89?,Kz91?,Kz93?,Kz94?,Kz95?,Kz96?,Kz98?)''
	static int ReihenfolgeKz[] = { 21/*!*/, 35, 36, 37, 39, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 59, 60, 61 ,62 ,63 ,64, 65, 66, 67, 69, 73, 74, 76, 77, 80, 81, 83, 84, 85, 86, 89, 91, 93, 94, 95, 96, 98 };
	static int ReihenfolgeKzAnzahl = sizeof(ReihenfolgeKz) / sizeof(ReihenfolgeKz[0]);
	int position;
	for (position = 0; position < ReihenfolgeKzAnzahl; position++)
	{
		int feldID = ReihenfolgeKz[position];
		if (feldID <= 0 || feldID > 99)
		{
			AfxMessageBox("Feld-ID außerhalb des gültigen Bereichs.");
			return "";
		}

		if (!Feldwerte[feldID].IsEmpty())
		{
			CString XMLKnoten;
			XMLKnoten.Format(_T("<Kz%d>%s</Kz%d> "), feldID, (LPCTSTR)XMLEscape(Feldwerte[feldID].GetBuffer(0)), feldID);

			if (feldID == 21)
				DatenTeil.Replace("§§§Kz21Platzhalter§§§", XMLKnoten);  // Extrawurst für Feld 21...
			else
				DatenTeil += XMLKnoten;
		}
	}
	DatenTeil.Replace("§§§Kz21Platzhalter§§§", "");  // Kz21-Platzhalter wurde nicht gebraucht? Dann löschen.

	DatenTeil += _T("</Umsatzsteuervoranmeldung> \
</Steuerfall> \
</Anmeldungssteuern> \
</Nutzdaten> \
</Nutzdatenblock> \
</DatenTeil>");
	return DatenTeil;
}

CString CEricFormularlogikUStVA::GetLoginfo()
{
	return _T(" / UST-Zahlbetrag: ") + m_FeldWert83Merken + _T(" / Zeitraum: ") + m_MomentanerFormularAnzeigename;
}

void CEricFormularlogikUStVA::UebertragungAbschliessen()
{
	// die UST-Schuld mit dem aktuellen Voranmeldungszeitraum speichern
	m_pDokumentCtrl->SpeichereBenutzerdefWert(_T("Elster"), _T("UST-Zahlbetrag-") + m_Jahr + _T("-") + m_Zeitraum, m_FeldWert83Merken);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	LISTE AUFBAUEN
//
void CEricFormularlogikUStVA::UpdateListe(CString &csFormularDateipfad, CString (&m_ListeInhalt)[500][5], CQuickList* pListe, BOOL bNurSpaltenbreitenAnpassen)
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
	static _TCHAR* Spaltentitel[] = { _T("Beschreibung"), _T("Feld-Nr."), _T("Bem.Grundl."), _T("Feld-Nr."), _T("Steuer") };
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
	m_pFormularCtrl->WaehleFormular(m_FormularDateipfad);

	// Formularlayout in Liste übertragen
	// Spalten: 1. EC&T FeldID Bemessungsgrundlage, 2. EC&T FeldID Steuer, 
	//          3. tatsächliches KZ Bemessungsgrundlage, 4. tatsächliches KZ Steuer
	static int Formularlayout[][4] = {
		81, 1081, 0, 0,
		86, 1086, 0, 0,
		87, 0, 0, 0,
		35, 36, 0, 0,
		77, 0, 0, 0,
		76, 80, 0, 0,
		0, 0, 0, 0,
		41, 0, 0, 0,
		44, 0, 0, 0,
		49, 0, 0, 0,
		43, 0, 0, 0,
		48, 0, 0, 0,
		0, 0, 0, 0,
		91, 0, 0, 0,
		89, 1189, 0, 0,
		93, 1193, 0, 0,
		90, 0, 0, 0,
		95, 98, 0, 0,
		94, 96, 0, 0,
		0, 0, 0, 0,
		46, 47, 0, 0,
		// veraltet	52, 53, 0, 0,	
					73, 74, 0, 0,
					// veraltet	78, 79, 0, 0,	
								84, 85, 0, 0,
								0, 0, 0, 0,
								42, 0, 0, 0,
								// veraltet	68, 0, 0, 0,
											60, 0, 0, 0,
											21, 0, 0, 0,
											45, 0, 0, 0,
											0, 0, 0, 0,
											-IDS_SEPARATOR, 0, 0, 0,
											-IDS_UMSATZSTEUER, 2003, 0, 0,
											0, 0, 0, 0,
											0, 1466, 0, 66,
											0, 61, 0, 0,
											0, 62, 0, 0,
											0, 67, 0, 0,
											0, 63, 0, 0,
											0, 59, 0, 0,
											0, 64, 0, 0,
											-IDS_SEPARATOR, 0, 0, 0,
											-IDS_VERBLEIBENDER_BETRAG, 2004, 0, 0,
											0, 0, 0, 0,
											0, 65, 0, 0,
											0, 69, 0, 0,
											-IDS_SEPARATOR, 0, 0, 0,
											0, 39, 0, 0,
											0, 0, 0, 0,
											-IDS_SEPARATOR, 0, 0, 0,
											0, 83, 0, 0
	};
	static int AnzahlFormularlayoutZeilen = sizeof(Formularlayout) / sizeof(Formularlayout[0]);

	// Hinweis: Dies ist kein Beispiel für durchdachten Code... aber er funktioniert!

	int Zeile, Spalte;
	CString FeldWert83Merken;
	BOOL bSternchenHinweisAnzeigen = FALSE;
	for (Zeile = 0; Zeile < sizeof(m_ListeInhalt) / sizeof(m_ListeInhalt[0]); Zeile++)	// Listenspeicher initialisieren
		for (Spalte = 0; Spalte < sizeof(m_ListeInhalt[0]) / sizeof(m_ListeInhalt[0][0]); Spalte++)
			m_ListeInhalt[Zeile][Spalte] = _T("");
	for (Zeile = 0; Zeile < AnzahlFormularlayoutZeilen; Zeile++)
	{
		extern CECTElsterApp theApp;
		CString ResourceString;

		int LayoutWert = Formularlayout[Zeile][0];
		int LayoutWert2 = Formularlayout[Zeile][1];
		if (LayoutWert < 0)			// negative Werte sind Ressource Strings
		{
			LoadString(theApp.m_hInstance, -LayoutWert, ResourceString.GetBuffer(10000), 10000);
			m_ListeInhalt[Zeile][LayoutWert != -IDS_SEPARATOR ? 0 : 4] = ResourceString;  // Separator nur in der Summenspalte anzeigen, alle anderen IDS_STRINGs in der ersten Spalte anzeigen

			if (LayoutWert2 > 0)
			{
				CString Feldwert2 = m_pFormularCtrl->HoleFeldwertUeberID(LayoutWert2);
				if (Formularlayout[Zeile][3]) LayoutWert2 = Formularlayout[Zeile][3]; // tatsächliches KZ, nicht EC&T FeldID anzeigen
				if (LayoutWert2 < 100)
				{
					CString Feldwert2AlsString;
					Feldwert2AlsString.Format(_T("%d"), LayoutWert2);
					m_ListeInhalt[Zeile][3] = Feldwert2AlsString;	// Feld ID (KZ) in 4. Spalte eintragen
				}
				m_ListeInhalt[Zeile][4] = Feldwert2;				// den Feldwert zu der ID in 5. Spalte eintragen
			}
		}
		else if (LayoutWert > 0 || LayoutWert2 > 0)	// positive Werte sind FeldIDs
		{
			CString Feldbeschreibung = m_pFormularCtrl->HoleFeldbeschreibungUeberID(max(LayoutWert, LayoutWert2));
			m_ListeInhalt[Zeile][0] = Feldbeschreibung;
			//m_pListe->InsertItem (Zeile, Feldbeschreibung);		

			if (LayoutWert > 0)
			{
				CString Feldwert = m_pFormularCtrl->HoleFeldwertUeberID(LayoutWert);
				if (Formularlayout[Zeile][3]) LayoutWert = Formularlayout[Zeile][2]; // tatsächliches KZ, nicht EC&T FeldID anzeigen
				if (LayoutWert < 100)
				{
					CString FeldwertAlsString;
					FeldwertAlsString.Format(_T("%d"), LayoutWert);
					if ((LayoutWert != 68 && LayoutWert != 52 && LayoutWert != 78) || Feldwert != _T(""))  // veraltete Felder nur anzeigen, wenn sie Werte enthalten (Validierung springt dann an!)
						m_ListeInhalt[Zeile][1] = FeldwertAlsString;	// Feld ID (KZ) in 2. Spalte eintragen
				}
				m_ListeInhalt[Zeile][2] = Feldwert;					// den Feldwert zu der ID in 3. Spalte eintragen
			}

			if (LayoutWert2 > 0)
			{
				CString Feldwert2 = m_pFormularCtrl->HoleFeldwertUeberID(LayoutWert2);
				if (Formularlayout[Zeile][3]) LayoutWert2 = Formularlayout[Zeile][3]; // tatsächliches KZ, nicht EC&T FeldID anzeigen
				if (LayoutWert2 == 83)
					FeldWert83Merken = Feldwert2;
				if (LayoutWert2 < 100)
				{
					CString Feldwert2AlsString;
					Feldwert2AlsString.Format(_T("%d"), LayoutWert2);
					if ((LayoutWert2 != 53 && LayoutWert2 != 79) || Feldwert2 != _T(""))  // veraltete Felder nur anzeigen, wenn sie Werte enthalten (Validierung springt dann an!)
						m_ListeInhalt[Zeile][3] = Feldwert2AlsString;	// Feld ID (KZ) in 4. Spalte eintragen
				}
				if (LayoutWert2 > 100 && (LayoutWert2 - 1000 == LayoutWert || LayoutWert2 - 1100 == LayoutWert) && Feldwert2 != _T(""))
				{
					Feldwert2 += _T("*");
					bSternchenHinweisAnzeigen = TRUE;
				}
				m_ListeInhalt[Zeile][4] = Feldwert2;			// den Feldwert zu der ID in 5. Spalte eintragen
			}
#ifdef TESTVERBINDUNG
			else
				m_ListeInhalt[Zeile][3] = _T("TEST!");
#endif
		}
	}

	// Hinweis auf bereits übertragene Voranmeldungen für den Zeitraum
	int nZeitraum = m_pFormularCtrl->HoleVoranmeldungszeitraum();
	if (nZeitraum > 12) nZeitraum += 28;	// 1-12 Monat; 1. Quartal == 41, 4. Q. == 44
	CString Zeitraum;
	Zeitraum.Format(_T("%d"), nZeitraum);
	CString Jahr;
	Jahr.Format(_T("%-0.0d"), (int)m_pDokumentCtrl->GetJahr());
	CString FeldWert83VonLetzterUebertragung = m_pDokumentCtrl->HoleBenutzerdefWert(_T("Elster"), _T("UST-Zahlbetrag-") + Jahr + _T("-") + Zeitraum);
	if (FeldWert83VonLetzterUebertragung.GetLength())
	{
		if (FeldWert83VonLetzterUebertragung == FeldWert83Merken)
		{
			m_ListeInhalt[++Zeile][0] = _T("Der Umsatzsteuerbetrag entspricht dem bei der letzten Übertragung übermittelten.");
			m_ListeInhalt[++Zeile][0] = _T("Es ist wahrscheinlich keine weitere Datenübertragung nötig.");
		}
		else
		{
			m_ListeInhalt[++Zeile][0] = _T("ACHTUNG: Der Umsatzsteuerbetrag entspricht nicht dem bei der letzten Übertragung übermittelten. (") + FeldWert83VonLetzterUebertragung + _T(")");
			m_ListeInhalt[++Zeile][0] = _T("Es ist deshalb wohl eine berichtigte Anmeldung für diesen Voranmeldungszeitraum nötig.");
		}
		m_KorrigierteAnmeldung = TRUE;
	}
	else
	{
		m_KorrigierteAnmeldung = FALSE;
	}
#ifdef TESTVERBINDUNG
	++Zeile;
	m_ListeInhalt[++Zeile][0] = _T("ACHTUNG: Das installierte OCX ist nur eine Testversion und baut nur eine Testverbindung auf.");
	m_ListeInhalt[++Zeile][0] = _T("               Es werden keine Daten als reale Vorgänge behandelt.");
#endif
	if (bSternchenHinweisAnzeigen)
	{
		++Zeile;
		m_ListeInhalt[++Zeile][0] = _T("Hinweis: Mit '*' gekennzeichnete Steuer sind nur aus der Bemessungsgrundlage errechnete Zwischenwerte und werden nicht gesondert an das Finanzamt übertragen.");
	}

	m_pListe->SetItemCount(Zeile);
	m_pListe->RedrawItems(0, Zeile - 1);
	m_pListe->InvalidateRect(NULL, FALSE);
}