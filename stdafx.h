#pragma once

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Comon Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxwin.h>
#include <afxdisp.h>

template<class ARG_KEY>
AFX_INLINE UINT AFXAPI HashKey(CString& key)
{
	int ll = key.GetLength();

	if (ll == 0) return 0;
	UINT ui = key[0];

	if (ll > 1)
	{
		for (int ii = 1; ii < ll; ii++)
		{
			ui ^= (key[ii] << ((ii + key[ii - 1]) % 24));
		} // for
	} // if

	return ui;
}