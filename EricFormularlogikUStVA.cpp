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
	+ m_EmailAdresse) + _T("</DatenLieferant> \
</NutzdatenHeader> \
<Nutzdaten> \
<Anmeldungssteuern ") + (atoi(m_Jahr) < 2021 ? _T("art = \"UStVA\"") : _T("xmlns=\"http://finkonsens.de/elster/elsteranmeldung/ustva/v") + m_Jahr + _T("\"")) + _T(" version=\"") + m_Jahr + (atoi(m_Jahr) < 2021 ? _T("01") : _T("")) + _T("\"> \
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