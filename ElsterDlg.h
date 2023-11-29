// ElsterDlg.h : Declaration of the CElsterDlg property page class.
// CElsterDlg : See ElsterDlg.cpp for implementation.
//
// Copyright (C) 2019  Thomas Mielke
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
#include "afxwin.h"
#include "einstellungctrl.h"
#include "dokumentctrl.h"
#include "afxcmn.h"
#include "resource.h"
#include "AboutDlg.h"
#include "QuickList.h"  // https://www.codeproject.com/Articles/8112/CQuickList
#include "EricFormularlogik.h"
#include <vector>
#include "CBuchungCtrl.h"

// CElsterDlg dialog

#define FORMULARTYP_EUER	1
#define FORMULARTYP_USTVA	2

struct Betrieb
{
	CString name;
	CString art;
	CString rechtsform;
	CString steuernummer;
	CString einkunftsart;
	CString inhaber;
};

class CElsterDlg : public CDialog
{
//	DECLARE_DYNAMIC(CElsterDlg)

public:
	CElsterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CElsterDlg();

// Dialog Data
	enum { IDD = IDD_ELSTERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	long m_dokID; // enthält das Dokument-Handle, um auf die EC&T-ActiveX-Objekte zugreifen zu können

private:
	// Dialogfeld-Variablen
	CString m_Datei;  // Dateipfad zur Elster-Signaturdatei	
	CString m_Passwort;  // Passwort für Elster-Signaturdatei
	CString m_EmailAdresse;
	CString m_Telefon;
	BOOL m_KorrigierteAnmeldung;
	BOOL m_BelegeWerdenNachgereicht;
	BOOL m_VerrechnungDesErstattungsanspruchs;
	BOOL m_EinzugsermaechtigungWiderrufen;
	std::vector<Betrieb> m_Betriebe;

	// Controls
	CFormularCtrl m_FormularCtrl;
	CString m_FormularDateipfad;    // ... des aktuell ausgewählten Formulars
	CEinstellung m_EinstellungCtrl;
	CDokumentCtrl m_DokumentCtrl;
	CBuchungCtrl m_BuchungCtrl;
	CComboBox m_VoranmeldungszeitraumCtrl;
	CComboBox m_FinanzamtCtrl;
	CQuickList m_Liste;			// Ersatz für CListCtrl, siehe https://www.codeproject.com/Articles/8112/CQuickList
	CEdit m_ErgebnisCtrl;		// teilt sich Platz im Dialog mit m_Liste
	CButton m_Zeige;

	// Variablen für die Steuerung von m_Liste 
	CEricFormularlogik* m_pEric;						// abhängig vom Formular unterschiedliche Logik (EÜR/UStVA)
	CString m_ListeInhalt[500][6];						// ist owner-drawn, Spalten: Feldbeschreibung, KZ, Feldwert, Elster-Feldname(Pfad)
	CMap<CString, CString&, CString, CString&> m_ListeHinweise;	// nach Validierung werden Einträge des ListCtrl 
	CMap<CString, CString&, CString, CString&> m_ListeFehler;		// ggf. farbig hervorgehoben und Hinweise/Fehler gezeigt

	// Member-Funktionen
	void EricKontext(BOOL bNurValidieren, CTime &Jetzt, CString &MomentanerFormularAnzeigename, CString& csBetrieb, CString &Jahr, CString &Zeitraum, CQuickList *pListe);
	void ERiC(BOOL bNurValidieren);
	LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	void UpdateSteuerelemente(int formulartyp);

	// Message-Handler
	DECLARE_MESSAGE_MAP()
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnSelchangeFinanzamt();
	afx_msg void OnEnKillfocusTelefon();
	afx_msg void OnEnKillfocusEmailAdresse();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedInfo();
	afx_msg void OnCbnSelchangeVoranmeldungszeitraum();
	afx_msg void OnBnClickedZeige();
	afx_msg void OnBnClickedFinanzamtNichtInDerListe();
	afx_msg void OnBnClickedSignaturoptionen();
	afx_msg void OnBnClickedDurchsuchen();
	afx_msg void OnEnKillfocusDatei();
	afx_msg void OnNMClickListe(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAktualisieren();
	CComboBox m_RechtsformCtrl;
	CComboBox m_GrundstuecksveraeusserungenCtrl;
	CComboBox m_EinkunftsartCtrl;
	CComboBox m_BetriebsinhaberCtrl;
public:
	afx_msg void OnCbnSelchangeGrundstuecksveraeusserungen();
	afx_msg void OnCbnSelchangeEinkunftsart();
	afx_msg void OnCbnSelchangeRechtsform();
	afx_msg void OnCbnSelchangeBetriebsinhaber();
};
