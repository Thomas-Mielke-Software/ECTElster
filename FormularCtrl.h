// FormularCtrl.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CFormularCtrl

class CFormularCtrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CFormularCtrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xDFA091A6, 0xD040, 0x444F, { 0x80, 0xA, 0x65, 0x6B, 0xEA, 0x33, 0x61, 0xC4 } };
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

	void SetzeDokumentID(long ID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ID);
	}
	CString HoleFormularnamen(long Index, LPCTSTR Filter)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index, Filter);
		return result;
	}
	CString HoleFormularpfad(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	long HoleFormularanzahl()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void WaehleFormular(LPCTSTR Formular)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Formular);
	}
	long HoleFeldanzahl()
	{
		long result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long HoleFeldIDUeberIndex(long Index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Index);
		return result;
	}
	CString HoleFeldwertUeberIndex(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	CString HoleFeldwertUeberID(long FeldID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FeldID);
		return result;
	}
	CString HoleFeldbeschreibungUeberID(long FeldID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xA, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FeldID);
		return result;
	}
	long HoleVoranmeldungszeitraum()
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xB, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

};
