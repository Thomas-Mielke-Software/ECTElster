// ElsterDlg.cpp : implementation file
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

#include "stdafx.h"
#include "ECTElster.h"
#include "ElsterDlg.h"
#include "tmfanr.h"
#include "XMLite.h"  // https://www.codeproject.com/Articles/3426/XMLite-simple-XML-parser
#include "regexp.h"  // http://www.codeguru.com/Cpp/Cpp/string/regex/article.php/c2791

#include "EricApi.h"
#include "eric_fehlercodes.h"
#include "eric_types.h"

// auskommentieren, um echte Daten senden zu können:
#define TESTVERBINDUNG
//!!!!!!!!!!!! copyright date hochsetzen!!!


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
//	, m_Signaturoption(0)
{

}

CElsterDlg::~CElsterDlg()
{
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
	ON_NOTIFY(NM_CLICK, IDC_LISTE, &CElsterDlg::OnNMClickListe)
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

//	EnableDynamicLayout(TRUE);
/*	AddAnchor(IDC_FINANZAMT_STATIC, TOP_RIGHT);
	AddAnchor(IDC_FINANZAMT, TOP_RIGHT);
	AddAnchor(IDC_FINANZAMT_NICHT_IN_DER_LISTE, TOP_RIGHT);	

	AddAnchor(IDC_SIGNATUROPTIONENRAHMEN, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_DATEI_STATIC, TOP_LEFT, TOP_LEFT);
	AddAnchor(IDC_DATEI, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_DURCHSUCHEN, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_PASSWORT_STATIC, TOP_LEFT, TOP_LEFT);
	AddAnchor(IDC_PASSWORT, TOP_LEFT, TOP_LEFT);
	AddAnchor(IDC_PASSWORT_STATUS, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_SIGNATURANMERKUNG_STATIC, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_LISTE, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_ERGEBNIS, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_AKTUALISIEREN, BOTTOM_RIGHT);

	AddAnchor(IDC_KORRIGIERTE_ANMELDUNG, BOTTOM_LEFT); 
	AddAnchor(IDC_BELEGE_WERDEN_NACHGEREICHT, BOTTOM_LEFT); 
	AddAnchor(IDC_VERRECHNUNG_DES_ERSTATTUNGSANSPRUCHS, BOTTOM_LEFT); 
	AddAnchor(IDC_EINZUGSERMAECHTIGUNG_WIDERRUFEN, BOTTOM_LEFT); 
	AddAnchor(IDC_EMAIL_ADRESSE_STATIC, BOTTOM_CENTER); 
	AddAnchor(IDC_EMAIL_ADRESSE, BOTTOM_CENTER); 
	AddAnchor(IDC_TELEFON_STATIC, BOTTOM_CENTER); 
	AddAnchor(IDC_TELEFON, BOTTOM_CENTER); 
	AddAnchor(IDC_SIGNATUROPTIONEN, BOTTOM_RIGHT);
	AddAnchor(IDC_INFO, BOTTOM_RIGHT);
	AddAnchor(IDC_ZEIGE, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);	
*/	
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
		m_FinanzamtCtrl.SetItemData(m_FinanzamtCtrl.GetCount()-1, ListOfFinanzaemter[i].iFA);	// Index speichern, um darüber bei Verarbeitung den Pfad zu gewinnen
	}
	int nBundesfinanzamtsnummer = atoi(m_EinstellungCtrl.HoleEinstellung(_T("[Allgemein]bundesfinanzamtsnummer")));
	if (!nBundesfinanzamtsnummer)
		i = m_FinanzamtCtrl.FindString(0, m_EinstellungCtrl.HoleEinstellung(_T("fname")));	// wenn noch nicht gewählt, versuchen obs mit der Finanzamtsadresse aus den Einstellungen klappt
	else
		for(i = 0; ListOfFinanzaemter[i].iFA; i++)
			if (ListOfFinanzaemter[i].iFA == nBundesfinanzamtsnummer)
				break;
	if (i >= 0) m_FinanzamtCtrl.SetCurSel(m_FinanzamtCtrl.FindString(0, ListOfFinanzaemter[i].szFA));

	// damit das Formular weiß, welche Daten es einsetzen soll
	m_FormularCtrl.SetzeDokumentID(m_dokID);
	m_DokumentCtrl.SetID(m_dokID);	// und das Dokument-Objekt sowieso...

	// Formular/Voranm.zeitr. Combobox aufbauen
	m_VoranmeldungszeitraumCtrl.ResetContent();
	int n = m_FormularCtrl.HoleFormularanzahl();
	for (i = 0; i < n; i++)
	{
		CString Formularname = m_FormularCtrl.HoleFormularnamen(i, _T("Umsatzsteuer-Voranmeldung"));
		if (Formularname.GetLength() && m_FormularCtrl.HoleVoranmeldungszeitraum() != 0 && (Formularname.GetLength() < 21 || Formularname.Left(21) != "Umsatzsteuererklärung"))
		{
			m_VoranmeldungszeitraumCtrl.AddString(Formularname);
			m_VoranmeldungszeitraumCtrl.SetItemData(m_VoranmeldungszeitraumCtrl.GetCount()-1, i);	// Index speichern, um darüber bei Verarbeitung den Pfad zu gewinnen
		}
	}
	

	// Vorauswahl der Formulars nach heutigem Datum -- unter Einbeziehung der Dauerfristverlängerung
	int jj = m_DokumentCtrl.GetJahr();
	int nDauerfristverlaengerungTage = 0;
	CString Key;
	Key.Format(_T("Sondervorauszahlung%-04.4d"), jj);
	if (atoi(m_DokumentCtrl.HoleBenutzerdefWert(_T("Dauerfristverlängerung"), Key)))
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
			case 3:  Zeitraum = _T("März"); break;
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
		UpdateData(FALSE);
		UpdateListe();
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

// Item-Callback für CQuickList
LRESULT CElsterDlg::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
    ASSERT((HWND)wParam == m_Liste.GetSafeHwnd());

    CQuickList::CListItemData* data = (CQuickList::CListItemData*)lParam;

    // item & subitem 
    int item = data->GetItem();
    int subItem = data->GetSubItem();
	if (item > sizeof(m_ListeInhalt)/sizeof(m_ListeInhalt[0])) return 0;
	if (subItem > sizeof(m_ListeInhalt[0])/sizeof(m_ListeInhalt[0][0])) return 0;

	data->m_text = m_ListeInhalt[item][subItem];	// Zellentext anzeigen
	data->m_tooltip = m_ListeInhalt[item][0];		// Tooltip funktioniert im ActiveX leider nicht...
	if (subItem) // 2. oder höhere Spalte? ggf. farbig markieren
	{	
		int subItemDesFeldkennzeichens = subItem & 0x1 ? subItem : subItem - 1; // entweder 2. oder 4. Spalte ermitteln, wo das jeweils zugehörige Kennzeichen drin steht
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

	if (nIDEvent == 2)	// ERiC-Validierung mit Zeitverzögerung
	{
		KillTimer(2);
		ERiC(TRUE);
		m_Liste.InvalidateRect(NULL, FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	LISTE AUFBAUEN
//
void CElsterDlg::UpdateListe(BOOL bNurSpaltenbreitenAnpassen)
{
	// Liste mit Feldern initialisieren
	if (m_Liste && m_FormularCtrl/* && m_Liste.GetItemCount()*/)
	{
		UpdateData();

		// ggf. alte Liste löschen
		while ( m_Liste.DeleteColumn ( 0 ) );

		// wieviel Platz haben wir?
		RECT r;
		m_Liste.GetWindowRect(&r);
		int ListeBreite = r.right - r.left - 21;

		if (ListeBreite < 770) 
			GetDlgItem(IDC_FINANZAMT_STATIC)->ShowWindow(SW_HIDE);
		else
			GetDlgItem(IDC_FINANZAMT_STATIC)->ShowWindow(SW_SHOW);

		// Spaltenbreiten bestimmen
		static _TCHAR *Spaltentitel[] = { _T("Beschreibung"), _T("Feld-Nr."), _T("Bem.Grundl."), _T("Feld-Nr."), _T("Steuer") };
		static int AnzahlSpalten = sizeof(Spaltentitel) / sizeof(Spaltentitel[0]);
		int Spaltenbreite[sizeof(Spaltentitel) / sizeof(Spaltentitel[0])];
		int Spaltenzaehler;
		for (Spaltenzaehler = 0; Spaltenzaehler < AnzahlSpalten; Spaltenzaehler++)
			Spaltenbreite[Spaltenzaehler] = m_Liste.GetStringWidth(Spaltentitel[Spaltenzaehler]) * 4 / 3;
		Spaltenbreite[2] = Spaltenbreite[4] = max(Spaltenbreite[2], Spaltenbreite[4]); // Spalte 2 und 4 sollen gleich breit sein
		Spaltenbreite[0] = ListeBreite - Spaltenbreite[1] - Spaltenbreite[2] - Spaltenbreite[3] - Spaltenbreite[4];	// Beschreibungstext bekommt war übrig ist...
		if (Spaltenbreite[0] < Spaltenbreite[1] + Spaltenbreite[2])	// ... es sei denn, es ist zu wenig
			Spaltenbreite[0] = Spaltenbreite[1] + Spaltenbreite[2];

		// Listenheader aufbauen
		m_Liste.SetExtendedStyle ( m_Liste.GetExtendedStyle() | LVS_EX_FULLROWSELECT );
		for (Spaltenzaehler = 0; Spaltenzaehler < AnzahlSpalten; Spaltenzaehler++)
			m_Liste.InsertColumn(Spaltenzaehler, Spaltentitel[Spaltenzaehler], LVCFMT_LEFT, Spaltenbreite[Spaltenzaehler], -1);

		if (bNurSpaltenbreitenAnpassen) return;	// --> hier schon raus, wenn sich der Inhalt nicht geändert hat
		
		// Formular-Dateipfad holen
		CString FormularDateipfad;
		FormularDateipfad = m_FormularCtrl.HoleFormularpfad(m_VoranmeldungszeitraumCtrl.GetItemData(m_VoranmeldungszeitraumCtrl.GetCurSel()));
		if (!FormularDateipfad.GetLength())
		{
			AfxMessageBox(_T("Ich kann den Dateipfad des Formulars nicht ermitteln. Bitte überprüfen, ob die entsprechenden UStVA-Formulare für das aktuelle Jahr installiert sind. Die sind üblicherweise in der aktuellen Version des EC&T-Hauptprogramms enthalten oder sind zum Entpacken ins Programmverzeichnis in Form eines Zip-Archivs auf www.easyct.de unter Downloads -> Formular-Archiv verfügbar. Die Formulare für das neue Jahr werden üblicher Weise im Laufe des Januars über ein Hauptprogramm-Update verfügbar."));
			return;
		}

		// Formular vorbereiten
		m_FormularCtrl.WaehleFormular(FormularDateipfad);

		// Formularlayout in Liste übertragen
		// Spalten: 1. EC&T FeldID Bemessungsgrundlage, 2. EC&T FeldID Steuer, 
		//          3. tatsächliches KZ Bemessungsgrundlage, 4. tatsächliches KZ Steuer
		static int Formularlayout[][4] = { 
			41, 0, 0, 0,
			44, 0, 0, 0,
			49, 0, 0, 0,
			43, 0, 0, 0,
			48, 0, 0, 0,
			0, 0, 0, 0,
			81, 1081, 0, 0,
			86, 1086, 0, 0,
			35, 36, 0, 0,
			0, 0, 0, 0,
			77, 0, 0, 0,
			76, 80, 0, 0,
			0, 0, 0, 0,
			91, 0, 0, 0,
			89, 1189, 0, 0,
			93, 1193, 0, 0,
			95, 98, 0, 0,
			94, 96, 0, 0,
			0, 0, 0, 0,
			42, 0, 0, 0,
			68, 0, 0, 0,	// veraltet
			60, 0, 0, 0,
			21, 0, 0, 0,
			45, 0, 0, 0,
			0, 0, 0, 0,
			46, 47, 0, 0,			
			52, 53, 0, 0,	// veraltet
			73, 74, 0, 0,
			78, 79, 0, 0,	// veraltet
			84, 85, 0, 0,
			0, 0, 0, 0,
			-IDS_SEPARATOR, 0, 0, 0,
			-IDS_UMSATZSTEUER, 2003, 0, 0,
			0, 0, 0, 0,
			0, 1466, 0, 66,
			0, 61, 0, 0,
			0, 62, 0, 0,
			0, 67, 0, 0,
			0, 63, 0, 0,
			0, 64, 0, 0,
			0, 59, 0, 0,
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
		for (Zeile = 0; Zeile < sizeof(m_ListeInhalt)/sizeof(m_ListeInhalt[0]); Zeile++)	// Listenspeicher initialisieren
			for (Spalte = 0; Spalte < sizeof(m_ListeInhalt[0])/sizeof(m_ListeInhalt[0][0]); Spalte++)
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
					CString Feldwert2 = m_FormularCtrl.HoleFeldwertUeberID(LayoutWert2);
					if (Formularlayout[Zeile][3]) LayoutWert2 = Formularlayout[Zeile][3]; // tatsächliches KZ, nicht EC&T FeldID anzeigen
					if (LayoutWert2 < 100 )
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
				CString Feldbeschreibung = m_FormularCtrl.HoleFeldbeschreibungUeberID(max(LayoutWert, LayoutWert2));
				m_ListeInhalt[Zeile][0] = Feldbeschreibung;						
				//m_Liste.InsertItem (Zeile, Feldbeschreibung);		

				if (LayoutWert > 0)
				{
					CString Feldwert = m_FormularCtrl.HoleFeldwertUeberID(LayoutWert);
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
					CString Feldwert2 = m_FormularCtrl.HoleFeldwertUeberID(LayoutWert2);
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
		int nZeitraum = m_FormularCtrl.HoleVoranmeldungszeitraum();
		if (nZeitraum > 12) nZeitraum += 28;	// 1-12 Monat; 1. Quartal == 41, 4. Q. == 44
		CString Zeitraum; 
		Zeitraum.Format(_T("%d"), nZeitraum);
		CString Jahr;
		Jahr.Format(_T("%-0.0d"), (int)m_DokumentCtrl.GetJahr());
		CString FeldWert83VonLetzterUebertragung = m_DokumentCtrl.HoleBenutzerdefWert(_T("Elster"), _T("UST-Zahlbetrag-") + Jahr + _T("-") + Zeitraum);
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
			UpdateData(FALSE);
		}
		else
		{
			m_KorrigierteAnmeldung = FALSE;
			UpdateData(FALSE);
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

		m_Liste.SetItemCount(Zeile);
		m_Liste.RedrawItems(0, Zeile-1);
		m_Liste.InvalidateRect(NULL, FALSE);
		SetTimer(2, 1, NULL);
	}
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
// benutzt die ERiC-Bibliothek um entweder die Hinweis- bzw. Fehlerliste zu befüllen oder aber Daten an Elster zu versenden
//
void CElsterDlg::ERiC(BOOL bNurValidieren = FALSE)
{
	extern CECTElsterApp theApp;

	UpdateData();

	if (!bNurValidieren)
	{
		if (m_Datei.GetLength() >= 4 && m_Datei.Right(4) == _T(".pfx"))
		{
			CFileStatus FileStatus;
			BOOL bFileExists = CFile::GetStatus(m_Datei,FileStatus);
			if (!bFileExists)
			{
				AfxMessageBox(_T("Die .pfx-Zertifikatsdatei konnte nicht im angegebenen Pfad gefunden werden."));
				GetDlgItem(IDC_DATEI)->SetFocus();
				return;
			}

			if (m_Passwort.GetLength() == 0)
			{
				AfxMessageBox(_T("Bitte ein Passwort für die Signaturdatei angeben."));
				GetDlgItem(IDC_PASSWORT)->SetFocus();
				return;
			}
		}
	}

	int nZeitraum = m_FormularCtrl.HoleVoranmeldungszeitraum();
	if (nZeitraum > 12) nZeitraum += 28;	// 1-12 Monat; 1. Quartal == 41, 4. Q. == 44
	if (nZeitraum > 44 || nZeitraum < 1) nZeitraum = 1;	// Flickschusterei
	CString Zeitraum; 
	Zeitraum.Format(_T("%-02.2d"), nZeitraum);
	CString Jahr;
	Jahr.Format(_T("%-0.0d"), (int)m_DokumentCtrl.GetJahr());
	// timecheck
	CTime Jetzt = CTime::GetCurrentTime();
	if (Jetzt-CTime(m_DokumentCtrl.GetJahr(), nZeitraum <= 12 ? nZeitraum : (nZeitraum - 41) * 3 + 1, 1, 0, 0, 0)
		> CTimeSpan(180, 0, 0, 0))
	{
		int n;
		if (!bNurValidieren && (n=AfxMessageBox(_T("Die Frist für die Abgabe der UST-Voranmeldung für den ausgewählten Zeitraum ist schon seit einiger Zeit verstrichen. \
Vielleicht ist das falsche Buchungsjahr geöffnet oder der falsche Zeitraum ausgewählt? Soll die Voranmeldung trotzdem verschickt werden?"), MB_YESNO)) != IDYES)
		{
			m_VoranmeldungszeitraumCtrl.SetFocus();			
			return;
		}
	}
	//CString Steuernummer;
	int nBundesfinanzamtsnummer = m_FinanzamtCtrl.GetItemData(m_FinanzamtCtrl.GetCurSel());	// Index speichern, um darüber bei Verarbeitung den Pfad zu gewinnen
	if (nBundesfinanzamtsnummer<=0 || nBundesfinanzamtsnummer >= 10000)
	{
		AfxMessageBox(_T("Bitte erst ein Finanzamt wählen."));
		m_FinanzamtCtrl.SetFocus();
		return;
	}
	int i;
	for(i = 0; ListOfFinanzaemter[i].iFA; i++)
		if (ListOfFinanzaemter[i].iFA == nBundesfinanzamtsnummer)
		{
			if (ListOfFinanzaemter[i].StDUEV != _T('J'))
			{
				if (AfxMessageBox(_T("Das gewählte Finanzamt unterstützt nach dem Stand dieses Plugins nicht die elektronische Datenübertragung. Trotzdem versuchen?)"), MB_YESNO) == IDNO)
					return;
			}
			break;
		}
	CString Bundesfinanzamtsnummer;
	CString NormaleSteuernummer = m_EinstellungCtrl.HoleEinstellung(_T("fsteuernummer"));
	if (NormaleSteuernummer.GetLength() != 10 && NormaleSteuernummer.GetLength() != 11)
	{
		AfxMessageBox(_T("Die Steuernummer in den EC&&T-Einstellungen ist ungültig. Es wird eine 10- bzw. 11-stellige Steuernummer benötigt."));
		return;
	}	
	char Pruefziffer_vor_Steuernummerumstellung[1000];
	char Pruefziffer_nach_Steuernummerumstellung[1000];
	Bundesfinanzamtsnummer.Format(_T("%-04.4d0%s"), nBundesfinanzamtsnummer, NormaleSteuernummer.Right(8).GetBuffer(0));
	int nNullWennOk = Pruefe_Steuernummer(Bundesfinanzamtsnummer, Pruefziffer_vor_Steuernummerumstellung, Pruefziffer_nach_Steuernummerumstellung);
	if (nNullWennOk)
	{
		CString csMessage;
		csMessage = (CString)_T("Die Bundesfinanzamtsnummer ") + Bundesfinanzamtsnummer + _T(", die EC&T aus der 'Vorwahl' des ausgewählten Finanzamts (erste vier Stellen) und Ihrer persönlichen Steuernummer (davon die letzten acht Ziffern) generiert hat, ist gemäß der Prüfziffer ungültig. Bitte überprüfen Sie, ob Ihre in den Einstellungen eingegebene Steuernummer mit der auf einem aktuellen offiziellen Schreiben des Finanzamts übereinstimmt. Überprüfen Sie ferner, ob das oben rechts ausgewählte Finanzamt das richtige ist.");
		if (m_EinstellungCtrl.HoleEinstellung(_T("fname")).Find(_T("Hamburg")) != -1)
			csMessage += _T("\n\nAchtung Hamburg: Die hamburger Finanzämter wurden neu nummeriert. Die neue Steuernummer sollte den Steuerpflichtigen per Post zugegangen sein. Damit diese funktioniert, muss oben in der Liste auch passend das 'neue Finanzamt' ausgewählt sein. Man erkennt es an der neuen Finanzamtsnummer hinter dem Namen, die den ersten beiden Stellen der Steuernummer entspricht, z.B. 'Altona -41-'.");
		AfxMessageBox(csMessage);
		return;
	}
	CString EmpfaengerFinanzamt = Bundesfinanzamtsnummer.Left(4);

#ifdef TESTVERBINDUNG
	CString HerstellerID = _T("74931");
	CString Testmerker = _T("700000001");
#else
	CString HerstellerID = _T("21734");
	CString Testmerker = _T("");
#endif

	CString DatenTeil = 
_T("<DatenTeil> \
<Nutzdatenblock> \
<NutzdatenHeader version=\"11\"> \
<NutzdatenTicket>") + Jetzt.Format(_T("%Y%m%d%H%M%S")) + _T("</NutzdatenTicket> \
<Empfaenger id=\"F\">") + XMLEscape(EmpfaengerFinanzamt) + _T("</Empfaenger> \
<Hersteller> \
<ProduktName>EasyCash&amp;Tax Elster Plugin</ProduktName> \
<ProduktVersion>") + XMLEscape(theApp.GetVersion()) + _T("</ProduktVersion> \
</Hersteller> \
<DatenLieferant>") + XMLEscape(m_EinstellungCtrl.HoleEinstellung(_T("vorname")) + _T(" ") + m_EinstellungCtrl.HoleEinstellung(_T("name")) + _T("; ")
+ m_EmailAdresse) + "</DatenLieferant> \
</NutzdatenHeader> \
<Nutzdaten> \
<Anmeldungssteuern art=\"UStVA\" version=\"" + Jahr + "01\"> \
<DatenLieferant> \
<Name>" + XMLEscape(m_EinstellungCtrl.HoleEinstellung(_T("vorname")) + " " + m_EinstellungCtrl.HoleEinstellung(_T("name"))) + _T("</Name> \
<Strasse>") + XMLEscape(m_EinstellungCtrl.HoleEinstellung(_T("strasse"))) + _T("</Strasse> \
<PLZ>") + XMLEscape(m_EinstellungCtrl.HoleEinstellung(_T("plz"))) + _T("</PLZ> \
<Ort>") + XMLEscape(m_EinstellungCtrl.HoleEinstellung(_T("ort"))) + _T("</Ort> \
") + (CString)(m_Telefon.IsEmpty() ? _T("") : _T("<Telefon>") + XMLEscape(m_Telefon) + _T("</Telefon>")) + _T(" \
") + (CString)(m_EmailAdresse.IsEmpty() ? _T("") : _T("<Email>") + XMLEscape(m_EmailAdresse) + _T("</Email>")) + _T(" \
</DatenLieferant> \
<Erstellungsdatum>") + Jetzt.Format(_T("%Y%m%d")) + _T("</Erstellungsdatum> \
<Steuerfall> \
<Umsatzsteuervoranmeldung> \
<Jahr>") + Jahr + _T("</Jahr> \
<Zeitraum>") + Zeitraum + _T("</Zeitraum> \
<Steuernummer>") + XMLEscape(Bundesfinanzamtsnummer) + _T("</Steuernummer> \
<Kz09>") + XMLEscape(HerstellerID) + _T("</Kz09> \
") + (CString)(m_KorrigierteAnmeldung ? _T("<Kz10>1</Kz10>") : _T("")) + _T(" \
") + (CString)(m_BelegeWerdenNachgereicht ? _T("<Kz22>1</Kz22>") : _T("")) + _T(" \
") + (CString)(m_EinzugsermaechtigungWiderrufen ? _T("<Kz26>1</Kz26>") : _T("")) + _T(" \
") + (CString)(m_VerrechnungDesErstattungsanspruchs ? _T("<Kz29>1</Kz29>") : _T(""));

	// weitere Kennziffern aus der Liste lesen
	int Zeile;
	CString FeldWert83Merken;
	for (Zeile = 0; Zeile < m_Liste.GetItemCount(); Zeile++)
	{
		int Spalte;
		for (Spalte = 1; Spalte < 4; Spalte+=2)
		{
			CString FeldID = m_Liste.GetItemText(Zeile, Spalte);
			CString FeldWert = m_Liste.GetItemText(Zeile, Spalte+1);
#ifdef TESTVERBINDUNG
			if (Spalte == 3 && FeldWert == _T("TEST!"))
				FeldWert = _T("");
#endif
			if (FeldID.GetLength() && FeldWert.GetLength())
			{
				CString XMLKnoten;
				if (atoi(FeldID) == 83) FeldWert83Merken = FeldWert;
				FeldWert.Replace(_T(','), _T('.'));
				XMLKnoten.Format(_T("<Kz%d>%s</Kz%d> "), atoi(FeldID), XMLEscape(FeldWert.GetBuffer(0)), atoi(FeldID));
				DatenTeil += XMLKnoten;
			}
		}
	}

	DatenTeil += _T("</Umsatzsteuervoranmeldung> \
</Steuerfall> \
</Anmeldungssteuern> \
</Nutzdaten> \
</Nutzdatenblock> \
</DatenTeil>");	


    // Ein typedef und ein Funktionszeiger fuer notwendige ERiC API-Funktion.

    typedef int (STDCALL *EricInitialisiereFun)(const char *pluginPfad, const char *logPfad);
    EricInitialisiereFun EricInitialisierePtr;
    
    typedef int (STDCALL *EricBeendeFun)();
    EricBeendeFun EricBeendePtr;

    typedef int (STDCALL *EricBearbeiteVorgangFun)(
        const char* datenpuffer,
        const char* datenartVersion,
        uint32_t bearbeitungsFlags,
        const eric_druck_parameter_t *druckParameter,
        const eric_verschluesselungs_parameter_t *cryptoParameter,
        EricTransferHandle *transferHandle,
        EricRueckgabepufferHandle rueckgabeXmlPuffer,
        EricRueckgabepufferHandle serverantwortXmlPuffer);
    EricBearbeiteVorgangFun EricBearbeiteVorgangPtr;

    typedef int (STDCALL *EricGetHandleToCertificateFun)(EricZertifikatHandle *hToken,
        uint32_t *iInfoPinSupport,
        const char *pathToKeystore);
    EricGetHandleToCertificateFun EricGetHandleToCertificatePtr;

    typedef int (STDCALL *EricCloseHandleToCertificateFun)(EricZertifikatHandle hToken);
    EricCloseHandleToCertificateFun EricCloseHandleToCertificatePtr;

    typedef int (STDCALL *EricEinstellungAlleZuruecksetzenFun)();
    EricEinstellungAlleZuruecksetzenFun EricEinstellungAlleZuruecksetzenPtr;

    typedef int (STDCALL *EricEinstellungSetzenFun)(const char* name, const char* wert);
    EricEinstellungSetzenFun EricEinstellungSetzenPtr;

    typedef int (STDCALL *EricHoleFehlerTextFun)(
        int fehlerkode, EricRueckgabepufferHandle rueckgabePuffer);
    EricHoleFehlerTextFun EricHoleFehlerTextPtr;

    typedef int (STDCALL *EricPruefeSteuernummerFun)(
        const char *Steuernummer);
    EricPruefeSteuernummerFun EricPruefeSteuernummerPtr;

    typedef int (STDCALL *EricSystemCheckFun)();
    EricSystemCheckFun EricSystemCheckPtr;

//    typedef int (STDCALL *EricRegistriereGlobalenFortschrittCallbackFun)(
//        EricFortschrittCallback func,
//        void *userData);
//    EricRegistriereGlobalenFortschrittCallbackFun EricRegistriereGlobalenFortschrittCallbackPtr;

//    typedef int (STDCALL *EricRegistriereFortschrittCallbackFun)(
//        EricFortschrittCallback func,
//        void *userData);
//    EricRegistriereFortschrittCallbackFun EricRegistriereFortschrittCallbackPtr;

    typedef EricRueckgabepufferHandle (STDCALL *EricRueckgabepufferErzeugenFun) (void);
    EricRueckgabepufferErzeugenFun EricRueckgabepufferErzeugenPtr;

    typedef const char* (STDCALL *EricRueckgabepufferInhaltFun) (EricRueckgabepufferHandle handle);
    EricRueckgabepufferInhaltFun EricRueckgabepufferInhaltPtr;

    typedef uint32_t (STDCALL *EricRueckgabepufferLaengeFun) (EricRueckgabepufferHandle handle);
    EricRueckgabepufferLaengeFun EricRueckgabepufferLaengePtr;

    typedef int (STDCALL *EricRueckgabepufferFreigebenFun) (EricRueckgabepufferHandle handle);
    EricRueckgabepufferFreigebenFun EricRueckgabepufferFreigebenPtr;

    typedef int (STDCALL *EricEntladePluginsFun)();
    EricEntladePluginsFun EricEntladePluginsPtr;

    typedef int(STDCALL *EricCreateTHFun) (
        const char* xml,
        const char* verfahren,
        const char* datenart,
        const char* vorgang,
        const char* testmerker,
        const char* herstellerId,
        const char* datenLieferant,
        const char* versionClient,
        const char* publicKey,
        EricRueckgabepufferHandle rueckgabeXmlPuffer);
    EricCreateTHFun EricCreateTHPtr;

    typedef int(STDCALL *EricHoleZertifikatEigenschaftenFun) (
        EricZertifikatHandle hToken,
        const char* pin,
        EricRueckgabepufferHandle rueckgabeXmlPuffer);
    EricHoleZertifikatEigenschaftenFun EricHoleZertifikatEigenschaftenPtr;



	char temp_path[MAX_PATH+1];
	if (!GetTempPath(sizeof(temp_path), temp_path)) { AfxMessageBox(_T("Konnte keinen Pfad zum Temp-Verzeichnis bekommen. :(")); return; }

	HINSTANCE libEricApi = ::LoadLibrary(_T("ericapi.dll"));

    EricInitialisierePtr                          = (EricInitialisiereFun)GetProcAddress(libEricApi, _T("EricInitialisiere"));
    EricBeendePtr                                 = (EricBeendeFun)GetProcAddress(libEricApi, _T("EricBeende"));
    EricBearbeiteVorgangPtr                       = (EricBearbeiteVorgangFun)GetProcAddress(libEricApi, _T("EricBearbeiteVorgang"));
    EricGetHandleToCertificatePtr                 = (EricGetHandleToCertificateFun)GetProcAddress(libEricApi, _T("EricGetHandleToCertificate"));
    EricCloseHandleToCertificatePtr               = (EricCloseHandleToCertificateFun)GetProcAddress(libEricApi, _T("EricCloseHandleToCertificate"));
    EricHoleFehlerTextPtr                         = (EricHoleFehlerTextFun)GetProcAddress(libEricApi, _T("EricHoleFehlerText"));
    EricPruefeSteuernummerPtr                     = (EricPruefeSteuernummerFun)GetProcAddress(libEricApi, _T("EricPruefeSteuernummer"));
    EricSystemCheckPtr                            = (EricSystemCheckFun)GetProcAddress(libEricApi, _T("EricSystemCheck"));
    EricEinstellungSetzenPtr                      = (EricEinstellungSetzenFun)GetProcAddress(libEricApi, _T("EricEinstellungSetzen"));
    EricEinstellungAlleZuruecksetzenPtr           = (EricEinstellungAlleZuruecksetzenFun)GetProcAddress(libEricApi, _T("EricEinstellungAlleZuruecksetzen"));
    //EricRegistriereGlobalenFortschrittCallbackPtr = 
    //EricRegistriereFortschrittCallbackPtr         = 
    EricRueckgabepufferErzeugenPtr                = (EricRueckgabepufferErzeugenFun)GetProcAddress(libEricApi, _T("EricRueckgabepufferErzeugen"));
    EricRueckgabepufferInhaltPtr                  = (EricRueckgabepufferInhaltFun)GetProcAddress(libEricApi, _T("EricRueckgabepufferInhalt"));
    EricRueckgabepufferLaengePtr                  = (EricRueckgabepufferLaengeFun)GetProcAddress(libEricApi, _T("EricRueckgabepufferLaenge"));
    EricRueckgabepufferFreigebenPtr               = (EricRueckgabepufferFreigebenFun)GetProcAddress(libEricApi, _T("EricRueckgabepufferFreigeben"));
    EricEntladePluginsPtr                         = (EricEntladePluginsFun)GetProcAddress(libEricApi, _T("EricEntladePlugins"));
    EricCreateTHPtr                               = (EricCreateTHFun)GetProcAddress(libEricApi, _T("EricCreateTH"));
    EricHoleZertifikatEigenschaftenPtr            = (EricHoleZertifikatEigenschaftenFun)GetProcAddress(libEricApi, _T("EricHoleZertifikatEigenschaften"));

	AfxGetApp()->BeginWaitCursor();
	EricRueckgabepufferHandle transferheaderPufferHandle = NULL;
	EricRueckgabepufferHandle fehlerPufferHandle = NULL;
	EricRueckgabepufferHandle ergebnisPufferHandle = NULL;
	EricRueckgabepufferHandle serverantwortPufferHandle = NULL;
	EricRueckgabepufferHandle ticketPufferHandle = NULL;
	EricRueckgabepufferHandle threturncodePufferHandle = NULL;
	EricRueckgabepufferHandle therrormessagePufferHandle = NULL;
	EricRueckgabepufferHandle threturncodeerrormessagePufferHandle = NULL;
	EricZertifikatHandle zertifikat = NULL;

	//EricEinstellungSetzenPtr(_T("basis.log_dir"), temp_path);
	//EricEinstellungSetzenPtr(_T("basis.home_dir"), _T("C:\\Program Files (x86)\\EasyCash&Tax\\Plugins\\Elster-Export"));
	//EricEinstellungSetzenPtr(_T("basis.data_dir"), temp_path);

	CString PDFDatei;
	char pluginpfad[1000];
	char *cp;

	// Pluginpfad für EricInitialisiere herausfummeln
	DWORD dw = GetModuleFileName(theApp.m_hInstance, pluginpfad, sizeof(pluginpfad));
	if (!dw)
	{
		AfxMessageBox(_T("Konnte Moduldateinamen des Plugins nicht ermitteln."));
		goto PufferFreigeben; 
	}
	if (dw < sizeof(pluginpfad) && (cp = strrchr(pluginpfad, _T('\\'))))
		*cp = _T('\0');
	else	
	{
		AfxMessageBox(_T("Konnte Pfad nicht aus Moduldateinamen des Plugins ermitteln."));
		goto PufferFreigeben; 
	}
	EricInitialisierePtr(pluginpfad, temp_path);
	
    eric_druck_parameter_t druckEinstellungen;
    druckEinstellungen.vorschau   = 0;
    druckEinstellungen.ersteSeite = 0;
	PDFDatei = (CString)temp_path + Jetzt.Format(_T("Elster-Übertragungsbericht-%Y%m%d%H%M%S.pdf"));
    druckEinstellungen.pdfName    = PDFDatei;
    druckEinstellungen.fussText   = _T("EasyCash&Tax Elster-Export");
	druckEinstellungen.duplexDruck = 0;
	druckEinstellungen.version    = 2;

	int rc;
	eric_verschluesselungs_parameter_t verschluesselung;
	if (!bNurValidieren)
	{
		uint32_t pinErforderlich = 1;
		rc = EricGetHandleToCertificatePtr(&zertifikat, &pinErforderlich, (LPCTSTR)m_Datei);

		if (rc)
		{		
			EricHoleFehlerText(rc, fehlerPufferHandle);
			const char* klartextFehler = EricRueckgabepufferInhalt(fehlerPufferHandle); 
			if(!klartextFehler) klartextFehler = _T("");
			CString Fehlertext;
			Fehlertext.Format(_T("Das Vorbereiten des Zertifikats schlug fehl (ERiC-Fehler %d). %s"), (int)rc, klartextFehler);
			AfxMessageBox(Fehlertext);
			goto PufferFreigeben; 
		}

		if (m_Passwort.GetLength() > 20) m_Passwort = m_Passwort.Left(20);
		verschluesselung.pin              = m_Passwort;
		verschluesselung.zertifikatHandle = zertifikat;
		verschluesselung.abrufCode		  = NULL;
		verschluesselung.version		  = 2;
	}

    // Platz für den Meldungstext
    char msg[1000];
    int len = sizeof(msg);
    char *Transferticket = NULL;

	transferheaderPufferHandle = EricRueckgabepufferErzeugen();
	fehlerPufferHandle = EricRueckgabepufferErzeugen();

/*	rc = EricCheckXML(0, DatenTeil, fehlerPufferHandle);
	if (rc)
	{		
		EricHoleFehlerText(rc, fehlerPufferHandle);
		const char* klartextFehler = EricRueckgabepufferInhalt(fehlerPufferHandle); 
		if(!klartextFehler) klartextFehler = _T("");
		CString Fehlertext = (CString)_T("Das Vorbereiten des Datenpakets schlug fehl. ") + (CString)klartextFehler;		
		AfxMessageBox(Fehlertext);
		goto PufferFreigeben; 
	}
*/

    // Header erzeugen
	{
		CString Datenlieferant;
		Datenlieferant = m_EinstellungCtrl.HoleEinstellung(_T("vorname")) + _T(" ") + m_EinstellungCtrl.HoleEinstellung(_T("name"));
		rc = EricCreateTHPtr(DatenTeil, _T("ElsterAnmeldung"), _T("UStVA"), _T("send-Auth"), Testmerker, HerstellerID, Datenlieferant.GetBuffer(0), _T("2.3"), _T(""), transferheaderPufferHandle);
		if (rc)
		{		
//			EricHoleFehlerText(rc, fehlerPufferHandle);
			const char* klartextFehler = /*EricRueckgabepufferInhalt(fehlerPufferHandle); 
			if(!klartextFehler)*/ klartextFehler = _T("");
			CString Fehlertext;
			Fehlertext.Format(_T("Das Vorbereiten des Datenpakets schlug fehl (ERiC-Fehler %d). %s"), (int)rc, klartextFehler);
			AfxMessageBox(Fehlertext);
			goto PufferFreigeben; 
		}
	}

	ergebnisPufferHandle = EricRueckgabepufferErzeugen();
	serverantwortPufferHandle = EricRueckgabepufferErzeugen();

    // Daten verschicken
    rc = EricBearbeiteVorgangPtr(
        EricRueckgabepufferInhalt(transferheaderPufferHandle),
		(LPCTSTR)((CString)_T("UStVA_") + Jahr),
		bNurValidieren ? ERIC_VALIDIERE | ERIC_PRUEFE_HINWEISE : ERIC_VALIDIERE | ERIC_SENDE | ERIC_DRUCKE, 
		&druckEinstellungen, 
		bNurValidieren ? NULL : &verschluesselung, 
		NULL, 
		ergebnisPufferHandle, 
		serverantwortPufferHandle);

	// PIN Status anzeigen neben Passwort-Feld
	if (!bNurValidieren)
	{
		uint32_t pinStatus;
		int rc2 = EricGetPinStatus(zertifikat, &pinStatus, 0);

		if (rc2)
		{		
			EricHoleFehlerText(rc, fehlerPufferHandle);
			const char* klartextFehler = EricRueckgabepufferInhalt(fehlerPufferHandle); 
			if(!klartextFehler) klartextFehler = _T("");
			if (strcmp(klartextFehler, _T("Verarbeitung fehlerfrei")))
			{
				CString Fehlertext;
				Fehlertext.Format(_T("Die Abfrage des PIN-Status schlug fehl (ERiC-Fehler %d). %s"), (int)rc2, klartextFehler);
				AfxMessageBox(Fehlertext);
				goto PufferFreigeben; 
			}
		}

		switch (pinStatus)
		{
		case 1: SetDlgItemText(IDC_PASSWORT_STATUS, _T("PIN gesperrt!")); break;
		case 2: SetDlgItemText(IDC_PASSWORT_STATUS, _T("Die letzte PIN/Passwort-Eingabe war fehlerhaft.")); break;
		case 3: SetDlgItemText(IDC_PASSWORT_STATUS, _T("ACHTUNG: Sperrung der PIN beim nächsten fehlerhaften Versuch!")); break;
		default: SetDlgItemText(IDC_PASSWORT_STATUS, _T("PIN ok")); break;
		}
	}

    if (rc)
    {		
        EricHoleFehlerText(rc, fehlerPufferHandle);
		const char* klartextFehler = EricRueckgabepufferInhalt(fehlerPufferHandle); 
		if (!klartextFehler) klartextFehler = _T("");
		CString Fehlertext;
		Fehlertext.Format(_T("Das Versenden des Datenpakets schlug fehl (ERiC-Fehler %d). %s"), (int)rc, klartextFehler);

		if (Fehlertext.Find(_T("Fehler im Transferheader.")) >= 0)
		{
			Fehlertext += _T(" Bitte stellen Sie zunächst sicher, dass Sie ein gültiges Zertifikat (.pfx-Datei) verwenden, z.B. indem Sie sich bei www.elsteronline.de einloggen. Im Zweifelsfall probieren Sie sonst, die Voranmeldung manuell mit ElsterFormular (von www.elster.de) zu übertragen.");
			if (!bNurValidieren) AfxMessageBox(Fehlertext);
		}
		else if (Fehlertext.Find(_T("Datenartversion")) >= 0)
		{
			Fehlertext += _T(" Wahrscheinlich ist die Version dieses Plugins zu veraltet. Bitte schauen Sie nach Updates (oben im EC&&T-Menü, das rote Puzzlestück.");
			if (!bNurValidieren) AfxMessageBox(Fehlertext);
		}
		else if (rc == ERIC_GLOBAL_HINWEISE || rc == ERIC_GLOBAL_PRUEF_FEHLER || Fehlertext.Find(_T("Datensatz nicht plausibel.")) >= 0)
		{
			// mit Meldung anfangen, dass Fehler oder Hinweise von ERiC zurückgegeben wurden
			int Zeile = m_Liste.GetItemCount() + 2;  // unter letzte Zeile einfügen (mit einer Leerzeile Abstand)
			m_ListeInhalt[Zeile++][0] = klartextFehler;
			m_Liste.SetItemCount(Zeile);

			const char *ergebnisPuffer = EricRueckgabepufferInhalt(ergebnisPufferHandle);

			// Fehler und ggf. Hinweise aus XML-Antwort lesen
			XDoc xmlDoc;
			xmlDoc.Load(ergebnisPuffer);
			LPXNode root = xmlDoc.GetRoot();
			LPXNode vorgang = xmlDoc.Find(_T("EricBearbeiteVorgang"));
			if (vorgang)
			{
				int meldungIndex;
				for (meldungIndex = 0; meldungIndex < vorgang->GetChildCount(); meldungIndex++)
				{
					LPXNode meldung = vorgang->GetChild(meldungIndex);
					CString csMeldungTyp = meldung->name;	// 'Fehler' oder 'Hinweis'
					LPXNode feldidentifikator = meldung->Find(_T("Feldidentifikator"));
					LPXNode text = meldung->Find(_T("Text"));
					if (feldidentifikator && text)
					{
						CString csFeldidentifikator = feldidentifikator->GetText();
						CString csText = text->GetText();

						int nErsteZiffer;
						for (nErsteZiffer = csFeldidentifikator.GetLength() - 1; nErsteZiffer >= 0; nErsteZiffer--)
							if (!isdigit(csFeldidentifikator[nErsteZiffer]))
							{
								nErsteZiffer++;
								break;
							}
						csFeldidentifikator = csFeldidentifikator.Mid(nErsteZiffer);
						int nFeldidentifikator = atoi(csFeldidentifikator);
							
						if (nFeldidentifikator && csText != _T(""))
						{
							CString csTatsaechlicheFeldnummer;

							CStringArray csaErsetzteAusdruecke;
							int nAnzahlErsetzteAusdruecke = RegSearchReplace(csText, _T("(\\&apos;\\$/Elster\\[[0-9]+\\]/DatenTeil\\[[0-9]+\\]/Nutzdatenblock\\[[0-9]+\\]/Nutzdaten\\[[0-9]+\\]/Anmeldungssteuern\\[[0-9]+\\]/Steuerfall\\[[0-9]+\\]/Umsatzsteuervoranmeldung\\[[0-9]+\\]/Kz)([0-9]+)(\\[[0-9]+\\]\\$\\&apos;)"), _T("\\2"), csaErsetzteAusdruecke);
							int i;
							for (i = 0; i < csaErsetzteAusdruecke.GetSize() && i < nAnzahlErsetzteAusdruecke; i++) 
							{
								// zu CMap hinzufügen:
								int nTatsaechlicheFeldnummer = atoi(csaErsetzteAusdruecke[i]);
								if (csMeldungTyp == _T("Hinweis"))
									m_ListeHinweise.SetAt(nTatsaechlicheFeldnummer, csText);
								else
									m_ListeFehler.SetAt(nTatsaechlicheFeldnummer, csText);
							}

							/*
							while (nPosStartPlatzhalter = csText.Find(csKzPlatzhalterVorn, nPosStartPlatzhalter)) >= 0)
							{
								int nStartFeldnummerPos = nPosStartPlatzhalter + csKzPlatzhalterVorn.GetLength();
								int nEndeFeldnummerPos = nStartFeldnummerPos;
								while (isdigit(csText[nEndeFeldnummerPos]))
									nEndeFeldnummerPos++;
								csFeldidentifikator = csText.Mid(nEndeFeldnummerPos, csKzPlatzhalterHinten.GetLength());
								if (csFeldidentifikator == csKzPlatzhalterHinten)
								{
									csTatsaechlicheFeldnummer = csText.Mid(nStartFeldnummerPos, nEndeFeldnummerPos - nStartFeldnummerPos);
									csText.Replace(csKzPlatzhalterVorn + csTatsaechlicheFeldnummer + csKzPlatzhalterHinten, csTatsaechlicheFeldnummer);
								}
								else
									nPosStartPlatzhalter = nEndeFeldnummerPos + csKzPlatzhalterHinten.GetLength();
							}

							// zu CMap hinzufügen:
							int nTatsaechlicheFeldnummer = atoi(csTatsaechlicheFeldnummer);
							m_ListeHinweise.SetAt(nTatsaechlicheFeldnummer, csText);
							*/
						}

						// allgemeiner Fehler- oder Hinweistext ohne Zuordnung zu einem Feldkennzeichen
						// unten im ListCtrl anhängen
						if (bNurValidieren)
						{
							int Zeile = m_Liste.GetItemCount() + 1;  // unter letzte Zeile einfügen (mit einer Leerzeile Abstand)
							csMeldungTyp.ReleaseBuffer();
							if (csMeldungTyp == _T("FehlerRegelpruefung")) csMeldungTyp = _T("Fehler");
							m_ListeInhalt[Zeile++][0] = csMeldungTyp + _T(": ") + csText;
							m_Liste.SetItemCount(Zeile);
							m_Liste.EnsureVisible(Zeile - 1, FALSE);
						}
					}
				}
			}

			if (!bNurValidieren)
			{
				Fehlertext += _T(" Bitte überprüfen Sie die rot unterlegten Beträge bzw. Meldungen im Einzelnen noch einmal. Insbesondere bei Buchungen mit dem Leistungsempfänger als Steuerschuldner und innergem. Erwerben kann es zu Unstimmigkeiten kommen. Die Such-Funktion auf www.easyct.de kann hier hilfreich sein. Außerdem stellen Sie bitte sicher, dass in den Einstellungen unter pers. Angaben ein korrekter Name eingetragen ist in der Form Vorname - Leerzeichen - Nachname.\r\n\r\nDetails anzeigen?");
				if (AfxMessageBox(Fehlertext, MB_YESNO) == IDYES)
				{
					if (ergebnisPuffer)
					{						
						Fehlertext = (CString)ergebnisPuffer + _T("\r\n\r\nWenn es den Verdacht gibt, dass es sich um einen Fehler in der Software handelt, bitte den Datensatz speichern und an thomas@mielke.software senden.\r\n\r\nDatensatz jetzt speichern?");

						if (AfxMessageBox(Fehlertext, MB_YESNO) == IDYES)
						{
							CString DatensatzXMLDatei = m_EinstellungCtrl.HoleEinstellung(_T("[ElsterExport]ProtokollPfad"));
							if (DatensatzXMLDatei == _T("")) DatensatzXMLDatei = m_Datei;
							int nPos = DatensatzXMLDatei.ReverseFind(_T('\\'));
							if (nPos >= 0)
							{	// neuen Dateinamen an alten Pfad anhängen
								DatensatzXMLDatei = DatensatzXMLDatei.Left(nPos+1);
								DatensatzXMLDatei += _T("ElsterUStVADatensatz.xml");
							}
							else
								DatensatzXMLDatei = _T("C:\\ElsterUStVADatensatz.xml");

							CFileDialog SaveDialog (FALSE,													// Save Dialog
												   _T("xml"),												// DefExt
												   DatensatzXMLDatei,									    // Filename
												   OFN_ENABLESIZING | OFN_SHOWHELP | OFN_EXPLORER,			// Flags 
												   _T("Elster USTVa-Datensatz XML-Datei (*.xml)|*.xml||"),	// Filter
												   (CWnd*) this);											// ParentWnd 

							// display dialog 
							if ( SaveDialog.DoModal() == IDOK )
							{
								DatensatzXMLDatei = SaveDialog.GetPathName();	

								CFile f;
								f.Open(DatensatzXMLDatei, CFile::modeCreate|CFile::modeWrite);
								f.Write(ergebnisPuffer, strlen(ergebnisPuffer));
								f.Write(_T("\r\n\r\n"), 4);
								f.Write(DatenTeil, strlen(DatenTeil));
								f.Close();
							}
						}
					}
				}
			}
		}
		else
			AfxMessageBox(Fehlertext);

		goto PufferFreigeben; 
    }
	else if (!bNurValidieren)
	{	// erfolgreich übertragen
		if (!m_ErgebnisCtrl.IsWindowVisible())
			OnBnClickedZeige();
		GetDlgItem(IDOK)->EnableWindow(FALSE);

	/*	ticketPufferHandle = EricRueckgabepufferErzeugen();
		threturncodePufferHandle = EricRueckgabepufferErzeugen();
		therrormessagePufferHandle = EricRueckgabepufferErzeugen();
		threturncodeerrormessagePufferHandle = EricRueckgabepufferErzeugen();

		if (EricRueckgabepufferInhalt(serverantwortPufferHandle))
			EricGetErrormessagesFromXMLAnswer(EricRueckgabepufferInhalt(serverantwortPufferHandle), 
											  ticketPufferHandle, 
											  threturncodePufferHandle, therrormessagePufferHandle, 
											  threturncodeerrormessagePufferHandle);
	
		AfxMessageBox(_T("Transferticket: ") + (CString)EricRueckgabepufferInhalt(ticketPufferHandle) + _T(" ") + (CString)EricRueckgabepufferInhalt(threturncodePufferHandle) + _T(" ") + (CString)EricRueckgabepufferInhalt(therrormessagePufferHandle) + _T(" ") + (CString)EricRueckgabepufferInhalt(threturncodeerrormessagePufferHandle));
	*/
	}

	if (*EricRueckgabepufferInhalt(serverantwortPufferHandle))
	{
		CString Code = _T("0");
		CString Antwort = EricRueckgabepufferInhalt(serverantwortPufferHandle);
		CString Haupttext;
		CString LogZeile;
		CString TransferTicket = EricRueckgabepufferInhalt(ticketPufferHandle);
		CString Infotext;	

		// TransferHeader Block checken
		{
			int nPosAnfang; 
			int nPosTransferHeader = Antwort.Find(_T("<TransferHeader"), 0);
			if (nPosTransferHeader == -1)
				nPosAnfang = -1;
			else
				nPosAnfang = Antwort.Find(_T("<Code>"), nPosTransferHeader);
			int nPosEnde;
			if (nPosAnfang == -1)
				nPosEnde = -1;
			else
				nPosEnde = Antwort.Find(_T("</Code>"), nPosAnfang);
			if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
			{
				nPosAnfang += 6; 
				Code = Antwort.Mid(nPosAnfang, nPosEnde-nPosAnfang);
				if (atoi(Code))
				{	// Fehlercode ausgeben
					int nPosAnfang = Antwort.Find(_T("<Text>"), nPosEnde);
					int nPosEnde = Antwort.Find(_T("</Text>"), nPosAnfang);
					if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
					{
						nPosAnfang += 6; 
						Haupttext = _T("Fehler bei der Verarbeitung des Transfer-Headers auf dem Finanzamts-Computer: \r\n") + Antwort.Mid(nPosAnfang, nPosEnde-nPosAnfang) + _T("\r\n\r\n");
					}
				}

				// Infotexte anhängen
				{
					int nPosAnfang = 0;
					int nPosEnde = 0;
					while (nPosEnde >= 0 && nPosAnfang >= 0)
					{
						nPosAnfang = Antwort.Find(_T("<Info>"), nPosEnde);
						if (nPosAnfang >= 0)
						{
							if (Infotext.Left(5) != _T("Info:")) Infotext += _T("Info:\r\n");
							nPosEnde = Antwort.Find(_T("</Info>"), nPosAnfang);
							if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
							{
								nPosAnfang += 6; 
								Infotext += _T("\r\n") + Antwort.Mid(nPosAnfang, nPosEnde-nPosAnfang);
							}
						}
					}
				}
			}
		}

		// Nutzdatenblock checken, wenn TransferHeader ok war
		if (!atoi(Code))
		{
			int nPosAnfang; 
			int nPosNutzdatenblock = Antwort.Find(_T("<Nutzdatenblock>"), 0);
			if (nPosNutzdatenblock == -1)
				nPosAnfang = -1;
			else
				nPosAnfang = Antwort.Find(_T("<Code>"), nPosNutzdatenblock);
			int nPosEnde;
			if (nPosAnfang == -1)
				nPosEnde = -1;
			else
				nPosEnde = Antwort.Find(_T("</Code>"), nPosAnfang);
			if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
			{
				nPosAnfang += 6; 
				Code = Antwort.Mid(nPosAnfang, nPosEnde-nPosAnfang);
				if (atoi(Code))
				{		// Fehlercode ausgeben
					int nPosAnfang = Antwort.Find(_T("<Text>"), nPosEnde);
					int nPosEnde = Antwort.Find(_T("</Text>"), nPosAnfang);
					if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
					{
						nPosAnfang += 6; 
						Haupttext = _T("Fehler bei der Verarbeitung der Nutzdaten auf dem Finanzamts-Computer: \r\n") + Antwort.Mid(nPosAnfang, nPosEnde-nPosAnfang);
						// Interpretationen hinzufügen:
						if (-1 != Antwort.Find(_T("element &apos;Jahr&apos; is not valid"), 0)) Haupttext += _T("\r\nInterpretation des Elster-Export Plugins: \r\n\
Die Buchungsdatei enthält ein falsches Buchungsjahr. Bitte oben in der Werkzeugleiste auf \r\n\
den Knopf mit dem Maulschlüssel drücken oder unter Menü->Ansicht->Einstellungen->Allgemein \r\n\
auf der rechten Seite nach dem Wert 'Buchungsjahr' suchen und ggf korrigieren. \r\n\
Hinweis: EC&T benötigt für jedes Jahr eine eigene Buchungsdatei. Man kann die relevanten \r\n\
Daten (AfAs) über die Funktion Jahreswechsel (im Datei-Menü) in eine neue Buchungsdatei \r\n\
übertragen. Dabei wird auch die Jahreszahl hochgesetzt.");
						CString csParseElementStart = _T("element &apos;");
						CString csParseElementEnde  = _T("&apos;");
						CString csParseValueStart  = _T("Value &apos;");
						CString csParseValueEnd  = _T("&apos;");
						int nPosParseElementStart = Antwort.Find(csParseElementStart, 0);
						int nPosParseElementEnde = Antwort.Find(csParseElementEnde, nPosParseElementStart+csParseElementStart.GetLength());
						int nPosParseValueStart = Antwort.Find(csParseValueStart, 0);
						int nPosParseValueEnde  = Antwort.Find(csParseValueEnd, nPosParseValueStart+csParseValueStart.GetLength());							
						if (nPosParseElementStart >= 0) 
						{
							if (nPosParseElementEnde > nPosParseElementStart)
							{
								CString csElement = Antwort.Mid(nPosParseElementStart+csParseElementStart.GetLength(), nPosParseElementEnde-nPosParseElementStart-csParseElementStart.GetLength());
								CString csWert;
								if (nPosParseValueEnde > nPosParseValueStart)
								{
									csWert = Antwort.Mid(nPosParseValueStart+csParseValueStart.GetLength(), nPosParseValueEnde-nPosParseValueStart-csParseValueStart.GetLength());
								}
								Haupttext += _T("\r\nInterpretation des Elster-Export Plugins: \r\n");
								if (csWert.GetLength() > 0)
									Haupttext += _T("\
Der Wert des Datenelements '") + csElement + _T("' ist '") + csWert + _T("'.\r\n\
Das findet der Finanzamtsserver nicht korrekt. Bitte oben in der Werkzeugleiste auf den Knopf \r\n\
mit dem Maulschlüssel drücken oder unter Menü->Ansicht->Einstellungen->pers.Daten und den \r\n\
entsprechenden Wert ändern.");
								else
									Haupttext += _T("\
Den Wert des Datenelements '") + csElement + _T("' findet der Finanzamtsserver nicht korrekt. \r\n\
Bitte oben in der Werkzeugleiste auf den Knopf mit dem Maulschlüssel drücken oder unter \r\n\
Menü->Ansicht->Einstellungen->pers.Daten und den entsprechenden Wert ändern.");
							}
						}
						Haupttext += _T("\r\n\r\n");
					}
				}
				/*else	// Transfer Ticket ausgeben
				{
					int nPosAnfang = Antwort.Find(_T("<TransferTicket>"), 0);
					int nPosEnde = Antwort.Find(_T("</TransferTicket>"), nPosAnfang);
					if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
					{
						nPosAnfang += 16; 
						Haupttext = _T("Umsatzsteuervoranmeldung wurde erfolgreich übertragen.\r\nTransfer-Ticket (wichtig für Nachfragen und Recherchen beim Finanzamt): ") + (TransferTicket = Antwort.Mid(nPosAnfang, nPosEnde-nPosAnfang)) + _T("\r\n(Hinweis: Das Ticket wurde im EC&T-Dokument gespeichert und kann jederzeit mit 'Zeige Log' angezeigt werden.)\r\n\r\n");
					}
				}*/

				// Infotexte anhängen
				{
					int nPosAnfang = 0;
					int nPosEnde = 0;
					while (nPosEnde >= 0 && nPosAnfang >= 0)
					{
						nPosAnfang = Antwort.Find(_T("<Info>"), nPosEnde);
						if (nPosAnfang >= 0)
						{
							if (Infotext.Left(5) != _T("Info:")) Infotext += _T("Info:\r\n");
							nPosEnde = Antwort.Find(_T("</Info>"), nPosAnfang);
							if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
							{
								nPosAnfang += 6; 
								Infotext += _T("\r\n") + Antwort.Mid(nPosAnfang, nPosEnde-nPosAnfang);
							}
						}
					}
				}
			}

			// Log einblenden und aktualisieren
			CString AltesUebertragungslog = m_DokumentCtrl.HoleBenutzerdefWert(_T("Elster"), _T("Übertragungslog"));
			Infotext += _T("\r\n\r\nFrühere Übertragungen:") + (AltesUebertragungslog.GetLength() ? AltesUebertragungslog : _T(" keine"));
			CString MomentanerFormularAnzeigename;
			m_VoranmeldungszeitraumCtrl.GetLBText(m_VoranmeldungszeitraumCtrl.GetCurSel(), MomentanerFormularAnzeigename);
			LogZeile = (CString)_T("\r\n") + Jetzt.Format(_T("Übertragung vom %d.%m.%Y %H:%M:%S: ")) + Jetzt.Format(_T("EC&T-Ticket %Y%m%d%H%M%S / Finanzamts-Ticket ")) + (TransferTicket.GetLength() ? TransferTicket : _T("-"))
				+ _T(" / UST-Zahlbetrag: ") + FeldWert83Merken + _T(" / Zeitraum: ") + MomentanerFormularAnzeigename + _T("  ->  ") + (atoi(Code) ? Haupttext : _T("OK"));
			CString NeuesUebertragungslog = LogZeile + AltesUebertragungslog;
			m_DokumentCtrl.SpeichereBenutzerdefWert(_T("Elster"), _T("Übertragungslog"), NeuesUebertragungslog);

			// die UST-Schuld mit dem aktuellen Voranmeldungszeitraum speichern
			m_DokumentCtrl.SpeichereBenutzerdefWert(_T("Elster"), _T("UST-Zahlbetrag-") + Jahr + _T("-") + Zeitraum, FeldWert83Merken);
		}

		m_ErgebnisCtrl.SetWindowText(Haupttext + LogZeile + Infotext);
		UpdateData(FALSE);

		CString PDFDateiNeu = m_EinstellungCtrl.HoleEinstellung(_T("[ElsterExport]ProtokollPfad"));
		if (PDFDateiNeu == _T("")) PDFDateiNeu = m_Datei;
		int nPos = PDFDateiNeu.ReverseFind(_T('\\'));
		if (nPos >= 0)
		{	// neuen Dateinamen an alten Pfad anhängen
			PDFDateiNeu = PDFDateiNeu.Left(nPos+1);
			nPos = PDFDatei.ReverseFind(_T('\\'));
			if (nPos >= 0) 
				PDFDateiNeu += PDFDatei.Mid(nPos+1);
			else
				PDFDateiNeu = PDFDatei;
		}
		else
			PDFDateiNeu = PDFDatei;

		CFileDialog SaveDialog (FALSE,												    // Save Dialog
							   _T("pdf"),												// DefExt
							   PDFDateiNeu,											    // Filename
							   OFN_ENABLESIZING | OFN_SHOWHELP | OFN_EXPLORER,			// Flags 
							   _T("Portable Document Format Datei (*.pdf)|*.pdf||"),	// Filter
							   (CWnd*) this);											// ParentWnd 

		// display dialog 
		if ( SaveDialog.DoModal() == IDOK )
		{
			PDFDateiNeu = SaveDialog.GetPathName();		

			if (PDFDateiNeu != PDFDatei)
			{
				if (CopyFile(PDFDatei, PDFDateiNeu, FALSE))
					DeleteFile(PDFDatei);
			}

			m_EinstellungCtrl.SpeichereEinstellung(_T("[ElsterExport]ProtokollPfad"), PDFDateiNeu);
			if ((DWORD)ShellExecute(m_hWnd, _T("open"), PDFDateiNeu, NULL, temp_path, SW_SHOWNORMAL) <= 32)
				AfxMessageBox(_T("Sorry, konte das Übergabeprotokoll-PDF nicht öffnen. (Kein PDF-Reader installiert?)"));
		}
	}
	//else
	//	AfxMessageBox(_T("Fehler: Leeres Antwort-Datenpaket. Das sollte nicht vorkommen. Bitte diesen Fehler melden!"));

PufferFreigeben:

    if (transferheaderPufferHandle) EricRueckgabepufferFreigeben(transferheaderPufferHandle);
    if (fehlerPufferHandle) EricRueckgabepufferFreigeben(fehlerPufferHandle);
    if (ergebnisPufferHandle) EricRueckgabepufferFreigeben(ergebnisPufferHandle);
    if (serverantwortPufferHandle) EricRueckgabepufferFreigeben(serverantwortPufferHandle);
    if (ticketPufferHandle) EricRueckgabepufferFreigeben(ticketPufferHandle);
    if (threturncodePufferHandle) EricRueckgabepufferFreigeben(threturncodePufferHandle);
    if (therrormessagePufferHandle) EricRueckgabepufferFreigeben(therrormessagePufferHandle);
    if (threturncodeerrormessagePufferHandle) EricRueckgabepufferFreigeben(threturncodeerrormessagePufferHandle);

	// if (zertifikat) EricCloseHandleToCertificate(zertifikat); crashing...

	EricBeendePtr();
	::FreeLibrary(libEricApi);

	AfxGetApp()->EndWaitCursor();	
	m_Liste.InvalidateRect(NULL, FALSE);
}

int CElsterDlg::RegSearchReplace(CString& string, LPCTSTR sSearchExp, LPCTSTR sReplaceExp, CStringArray& csaReplaceCount)
{
	int nPos = 0;
	int nReplaced = 0;
	CRegExp r;
	LPTSTR str = (LPTSTR)(LPCTSTR)string;
 
	r.RegComp( sSearchExp );
	while( (nPos = r.RegFind((LPTSTR)str)) != -1 )
	{
		nReplaced++;
		TCHAR *pReplaceStr = r.GetReplaceString( sReplaceExp );
		csaReplaceCount.Add(pReplaceStr);
 
		int offset = str-(LPCTSTR)string+nPos;
		string = string.Left(offset) + pReplaceStr + string.Mid(offset + r.GetFindLen());
 
		// Replace könnte eine Reallocation verursacht haben
		str = (LPTSTR)(LPCTSTR)string + offset + _tcslen(pReplaceStr);
		delete pReplaceStr;
	}

	return nReplaced;  // Liste aller ersetzen Ausdrücke zurückgeben
}

void CElsterDlg::PrintString(CString Dokumentname, CString Text)
{
	Text.Replace(_T("&"), _T("&&"));	// Konvertierung von '&' vermeiden

	DEVMODE *pdm = NULL;

	CPrintDialog printdlg(FALSE);

	printdlg.m_pd.nFromPage = 1;
	printdlg.m_pd.nToPage   = 1;
	printdlg.m_pd.nMinPage  = 1;
	printdlg.m_pd.nMaxPage  = 1;
	printdlg.m_pd.Flags &= ~PD_NOPAGENUMS;

	if (printdlg.DoModal() != IDOK) return;

	if (!(pdm = printdlg.GetDevMode())) return;

	// mit device context
	{
		CString cs;

		DOCINFO di;
		di.cbSize = sizeof(di);
		di.lpszDocName = Dokumentname.GetBuffer(0);
		di.lpszOutput = NULL;
		di.lpszDatatype = NULL;
		di.fwType = 0;

		HDC hdc = printdlg.GetPrinterDC();

		int printer_charwidth = GetDeviceCaps(hdc, HORZRES) / 80;
		int printer_charheight = GetDeviceCaps(hdc, VERTRES) / 72;

		StartDoc(hdc, &di);

		HFONT hfont = CreateFont(printer_charheight, printer_charwidth, 
								 0, 0, 400, FALSE, FALSE, FALSE, ANSI_CHARSET, 
								 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
								 DEFAULT_QUALITY, FIXED_PITCH|FF_MODERN, 
								 _T("Arial"));

		HFONT holdfont = (HFONT)SelectObject(hdc, (HGDIOBJ)hfont);

		SetMapMode(hdc, MM_TEXT);

		RECT r;
		r.left = (int)((float)printer_charwidth * (float)7);
		r.top = (int)((float)printer_charheight * (float)4);
		r.right = (int)((float)printer_charwidth * (float)78);
		r.bottom = (int)((float)printer_charheight * (float)68);

		DrawText(hdc, Text.GetBuffer(0), Text.GetLength(), &r, DT_LEFT|DT_WORDBREAK);

		SelectObject(hdc, holdfont);
		EndPage(hdc);
		EndDoc(hdc);
		DeleteDC(hdc);
	}

	GlobalFree(printdlg.m_pd.hDevMode);
	GlobalFree(printdlg.m_pd.hDevNames);
	if (pdm) GlobalUnlock(pdm);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CONTROL & DIALOG VERHALTEN
//

// Knöpfe (außer Senden)

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
			CString AltesUebertragungslog = m_DokumentCtrl.HoleBenutzerdefWert(_T("Elster"), _T("Übertragungslog"));
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
	UpdateListe();
}


void CElsterDlg::OnCbnSelchangeFinanzamt()
{
	int nBundesfinanzamtsnummer = m_FinanzamtCtrl.GetItemData(m_FinanzamtCtrl.GetCurSel());	// Index speichern, um darüber bei Verarbeitung den Pfad zu gewinnen
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

	UpdateListe(TRUE);
}

BOOL CElsterDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

CString CElsterDlg::XMLEscape(CString StringZumEscapen)
{
	StringZumEscapen.Replace(_T("&"), _T("&amp;"));
	StringZumEscapen.Replace(_T("\""), _T("&quot;"));
	StringZumEscapen.Replace(_T("\'"), _T("&apos;"));
	StringZumEscapen.Replace(_T("<"), _T("&lt;"));
	StringZumEscapen.Replace(_T(">"), _T("&gt;"));
	return StringZumEscapen;
}

void CElsterDlg::OnBnClickedFinanzamtNichtInDerListe()
{
	if ((DWORD)ShellExecute(m_hWnd, _T("open"), _T("http://gemfa.bfinv.de/gemfai.exe"), NULL, _T("."), SW_SHOWNORMAL) <= 32)
			AfxMessageBox(_T("Sorry, kann Browser nicht öffnen, um auf die Seite mit der Finanzamtssuche zu gehen. Bitte manuall bei http://gemfa.bfinv.de/gemfai.exe nach dem zuständigen Finanzamt schauen!"));
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
	UpdateListe();

	// falls Übertragunslog angezeigt wird und die Liste überlagert:
	m_Zeige.SetWindowText(_T("&Zeige Log"));
	m_ErgebnisCtrl.ShowWindow(SW_HIDE);
	m_Liste.ShowWindow(SW_SHOW);

	GetDlgItem(IDOK)->EnableWindow();	 // Senden-Button kann wieder gedrückt werden
	m_Liste.InvalidateRect(NULL, TRUE);	 // Liste neu aufbauen
}
