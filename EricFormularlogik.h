// EricFormularlogik.h : Declaration of the CElsterDlg property page class.
// CEricFormularlogik : See EricFormularlogik.cpp for implementation.
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

#include "FormularCtrl.h"
#include "einstellungctrl.h"
#include "dokumentctrl.h"
#include "QuickList.h"  // https://www.codeproject.com/Articles/8112/CQuickList

// CEricFormularlogik benutzt die ERiC-Bibliothek um entweder die Hinweis- bzw. 
// Fehlerliste zu befüllen oder aber Daten an Elster zu versenden
class CEricFormularlogik : public CObject
{
	DECLARE_DYNAMIC(CEricFormularlogik)
public:
	// Dialogfeld-Variablen
	CString *m_pDatei;  // Dateipfad zur Elster-Signaturdatei	
	CString *m_pPasswort;  // Passwort für Elster-Signaturdatei
	CString *m_pEmailAdresse;
	CString *m_pTelefon;
	BOOL m_KorrigierteAnmeldung;
	BOOL m_BelegeWerdenNachgereicht;
	BOOL m_VerrechnungDesErstattungsanspruchs;
	BOOL m_EinzugsermaechtigungWiderrufen;
	CString m_csRechtsform;
	CString m_csEinkunftsart;
	CString m_csBetriebsinhaber;
	CString m_csGrundstuecksveraeusserungen;

	// Pointer auf Dialog-Controls
	CFormularCtrl *m_pFormularCtrl;
	CString m_FormularDateipfad;    // ... des aktuell im Dialog ausgewählten Formulars
	CEinstellung *m_pEinstellungCtrl;
	CDokumentCtrl *m_pDokumentCtrl;
	CQuickList *m_pListe;			// Ersatz für CListCtrl, siehe https://www.codeproject.com/Articles/8112/CQuickList

	// Variablen für die Steuerung von m_Liste in ElsterDlg
	CMap<CString, CString&, CString, CString&>* m_pListeHinweise;	// nach Validierung werden Einträge des ListCtrl 
	CMap<CString, CString&, CString, CString&>* m_pListeFehler;		// ggf. farbig hervorgehoben und Hinweise/Fehler gezeigt

	// sonstige
	CTime m_Jetzt;
	CString m_EmpfaengerFinanzamt;
	CString m_EmailAdresse;
	CString m_Telefon;
	CString m_Jahr;
	CString m_Zeitraum;
	CString m_Bundesfinanzamtsnummer;
	CString m_HerstellerID;
	CString m_MomentanerFormularAnzeigename;
	CString m_AppVersion;
	
#define RENDER_NEUTRAL 0
#define RENDER_LOG_ZEIGEN1 1
#define RENDER_LOG_ZEIGEN2 2

public:

	CString m_PinStatus;

	CEricFormularlogik();
	void Init(
		CFormularCtrl* pFormularCtrl,
		CEinstellung* pEinstellungCtrl,
		CDokumentCtrl* pDokumentCtrl);
	CString Render(HWND m_hWnd,
		CString &FormularDateipfad,
		CQuickList* pListe,
		CString(&m_pListeInhalt)[500][6],
		CMap<CString, CString&, CString, CString&> *pListeHinweise,
		CMap<CString, CString&, CString, CString&> *pListeFehler,
		CString &Datei,
		CString &Passwort,
		CString &EmailAdresse,
		CString &Telefon,
		BOOL bKorrigierteAnmeldung,
		BOOL bBelegeWerdenNachgereicht,
		BOOL bVerrechnungDesErstattungsanspruchs,
		BOOL bEinzugsermaechtigungWiderrufen,
		CString &csRechtsform,
		CString &csEinkunftsart,
		CString &csBetriebsinhaber,
		CString &csGrundstuecksveraeusserungen,
		CTime &Jetzt,
		CString &Jahr,
		CString &Zeitraum,
		CString &Bundesfinanzamtsnummer,
		CString &EmpfaengerFinanzamt,
		CString &MomentanerFormularAnzeigename,
		BOOL bNurValidieren);

	// virtuelle Funktionen zur Beschickung der unterschiedlichen Formulare
	virtual void UpdateListe(CString& csFormularDateipfad, CString& csBetrieb, CString (&m_pListeInhalt)[500][6], CQuickList* pListe, BOOL bNurSpaltenbreitenAnpassen = FALSE) { pListe->SetItemCount(0); return; };
	virtual CString GetDatenteil() { return (CString)""; };
	virtual CString GetLoginfo() { return (CString)""; };
	virtual void UebertragungAbschliessen() {};
	virtual CString GetVerfahren() { return (CString)""; };
	virtual CString GetDatenart() { return (CString)""; };
};
