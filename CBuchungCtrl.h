#pragma once

// Mit Microsoft Visual C++ generierte IDispatch-Wrapperklasse(n)

// HINWEIS: Die Inhalte dieser Datei dürfen nicht geändert werden.
//  Die Änderungen werden überschrieben, wenn diese Klasse von Microsoft Visual C++ erneut generiert wird.

/////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"

class CBuchungCtrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBuchungCtrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= {0x3bc20630, 0x93ba, 0x47b2, {0x82, 0xd1, 0x82, 0x0c, 0x7f, 0x19, 0x86, 0xa1}};
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = nullptr)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				UINT nID, CFile* pPersist = nullptr, BOOL bStorage = FALSE,
				BSTR bstrLicKey = nullptr)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attribute
public:


// Vorgänge
public:
// _DBuchung

// Funktionen
//

	CY HoleNetto()
	{
		CY result;
		InvokeHelper(0xD, DISPATCH_METHOD, VT_CY, (void*)&result, nullptr);
		return result;
	}

	CY HoleBuchungsjahrNetto(long dokID)
	{
		CY result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0xE, DISPATCH_METHOD, VT_CY, (void*)&result, parms, dokID);
		return result;
	}

	CString HoleBenutzerdefWert(LPCTSTR PluginName, LPCTSTR SchluesselName)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR;
		InvokeHelper(0xF, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, PluginName, SchluesselName);
		return result;
	}

	void SpeichereBenutzerdefWert(LPCTSTR PluginName, LPCTSTR SchluesselName, LPCTSTR Wert)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, PluginName, SchluesselName, Wert);
	}

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}

// Eigenschaften
//

	long Get_ID()
	{
		long result;
		GetProperty(0x0, VT_I4, (void*)&result);
		return result;
	}

	void Set_ID(long propVal)
	{
		SetProperty(0x0, VT_I4, propVal);
	}

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

	CY GetBetrag()
	{
		CY result;
		GetProperty(0x2, VT_CY, (void*)&result);
		return result;
	}

	void SetBetrag(CY propVal)
	{
		SetProperty(0x2, VT_CY, propVal);
	}

	double GetMWSt()
	{
		double result;
		GetProperty(0x3, VT_R8, (void*)&result);
		return result;
	}

	void SetMWSt(double propVal)
	{
		SetProperty(0x3, VT_R8, propVal);
	}

	short GetAbschreibungNr()
	{
		short result;
		GetProperty(0x4, VT_I2, (void*)&result);
		return result;
	}

	void SetAbschreibungNr(short propVal)
	{
		SetProperty(0x4, VT_I2, propVal);
	}

	short GetAbschreibungJahre()
	{
		short result;
		GetProperty(0x5, VT_I2, (void*)&result);
		return result;
	}

	void SetAbschreibungJahre(short propVal)
	{
		SetProperty(0x5, VT_I2, propVal);
	}

	CString GetBeschreibung()
	{
		CString result;
		GetProperty(0x6, VT_BSTR, (void*)&result);
		return result;
	}

	void SetBeschreibung(CString propVal)
	{
		SetProperty(0x6, VT_BSTR, propVal);
	}

	DATE GetDatum()
	{
		DATE result;
		GetProperty(0x7, VT_DATE, (void*)&result);
		return result;
	}

	void SetDatum(DATE propVal)
	{
		SetProperty(0x7, VT_DATE, propVal);
	}

	CString GetKonto()
	{
		CString result;
		GetProperty(0x8, VT_BSTR, (void*)&result);
		return result;
	}

	void SetKonto(CString propVal)
	{
		SetProperty(0x8, VT_BSTR, propVal);
	}

	CString GetBelegnummer()
	{
		CString result;
		GetProperty(0x9, VT_BSTR, (void*)&result);
		return result;
	}

	void SetBelegnummer(CString propVal)
	{
		SetProperty(0x9, VT_BSTR, propVal);
	}

	CY GetAbschreibungRestwert()
	{
		CY result;
		GetProperty(0xA, VT_CY, (void*)&result);
		return result;
	}

	void SetAbschreibungRestwert(CY propVal)
	{
		SetProperty(0xA, VT_CY, propVal);
	}

	BOOL GetAbschreibungDegressiv()
	{
		BOOL result;
		GetProperty(0xB, VT_BOOL, (void*)&result);
		return result;
	}

	void SetAbschreibungDegressiv(BOOL propVal)
	{
		SetProperty(0xB, VT_BOOL, propVal);
	}

	double GetAbschreibungSatz()
	{
		double result;
		GetProperty(0xC, VT_R8, (void*)&result);
		return result;
	}

	void SetAbschreibungSatz(double propVal)
	{
		SetProperty(0xC, VT_R8, propVal);
	}

	long GetAbschreibungGenauigkeit()
	{
		long result;
		GetProperty(0x11, VT_I4, (void*)&result);
		return result;
	}

	void SetAbschreibungGenauigkeit(long propVal)
	{
		SetProperty(0x11, VT_I4, propVal);
	}

	CString GetBetrieb()
	{
		CString result;
		GetProperty(0x12, VT_BSTR, (void*)&result);
		return result;
	}

	void SetBetrieb(CString propVal)
	{
		SetProperty(0x12, VT_BSTR, propVal);
	}

	CString GetBestandskonto()
	{
		CString result;
		GetProperty(0x13, VT_BSTR, (void*)&result);
		return result;
	}

	void SetBestandskonto(CString propVal)
	{
		SetProperty(0x13, VT_BSTR, propVal);
	}



};
