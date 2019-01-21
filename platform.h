/*
 * Copyright � LfSt Bayern, Muenchen, IuK 111, 2006-2012
 */

#ifndef ERIC_PLATFORM_H
#define ERIC_PLATFORM_H

/**
 * @file
 * @brief Konstanten f�r verschiedene Betriebssysteme
 */


/* OS X */

#if defined(__APPLE__)
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
		#define __IOS_SIMULATOR__
		#define __IOS__

		#define FILENAMES_CASE_SENSITIVE 0
		#define strcasecmp strcmp

    #elif TARGET_OS_IPHONE
		#define __IOS_DEVICE__
		#define __IOS__

		#define FILENAMES_CASE_SENSITIVE 0
		#define strcasecmp strcmp

    #elif TARGET_OS_MAC
		#define OS_X
		#define MAC_OS_X
		#define MAC_OS
		#ifndef PLATFORM_NAME
		#   define PLATFORM_NAME "Mac OS X"
		#endif

		#define FILENAMES_CASE_SENSITIVE 0
		#define strcasecmp strcmp

		#if defined(__amd64__) || defined(__x86_64__)
		#   define ARCH_AMD64
		#endif
    #endif
#endif


/* Windows */

#if defined(WINDOWS) || defined(_WIN32) || defined(_MSC_VER)
#   define WINDOWS_OS
#   ifndef PLATFORM_NAME
#       define PLATFORM_NAME "Windows"
#   endif
#   if defined(_M_AMD64) || defined(_M_X64)
#      define ARCH_AMD64
#   endif
#endif

/* Linux */

#if defined(linux) || defined(__linux__)

#define LINUX_OS

#if !defined(__i386__) && defined(i386)
#   define __i386__
#endif

#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)

#   ifndef PLATFORM_NAME
#       define PLATFORM_NAME "Linux/x86"
#   endif

#else

#   if defined(__amd64__) || defined(__x86_64__)
#      define ARCH_AMD64
#      ifndef PLATFORM_NAME
#          define PLATFORM_NAME "Linux/x86-64"
#      endif
#   else
#      ifndef PLATFORM_NAME
#          define PLATFORM_NAME "Linux" /* Generic Linux */
#      endif
#   endif

#endif

#if !defined(LINUX) && defined(LINUX_OS)
#   define LINUX 1
#endif

#endif


/**************************************************/

/* Der gesamte Code innerhalb dieses #ifndef-Blocks wird von Doxygen
 * f�r die Generierung der ERiC API HTML-Referenz ignoriert.
 */
#ifndef ERIC_DOXYGEN_MODE


typedef unsigned char BYTE;
typedef unsigned char byte;


#define ERIC_UINT32_MAX 0xffffffffUL

/**
 * @brief Definition eines vorzeichenlosen, 32 Bit breiten Integer-Typs.
 *
 * Der Typ uint32_t wird f�r Ganzzahl-Ausgaben der ERiC API verwendet, um
 * unabh�ngig von Betriebssystem und Compiler eine eindeutig spezifizierte
 * Bit-Breite und Kodierung zu gew�hrleisten.
 *
 * Die in ERiC implementierte Definition von uint32_t kommt nur zum Einsatz,
 * wenn im Code des ERiC-Anwenders kein Pr�prozessor-define gleichen Namens
 * existiert und wenn der Header <stdint.h> oder eine Compiler-spezifische
 * Variante davon nicht bereits importiert wurden.  Eventuelle Konflikte zu
 * gleichnamigen Typ-Definitionen im Code des ERiC-Anwenders k�nnen evtl. durch
 * Auskommentieren der ERiC-spezifischen Variante gel�st werden.
 *
 * @note In der HTML-Darstellung dieser Dokumentation wird keine konkrete
 * Implementierung angezeigt, sondern nur der Platzhalter
 * <i>__plattformabhaengige_Implementierung__</i>.  Die f�r eine bestimmte
 * Plattform tats�chlich verwendete Typdefinition wird aus dem betreffenden
 * Quell-Code in der Datei ericdef.h ersichtlich.
 */

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(_STDINT_H_) || defined(_STDINT_H) || defined(_STDINT) || defined(_MSC_STDINT_H_)
#    define ERIC_STDINT_VORHANDEN 1
#else
#    if defined(__has_include)
#        if __has_include("stdint.h")
#            include "stdint.h"
#            define ERIC_STDINT_VORHANDEN 1
#        endif
#    elif defined(__STDC__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
         /* Wenn der Compiler den C99-Standard unterst�tzt, gibt es eine stdint.h */
#        include "stdint.h"
#        define ERIC_STDINT_VORHANDEN 1
#	elif (_MSC_VER >= 1600)
         /* Visual C++ bringt eine "stdint.h" ab Version 16 (d.h. VC++ 2010) mit */
#        include "stdint.h"
#        define ERIC_STDINT_VORHANDEN 1
#   endif
#endif

#if !defined(ERIC_STDINT_VORHANDEN)
#    define ERIC_STDINT_VORHANDEN 0
#    if !defined(uint32_t)
#        if (UINT_MAX == ERIC_UINT32_MAX)
             typedef unsigned int uint32_t;
#            define __uint32_t_defined
#        elif (USHRT_MAX == ERIC_UINT32_MAX)
             typedef unsigned short uint32_t;
#            define __uint32_t_defined
#        endif
#    endif
#endif

#else
/* Dieser Block wird von Doxygen f�r die Generierung der
   ERiC API HTML-Referenz ausgewertet */

/**
 * @brief Definition eines vorzeichenlosen, 32 Bit breiten Integer-Typs.
 *
 * Siehe Quellcode von platform.h f�r Implementierung.
 */
typedef __plattformabhaengige_Implementierung__ uint32_t;


#endif /* ERIC_DOXYGEN_MODE */


/*****************************************************/


#ifdef WINDOWS_OS
#  ifndef MSVC
#    define WINDOWS_GNUC true
#  endif
#endif


#ifdef WINDOWS_OS
#  define STDCALL               __stdcall
#  ifndef CDECL
#    define CDECL               __cdecl
#  endif

#else
#  if defined(__GNUC__) || defined(__GNUG__)
#    ifdef STDCALL
#      undef STDCALL
#    endif

#    ifdef CDECL
#      undef CDECL
#    endif

#    if defined(__i386__) && !defined(__IOS__) && !defined(ANDROID_OS)
#      define STDCALL           __attribute__((__stdcall__))
#    else
#      define STDCALL
#    endif
#    define CDECL               __attribute__((__cdecl__))
#  endif
#  if defined(_AIX)
#    ifdef STDCALL
#     undef STDCALL
#    endif
#    define STDCALL
#  endif
#endif

#if defined(__GNUC__)   &&  (__GNUC__ - 0 > 3 || (__GNUC__ - 0 == 3 && __GNUC_MINOR__ - 0 >= 5))
#  define GCC_VISIBILITY_SUPPORTED
#endif


#ifndef ERIC_DECL_EXPORT
#  ifdef WINDOWS_OS
#    define ERIC_DECL_EXPORT __declspec(dllexport)
# elif defined(_AIX)
#     define ERIC_DECL_EXPORT __attribute__((visibility("default")))
# else
#   if defined(__GNUC__)   &&  (__GNUC__ - 0 > 3 || (__GNUC__ - 0 == 3 && __GNUC_MINOR__ - 0 >= 5))
#     define ERIC_DECL_EXPORT __attribute__((visibility("default")))
#   endif
# endif

#  ifndef ERIC_DECL_EXPORT
#    define ERIC_DECL_EXPORT
#  endif
#endif

#ifndef ERIC_DECL_IMPORT
#  if defined(WINDOWS_OS)
#    define ERIC_DECL_IMPORT __declspec(dllimport)
# elif defined(_AIX)
#     define ERIC_DECL_IMPORT __attribute__((visibility("default")))
#  else
#   if defined(__GNUC__)   &&  (__GNUC__ - 0 > 3 || (__GNUC__ - 0 == 3 && __GNUC_MINOR__ - 0 >= 5))
#     define ERIC_DECL_IMPORT __attribute__((visibility("default")))
#   endif
#  endif

#  ifndef ERIC_DECL_IMPORT
#    define ERIC_DECL_IMPORT
#  endif
#endif


/************************************************/


#if _MSC_VER > 1000
#  ifndef MSVC
#    define MSVC
#  endif
#  pragma warning( disable : 4503 ) /* Laenge des ergaenzten Namens ueberschritten,                                                                         */
#  pragma warning( disable : 4786 ) /* Bezeichner wurde auf '255' Zeichen in den Browser-Informationen reduziert                                            */
#  pragma warning( disable : 4251 ) /* benoetigt eine DLL-Schnittstelle, die von Clients von class 'XYZ' verwendet wird                                     */
#  pragma warning( disable : 4355 ) /* this wird in initialisierungslisten verwendet                                                                        */
#  pragma warning( disable : 4275 ) /* class 'xxx' ist keine DLL-Schnittstelle und wurde als Basisklasse f�r die DLL-Schnittstelle class 'yyy' verwendet    */
#  pragma warning( disable : 4231 ) /* key word extern before template explicit instanciation is not standard but required for class EricSafeInt            */
#endif


/************************************************/


#ifdef MSVC
#  pragma warning( disable : 4250 ) /* 'class1' : inherits 'class2::member' via dominance */
#  define ATOI64 _atoi64
#  define I64(C)   C
#else
#  define ATOI64 atoll
#  define I64(C)   C##LL
#endif


#ifndef HAS_FUTIME
#  if defined(LINUX_OS) || defined(MAC_OS_X) || defined(__IOS__)
#    define HAS_FUTIME 0
#  else
#    define HAS_FUTIME 1
#  endif
#endif

#ifndef UTIME_NEEDS_CLOSED_FILE
#  if defined(LINUX_OS) || defined(MAC_OS_X) || defined(__IOS__)
#    define UTIME_NEEDS_CLOSED_FILE 1
#  else
#    define UTIME_NEEDS_CLOSED_FILE 0
#  endif
#endif

#if HAS_FUTIME && !defined(_AIX)
#  include <sys/utime.h>
#else
#  include <utime.h>
#endif

#endif /* ERIC_PLATFORM_H */

