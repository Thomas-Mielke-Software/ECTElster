

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Mar 03 11:46:30 2020
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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_ECTElsterLib,0x5AC861D7,0x8E3E,0x4F83,0x83,0xAA,0xC9,0x16,0x64,0x8D,0xC8,0x00);


MIDL_DEFINE_GUID(IID, DIID__DECTElster,0xEE4BCF66,0x82D9,0x4103,0x8C,0xA4,0x97,0x82,0xB1,0xA2,0x8D,0x0A);


MIDL_DEFINE_GUID(IID, DIID__DECTElsterEvents,0xC66F423E,0x538C,0x4120,0xA4,0xE6,0x5C,0x41,0x34,0x5B,0x3D,0xAE);


MIDL_DEFINE_GUID(CLSID, CLSID_ECTElster,0xDF841939,0x5148,0x4E3A,0x8D,0x6E,0xDD,0x99,0xDE,0x04,0x97,0x76);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



