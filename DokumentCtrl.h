// DokumentCtrl.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDokumentCtrl

class CDokumentCtrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDokumentCtrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xED09076F, 0xB744, 0x47A1, { 0x8D, 0x60, 0xF9, 0xEF, 0x9, 0x3F, 0x19, 0xF0 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:

// Operations
public:

	long FindeErsteEinnahmenBuchung()
	{
		long result;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long FindeErsteAusgabenBuchung()
	{
		long result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long FindeNaechsteBuchung(long BuchungID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, BuchungID);
		return result;
	}
	long NeueEinnahmenbuchungEinfuegen()
	{
		long result;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long NeueAusgabenbuchungEinfuegen()
	{
		long result;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Sortieren()
	{
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LoescheBuchung(long BuchungID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, BuchungID);
	}
	CY HoleEinnahmenSumme()
	{
		CY result;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_CY, (void*)&result, NULL);
		return result;
	}
	CY HoleAusgabenSumme()
	{
		CY result;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_CY, (void*)&result, NULL);
		return result;
	}
	long HoleDokumentVersion()
	{
		long result;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString HoleBenutzerdefWert(LPCTSTR PluginName, LPCTSTR SchluesselName)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, PluginName, SchluesselName);
		return result;
	}
	void SpeichereBenutzerdefWert(LPCTSTR PluginName, LPCTSTR SchluesselName, LPCTSTR Wert)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, PluginName, SchluesselName, Wert);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

	long GetID()
	{
		long result;
		GetProperty(0x1, VT_I4, (void*)&result);
		return result;
	}
	void SetID(long propVal)
	{
		SetProperty(0x1, VT_I4, propVal);
	}
	short GetJahr()
	{
		short result;
		GetProperty(0x2, VT_I2, (void*)&result);
		return result;
	}
	void SetJahr(short propVal)
	{
		SetProperty(0x2, VT_I2, propVal);
	}
	long GetLaufendeBuchungsnummerEinnahmen()
	{
		long result;
		GetProperty(0x3, VT_I4, (void*)&result);
		return result;
	}
	void SetLaufendeBuchungsnummerEinnahmen(long propVal)
	{
		SetProperty(0x3, VT_I4, propVal);
	}
	long GetLaufendeBuchungsnummerAusgaben()
	{
		long result;
		GetProperty(0x4, VT_I4, (void*)&result);
		return result;
	}
	void SetLaufendeBuchungsnummerAusgaben(long propVal)
	{
		SetProperty(0x4, VT_I4, propVal);
	}
	CString GetWaehrung()
	{
		CString result;
		GetProperty(0x5, VT_BSTR, (void*)&result);
		return result;
	}
	void SetWaehrung(CString propVal)
	{
		SetProperty(0x5, VT_BSTR, propVal);
	}
	long GetAbschreibungGenauigkeit()
	{
		long result;
		GetProperty(0x6, VT_I4, (void*)&result);
		return result;
	}
	void SetAbschreibungGenauigkeit(long propVal)
	{
		SetProperty(0x6, VT_I4, propVal);
	}
	long GetLaufendeBuchungsnummerBank()
	{
		long result;
		GetProperty(0x7, VT_I4, (void*)&result);
		return result;
	}
	void SetLaufendeBuchungsnummerBank(long propVal)
	{
		SetProperty(0x7, VT_I4, propVal);
	}
	long GetLaufendeBuchungsnummerKasse()
	{
		long result;
		GetProperty(0x8, VT_I4, (void*)&result);
		return result;
	}
	void SetLaufendeBuchungsnummerKasse(long propVal)
	{
		SetProperty(0x8, VT_I4, propVal);
	}
};
