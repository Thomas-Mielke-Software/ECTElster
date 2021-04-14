

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Apr 14 13:54:02 2021
 */
/* Compiler settings for ECTElster.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __ECTElsteridl_h__
#define __ECTElsteridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DECTElster_FWD_DEFINED__
#define ___DECTElster_FWD_DEFINED__
typedef interface _DECTElster _DECTElster;

#endif 	/* ___DECTElster_FWD_DEFINED__ */


#ifndef ___DECTElsterEvents_FWD_DEFINED__
#define ___DECTElsterEvents_FWD_DEFINED__
typedef interface _DECTElsterEvents _DECTElsterEvents;

#endif 	/* ___DECTElsterEvents_FWD_DEFINED__ */


#ifndef __ECTElster_FWD_DEFINED__
#define __ECTElster_FWD_DEFINED__

#ifdef __cplusplus
typedef class ECTElster ECTElster;
#else
typedef struct ECTElster ECTElster;
#endif /* __cplusplus */

#endif 	/* __ECTElster_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_ECTElster_0000_0000 */
/* [local] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_ECTElster_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ECTElster_0000_0000_v0_0_s_ifspec;


#ifndef __ECTElsterLib_LIBRARY_DEFINED__
#define __ECTElsterLib_LIBRARY_DEFINED__

/* library ECTElsterLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_ECTElsterLib;

#ifndef ___DECTElster_DISPINTERFACE_DEFINED__
#define ___DECTElster_DISPINTERFACE_DEFINED__

/* dispinterface _DECTElster */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DECTElster;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("EE4BCF66-82D9-4103-8CA4-9782B1A28D0A")
    _DECTElster : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DECTElsterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DECTElster * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DECTElster * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DECTElster * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DECTElster * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DECTElster * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DECTElster * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DECTElster * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DECTElsterVtbl;

    interface _DECTElster
    {
        CONST_VTBL struct _DECTElsterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DECTElster_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DECTElster_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DECTElster_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DECTElster_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DECTElster_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DECTElster_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DECTElster_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DECTElster_DISPINTERFACE_DEFINED__ */


#ifndef ___DECTElsterEvents_DISPINTERFACE_DEFINED__
#define ___DECTElsterEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DECTElsterEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DECTElsterEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C66F423E-538C-4120-A4E6-5C41345B3DAE")
    _DECTElsterEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DECTElsterEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DECTElsterEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DECTElsterEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DECTElsterEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DECTElsterEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DECTElsterEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DECTElsterEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DECTElsterEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DECTElsterEventsVtbl;

    interface _DECTElsterEvents
    {
        CONST_VTBL struct _DECTElsterEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DECTElsterEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DECTElsterEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DECTElsterEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DECTElsterEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DECTElsterEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DECTElsterEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DECTElsterEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DECTElsterEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ECTElster;

#ifdef __cplusplus

class DECLSPEC_UUID("DF841939-5148-4E3A-8D6E-DD99DE049776")
ECTElster;
#endif
#endif /* __ECTElsterLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


