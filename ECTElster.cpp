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
