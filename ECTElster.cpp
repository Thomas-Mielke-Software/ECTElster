// ECTElster.cpp : Implementation of CECTElsterApp and OCX registration.
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
#include "FormularCtrl.h"
#include "EinstellungCtrl.h"
#include "regexp.h"  // http://www.codeguru.com/Cpp/Cpp/string/regex/article.php/c2791

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CECTElsterApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x5AC861D7, 0x8E3E, 0x4F83, { 0x83, 0xAA, 0xC9, 0x16, 0x64, 0x8D, 0xC8, 0x0 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CECTElsterApp::InitInstance - DLL initialization

BOOL CECTElsterApp::InitInstance()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		AfxEnableControlContainer();	// wichtig, um auf die EC&T-Controls zuzugreifen!
	}

	return bInit;
}



// CECTElsterApp::ExitInstance - DLL termination

int CECTElsterApp::ExitInstance()
{
	CoUninitialize();

	return COleControlModule::ExitInstance();
}


CString & CECTElsterApp::GetVersion(void)
{
  static CString Version; 

  // get filename
  CString ModuleFilename;
  if ( GetModuleFileName ( theApp.m_hInstance, ModuleFilename.GetBufferSetLength ( MAX_PATH + 1 ), MAX_PATH ) == 0 )
    return Version;
  ModuleFilename.ReleaseBuffer();

  // determine version info size
  DWORD VersionInfoSize;
  DWORD NonsenseHandle;
  VersionInfoSize = GetFileVersionInfoSize ( ModuleFilename.GetBuffer ( 0 ), &NonsenseHandle );
  if ( VersionInfoSize == 0 )
    return Version;

	// allocate memory for and then get version info data block
  LPVOID VersionData;
  VersionData = malloc ( VersionInfoSize );
  if ( !GetFileVersionInfo ( ModuleFilename.GetBuffer ( 0 ), NonsenseHandle, VersionInfoSize, VersionData ) ) 
    return Version;
  
  // get fixed version info
  VS_FIXEDFILEINFO* FixedFileInfo = NULL;
  UINT FixedFileInfoSize;
  if ( !VerQueryValue ( VersionData, _T("\\"), (void**) &FixedFileInfo, &FixedFileInfoSize ) )
  {
    free ( VersionData );
    return Version;
  }
  
  Version.Format ( "Version %d.%d.%d.%d", HIWORD ( FixedFileInfo->dwProductVersionMS ), LOWORD ( FixedFileInfo->dwProductVersionMS ), HIWORD ( FixedFileInfo->dwProductVersionLS ), LOWORD ( FixedFileInfo->dwProductVersionLS ) );

  // free version info block
  free ( VersionData );

  return Version;
}


// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


// globale Hilfsfunktionen

extern CString XMLEscape(CString StringZumEscapen)
{
	StringZumEscapen.Replace(_T("&"), _T("&amp;"));
	StringZumEscapen.Replace(_T("\""), _T("&quot;"));
	StringZumEscapen.Replace(_T("\'"), _T("&apos;"));
	StringZumEscapen.Replace(_T("<"), _T("&lt;"));
	StringZumEscapen.Replace(_T(">"), _T("&gt;"));
	return StringZumEscapen;
}


extern void Ansi2Utf8(CString ansiText, CStringA& utf8Text)
{
	// UTF-8-Konvertierung (für Konvertierung in UTF-8 erst mal in UTF-16 konvertieren: CP_ACP -> CP_UTF16 -> CP_UTF8)
	CStringW utf16String('\0', MultiByteToWideChar(CP_ACP, 0, ansiText.GetBuffer(), -1, NULL, 0));
	MultiByteToWideChar(CP_ACP, 0, ansiText.GetBuffer(), -1, utf16String.GetBuffer(), utf16String.GetLength());
	CStringA utf8String('\0', WideCharToMultiByte(CP_UTF8, 0, utf16String.GetBuffer(), -1, NULL, 0, NULL, NULL));
	WideCharToMultiByte(CP_UTF8, 0, utf16String.GetBuffer(), -1, utf8String.GetBuffer(), utf8String.GetLength(), NULL, NULL);
	utf8Text = utf8String;
}

extern void Utf8toAnsi(CStringA utf8Text, CString& ansiText)
{
	// UTF-8-Konvertierung (für Konvertierung in ANSI erst mal in UTF-16 konvertieren: CP_UTF8 -> CP_UTF16 -> CP_ACP)
	CStringW utf16String('\0', MultiByteToWideChar(CP_UTF8, 0, utf8Text.GetBuffer(), -1, NULL, 0));
	MultiByteToWideChar(CP_UTF8, 0, utf8Text.GetBuffer(), -1, utf16String.GetBuffer(), utf16String.GetLength());
	CStringA ansiString('\0', WideCharToMultiByte(CP_ACP, 0, utf16String.GetBuffer(), -1, NULL, 0, NULL, NULL));
	WideCharToMultiByte(CP_ACP, 0, utf16String.GetBuffer(), -1, ansiString.GetBuffer(), ansiString.GetLength(), NULL, NULL);
	ansiText = ansiString;
}

extern int RegSearchReplace(CString& string, LPCTSTR sSearchExp, LPCTSTR sReplaceExp, CStringArray& csaReplaceCount)
{
	int nPos = 0;
	int nReplaced = 0;
	CRegExp r;
	LPTSTR str = (LPTSTR)(LPCTSTR)string;

	r.RegComp(sSearchExp);
	while ((nPos = r.RegFind((LPTSTR)str)) != -1)
	{
		nReplaced++;
		TCHAR* pReplaceStr = r.GetReplaceString(sReplaceExp);
		csaReplaceCount.Add(pReplaceStr);

		int offset = str - (LPCTSTR)string + nPos;
		string = string.Left(offset) + pReplaceStr + string.Mid(offset + r.GetFindLen());

		// Replace könnte eine Reallocation verursacht haben
		str = (LPTSTR)(LPCTSTR)string + offset + _tcslen(pReplaceStr);
		delete pReplaceStr;
	}

	return nReplaced;  // Liste aller ersetzen Ausdrücke zurückgeben
}

extern void PrintString(CString Dokumentname, CString Text)
{
	Text.Replace(_T("&"), _T("&&"));	// Konvertierung von '&' vermeiden

	DEVMODE* pdm = NULL;

	CPrintDialog printdlg(FALSE);

	printdlg.m_pd.nFromPage = 1;
	printdlg.m_pd.nToPage = 1;
	printdlg.m_pd.nMinPage = 1;
	printdlg.m_pd.nMaxPage = 1;
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
			DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN,
			_T("Arial"));

		HFONT holdfont = (HFONT)SelectObject(hdc, (HGDIOBJ)hfont);

		SetMapMode(hdc, MM_TEXT);

		RECT r;
		r.left = (int)((float)printer_charwidth * (float)7);
		r.top = (int)((float)printer_charheight * (float)4);
		r.right = (int)((float)printer_charwidth * (float)78);
		r.bottom = (int)((float)printer_charheight * (float)68);

		DrawText(hdc, Text.GetBuffer(0), Text.GetLength(), &r, DT_LEFT | DT_WORDBREAK);

		SelectObject(hdc, holdfont);
		EndPage(hdc);
		EndDoc(hdc);
		DeleteDC(hdc);
	}

	GlobalFree(printdlg.m_pd.hDevMode);
	GlobalFree(printdlg.m_pd.hDevNames);
	if (pdm) GlobalUnlock(pdm);
}

// formatiert CURRENCY immer auf zwei Nachkommastellen
CString FormatCy2d(COleCurrency& c, DWORD dwFlags, LCID lcid)
{
	if (lcid != MAKELCID(MAKELANGID(0x07, 0x01), SORT_DEFAULT))  // currently only German format supported
		AfxThrowNotSupportedException();

	CString temp;
	temp = c.Format(dwFlags, lcid);
	TCHAR* tempBuffer = temp.GetBuffer(50);
	TCHAR* cp = strchr(tempBuffer, _T(','));
	int size = strlen(tempBuffer);
	if (cp == tempBuffer + size - 3)  // two decimals -> everything fine, just return
		temp.ReleaseBuffer(size); 
	else if (cp == NULL)	// no decimals
	{
		strcat(tempBuffer, _T(",00"));
		temp.ReleaseBuffer(size + 3);
	}
	else
	{	// check for only one decimal, like "1,2"
		if (size > 2 && tempBuffer[size - 2] == _T(',') && isdigit(tempBuffer[size - 1]))
		{
			tempBuffer[size] = _T('0');
			tempBuffer[size + 1] = _T('\0');
			temp.ReleaseBuffer(size + 1);
		}
	}
	return temp;
}