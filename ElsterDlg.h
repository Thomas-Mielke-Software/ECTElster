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
#include "AboutDlg.h"
#include "QuickList.h"  // https://www.codeproject.com/Articles/8112/CQuickList

// CElsterDlg dialog

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

	// Controls
	CFormularCtrl m_FormularCtrl;
	CComboBox m_VoranmeldungszeitraumCtrl;
	CEinstellung m_EinstellungCtrl;
	CDokumentCtrl m_DokumentCtrl;
	CComboBox m_FinanzamtCtrl;
	CQuickList m_Liste;			// Ersatz für CListCtrl, siehe https://www.codeproject.com/Articles/8112/CQuickList
	CEdit m_ErgebnisCtrl;		// teilt sich Platz im Dialog mit m_Liste
	CButton m_Zeige;

	// Variablen für die Steuerung von m_Liste 
	CString m_ListeInhalt[500][5];						// ist owner-drawn
	CMap<int, int, CString, CString> m_ListeHinweise;	// nach Validierung werden Einträge des ListCtrl 
	CMap<int, int, CString, CString> m_ListeFehler;		// ggf. farbig hervorgehoben und Hinweise/Fehler gezeigt
	LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	void UpdateListe(BOOL bNurSpaltenbreitenAnpassen = FALSE);

	// Message-Handler
	DECLARE_MESSAGE_MAP()
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

	// Hilfsfunktionen
	void ERiC(BOOL bNurValidieren);
	int RegSearchReplace(CString& string, LPCTSTR sSearchExp, LPCTSTR sReplaceExp, CStringArray& csaReplaceCount);	// Helfer für reguläre Ausdrücke
	void PrintString(CString Dokumentname, CString Text);
	CString XMLEscape(CString StringZumEscapen);
};
