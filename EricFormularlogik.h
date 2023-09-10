// EricFormularlogik.h : Declaration of the CElsterDlg property page class.
// CEricFormularlogik : See EricFormularlogik.cpp for implementation.
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

#include "ElsterDlg.h"

// CEricFormularlogik benutzt die ERiC-Bibliothek um entweder die Hinweis- bzw. 
// Fehlerliste zu bef�llen oder aber Daten an Elster zu versenden
class CEricFormularlogik : public CObject
{
public:
	// Dialogfeld-Variablen
	CString *m_pDatei;  // Dateipfad zur Elster-Signaturdatei	
	CString *m_pPasswort;  // Passwort f�r Elster-Signaturdatei
	CString *m_pEmailAdresse;
	CString *m_pTelefon;
	BOOL m_KorrigierteAnmeldung;
	BOOL m_BelegeWerdenNachgereicht;
	BOOL m_VerrechnungDesErstattungsanspruchs;
	BOOL m_EinzugsermaechtigungWiderrufen;

	// Pointer auf Dialog-Controls
	CFormularCtrl *m_pFormularCtrl;
	CString m_FormularDateipfad;    // ... des aktuell im Dialog ausgew�hlten Formulars
	CEinstellung *m_pEinstellungCtrl;
	CDokumentCtrl *m_pDokumentCtrl;
	CQuickList *m_pListe;			// Ersatz f�r CListCtrl, siehe https://www.codeproject.com/Articles/8112/CQuickList

	// Variablen f�r die Steuerung von m_Liste 
	CString m_ListeInhalt[500][5];						// ist owner-drawn
	CMap<int, int, CString, CString> *m_pListeHinweise;	// nach Validierung werden Eintr�ge des ListCtrl 
	CMap<int, int, CString, CString> *m_pListeFehler;		// ggf. farbig hervorgehoben und Hinweise/Fehler gezeigt

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
	CString Render(HWND m_hWnd,
				CFormularCtrl* pFormularCtrl,
				CString &FormularDateipfad,
				CEinstellung* pEinstellungCtrl,
				CDokumentCtrl* pDokumentCtrl,
				CQuickList* pListe,
				CMap<int, int, CString, CString> *pListeHinweise,
				CMap<int, int, CString, CString> *pListeFehler,
				CString &Datei,
				CString &Passwort,
				CString &EmailAdresse,
				CString &Telefon,
				BOOL bKorrigierteAnmeldung,
				BOOL bBelegeWerdenNachgereicht,
				BOOL bVerrechnungDesErstattungsanspruchs,
				BOOL bEinzugsermaechtigungWiderrufen,
				CTime &Jetzt,
				CString &Jahr,
				CString &Zeitraum,
				CString &Bundesfinanzamtsnummer,
				CString &EmpfaengerFinanzamt,
				CString &MomentanerFormularAnzeigename,
				BOOL bNurValidieren);

	// virtuelle Funktionen zur Beschickung der unterschiedlichen Formulare
	virtual CString GetDatenteil() { return (CString)""; };
	virtual CString GetLoginfo() { return (CString)""; };
	virtual void UebertragungAbschliessen() {};
	virtual CString GetVerfahren() { return (CString)""; };
	virtual CString GetDatenart() { return (CString)""; };
};
