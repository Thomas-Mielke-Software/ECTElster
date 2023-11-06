// ElsterDlg.cpp : implementation file
//
// Copyright (C) 2019  Thomas Mielke
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
#include "ElsterDlg.h"
#include "EricFormularlogik.h"
#include "EricFormularlogikUStVA.h"
#include "EricFormularlogikEUeR.h"
#include "tmfanr.h"
#include "XMLite.h"  // https://www.codeproject.com/Articles/3426/XMLite-simple-XML-parser

#include "EricApi.h"
#include "eric_fehlercodes.h"
#include "eric_types.h"

// CElsterDlg dialog

// IMPLEMENT_DYNAMIC(CElsterDlg, CDialog)

CElsterDlg::CElsterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CElsterDlg::IDD, pParent)
	, m_dokID(0)
	, m_EmailAdresse(_T(""))
	, m_Telefon(_T(""))
	, m_KorrigierteAnmeldung(FALSE)
	, m_BelegeWerdenNachgereicht(FALSE)
	, m_VerrechnungDesErstattungsanspruchs(FALSE)
	, m_EinzugsermaechtigungWiderrufen(FALSE)
	, m_Datei(_T(""))
	, m_Passwort(_T(""))
	, m_pEric(NULL)
//	, m_Signaturoption(0)
{

}

CElsterDlg::~CElsterDlg()
{
	if (m_pEric) delete m_pEric;
}

void CElsterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORMULARCTRL1, m_FormularCtrl);
	DDX_Control(pDX, IDC_VORANMELDUNGSZEITRAUM, m_VoranmeldungszeitraumCtrl);
	DDX_Control(pDX, IDC_EINSTELLUNGCTRL1, m_EinstellungCtrl);
	DDX_Control(pDX, IDC_DOKUMENTCTRL1, m_DokumentCtrl);
	DDX_Control(pDX, IDC_LISTE, m_Liste);
	DDX_Text(pDX, IDC_EMAIL_ADRESSE, m_EmailAdresse);
	DDX_Text(pDX, IDC_TELEFON, m_Telefon);
	DDX_Control(pDX, IDC_FINANZAMT, m_FinanzamtCtrl);
	DDX_Check(pDX, IDC_KORRIGIERTE_ANMELDUNG, m_KorrigierteAnmeldung);
	DDX_Check(pDX, IDC_BELEGE_WERDEN_NACHGEREICHT, m_BelegeWerdenNachgereicht);
	DDX_Check(pDX, IDC_VERRECHNUNG_DES_ERSTATTUNGSANSPRUCHS, m_VerrechnungDesErstattungsanspruchs);
	DDX_Check(pDX, IDC_EINZUGSERMAECHTIGUNG_WIDERRUFEN, m_EinzugsermaechtigungWiderrufen);
	DDX_Control(pDX, IDC_ERGEBNIS, m_ErgebnisCtrl);
	DDX_Control(pDX, IDC_ZEIGE, m_Zeige);
	DDX_Text(pDX, IDC_DATEI, m_Datei);
	DDX_Text(pDX, IDC_PASSWORT, m_Passwort);
}


BEGIN_MESSAGE_MAP(CElsterDlg, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CElsterDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_FINANZAMT, &CElsterDlg::OnCbnSelchangeFinanzamt)
	ON_EN_KILLFOCUS(IDC_TELEFON, &CElsterDlg::OnEnKillfocusTelefon)
	ON_EN_KILLFOCUS(IDC_EMAIL_ADRESSE, &CElsterDlg::OnEnKillfocusEmailAdresse)
	ON_BN_CLICKED(IDCANCEL, &CElsterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_INFO, &CElsterDlg::OnBnClickedInfo)
	ON_CBN_SELCHANGE(IDC_VORANMELDUNGSZEITRAUM, &CElsterDlg::OnCbnSelchangeVoranmeldungszeitraum)
	ON_BN_CLICKED(IDC_ZEIGE, &CElsterDlg::OnBnClickedZeige)
	ON_BN_CLICKED(IDC_FINANZAMT_NICHT_IN_DER_LISTE, &CElsterDlg::OnBnClickedFinanzamtNichtInDerListe)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SIGNATUROPTIONEN, &CElsterDlg::OnBnClickedSignaturoptionen)
	ON_BN_CLICKED(IDC_DURCHSUCHEN, &CElsterDlg::OnBnClickedDurchsuchen)
	ON_EN_KILLFOCUS(IDC_DATEI, &CElsterDlg::OnEnKillfocusDatei)
    ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, &CElsterDlg::OnGetListItem)
	#pragma warning(push)
	#pragma warning(disable:26454)
	ON_NOTIFY(NM_CLICK, IDC_LISTE, &CElsterDlg::OnNMClickListe)
	#pragma warning(pop)
	ON_BN_CLICKED(IDC_AKTUALISIEREN, &CElsterDlg::OnBnClickedAktualisieren)
END_MESSAGE_MAP()


// CElsterDlg message handlers

int CElsterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	INITDIALOG
//
BOOL CElsterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	EnableToolTips();			// Tooltip funktioniert im ActiveX leider nicht...

	AfxGetApp()->BeginWaitCursor();

	m_Telefon = m_EinstellungCtrl.HoleEinstellung(_T("[Allgemein]telefon"));
	m_EmailAdresse = m_EinstellungCtrl.HoleEinstellung(_T("[Allgemein]emailadresse"));

	//m_Signaturoption = atoi(m_EinstellungCtrl.HoleEinstellung("[ElsterExport]signaturoption"));
	//if (m_Signaturoption > 3) m_Signaturoption = 0;
	m_Datei = m_EinstellungCtrl.HoleEinstellung(_T("[ElsterExport]signaturdatei"));

	// Finanzamt-Combobox aufbauen
	int i;
	for(i = 0; ListOfFinanzaemter[i].iFA; i++)
	{
		m_FinanzamtCtrl.AddString(ListOfFinanzaemter[i].szFA);
		m_FinanzamtCtrl.SetItemData(m_FinanzamtCtrl.GetCount()-1, ListOfFinanzaemter[i].iFA);	// Index speichern, um dar�ber bei Verarbeitung den Pfad zu gewinnen
	}
	int nBundesfinanzamtsnummer = atoi(m_EinstellungCtrl.HoleEinstellung(_T("[Allgemein]bundesfinanzamtsnummer")));
	if (!nBundesfinanzamtsnummer)
		i = m_FinanzamtCtrl.FindString(0, m_EinstellungCtrl.HoleEinstellung(_T("fname")));	// wenn noch nicht gew�hlt, versuchen obs mit der Finanzamtsadresse aus den Einstellungen klappt
	else
		for(i = 0; ListOfFinanzaemter[i].iFA; i++)
			if (ListOfFinanzaemter[i].iFA == nBundesfinanzamtsnummer)
				break;
	if (i >= 0) m_FinanzamtCtrl.SetCurSel(m_FinanzamtCtrl.FindString(0, ListOfFinanzaemter[i].szFA));

	// damit das Formular wei�, welche Daten es einsetzen soll
	m_FormularCtrl.SetzeDokumentID(m_dokID);
	m_DokumentCtrl.SetID(m_dokID);	// und das Dokument-Objekt sowieso...

	// Formular/Voranm.zeitr. Combobox aufbauen
	m_VoranmeldungszeitraumCtrl.ResetContent();
	int n = m_FormularCtrl.HoleFormularanzahl();
	TCHAR* formulararten[2] = { _T("E/�-Rechnung"), _T("Umsatzsteuer-Voranmeldung") };
	for (int formularart = 0; formularart < sizeof(formulararten) / sizeof(formulararten[0]); formularart++)
		for (i = 0; i < n; i++)
		{
			CString Formularname = m_FormularCtrl.HoleFormularnamen(i, formulararten[formularart]);
			if (Formularname.GetLength() && m_FormularCtrl.HoleVoranmeldungszeitraum() != 0 &&
				((Formularname.GetLength() < 21 || Formularname.Left(21) != "Umsatzsteuererkl�rung")))
			{
				m_VoranmeldungszeitraumCtrl.AddString(Formularname);
				m_VoranmeldungszeitraumCtrl.SetItemData(m_VoranmeldungszeitraumCtrl.GetCount()-1, i);	// Index speichern, um dar�ber bei Verarbeitung den Pfad zu gewinnen
			}
		}	

	// Vorauswahl des Formulars nach heutigem Datum -- unter Einbeziehung der Dauerfristverl�ngerung
	int jj = m_DokumentCtrl.GetJahr();
	int nDauerfristverlaengerungTage = 0;
	CString Key;
	Key.Format(_T("Sondervorauszahlung%-04.4d"), jj);
	if (atoi(m_DokumentCtrl.HoleBenutzerdefWert(_T("Dauerfristverl�ngerung"), Key)))
		nDauerfristverlaengerungTage = 30;

	CTime now = CTime::GetCurrentTime();
	CString Zeitraum;
	if (atoi(m_EinstellungCtrl.HoleEinstellung(_T("monatliche_voranmeldung")).GetBuffer(0)))
	{ // Quartalsweise
		if (now >= CTime(jj+1, 1, 1, 0, 0, 0) + CTimeSpan(nDauerfristverlaengerungTage, 0, 0, 0)) // schon neues Jahr? dann letztes Quartal voreinstellen
			Zeitraum = _T("4. Quartal");
		else if (now >= CTime(jj, 10, 1, 0, 0, 0) + CTimeSpan(nDauerfristverlaengerungTage, 0, 0, 0))
			Zeitraum = _T("3. Quartal");
		else if (now >= CTime(jj, 7, 1, 0, 0, 0) + CTimeSpan(nDauerfristverlaengerungTage, 0, 0, 0))
			Zeitraum = _T("2. Quartal");
		else // if (now >= CTime(jj, 4, 1, 0, 0, 0))
			Zeitraum = _T("1. Quartal");
	}
	else
	{ // Monatlich
		now -= CTimeSpan(28, 0, 0, 0) + CTimeSpan(nDauerfristverlaengerungTage, 0, 0, 0);
		if (now.GetYear() < jj)
			Zeitraum = _T("Januar");
		else if (now.GetYear() > jj)
			Zeitraum = _T("Dezember");
		else
			switch (now.GetMonth())
			{
			case 1:  Zeitraum = _T("Januar"); break;
			case 2:  Zeitraum = _T("Februar"); break;
			case 3:  Zeitraum = _T("M�rz"); break;
			case 4:  Zeitraum = _T("April"); break;
			case 5:  Zeitraum = _T("Mai"); break;
			case 6:  Zeitraum = _T("Juni"); break;
			case 7:  Zeitraum = _T("Juli"); break;
			case 8:  Zeitraum = _T("August"); break;
			case 9:  Zeitraum = _T("September"); break;
			case 10: Zeitraum = _T("Oktober"); break;
			case 11: Zeitraum = _T("November"); break;
			case 12: Zeitraum = _T("Dezember"); break;
			}
	}
	CString Jahr;
	Jahr.Format(_T("%-0.04d"), jj);
	if (LB_ERR != m_VoranmeldungszeitraumCtrl.SetCurSel(m_VoranmeldungszeitraumCtrl.FindString(0, _T("Umsatzsteuer-Voranmeldung ") + Jahr + _T(" ") + Zeitraum)))
	{
		// Liste mit Feldern initialisieren
		if (m_Liste && m_FormularCtrl/* && m_Liste.GetItemCount()*/)
		{
			SetTimer(2, 1, NULL);
		}
	}
	else
		UpdateData(FALSE);

	AfxGetApp()->EndWaitCursor();
	//SetTimer(1, 1, NULL);

	TCHAR temp_path[MAX_PATH+1];
	if (!GetTempPath(sizeof(temp_path), temp_path)) { AfxMessageBox(_T("Konnte keinen Pfad zum Temp-Verzeichnis bekommen. :(")); return TRUE; }
	EricEinstellungSetzen(_T("basis.log_dir"), temp_path);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// Item-Callback f�r CQuickList
LRESULT CElsterDlg::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
    ASSERT((HWND)wParam == m_Liste.GetSafeHwnd());

    CQuickList::CListItemData* data = (CQuickList::CListItemData*)lParam;

    // item & subitem 
    int item = data->GetItem();
    int subItem = data->GetSubItem();
	if (item > sizeof(m_ListeInhalt)/sizeof(m_ListeInhalt[0])) return 0;
	if (subItem > sizeof(m_ListeInhalt[0])/sizeof(m_ListeInhalt[0][0])) return 0;

	#pragma warning(push)
	#pragma warning(disable:6385)
	data->m_text = m_ListeInhalt[item][subItem];	// Zellentext anzeigen
	#pragma warning(pop)
	data->m_tooltip = m_ListeInhalt[item][0];		// Tooltip funktioniert im ActiveX leider nicht...
	if (subItem) // 2. oder h�here Spalte? ggf. farbig markieren
	{	
		int subItemDesFeldkennzeichens = subItem & 0x1 ? subItem : subItem - 1; // entweder 2. oder 4. Spalte ermitteln, wo das jeweils zugeh�rige Kennzeichen drin steht
		int nFeldkennzeichen = atoi(m_ListeInhalt[item][subItemDesFeldkennzeichens]);
		if (nFeldkennzeichen)
		{
			if (m_ListeFehler[nFeldkennzeichen] != _T(""))			// Fehler rot
			{
				data->m_colors.m_backColor = RGB(255,64,64);
				data->m_tooltip = m_ListeFehler[nFeldkennzeichen];
			}
			else if (m_ListeHinweise[nFeldkennzeichen] != _T(""))	// Hinweise gelb
			{
				data->m_colors.m_backColor = RGB(255,255,32);
				data->m_tooltip = m_ListeHinweise[nFeldkennzeichen];
			}
		}
	}
	else if (m_ListeInhalt[item][0].Left(6) == _T("Fehler"))
				data->m_colors.m_backColor = RGB(255,64,64);
	else if (m_ListeInhalt[item][0].Left(9) == _T("Hinweis: "))
				data->m_colors.m_backColor = RGB(255,255,32);

    return 0;
}


void CElsterDlg::OnNMClickListe(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem >= 0)
	{
		CString csMeldungstext;

		// 2. Spalte
		int nFeldkennzeichen = atoi(m_ListeInhalt[pNMItemActivate->iItem][1]);
		if (nFeldkennzeichen)
		{
			csMeldungstext += m_ListeFehler[nFeldkennzeichen] + (m_ListeFehler[nFeldkennzeichen] != _T("") ? _T(" ") : _T(""));
			csMeldungstext += m_ListeHinweise[nFeldkennzeichen] + (m_ListeHinweise[nFeldkennzeichen] != _T("") ? _T(" ") : _T(""));
		}
		// 4. Spalte
		nFeldkennzeichen = atoi(m_ListeInhalt[pNMItemActivate->iItem][3]);
		if (nFeldkennzeichen)
		{
			csMeldungstext += m_ListeFehler[nFeldkennzeichen] + (m_ListeFehler[nFeldkennzeichen] != _T("") ? _T(" ") : _T(""));
			csMeldungstext += m_ListeHinweise[nFeldkennzeichen] + (m_ListeHinweise[nFeldkennzeichen] != _T("") ? _T(" ") :_T(""));
		}
		if (m_ListeInhalt[pNMItemActivate->iItem][0].Left(6) == _T("Fehler"))
				csMeldungstext += m_ListeInhalt[pNMItemActivate->iItem][0];
		if (m_ListeInhalt[pNMItemActivate->iItem][0].Left(9) == _T("Hinweis: "))
				csMeldungstext += m_ListeInhalt[pNMItemActivate->iItem][0];
		
		if (csMeldungstext.Trim() != _T(""))
			AfxMessageBox(csMeldungstext);
	}
	
	*pResult = 0;
}


void CElsterDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(1);

	if (nIDEvent == 2)	// ERiC-Validierung mit Zeitverz�gerung
	{
		KillTimer(2);
		UpdateData();
		CString MomentanerFormularAnzeigename;
		CTime Jetzt;
		CString Zeitraum = "";

		int jj = m_DokumentCtrl.GetJahr();
		CString Jahr;
		Jahr.Format(_T("%-0.04d"), jj);
		EricKontext(true, Jetzt, MomentanerFormularAnzeigename, Jahr, Zeitraum, &m_Liste);
		m_FormularDateipfad = m_FormularCtrl.HoleFormularpfad(m_VoranmeldungszeitraumCtrl.GetItemData(m_VoranmeldungszeitraumCtrl.GetCurSel()));
		if (m_pEric) m_pEric->UpdateListe(m_FormularDateipfad, m_ListeInhalt, &m_Liste);
		UpdateData(FALSE);  // UpdateListe() �ndert z.B. evtl. die "korrigierte Anmeldung" Checkbox
		ERiC(TRUE);
		m_Liste.InvalidateRect(NULL, FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}


#define ID_VIEW_JOURNAL_SWITCH          32788
extern "C" short Pruefe_Steuernummer(const char *ELSTERSteuernummer, char *Pruefziffer_vor_Steuernummerumstellung, char *Pruefziffer_nach_Steuernummerumstellung);


void CElsterDlg::OnBnClickedOk()
{
	ERiC(FALSE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VERSENDEN bzw. VALIDIEREN
//
// benutzt die ERiC-Bibliothek um entweder die Hinweis- bzw. Fehlerliste zu bef�llen oder aber Daten an Elster zu versenden
// f�r eine bessere Trennung von GUI und Logik ist es in CEricFormularlogik ausgelagert

void CElsterDlg::EricKontext(BOOL bNurValidieren, CTime& Jetzt, CString& MomentanerFormularAnzeigename, CString &Jahr, CString &Zeitraum, CQuickList* pListe)
{
	int nZeitraum = m_FormularCtrl.HoleVoranmeldungszeitraum();
	if (nZeitraum > 12) nZeitraum += 28;	// 1-12 Monat; 1. Quartal == 41, 4. Q. == 44
	if (nZeitraum > 44 || nZeitraum < 1) nZeitraum = 1;	// Flickschusterei
	Zeitraum.Format(_T("%-02.2d"), nZeitraum);

	Jetzt = CTime::GetCurrentTime();
	m_VoranmeldungszeitraumCtrl.GetLBText(m_VoranmeldungszeitraumCtrl.GetCurSel(), MomentanerFormularAnzeigename);
	if (MomentanerFormularAnzeigename.Left(12) == "E/�-Rechnung")  // eine Art dependency injection
	{
		if (!bNurValidieren && MomentanerFormularAnzeigename.Right(4) != Jahr)
		{
			AfxMessageBox("Das ausgew�hlte Formular '" + MomentanerFormularAnzeigename + "' passt nicht zum Buchungsjahr " + Jahr + " des ge�ffneten Dokuments. Fals das Buchungsjahr falsch gesetzt wurde, kann es in den Einstellungen -> Allgemein (rechts bei den Dokumenteigenschaften) korrigiert werden.");
			return;
		}
		if (m_pEric)
			if (!m_pEric->IsKindOf(RUNTIME_CLASS(CEricFormularlogikEUeR)))
			{	// nur wenn n�tig, die Formularlogik auf E�R wechseln
				delete m_pEric;
				m_pEric = NULL;
			}
		if (!m_pEric)
		{
			m_pEric = new CEricFormularlogikEUeR();
			m_pEric->Init(&m_FormularCtrl, &m_EinstellungCtrl, &m_DokumentCtrl);
		}
	}
	else
	{
		if (Jetzt - CTime(m_DokumentCtrl.GetJahr(), nZeitraum <= 12 ?  // check nach m�glicherweise falschem Voranmeldungszeitraum
			nZeitraum : (nZeitraum - 41) * 3 + 1, 1, 0, 0, 0) > CTimeSpan(180, 0, 0, 0))
		{
			int n;
			if (!bNurValidieren && (n = AfxMessageBox(_T("Die Frist f�r die Abgabe der UST-Voranmeldung f�r den ausgew�hlten Zeitraum ist schon seit einiger Zeit verstrichen. \
Vielleicht ist das falsche Buchungsjahr ge�ffnet oder der falsche Zeitraum ausgew�hlt? Soll die Voranmeldung trotzdem verschickt werden?"), MB_YESNO)) != IDYES)
			{
				m_VoranmeldungszeitraumCtrl.SetFocus();
				return;
			}
		}
		if (m_pEric)
			if (!m_pEric->IsKindOf(RUNTIME_CLASS(CEricFormularlogikUStVA)))
			{	// nur wenn n�tig, die Formularlogik auf UStVA wechseln
				delete m_pEric;
				m_pEric = NULL;
			}
		if (!m_pEric)
		{
			m_pEric = new CEricFormularlogikUStVA();
			m_pEric->Init(&m_FormularCtrl, &m_EinstellungCtrl, &m_DokumentCtrl);
		}
	}
}

void CElsterDlg::ERiC(BOOL bNurValidieren = FALSE)
{
	UpdateData();

	if (!bNurValidieren)
	{
		if (m_Datei.GetLength() >= 4 && m_Datei.Right(4) == _T(".pfx"))
		{
			CFileStatus FileStatus;
			BOOL bFileExists = CFile::GetStatus(m_Datei, FileStatus);
			if (!bFileExists)
			{
				AfxMessageBox(_T("Die .pfx-Zertifikatsdatei konnte nicht im angegebenen Pfad gefunden werden."));
				GetDlgItem(IDC_DATEI)->SetFocus();
				return;
			}

			if (m_Passwort.GetLength() == 0)
			{
				AfxMessageBox(_T("Bitte ein Passwort f�r die Signaturdatei angeben."));
				GetDlgItem(IDC_PASSWORT)->SetFocus();
				return;
			}
		}
	}

	CString Jahr;
	Jahr.Format(_T("%-0.0d"), (int)m_DokumentCtrl.GetJahr());
	int nBundesfinanzamtsnummer = m_FinanzamtCtrl.GetItemData(m_FinanzamtCtrl.GetCurSel());	// Index speichern, um dar�ber bei Verarbeitung den Pfad zu gewinnen
	if (nBundesfinanzamtsnummer <= 0 || nBundesfinanzamtsnummer >= 10000)
	{
		AfxMessageBox(_T("Bitte erst ein Finanzamt w�hlen."));
		m_FinanzamtCtrl.SetFocus();
		return;
	}
	int i;
	for (i = 0; ListOfFinanzaemter[i].iFA; i++)
		if (ListOfFinanzaemter[i].iFA == nBundesfinanzamtsnummer)
		{
			if (ListOfFinanzaemter[i].StDUEV != _T('J'))
			{
				if (AfxMessageBox(_T("Das gew�hlte Finanzamt unterst�tzt nach dem Stand dieses Plugins nicht die elektronische Daten�bertragung. Trotzdem versuchen?)"), MB_YESNO) == IDNO)
					return;
			}
			break;
		}
	CString Bundesfinanzamtsnummer;
	CString NormaleSteuernummer = m_EinstellungCtrl.HoleEinstellung(_T("fsteuernummer"));
	if (NormaleSteuernummer.GetLength() != 10 && NormaleSteuernummer.GetLength() != 11)
	{
		AfxMessageBox(_T("Die Steuernummer in den EC&T-Einstellungen ist ung�ltig. Es wird eine 10- bzw. 11-stellige Steuernummer ben�tigt."));
		return;
	}
	char Pruefziffer_vor_Steuernummerumstellung[1000];
	char Pruefziffer_nach_Steuernummerumstellung[1000];
	Bundesfinanzamtsnummer.Format(_T("%-04.4d0%s"), nBundesfinanzamtsnummer, NormaleSteuernummer.Right(8).GetBuffer(0));
	int nNullWennOk = Pruefe_Steuernummer(Bundesfinanzamtsnummer, Pruefziffer_vor_Steuernummerumstellung, Pruefziffer_nach_Steuernummerumstellung);
	if (nNullWennOk)
	{
		CString csMessage;
		csMessage = (CString)_T("Die Bundesfinanzamtsnummer ") + Bundesfinanzamtsnummer + _T(", die EC&T aus der 'Vorwahl' des ausgew�hlten Finanzamts (erste vier Stellen) und Ihrer pers�nlichen Steuernummer (davon die letzten acht Ziffern) generiert hat, ist gem�� der Pr�fziffer ung�ltig. Bitte �berpr�fen Sie, ob Ihre in den Einstellungen eingegebene Steuernummer mit der auf einem aktuellen offiziellen Schreiben des Finanzamts �bereinstimmt. �berpr�fen Sie ferner, ob das oben rechts ausgew�hlte Finanzamt das richtige ist.");
		if (m_EinstellungCtrl.HoleEinstellung(_T("fname")).Find(_T("Hamburg")) != -1)
			csMessage += _T("\n\nAchtung Hamburg: Die hamburger Finanz�mter wurden neu nummeriert. Die neue Steuernummer sollte den Steuerpflichtigen per Post zugegangen sein. Damit diese funktioniert, muss oben in der Liste auch passend das 'neue Finanzamt' ausgew�hlt sein. Man erkennt es an der neuen Finanzamtsnummer hinter dem Namen, die den ersten beiden Stellen der Steuernummer entspricht, z.B. 'Altona -41-'.");
		AfxMessageBox(csMessage);
		return;
	}
	CString EmpfaengerFinanzamt = Bundesfinanzamtsnummer.Left(4);

	CString MomentanerFormularAnzeigename;
	CTime Jetzt;
	CString Zeitraum;
	EricKontext(bNurValidieren, Jetzt, MomentanerFormularAnzeigename, Jahr, Zeitraum, &m_Liste);
	CString csErgebnis = m_pEric->Render(
		m_hWnd,
		m_FormularDateipfad,
		&m_Liste,
		m_ListeInhalt,
		&m_ListeHinweise,
		&m_ListeFehler,
		m_Datei,
		m_Passwort,
		m_EmailAdresse,
		m_Telefon,
		m_KorrigierteAnmeldung,
		m_BelegeWerdenNachgereicht,
		m_VerrechnungDesErstattungsanspruchs,
		m_EinzugsermaechtigungWiderrufen,
		Jetzt,
		Jahr,
		Zeitraum,
		Bundesfinanzamtsnummer,
		EmpfaengerFinanzamt,
		MomentanerFormularAnzeigename,
		bNurValidieren);

	if (!csErgebnis.IsEmpty())
	{
		if (!m_ErgebnisCtrl.IsWindowVisible())
			OnBnClickedZeige();
		GetDlgItem(IDOK)->EnableWindow(FALSE);

		m_ErgebnisCtrl.SetWindowText(csErgebnis);
		UpdateData(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CONTROL & DIALOG VERHALTEN
//

// Kn�pfe (au�er Senden)

void CElsterDlg::OnBnClickedZeige()
{
	CString Caption;
	m_Zeige.GetWindowText(Caption.GetBuffer(1000), 1000);
	Caption.ReleaseBuffer();
	if (Caption == _T("&Zeige Log"))
	{
		m_Zeige.SetWindowText(_T("&Zeige Voranm."));
		m_Liste.ShowWindow(SW_HIDE);
		m_ErgebnisCtrl.ShowWindow(SW_SHOW);
		if (!m_ErgebnisCtrl.GetWindowTextLength())
		{
			// Log einblenden 
			CString AltesUebertragungslog = m_DokumentCtrl.HoleBenutzerdefWert(_T("Elster"), _T("�bertragungslog"));
			m_ErgebnisCtrl.SetWindowText(AltesUebertragungslog);	
		}
	}
	else
	{
		m_Zeige.SetWindowText(_T("&Zeige Log"));
		m_ErgebnisCtrl.ShowWindow(SW_HIDE);
		m_Liste.ShowWindow(SW_SHOW);
	}
}

void CElsterDlg::OnBnClickedInfo()
{
	extern CECTElsterApp theApp;
	CAboutDlg dlgAbout(this);
	dlgAbout.m_Version = theApp.GetVersion();
	dlgAbout.DoModal();
}

void CElsterDlg::OnBnClickedCancel()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_VIEW_JOURNAL_SWITCH, 0L);	
}

// Combo-Boxes

void CElsterDlg::OnCbnSelchangeVoranmeldungszeitraum()
{
	UpdateData();
	m_FormularDateipfad = m_FormularCtrl.HoleFormularpfad(m_VoranmeldungszeitraumCtrl.GetItemData(m_VoranmeldungszeitraumCtrl.GetCurSel()));
	if (m_pEric) m_pEric->UpdateListe(m_FormularDateipfad, m_ListeInhalt, &m_Liste);
	UpdateData(FALSE);
	SetTimer(2, 1, NULL);
}


void CElsterDlg::OnCbnSelchangeFinanzamt()
{
	int nBundesfinanzamtsnummer = m_FinanzamtCtrl.GetItemData(m_FinanzamtCtrl.GetCurSel());	// Index speichern, um dar�ber bei Verarbeitung den Pfad zu gewinnen
	CString Bundesfinanzamtsnummer;
	Bundesfinanzamtsnummer.Format(_T("%d"), nBundesfinanzamtsnummer);
	m_EinstellungCtrl.SpeichereEinstellung(_T("[Allgemein]bundesfinanzamtsnummer"), Bundesfinanzamtsnummer);
}

// Edit-controls

void CElsterDlg::OnEnKillfocusTelefon()
{
	UpdateData();
	m_EinstellungCtrl.SpeichereEinstellung(_T("[Allgemein]telefon"), m_Telefon);
}

void CElsterDlg::OnEnKillfocusEmailAdresse()
{
	UpdateData();
	m_EinstellungCtrl.SpeichereEinstellung(_T("[Allgemein]emailadresse"), m_EmailAdresse);
}

// Dialog Verschiedenes

void CElsterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_pEric)
	{
		UpdateData();
		m_FormularDateipfad = m_FormularCtrl.HoleFormularpfad(m_VoranmeldungszeitraumCtrl.GetItemData(m_VoranmeldungszeitraumCtrl.GetCurSel()));
		m_pEric->UpdateListe(m_FormularDateipfad, m_ListeInhalt, &m_Liste, TRUE);
		UpdateData(FALSE);
		SetTimer(2, 1, NULL);

		// "senden an Finanzamt" ausblenden, wenn Fenster zu schmal
		RECT r;
		m_Liste.GetWindowRect(&r);
		int ListeBreite = r.right - r.left - 21;
		if (ListeBreite < 770)
			GetDlgItem(IDC_FINANZAMT_STATIC)->ShowWindow(SW_HIDE);
		else
			GetDlgItem(IDC_FINANZAMT_STATIC)->ShowWindow(SW_SHOW);
	}
}

BOOL CElsterDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

void CElsterDlg::OnBnClickedFinanzamtNichtInDerListe()
{
	if ((DWORD)ShellExecute(m_hWnd, _T("open"), _T("https://www.bzst.de/DE/Service/Behoerdenwegweiser/Finanzamtsuche/GemFa/finanzamtsuche_node.html"), NULL, _T("."), SW_SHOWNORMAL) <= 32)
			AfxMessageBox(_T("Sorry, kann den Browser nicht �ffnen, um auf die Seite mit der Finanzamtssuche zu gehen. Bitte manuall bei https://www.bzst.de/DE/Service/Behoerdenwegweiser/Finanzamtsuche/GemFa/finanzamtsuche_node.html nach dem zust�ndigen Finanzamt schauen!"));
}

void CElsterDlg::OnBnClickedSignaturoptionen()
{
	CString csButtonText;
	GetDlgItemText(IDC_SIGNATUROPTIONEN, csButtonText);
	if (csButtonText == _T("&Signaturparameter ausblenden"))
	{
		CRect rectFrame, rectListe;
		GetDlgItem(IDC_SIGNATUROPTIONENRAHMEN)->GetWindowRect(rectFrame);
		ScreenToClient(&rectFrame);
		GetDlgItem(IDC_LISTE)->GetWindowRect(rectListe);
		ScreenToClient(&rectListe);
		rectListe.top = rectFrame.top;
		GetDlgItem(IDC_LISTE)->MoveWindow(rectListe);
		GetDlgItem(IDC_ERGEBNIS)->MoveWindow(rectListe);

		GetDlgItem(IDC_SIGNATUROPTIONENRAHMEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATEI_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATEI)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DURCHSUCHEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PASSWORT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PASSWORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SIGNATURANMERKUNG_STATIC)->ShowWindow(SW_HIDE);

		SetDlgItemText(IDC_SIGNATUROPTIONEN, _T("&Signaturparameter anzeigen"));
	}
	else
	{
		CRect rectFrame, rectListe;
		GetDlgItem(IDC_SIGNATUROPTIONENRAHMEN)->GetWindowRect(rectFrame);
		ScreenToClient(&rectFrame);
		GetDlgItem(IDC_LISTE)->GetWindowRect(rectListe);
		ScreenToClient(&rectListe);
		rectListe.top = rectFrame.bottom + 15;
		GetDlgItem(IDC_LISTE)->MoveWindow(rectListe);
		GetDlgItem(IDC_ERGEBNIS)->MoveWindow(rectListe);

		GetDlgItem(IDC_SIGNATUROPTIONENRAHMEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DATEI_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DATEI)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DURCHSUCHEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PASSWORT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PASSWORT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SIGNATURANMERKUNG_STATIC)->ShowWindow(SW_SHOW);

		SetDlgItemText(IDC_SIGNATUROPTIONEN, _T("&Signaturparameter ausblenden"));
	}

}

void CElsterDlg::OnBnClickedDurchsuchen()
{
	// init file dialog
	CFileDialog OpenDialog (TRUE,		                                            // OpenDialog
						   _T("pfx"),												// DefExt
						   _T(""),		                                            // Filename
						   OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_SHOWHELP | OFN_EXPLORER, // Flags 
						   _T("Elster Softwarezertifikat Dateien (*.pfx)|*.pfx|Signaturkartentreiberdateien (*.dll)|*.dll||"),	// Filter
						   (CWnd*) this);                                       // ParentWnd 

	// display dialog 
	if ( OpenDialog.DoModal() == IDOK )
	{
		m_Datei = OpenDialog.GetPathName();
		UpdateData(FALSE);
		GetDlgItem(IDC_DATEI)->SetFocus();
	}
}

void CElsterDlg::OnEnKillfocusDatei()
{
	UpdateData();
	m_EinstellungCtrl.SpeichereEinstellung(_T("[ElsterExport]signaturdatei"), m_Datei);
}

void CElsterDlg::OnBnClickedAktualisieren()
{
	UpdateData();
	m_FormularDateipfad = m_FormularCtrl.HoleFormularpfad(m_VoranmeldungszeitraumCtrl.GetItemData(m_VoranmeldungszeitraumCtrl.GetCurSel()));
	if (m_pEric) m_pEric->UpdateListe(m_FormularDateipfad, m_ListeInhalt, &m_Liste);
	UpdateData(FALSE);
	SetTimer(2, 1, NULL);

	// falls �bertragunslog angezeigt wird und die Liste �berlagert:
	m_Zeige.SetWindowText(_T("&Zeige Log"));
	m_ErgebnisCtrl.ShowWindow(SW_HIDE);
	m_Liste.ShowWindow(SW_SHOW);

	GetDlgItem(IDOK)->EnableWindow();	 // Senden-Button kann wieder gedr�ckt werden
	m_Liste.InvalidateRect(NULL, TRUE);	 // Liste neu aufbauen
}
