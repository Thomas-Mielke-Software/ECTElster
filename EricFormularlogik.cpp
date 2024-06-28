// EricFormularlogik.cpp : implementation file
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

// CEricFormularlogik benutzt die ERiC-Bibliothek um entweder die Hinweis- bzw. 
// Fehlerliste zu befüllen oder aber Daten an Elster zu versenden

#include "stdafx.h"
#include "ECTElster.h"
#include "ElsterDlg.h"
#include "EricFormularlogik.h"
#include "XMLite.h"  // https://www.codeproject.com/Articles/3426/XMLite-simple-XML-parser

// ERiC Includes
#include "EricApi.h"
#include "eric_fehlercodes.h"
#include "eric_types.h"

IMPLEMENT_DYNAMIC(CEricFormularlogik, CObject)

#pragma warning(push)
#pragma warning(disable:26495)  // "lazy" initialisierung in Render()
CEricFormularlogik::CEricFormularlogik()
{
}
#pragma warning(pop)

void CEricFormularlogik::Init(
	CFormularCtrl* pFormularCtrl,
	CEinstellung* pEinstellungCtrl,
	CDokumentCtrl* pDokumentCtrl,
	CBuchungCtrl* pBuchungCtrl)
{
	m_pFormularCtrl = pFormularCtrl;
	m_pEinstellungCtrl = pEinstellungCtrl;
	m_pDokumentCtrl = pDokumentCtrl;
	m_pBuchungCtrl = pBuchungCtrl;
}

CString CEricFormularlogik::Render(
	HWND m_hWnd,
	CString &FormularDateipfad,
	CQuickList* pListe,
	CString (&ListeInhalt)[500][6],
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
	CString& csRechtsform,
	CString& csEinkunftsart,
	CString& csBetriebsinhaber,
	CString&csGrundstuecksveraeusserungen,
	CTime &Jetzt,
	CString &Jahr,
	CString &Zeitraum,
	CString &Bundesfinanzamtsnummer,
	CString &EmpfaengerFinanzamt,
	CString &MomentanerFormularAnzeigename,
	CString &csBetrieb,
	BOOL bNurValidieren = FALSE)
{
	extern CECTElsterApp theApp;
	CString csReturn = "";

#ifdef TESTVERBINDUNG
	CString HerstellerID = _T("74931");
	CString Testmerker = _T("700000001");
#else
	CString HerstellerID = _T("21734");
	CString Testmerker = _T("");
#endif

	// EC&T-ActiveX-Controls
	m_FormularDateipfad = FormularDateipfad;

	// vorverdaute Dialog-Werte
	m_pDatei = &Datei;  // Zertifikatsdatei
	m_pPasswort = &Passwort;
	m_pEmailAdresse = &EmailAdresse;
	m_pTelefon = &Telefon;
	m_KorrigierteAnmeldung = bKorrigierteAnmeldung;
	m_BelegeWerdenNachgereicht = bBelegeWerdenNachgereicht;
	m_VerrechnungDesErstattungsanspruchs = bVerrechnungDesErstattungsanspruchs;
	m_EinzugsermaechtigungWiderrufen = bEinzugsermaechtigungWiderrufen;
	m_csRechtsform = csRechtsform;
	m_csEinkunftsart = csEinkunftsart;
	m_csBetriebsinhaber = csBetriebsinhaber;
	m_csGrundstuecksveraeusserungen = csGrundstuecksveraeusserungen;

	// normale Dialog-Controls
	m_pListe = pListe;
	m_pListeHinweise = pListeHinweise;
	m_pListeFehler = pListeFehler;
	m_pListeHinweise->RemoveAll();
	m_pListeFehler->RemoveAll();

	// sonstige
	m_Jetzt = Jetzt;
	m_EmpfaengerFinanzamt = EmpfaengerFinanzamt;
	m_EmailAdresse = EmailAdresse;
	m_Telefon = Telefon;
	m_Jahr = Jahr;
	m_Zeitraum = Zeitraum;
	m_Bundesfinanzamtsnummer = Bundesfinanzamtsnummer;
	m_HerstellerID = HerstellerID;
	m_MomentanerFormularAnzeigename = MomentanerFormularAnzeigename;
	m_AppVersion = theApp.GetVersion();
	m_csBetrieb = csBetrieb;

	CString DatenTeil = GetDatenteil();
	if (DatenTeil == "") return csReturn;

	CFile f;
	CString csDateipfadDatenteil;
	if (GetTempPath(MAX_PATH + 100, csDateipfadDatenteil.GetBuffer(MAX_PATH + 100)) > 0)
	{
		csDateipfadDatenteil.ReleaseBuffer();
		csDateipfadDatenteil += "ECTElsterDatenteil.xml";
		f.Open((LPCTSTR)csDateipfadDatenteil, CFile::modeCreate | CFile::modeWrite);
		f.Write(DatenTeil, DatenTeil.GetLength());
		f.Close();
	}
	// ___ typedef Funktionszeiger fuer notwendige ERiC API-Funktionen ___

	typedef int (STDCALL* EricInitialisiereFun)(const char* pluginPfad, const char* logPfad);
	EricInitialisiereFun EricInitialisierePtr;

	typedef int (STDCALL* EricBeendeFun)();
	EricBeendeFun EricBeendePtr;

	typedef int (STDCALL* EricBearbeiteVorgangFun)(
		const char* datenpuffer,
		const char* datenartVersion,
		uint32_t bearbeitungsFlags,
		const eric_druck_parameter_t* druckParameter,
		const eric_verschluesselungs_parameter_t* cryptoParameter,
		EricTransferHandle* transferHandle,
		EricRueckgabepufferHandle rueckgabeXmlPuffer,
		EricRueckgabepufferHandle serverantwortXmlPuffer);
	EricBearbeiteVorgangFun EricBearbeiteVorgangPtr;

	typedef int (STDCALL* EricGetHandleToCertificateFun)(EricZertifikatHandle* hToken,
		uint32_t* iInfoPinSupport,
		const char* pathToKeystore);
	EricGetHandleToCertificateFun EricGetHandleToCertificatePtr;

	typedef int (STDCALL* EricCloseHandleToCertificateFun)(EricZertifikatHandle hToken);
	EricCloseHandleToCertificateFun EricCloseHandleToCertificatePtr;

	typedef int (STDCALL* EricEinstellungAlleZuruecksetzenFun)();
	EricEinstellungAlleZuruecksetzenFun EricEinstellungAlleZuruecksetzenPtr;

	typedef int (STDCALL* EricEinstellungSetzenFun)(const char* name, const char* wert);
	EricEinstellungSetzenFun EricEinstellungSetzenPtr;

	typedef int (STDCALL* EricHoleFehlerTextFun)(
		int fehlerkode, EricRueckgabepufferHandle rueckgabePuffer);
	EricHoleFehlerTextFun EricHoleFehlerTextPtr;

	typedef int (STDCALL* EricPruefeSteuernummerFun)(
		const char* Steuernummer);
	EricPruefeSteuernummerFun EricPruefeSteuernummerPtr;

	typedef int (STDCALL* EricSystemCheckFun)();
	EricSystemCheckFun EricSystemCheckPtr;

	//    typedef int (STDCALL *EricRegistriereGlobalenFortschrittCallbackFun)(
	//        EricFortschrittCallback func,
	//        void *userData);
	//    EricRegistriereGlobalenFortschrittCallbackFun EricRegistriereGlobalenFortschrittCallbackPtr;

	//    typedef int (STDCALL *EricRegistriereFortschrittCallbackFun)(
	//        EricFortschrittCallback func,
	//        void *userData);
	//    EricRegistriereFortschrittCallbackFun EricRegistriereFortschrittCallbackPtr;

	typedef EricRueckgabepufferHandle(STDCALL* EricRueckgabepufferErzeugenFun) (void);
	EricRueckgabepufferErzeugenFun EricRueckgabepufferErzeugenPtr;

	typedef const char* (STDCALL* EricRueckgabepufferInhaltFun) (EricRueckgabepufferHandle handle);
	EricRueckgabepufferInhaltFun EricRueckgabepufferInhaltPtr;

	typedef uint32_t(STDCALL* EricRueckgabepufferLaengeFun) (EricRueckgabepufferHandle handle);
	EricRueckgabepufferLaengeFun EricRueckgabepufferLaengePtr;

	typedef int (STDCALL* EricRueckgabepufferFreigebenFun) (EricRueckgabepufferHandle handle);
	EricRueckgabepufferFreigebenFun EricRueckgabepufferFreigebenPtr;

	typedef int (STDCALL* EricEntladePluginsFun)();
	EricEntladePluginsFun EricEntladePluginsPtr;

	typedef int(STDCALL* EricCreateTHFun) (
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

	typedef int(STDCALL* EricHoleZertifikatEigenschaftenFun) (
		EricZertifikatHandle hToken,
		const char* pin,
		EricRueckgabepufferHandle rueckgabeXmlPuffer);
	EricHoleZertifikatEigenschaftenFun EricHoleZertifikatEigenschaftenPtr;

	char temp_path[MAX_PATH + 1];
	if (!GetTempPath(sizeof(temp_path), temp_path)) { AfxMessageBox(_T("Konnte keinen Pfad zum Temp-Verzeichnis bekommen. :(")); return csReturn; }

	HINSTANCE libEricApi = ::LoadLibrary(_T("ericapi.dll"));

	EricInitialisierePtr = (EricInitialisiereFun)GetProcAddress(libEricApi, _T("EricInitialisiere"));
	EricBeendePtr = (EricBeendeFun)GetProcAddress(libEricApi, _T("EricBeende"));
	EricBearbeiteVorgangPtr = (EricBearbeiteVorgangFun)GetProcAddress(libEricApi, _T("EricBearbeiteVorgang"));
	EricGetHandleToCertificatePtr = (EricGetHandleToCertificateFun)GetProcAddress(libEricApi, _T("EricGetHandleToCertificate"));
	EricCloseHandleToCertificatePtr = (EricCloseHandleToCertificateFun)GetProcAddress(libEricApi, _T("EricCloseHandleToCertificate"));
	EricHoleFehlerTextPtr = (EricHoleFehlerTextFun)GetProcAddress(libEricApi, _T("EricHoleFehlerText"));
	EricPruefeSteuernummerPtr = (EricPruefeSteuernummerFun)GetProcAddress(libEricApi, _T("EricPruefeSteuernummer"));
	EricSystemCheckPtr = (EricSystemCheckFun)GetProcAddress(libEricApi, _T("EricSystemCheck"));
	EricEinstellungSetzenPtr = (EricEinstellungSetzenFun)GetProcAddress(libEricApi, _T("EricEinstellungSetzen"));
	EricEinstellungAlleZuruecksetzenPtr = (EricEinstellungAlleZuruecksetzenFun)GetProcAddress(libEricApi, _T("EricEinstellungAlleZuruecksetzen"));
	//EricRegistriereGlobalenFortschrittCallbackPtr = 
	//EricRegistriereFortschrittCallbackPtr         = 
	EricRueckgabepufferErzeugenPtr = (EricRueckgabepufferErzeugenFun)GetProcAddress(libEricApi, _T("EricRueckgabepufferErzeugen"));
	EricRueckgabepufferInhaltPtr = (EricRueckgabepufferInhaltFun)GetProcAddress(libEricApi, _T("EricRueckgabepufferInhalt"));
	EricRueckgabepufferLaengePtr = (EricRueckgabepufferLaengeFun)GetProcAddress(libEricApi, _T("EricRueckgabepufferLaenge"));
	EricRueckgabepufferFreigebenPtr = (EricRueckgabepufferFreigebenFun)GetProcAddress(libEricApi, _T("EricRueckgabepufferFreigeben"));
	EricEntladePluginsPtr = (EricEntladePluginsFun)GetProcAddress(libEricApi, _T("EricEntladePlugins"));
	EricCreateTHPtr = (EricCreateTHFun)GetProcAddress(libEricApi, _T("EricCreateTH"));
	EricHoleZertifikatEigenschaftenPtr = (EricHoleZertifikatEigenschaftenFun)GetProcAddress(libEricApi, _T("EricHoleZertifikatEigenschaften"));

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
	char* cp;

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
	druckEinstellungen.vorschau = 0;
	druckEinstellungen.ersteSeite = 0;
	PDFDatei = (CString)temp_path + GetVersandbestaetigungPrefix() + Jetzt.Format(_T("Elster-Versandbestätigung-%Y%m%d%H%M%S.pdf"));
	druckEinstellungen.pdfName = PDFDatei;
	druckEinstellungen.fussText = _T("EasyCash&Tax Elster-Export");
	druckEinstellungen.duplexDruck = 0;
	druckEinstellungen.version = 2;

	int rc;
	eric_verschluesselungs_parameter_t verschluesselung;
	if (!bNurValidieren)
	{
		uint32_t pinErforderlich = 1;
		rc = EricGetHandleToCertificatePtr(&zertifikat, &pinErforderlich, (LPCTSTR)Datei);

		if (rc)
		{
			EricHoleFehlerText(rc, fehlerPufferHandle);
			const char* klartextFehler = EricRueckgabepufferInhalt(fehlerPufferHandle);
			if (!klartextFehler) klartextFehler = _T("");
			CString csKlartextFehlerUtf8;
			Utf8toAnsi(klartextFehler, csKlartextFehlerUtf8);
			CString Fehlertext;
			Fehlertext.Format(_T("Das Vorbereiten des Zertifikats schlug fehl (ERiC-Fehler %d). %s"), (int)rc, (LPCTSTR)csKlartextFehlerUtf8);
			AfxMessageBox(Fehlertext);
			goto PufferFreigeben;
		}

		verschluesselung.pin = Passwort;
		verschluesselung.zertifikatHandle = zertifikat;
		verschluesselung.abrufCode = NULL;
		verschluesselung.version = 2;
	}

	// Platz für den Meldungstext
	char msg[1000];
	int len = sizeof(msg);
	char* Transferticket = NULL;

	transferheaderPufferHandle = EricRueckgabepufferErzeugen();
	fehlerPufferHandle = EricRueckgabepufferErzeugen();

	/*	rc = EricCheckXML(0, utf8DatenTeil, fehlerPufferHandle);
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
		Datenlieferant = m_pEinstellungCtrl->HoleEinstellung(_T("vorname")) + _T(" ") + m_pEinstellungCtrl->HoleEinstellung(_T("name"));
		CStringA utf8Datenlieferant;
		Ansi2Utf8(Datenlieferant, utf8Datenlieferant);
		CStringA utf8DatenTeil;
		Ansi2Utf8(DatenTeil, utf8DatenTeil);
		rc = EricCreateTHPtr(utf8DatenTeil.GetBuffer(0), GetVerfahren(), GetDatenart(), _T("send-Auth"), Testmerker, HerstellerID, utf8Datenlieferant.GetBuffer(0), _T("2.3"), _T(""), transferheaderPufferHandle);
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
		(LPCTSTR)(GetDatenart() + _T("_") + Jahr),
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
			if (!klartextFehler) klartextFehler = _T("");
			if (strcmp(klartextFehler, _T("Verarbeitung fehlerfrei")))
			{
				CString csKlartextFehlerUtf8;
				Utf8toAnsi(klartextFehler, csKlartextFehlerUtf8);
				CString Fehlertext;
				Fehlertext.Format(_T("Die Abfrage des PIN-Status schlug fehl (ERiC-Fehler %d). %s"), (int)rc2, (LPCTSTR)csKlartextFehlerUtf8);
				AfxMessageBox(Fehlertext);
				goto PufferFreigeben;
			}
		}

		switch (pinStatus)
		{
		case 1: m_PinStatus = "PIN gesperrt!"; break;
		case 2: m_PinStatus = "Die letzte PIN/Passwort-Eingabe war fehlerhaft."; break;
		case 3: m_PinStatus = "ACHTUNG: Sperrung der PIN beim nächsten fehlerhaften Versuch!"; break;
		default: m_PinStatus = "PIN ok"; break;
		}
	}

	if (rc)
	{
		EricHoleFehlerText(rc, fehlerPufferHandle);
		const char* klartextFehler = EricRueckgabepufferInhalt(fehlerPufferHandle);
		if (!klartextFehler) klartextFehler = _T("");
		CString csKlartextFehlerUtf8;
		Utf8toAnsi(klartextFehler, csKlartextFehlerUtf8);
		CString Fehlertext;
		Fehlertext.Format(_T("Das Versenden des Datenpakets schlug fehl (ERiC-Fehler %d). %s"), (int)rc, (LPCTSTR)csKlartextFehlerUtf8);

		if (Fehlertext.Find(_T("Fehler im Transferheader.")) >= 0)
		{
			Fehlertext += _T(" Bitte stellen Sie zunächst sicher, dass Sie ein gültiges Zertifikat (.pfx-Datei) verwenden, z.B. indem Sie sich bei www.elsteronline.de einloggen. Im Zweifelsfall probieren Sie sonst, die Voranmeldung manuell mit ElsterFormular (von www.elster.de) zu übertragen.");
			if (!bNurValidieren) AfxMessageBox(Fehlertext);
		}
		else if (Fehlertext.Find(_T("Datenartversion")) >= 0)
		{
			Fehlertext += _T(" Wahrscheinlich ist die Version dieses Plugins zu veraltet und enthält nicht die nötige aktuelle Version der Softwarebibliothek der Finanzverwaltung. Bitte schauen Sie nach Updates (oben im EC&T-Menü, das rote Puzzlestück). Üblicherweise gibt es Ende Januar eine neue Release des Elster-Plugins. Registrierte Nutzer erhalten eine E-Mail-Benachrichtigung, sobald das Update zur Verfügung steht.");
			if (!bNurValidieren) AfxMessageBox(Fehlertext);
		}
		else if (rc == ERIC_PRINT_UNGUELTIGER_DATEI_PFAD)
		{
			long lRetCode = ERROR_SUCCESS;
			long lType = REG_SZ;
			long lCb;
			char data[100];
			HKEY hklm_codepage;
			*data = '\0';
			lCb = sizeof(data);
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Control\Nls\CodePage)", 0L, KEY_READ, &hklm_codepage) == ERROR_SUCCESS)
			{
				lRetCode = RegQueryValueEx(hklm_codepage, "OEMCP", NULL, (ULONG*)&lType, (LPBYTE)data, (ULONG*)&lCb);

				// nur string datatentyp akzeptieren
				if (lRetCode == ERROR_SUCCESS && lType == REG_SZ && atoi(data) == 65001)
					AfxMessageBox("Das Plugin hat eine problematische Windows-Einstellung entdeckt, die zu Problemen mit Pfadangaben führen wird. Es handelt sich um das experimentelle Feature 'UTF-8' bei den Regionseinstellungen von Windows. Zum Beheben klicke bitte aufs Windows-Startmenü und gib 'control panel' ein und drücke die Eingabetaste. Es sollte sich die alte 'Systemsteuerung' von Windows öffnen. Dort dann 'Zeit und Region' -> Region -> 'Gebietsschema ändern' und unten das Häkchen bei 'Beta: Unicode UTF-8 ...' entfernen.");
			}
		}
		else if (rc == ERIC_GLOBAL_HINWEISE || rc == ERIC_GLOBAL_PRUEF_FEHLER || Fehlertext.Find(_T("Datensatz nicht plausibel.")) >= 0)
		{
			// mit Meldung anfangen, dass Fehler oder Hinweise von ERiC zurückgegeben wurden
			int Zeile = m_pListe->GetItemCount() + 2;  // unter letzte Zeile einfügen (mit einer Leerzeile Abstand)
			CString csKlartextFehlerUtf8;
			Utf8toAnsi(klartextFehler, csKlartextFehlerUtf8);
			int nPos;
			if ((nPos = csKlartextFehlerUtf8.Find(". Zur Ermittlung")) > 0)
			{
				csKlartextFehlerUtf8 = csKlartextFehlerUtf8.Left(nPos) + " (auf farblich hinterlegte Zeilen klicken für die Meldung).";
			}
			ListeInhalt[Zeile++][0] = csKlartextFehlerUtf8;
			m_pListe->SetItemCount(Zeile);

			const char* ergebnisPuffer = EricRueckgabepufferInhalt(ergebnisPufferHandle);

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
						CString csText;
						Utf8toAnsi(text->GetText(), csText);

						int nErsteZiffer;
						nErsteZiffer = csFeldidentifikator.Find(_T("Kz"));
						if (nErsteZiffer >= 0 && nErsteZiffer < csFeldidentifikator.GetLength() - 2)
						{
							csFeldidentifikator = csFeldidentifikator.Mid(nErsteZiffer + 2);
						}
						int nFeldidentifikator = atoi(csFeldidentifikator);

						if (nFeldidentifikator && csText != _T(""))  // numerischer Feldidentifikator aus Feld-Kz der USt.-Voranm.
						{
							CString csTatsaechlicheFeldnummer;

							CStringArray csaErsetzteAusdruecke;
							int nAnzahlErsetzteAusdruecke = RegSearchReplace(csText, _T("(\\&apos;\\$/Elster\\[[0-9]+\\]/DatenTeil\\[[0-9]+\\]/Nutzdatenblock\\[[0-9]+\\]/Nutzdaten\\[[0-9]+\\]/Anmeldungssteuern\\[[0-9]+\\]/Steuerfall\\[[0-9]+\\]/Umsatzsteuervoranmeldung\\[[0-9]+\\]/Kz)([0-9]+)(\\[[0-9]+\\]\\$\\&apos;)"), _T("\\2"), csaErsetzteAusdruecke);
							
							int i;
							for (i = 0; i < csaErsetzteAusdruecke.GetSize() && i < nAnzahlErsetzteAusdruecke; i++)
							{
								// zu CMap hinzufügen:
								CString csTatsaechlicheFeldnummer = csaErsetzteAusdruecke[i];
								if (csMeldungTyp == _T("Hinweis"))
									m_pListeHinweise->SetAt(csTatsaechlicheFeldnummer, csText);
								else
									m_pListeFehler->SetAt(csTatsaechlicheFeldnummer, csText);
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
							m_pListeHinweise->SetAt(nTatsaechlicheFeldnummer, csText);
							*/
						}
						else // nicht-numerischer Feldidentifikator in Form des Elster-Feldnamen-Pfades
						{

							// Unternehmensart fehlt: für Menschen lesbar übersetzen
							CStringArray csaErsetzteE6000017;
							int nAnzahlErsetzteE6000017 = RegSearchReplace(csText, _T("(\\&apos;\\$/EUER\\[[0-9]+\\]/Allg\\[[0-9]+\\]/E6000017\\[[0-9]+\\]\\$\\&apos;)"), _T("'Unternehmensart'"), csaErsetzteE6000017);
							if (nAnzahlErsetzteE6000017)
								csText += _T(" Bitte die Unternehmensart unter Einstellungen -> pers. Daten (oder bei mehreren angelegten Betrieben im Filter-Menübereich unter Betrieb) korrigieren.");

							// Fall: keine Einnahmen
							if (csText == "Der Kontext &apos;/EUER[1]/BEin[1]&apos; ist leer.")
								csText = "Es liegen keine Einnahmenbuchungen vor. Wenn dies beabsichtigt ist, legen Sie eine Dummy-Betriebseinnahmen-Buchung mit dem Betrag 0,01 € mit 0% MWSt. an, z.B. für das Feld 'umsatzsteuerfreie Betriebseinnahmen'. Dann legen Sie eine korrespondierende Dummy-Betriebsausgaben-Buchung mit dem Betrag 0,01 € an, z.B. für das Feld 'übrige unbeschränkt abziehbare Betriebsausgaben'.";

							// Fall: keine Ausgaben 
							if (csText == "Der Kontext &apos;/EUER[1]/BAus[1]&apos; ist leer.")
								csText = "Es liegen keine Ausgabenbuchungen vor. Wenn dies beabsichtigt ist, legen Sie zunächst eine Dummy-Betriebseinnahmen-Buchung mit dem Betrag 0,01 € mit 0% MWSt. an, z.B. für das Feld 'umsatzsteuerfreie Betriebseinnahmen'. Dann legen Sie eine korrespondierende Dummy-Betriebsausgaben-Buchung mit dem Betrag 0,01 € an, z.B. für das Feld 'übrige unbeschränkt abziehbare Betriebsausgaben'.";

							// Fall: beschränkt abziehbaren Betriebsausgaben ohne den nicht abziehbaren Anteil erfasst
							if (csText == "Der Kontext &apos;/EUER[1]/BAus[1]/Beschr_abziehbar[1]/Nicht_abziehbar[1]&apos; ist leer.")
								csText = "Bei beschränkt abziehbaren Betriebsausgaben muss stets auch der Anteil angeben werden, der nicht abziehbar ist. Bitte EÜR Feld (1)162, (1)164, (1)165 und (1)168 checken.";

							CStringArray csaErsetzteAusdruecke;
							int nAnzahlErsetzteAusdruecke = RegSearchReplace(csFeldidentifikator, _T("(\\[[0-9]+\\])"), _T(""), csaErsetzteAusdruecke);
							if (nAnzahlErsetzteAusdruecke > 0 && csFeldidentifikator[0] == _T('/'))
							{
								// zu CMap hinzufügen:
								if (csMeldungTyp == _T("Hinweis"))
									m_pListeHinweise->SetAt(csFeldidentifikator, csText);
								else
									m_pListeFehler->SetAt(csFeldidentifikator, csText);
							}
						}

						// allgemeiner Fehler- oder Hinweistext ohne Zuordnung zu einem Feldkennzeichen
						// unten im ListCtrl anhängen
						if (bNurValidieren)
						{
							int Zeile = m_pListe->GetItemCount() + 1;  // unter letzte Zeile einfügen (mit einer Leerzeile Abstand)
							csMeldungTyp.ReleaseBuffer();
							if (csMeldungTyp == _T("FehlerRegelpruefung")) csMeldungTyp = _T("Fehler");
							ListeInhalt[Zeile++][0] = csMeldungTyp + _T(": ") + csText;
							m_pListe->SetItemCount(Zeile);
							m_pListe->EnsureVisible(Zeile - 1, FALSE);
						}
					}
				}
			}

			if (!bNurValidieren)
			{
				Fehlertext += _T(" Bitte überprüfen Sie die rot unterlegten Beträge bzw. Meldungen im Einzelnen noch einmal.");
				if (GetDatenart() == "UStVA")
					Fehlertext += _T(" Insbesondere bei Buchungen mit dem Leistungsempfänger als Steuerschuldner und innergem. Erwerben kann es zu Unstimmigkeiten kommen.");
				Fehlertext += _T(" Die  Such - Funktion auf www.easyct.de kann hier hilfreich sein. Außerdem stellen Sie bitte sicher, dass in den Einstellungen unter pers.Angaben ein korrekter Name eingetragen ist in der Form Vorname - Leerzeichen - Nachname.\r\n\r\nDetails anzeigen ? ");
				if (AfxMessageBox(Fehlertext, MB_YESNO) == IDYES)
				{
					if (ergebnisPuffer)
					{
						Fehlertext = (CString)ergebnisPuffer + _T("\r\n\r\nWenn es den Verdacht gibt, dass es sich um einen Fehler in der Software handelt, bitte den Datensatz speichern und an thomas@mielke.software senden.\r\n\r\nDatensatz jetzt speichern?");

						if (AfxMessageBox(Fehlertext, MB_YESNO) == IDYES)
						{
							CString DatensatzXMLDatei = m_pEinstellungCtrl->HoleEinstellung(_T("[ElsterExport]ProtokollPfad"));
							if (DatensatzXMLDatei == _T("")) DatensatzXMLDatei = Datei;
							int nPos = DatensatzXMLDatei.ReverseFind(_T('\\'));
							if (nPos >= 0)
							{	// neuen Dateinamen an alten Pfad anhängen
								DatensatzXMLDatei = DatensatzXMLDatei.Left(nPos + 1);
								DatensatzXMLDatei += _T("ElsterUStVADatensatz.xml");
							}
							else
								DatensatzXMLDatei = _T("C:\\ElsterUStVADatensatz.xml");

							CFileDialog SaveDialog(FALSE,													// Save Dialog
								_T("xml"),												// DefExt
								DatensatzXMLDatei,									    // Filename
								OFN_ENABLESIZING | OFN_SHOWHELP | OFN_EXPLORER,			// Flags 
								_T("Elster USTVa-Datensatz XML-Datei (*.xml)|*.xml||"),	// Filter
								(CWnd*)this);											// ParentWnd 

							// display dialog 
							if (SaveDialog.DoModal() == IDOK)
							{
								DatensatzXMLDatei = SaveDialog.GetPathName();

								CFile f;
								f.Open(DatensatzXMLDatei, CFile::modeCreate | CFile::modeWrite);
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
		csReturn = "OK";

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
				Code = Antwort.Mid(nPosAnfang, nPosEnde - nPosAnfang);
				if (atoi(Code))
				{	// Fehlercode ausgeben
					int nPosAnfang = Antwort.Find(_T("<Text>"), nPosEnde);
					int nPosEnde = Antwort.Find(_T("</Text>"), nPosAnfang);
					if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
					{
						nPosAnfang += 6;
						Haupttext = _T("Fehler bei der Verarbeitung des Transfer-Headers auf dem Finanzamts-Computer: \r\n") + Antwort.Mid(nPosAnfang, nPosEnde - nPosAnfang) + _T("\r\n\r\n");
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
								Infotext += _T("\r\n") + Antwort.Mid(nPosAnfang, nPosEnde - nPosAnfang);
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
				Code = Antwort.Mid(nPosAnfang, nPosEnde - nPosAnfang);
				if (atoi(Code))
				{		// Fehlercode ausgeben
					int nPosAnfang = Antwort.Find(_T("<Text>"), nPosEnde);
					int nPosEnde = Antwort.Find(_T("</Text>"), nPosAnfang);
					if (nPosAnfang >= 0 && nPosEnde > nPosAnfang)
					{
						nPosAnfang += 6;
						Haupttext = _T("Fehler bei der Verarbeitung der Nutzdaten auf dem Finanzamts-Computer: \r\n") + Antwort.Mid(nPosAnfang, nPosEnde - nPosAnfang);
						// Interpretationen hinzufügen:
						if (-1 != Antwort.Find(_T("element &apos;Jahr&apos; is not valid"), 0)) Haupttext += _T("\r\nInterpretation des Elster-Export Plugins: \r\n\
Die Buchungsdatei enthält ein falsches Buchungsjahr. Bitte oben in der Werkzeugleiste auf \r\n\
den Knopf mit dem Maulschlüssel drücken oder unter Menü->Ansicht->Einstellungen->Allgemein \r\n\
auf der rechten Seite nach dem Wert 'Buchungsjahr' suchen und ggf korrigieren. \r\n\
Hinweis: EC&T benötigt für jedes Jahr eine eigene Buchungsdatei. Man kann die relevanten \r\n\
Daten (AfAs) über die Funktion Jahreswechsel (im Datei-Menü) in eine neue Buchungsdatei \r\n\
übertragen. Dabei wird auch die Jahreszahl hochgesetzt.");
						CString csParseElementStart = _T("element &apos;");
						CString csParseElementEnde = _T("&apos;");
						CString csParseValueStart = _T("Value &apos;");
						CString csParseValueEnd = _T("&apos;");
						int nPosParseElementStart = Antwort.Find(csParseElementStart, 0);
						int nPosParseElementEnde = Antwort.Find(csParseElementEnde, nPosParseElementStart + csParseElementStart.GetLength());
						int nPosParseValueStart = Antwort.Find(csParseValueStart, 0);
						int nPosParseValueEnde = Antwort.Find(csParseValueEnd, nPosParseValueStart + csParseValueStart.GetLength());
						if (nPosParseElementStart >= 0)
						{
							if (nPosParseElementEnde > nPosParseElementStart)
							{
								CString csElement = Antwort.Mid(nPosParseElementStart + csParseElementStart.GetLength(), nPosParseElementEnde - nPosParseElementStart - csParseElementStart.GetLength());
								CString csWert;
								if (nPosParseValueEnde > nPosParseValueStart)
								{
									csWert = Antwort.Mid(nPosParseValueStart + csParseValueStart.GetLength(), nPosParseValueEnde - nPosParseValueStart - csParseValueStart.GetLength());
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
								Infotext += _T("\r\n") + Antwort.Mid(nPosAnfang, nPosEnde - nPosAnfang);
							}
						}
					}
				}
			}

			// Log einblenden und aktualisieren
			CString AltesUebertragungslog = m_pDokumentCtrl->HoleBenutzerdefWert(_T("Elster"), _T("Übertragungslog"));
			Infotext += _T("\r\n\r\nFrühere Übertragungen:") + (AltesUebertragungslog.GetLength() ? AltesUebertragungslog : _T(" keine"));
			LogZeile = (CString)_T("\r\n") + Jetzt.Format(_T("Übertragung vom %d.%m.%Y %H:%M:%S: ")) + m_Jetzt.Format(_T("EC&T-Ticket %Y%m%d%H%M%S / Finanzamts-Ticket ")) + (TransferTicket.GetLength() ? TransferTicket : _T("-"))
				+ GetLoginfo() + _T("  ->  ") + (atoi(Code) ? Haupttext : _T("OK"));
			CString NeuesUebertragungslog = LogZeile + AltesUebertragungslog;
			m_pDokumentCtrl->SpeichereBenutzerdefWert(_T("Elster"), _T("Übertragungslog"), NeuesUebertragungslog);

			UebertragungAbschliessen();
		}

		csReturn = Haupttext + LogZeile + Infotext;

		CString PDFDateiNeu = m_pEinstellungCtrl->HoleEinstellung(_T("[ElsterExport]ProtokollPfad"));
		if (PDFDateiNeu == _T("")) PDFDateiNeu = Datei;
		int nPos = PDFDateiNeu.ReverseFind(_T('\\'));
		if (nPos >= 0)
		{	// neuen Dateinamen an alten Pfad anhängen
			PDFDateiNeu = PDFDateiNeu.Left(nPos + 1);
			nPos = PDFDatei.ReverseFind(_T('\\'));
			if (nPos >= 0)
				PDFDateiNeu += PDFDatei.Mid(nPos + 1);
			else
				PDFDateiNeu = PDFDatei;
		}
		else
			PDFDateiNeu = PDFDatei;

		CFileDialog SaveDialog(FALSE,												    // Save Dialog
			_T("pdf"),												// DefExt
			PDFDateiNeu,											    // Filename
			OFN_ENABLESIZING | OFN_SHOWHELP | OFN_EXPLORER,			// Flags 
			_T("Portable Document Format Datei (*.pdf)|*.pdf||"),	// Filter
			(CWnd*)this);											// ParentWnd 

		// display dialog 
		if (SaveDialog.DoModal() == IDOK)
		{
			PDFDateiNeu = SaveDialog.GetPathName();

			if (PDFDateiNeu != PDFDatei)
			{
				if (CopyFile(PDFDatei, PDFDateiNeu, FALSE))
					DeleteFile(PDFDatei);
			}

			m_pEinstellungCtrl->SpeichereEinstellung(_T("[ElsterExport]ProtokollPfad"), PDFDateiNeu);
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
	m_pListe->InvalidateRect(NULL, FALSE);

	return csReturn;
}

