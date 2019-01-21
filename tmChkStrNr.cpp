
/****************************************************
*
*      Programm               : chkStrNr.cpp
*      Kurzbeschreibung: Implementierung von Pruefe_Steuernummer
*
****************************************************
*
*      Letzte Aenderung : 16.03.2007
*      Kurzbeschreibung:  Anpassung zu Steuernummerumstellungen Bayern/Saarland
*
*      BayLfSt
***************************************************/
#include "stdafx.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "tmfanr.h"             // Liste der Finanzaemter
// Aufrufkonvention
#ifdef _WIN32
#define DLL_EXP __stdcall
#else
#define DLL_EXP
#endif
#define Steuernummer_Falsch 5

/******************
 * Pruefverfahren *
 ******************/
#define V_MODI_11ER_VERF '1'
#define V_2ER_VERF       '2'
#define V_11ER_VERF      '3'

/*******************************************
 * Steuernummer in einzelnen Bestandteilen *
 *******************************************/
// Steuernummer im Vorsatz (ELSTER)
typedef struct ELSTER_Steuernummer {
    int F1,
        F2,
        F3,
        F4;
    int B1,
        B2,
        B3,
        B4;
    int U1,
        U2,
        U3,
        U4;
    int PZ;
} struct_ELSTER_Steuernummer;

/***********************************************************
 * Strukturtyp mit Multiplikatoren/Summanden               *
 * der einzelnen Bundeslaender sowie                       *
 * Flags fuer :                                            *
 * Algorythmus            --> VERFAHREN                    *
 * CHECK 2-stellig        --> ZUSATZ                       *
 * CHECK Verfahren-3 auf andere Endberechnung --> ZUSATZ-2 *
 ***********************************************************/
typedef struct Multiplikatoren_Summanden {
    int F1,
        F2,
        F3,
        F4;
    int B1,
        B2,
        B3,
        B4;
    int U1,
        U2,
        U3,
        U4;
    char VERFAHREN;
    char ZUSATZ;
    char ZUSATZ_2;
} struct_Multiplikatoren_Summanden;

/************************************************************
 * Strukturen mit Multiplikatoren/Summanden                 *
 * der einzelnen Bundeslaender sowie                        *
 * Flags fuer :                                             *
 * Algorythmus            --> VERFAHREN                     *
 * CHECK 2-stellig        --> ZUSATZ                        *
 * CHECK Verfahren-3 auf andere Endberechnung --> ZUSATZ-2  *
 ************************************************************/
// Baden-W.
static const struct_Multiplikatoren_Summanden Multis_BADEN_W = {
    0, 0, 9, 8,
    0, 7, 6, 5,
    4, 3, 2, 1,
    '2', '0', '1'
};

// Bayern-A
static const struct_Multiplikatoren_Summanden Multis_BAYERN_A = {
    0, 0, 0, 0,
    0, 7, 6, 5,
    4, 3, 2, 1,
    '2', '0', '1'
};

// Bayern-N und Saarland
static const struct_Multiplikatoren_Summanden Multis_BAYERN_N = {
    0, 5, 4, 3,
    0, 2, 7, 6,
    5, 4, 3, 2,
    '3', '2', '1'
};

// Berlin-A.
static const struct_Multiplikatoren_Summanden Multis_BERLIN_A = {
    0, 0, 0, 0,
    0, 7, 6, 5,
    8, 4, 3, 2,
    '3', '2', '1'
};

// Berlin-B.
static const struct_Multiplikatoren_Summanden Multis_BERLIN_B = {
    0, 0, 2, 9,
    0, 8, 7, 6,
    5, 4, 3, 2,
    '3', '2', '1'
};

// Branden-Sachsen
static const struct_Multiplikatoren_Summanden Multis_BRANDEN_SACHSEN = {
    0, 5, 4, 3,
    0, 2, 7, 6,
    5, 4, 3, 2,
    '3', '2', '1'
};

// Bremen
static const struct_Multiplikatoren_Summanden Multis_BREMEN = {
    0, 0, 4, 3,
    0, 2, 7, 6,
    5, 4, 3, 2,
    '3', '2', '1'
};

// Hamburg
static const struct_Multiplikatoren_Summanden Multis_HAMBURG = {
    0, 0, 4, 3,
    0, 2, 7, 6,
    5, 4, 3, 2,
    '3', '2', '1'
};

// Hessen
static const struct_Multiplikatoren_Summanden Multis_HESSEN = {
    0, 0, 9, 8,
    0, 7, 6, 5,
    4, 3, 2, 1,
    '2', '0', '1'
};

// Mecklen-Thuering.
static const struct_Multiplikatoren_Summanden Multis_MECKLEN_THUERING = {
    0, 5, 4, 3,
    0, 2, 7, 6,
    5, 4, 3, 2,
    '3', '2', '1'
};

// Niedersachsen
static const struct_Multiplikatoren_Summanden Multis_NIEDERSACHSEN = {
    0, 0, 2, 9,
    0, 8, 7, 6,
    5, 4, 3, 2,
    '3', '2', '1'
};

// NRW
static const struct_Multiplikatoren_Summanden Multis_NRW = {
    0, 3, 2, 1,
    7, 6, 5, 4,
    0, 3, 2, 1,
    '3', '2', '2'
};

// Rheinland-Pfalz
static const struct_Multiplikatoren_Summanden Multis_RHEINLAND_PFALZ = {
    0, 0, 1, 2,
    0, 1, 2, 1,
    2, 1, 2, 1,
    '1', '0', '1'
};


// Schleswig-H.
static const struct_Multiplikatoren_Summanden Multis_SCHLESWIG_H = {
    0, 0, 9, 8,
    0, 7, 6, 5,
    4, 3, 2, 1,
    '2', '0', '1'
};

/***********************************************
* Multiplikatoren fuer Verfahren 2: 2er-System *
************************************************/
static const struct_ELSTER_Steuernummer MULTIS_V2 = { 0, 0, 512, 256,   // F
    0, 128, 64, 32,             // B
    16, 8, 4, 2,                // U
    0                           // PZ
};

/*
atoin */
// Aus einem String einen Int lesen, nur n Stellen benutzen
static int atoin(
  const char *txt,
  int n) {
    int ret = 0;

    while (n > 0 && *txt >= '0' && *txt <= '9') {
        ret = ret * 10 + *txt - '0';
        ++txt;
        --n;
    }
    return ret;
}

#ifndef _WIN32
#include <stdio.h>

/*
itoa */
// itoa ist nur in Windows definiert
// Aus einem String einen Int lesen, nur n Stellen benutzen
static char *itoa(
  int value,
  char *string,
  int radix) {
    // assert(radix == 10);
    sprintf(string, "%d", value);
    return string;
}
#endif


/* Steuernummerprüfung Sonderfaelle Bayern, Berlin */


/* BAYERN_A_ODER_B */
// *** SONDERFALL BAYERN ***
static void BAYERN_A_ODER_B
(
  struct_ELSTER_Steuernummer ELSTER_Steuernummer_Ziffern,
  unsigned int BUFA_NUMMER,
  struct_Multiplikatoren_Summanden * MULTIS,
  struct_Multiplikatoren_Summanden BAYERN_A,
  struct_Multiplikatoren_Summanden BAYERN_N
) 
{
    // Bezirk nach MASKE 1;
       int Bezirk =   ELSTER_Steuernummer_Ziffern.B2 * 100 
	 	            + ELSTER_Steuernummer_Ziffern.B3 * 10 
				    + ELSTER_Steuernummer_Ziffern.B4 * 1;
    
	 // Standardbelegung auf neues Verfahren BAYERN_B 
        *MULTIS = BAYERN_N; // 11er - Verfahren
		
	 // Berücksichtigung der Ausnahmen, Verfahren BAYERN_A (2er - Verfahren)
		 if(
				 BUFA_NUMMER == 9143   // München Körperschaften mit allen Bezirken
			  || BUFA_NUMMER == 9144   // München I              mit allen Bezirken
			  || BUFA_NUMMER == 9147   // München II             mit allen Bezirken 
			  || BUFA_NUMMER == 9145   // München III            mit allen Bezirken
			  || BUFA_NUMMER == 9146   // München IV             mit allen Bezirken  
			  || BUFA_NUMMER == 9148   // München V              mit allen Bezirken
			  || BUFA_NUMMER == 9244   // Regensburg             mit allen Bezirken
			  
			  || (  // Bei bestimmten Finanzämtern bei Bezirken 001 - 299 und 800 - 999
					// das 2er - Verfahren verwenden
					  (
					       (Bezirk >= 1   && Bezirk <= 299) 
						|| (Bezirk >= 800 && Bezirk <= 999) 
					  )
					  &&
					  (	            
						   BUFA_NUMMER == 9163   // Traunstein
                        || BUFA_NUMMER == 9217   // Forchheim
						|| BUFA_NUMMER == 9220   // Gunzenhausen
					    || BUFA_NUMMER == 9247   // Schwabach
						|| BUFA_NUMMER == 9249   // Schweinfurt
						|| BUFA_NUMMER == 9252   // Uffenheim     					  
					  )
    			 )
		   )
		 {
		   // Belegung mit altem Verfahren BAYERN_A
    		 *MULTIS = BAYERN_A; // 2er - Verfahren
		 }
}




/* BERLIN_A_ODER_B */
// *** SONDERFALL BERLIN ***
// Westberlin 13 - 17, 19 - 30, 40
// Ostberlin  18, 31-37
// Verschiedene Multiplikatoren
//
// Verfahren wie Ostberlin darüberhinaus, wenn
// FA  13 - 17, 40   Bezirk 201 - 693
// FA       19          Bezirk 201 - 639
// FA  20 - 25        Bezirk 201 - 693
static void BERLIN_A_ODER_B(
  struct_ELSTER_Steuernummer ELSTER_Steuernummer_Ziffern,
  unsigned int BUFA_NUMMER,
  struct_Multiplikatoren_Summanden * MULTIS,
  struct_Multiplikatoren_Summanden BERLIN_A,
  struct_Multiplikatoren_Summanden BERLIN_B) {
    // Die letzten 2 Stellen der BUFA_Nummer als Zahl vom Typ Integer
    int iBUFA_NUMMER_LAST_2 = BUFA_NUMMER % 100;

    // Bezirk nach MASKE 1;
    int Bezirk = ELSTER_Steuernummer_Ziffern.B2 * 100 + ELSTER_Steuernummer_Ziffern.B3 * 10 + ELSTER_Steuernummer_Ziffern.B4 * 1;

    // Westberlin 10-17, 19-30, 40
    // Ostberlin 18, 31-37
    if (((iBUFA_NUMMER_LAST_2 < 31) && (iBUFA_NUMMER_LAST_2 != 18)) ||
      // Testfinanzamt 1140 Berlin
      (iBUFA_NUMMER_LAST_2 == 40)) {
        *MULTIS = BERLIN_A;
    } else {
        *MULTIS = BERLIN_B;
    }
    // Spezielle Steuerbezirke, bei denen die Multiplikatoren Berlin_B
    // (ehemals Ostberliner-Verfahren) zugeordnet werden
    if (((iBUFA_NUMMER_LAST_2 >= 13) && (iBUFA_NUMMER_LAST_2 <= 17) && (Bezirk >= 201) && (Bezirk <= 693)) || (
        // Berlin - Testfinanzamt 1140
        (iBUFA_NUMMER_LAST_2 == 40) && (Bezirk >= 201) && (Bezirk <= 693)) || ((iBUFA_NUMMER_LAST_2 == 19) && (Bezirk >= 201) && (Bezirk <= 639)) || ((iBUFA_NUMMER_LAST_2 == 19)   // Lohnsteueranmeldung
        // FA
        // 1119
        // Bezirk
        // 680
        && (Bezirk == 680)) || ((iBUFA_NUMMER_LAST_2 == 19) // Lohnsteueranmeldung FA 1119 Bezirk 684
        && (Bezirk == 684))
      || ((iBUFA_NUMMER_LAST_2 >= 20) && (iBUFA_NUMMER_LAST_2 <= 25) && (Bezirk >= 201) && (Bezirk <= 693)) || ((BUFA_NUMMER == 1116) && (Bezirk >= 1) && (Bezirk <= 29))) {
        *MULTIS = BERLIN_B;
    }
    // ENDE von BERLIN_A_ODER_B
}



/*
QUERSUMME */

/* QUERSUMME */
// ******** Quersumme ********
static int QUERSUMME(
  int Zahl) {
    int Ergebnis_Quersumme = 0;

    // Ueber alle Ziffern summieren
    for (; Zahl != 0; Zahl /= 10) {
        // Summieren der Zahl zum Ergebnis_Quersumme
        Ergebnis_Quersumme += Zahl % 10;
    }
    return Ergebnis_Quersumme;
}

/*
QUERQUERSUMME */
// Die Quersumme bilden, bis die Zahl zwischen 0 und 9 ist
static int QUERQUERSUMME(
  int Zahl) {
    while (Zahl > 9 || Zahl < 0) {
        Zahl = QUERSUMME(Zahl);
    }
    return Zahl;
}

////////////////////////////////////////////////////////////////////
// **** groesser_9 ****
static int groesser_9(
  int Zahl) {
    // Wenn einstellige Zahl, diese zurueckgeben,
    // wenn zweistellige Zahl die letze Ziffer der Zahl um 1 erhoeht
    // zurueckgeben
    if (Zahl <= 9) {
        return Zahl;
    } else {
        return ((Zahl % 10) + 1);
    }
}

// **** naechst_hoehere_Zahl_10 ****
static int naechst_hoehere_Zahl_10(
  int Zahl) {
    if (Zahl == 0) {
        return 10;
    } else {
        return ((Zahl + 9) / 10) * 10;
    }
}

/* naechst_hoehere_Zahl_11 */
// **** naechst_hoehere_Zahl_11 ****
static int naechst_hoehere_Zahl_11(
  int Zahl) {
    if (Zahl == 0) {
        return 11;
    } else {
        return ((Zahl + 10) / 11) * 11;
    }
}

/* Berechnungsverfahren Modi 11er 2er 11er */

/* MODI_11ER_VERF */
////////////////////////////////////////////////////////////////////
// **** MODI_11ER_VERF ****
static int MODI_11ER_VERF(
  struct_ELSTER_Steuernummer ZIFFERN,
  struct_Multiplikatoren_Summanden MULTIS) {
    int Gesamtergebnis;
    int Pruefziffer;

    // Multiplizieren jeder Ziffer mit den
    // in MULTIS gesetzten Mulitplikatoren
    Gesamtergebnis = groesser_9(ZIFFERN.F3 * MULTIS.F3) +
      groesser_9(ZIFFERN.F4 * MULTIS.F4) +
      groesser_9(ZIFFERN.B1 * MULTIS.B1) +
      groesser_9(ZIFFERN.B2 * MULTIS.B2) +
      groesser_9(ZIFFERN.B3 * MULTIS.B3) +
      groesser_9(ZIFFERN.B4 * MULTIS.B4) + groesser_9(ZIFFERN.U1 * MULTIS.U1) + groesser_9(ZIFFERN.U2 * MULTIS.U2) + groesser_9(ZIFFERN.U3 * MULTIS.U3) +
      groesser_9(ZIFFERN.U4 * MULTIS.U4);
    // Pruefziffer (Ergebnis) ist die
    // Differenz aus dem Gesamtergebnis und der nächst
    // höheren durch 10 teilbaren Zahl
    Pruefziffer = naechst_hoehere_Zahl_10(Gesamtergebnis) - Gesamtergebnis;
    return Pruefziffer;
}

/*
ZWEIER_VERF */

/* ZWEIER_VERF */
//////////////////////////////////////////////////////////////////
static int ZWEIER_VERF(
  struct_ELSTER_Steuernummer ZIFFERN,
  struct_Multiplikatoren_Summanden MULTIS) {
    struct_ELSTER_Steuernummer SUMMEN;
    int Gesamtergebnis = 0;
    int Pruefziffer;

    // Addition der einzelnen Ziffern
    // mit den Summanden (MULTIS)
    // Vom Ergebnis der Addition ist nur die
    // letzte Ziffer interessant
    // (Berechnet mit % 10 )
    SUMMEN.F1 = 0;
    SUMMEN.F2 = 0;
    SUMMEN.F3 = (ZIFFERN.F3 + MULTIS.F3) % 10;
    // Wenn Feld F3 nicht zur
    // Berechnung verwendet wird
    if (MULTIS.F3 == 0) {
        SUMMEN.F3 = 0;
    }
    SUMMEN.F4 = (ZIFFERN.F4 + MULTIS.F4) % 10;
    // Wenn Feld F4 nicht zur
    // Berechnung verwendet wird
    if (MULTIS.F4 == 0) {
        SUMMEN.F4 = 0;
    }
    SUMMEN.B1 = (ZIFFERN.B1 + MULTIS.B1) % 10;
    SUMMEN.B2 = (ZIFFERN.B2 + MULTIS.B2) % 10;
    SUMMEN.B3 = (ZIFFERN.B3 + MULTIS.B3) % 10;
    SUMMEN.B4 = (ZIFFERN.B4 + MULTIS.B4) % 10;
    SUMMEN.U1 = (ZIFFERN.U1 + MULTIS.U1) % 10;
    SUMMEN.U2 = (ZIFFERN.U2 + MULTIS.U2) % 10;
    SUMMEN.U3 = (ZIFFERN.U3 + MULTIS.U3) % 10;
    SUMMEN.U4 = (ZIFFERN.U4 + MULTIS.U4) % 10;
    SUMMEN.PZ = 0;
    // Multiplikation der letzten Stelle des
    // Summanden mit MULTIS-V2
    // Feste Werte in MULTIS-V2
    // * F3 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.F3 * MULTIS_V2.F3);
    // * F4 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.F4 * MULTIS_V2.F4);
    // * B2 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.B2 * MULTIS_V2.B2);
    // * B3 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.B3 * MULTIS_V2.B3);
    // * B4 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.B4 * MULTIS_V2.B4);
    // * U1 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.U1 * MULTIS_V2.U1);
    // * U2 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.U2 * MULTIS_V2.U2);
    // * U3 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.U3 * MULTIS_V2.U3);
    // * U4 *
    Gesamtergebnis += QUERQUERSUMME(SUMMEN.U4 * MULTIS_V2.U4);
    // Pruefziffer (Ergebnis) ist die
    // Differenz aus dem Gesamtergebnis und der nächst
    // höheren durch 10 teilbaren Zahl
    Pruefziffer = naechst_hoehere_Zahl_10(Gesamtergebnis) - Gesamtergebnis;
    return Pruefziffer;
}

/* ELFER_VERF */
//////////////////////////////////////////////////////////////
static int ELFER_VERF(
  struct_ELSTER_Steuernummer ZIFFERN,
  struct_Multiplikatoren_Summanden MULTIS) {
    int Gesamtergebnis = 0;
    int Pruefziffer = 0;

    // Multiplizieren jeder Ziffer
    // mit den in MULTIS gesetzten
    // Multiplikatoren und Addition
    // der einzelnen Produkte
    Gesamtergebnis = (ZIFFERN.F2 * MULTIS.F2)
      + (ZIFFERN.F3 * MULTIS.F3)
      + (ZIFFERN.F4 * MULTIS.F4)
      + (ZIFFERN.B1 * MULTIS.B1)
      + (ZIFFERN.B2 * MULTIS.B2) + (ZIFFERN.B3 * MULTIS.B3) + (ZIFFERN.B4 * MULTIS.B4) + (ZIFFERN.U1 * MULTIS.U1) + (ZIFFERN.U2 * MULTIS.U2) + (ZIFFERN.U3 * MULTIS.U3) +
      (ZIFFERN.U4 * MULTIS.U4);
    // Auswahl des Verfahrens anhand
    // von MULTIS.ZUSATZ_2
    switch (MULTIS.ZUSATZ_2) {
      case '1':
          // 1. Methode
          // Pruefziffer ( Ergebnis )
          // ist Differenz aus dem Gesamtergebnis
          // und der naechst hoeheren durch 11
          // teilbaren Zahl
          Pruefziffer = naechst_hoehere_Zahl_11(Gesamtergebnis) - Gesamtergebnis;
          break;
      case '2':
          // 2. Methode (nur NRW)
          // Pruefziffer ( Ergebnis )
          // wird berechnet durch:
          // Gesamtsumme durch 11 teilen und
          // davon den Rest hernehmen
          Pruefziffer = Gesamtergebnis % 11;
          break;
    }
    return Pruefziffer;
}

/* Pruefe_BUFA_Nr */
int DLL_EXP Pruefe_BUFA_Nr_V2(
  char *cSteuernummer,
  short Unterfallart) {
    // Variablenvereinbarung
    int iBufa;
    int i;
    const FAListe *pFinanzaemter = NULL;
    int search;
    int bFound;

    // Festhalten ob nur mit bei Anmeldungssteuern
    // zulässigen Finanzämtern gearbeitet werden soll
    short isAnmeldungssteuern = 0;

    if (                        // Anmeldungssteuern 
      Unterfallart == 54
      || Unterfallart == 55
      || Unterfallart == 56
      || Unterfallart == 57 || Unterfallart == 58 || Unterfallart == 59 || Unterfallart == 60 || Unterfallart == 61 || Unterfallart == 62 || Unterfallart == 63
      || Unterfallart == 64) {
        // nur Finanzämter, die auch für Anmeldungssteuern freigegeben sind
        isAnmeldungssteuern = 1;
    } else {
        // alle Finanzämter
        isAnmeldungssteuern = 0;
    }
    // Stellenzahl prüfen
    if ((strlen(cSteuernummer) != 13) && (strlen(cSteuernummer) != 4)) {
        // Fehler: weder BUFA-Nr (4 Stellen) noch Steuernummer (13 Stellen)
        return 1;
    }
    // Initialisierungen
    i = 0;
    // Erste vier Stellen der Steuernummer kopieren (=Bundesfinanzamtsnummer)
    iBufa = atoin(cSteuernummer, 4);
    // Alle Finanzaemter
    pFinanzaemter = ListOfFinanzaemter;
    
    bFound = 0;
    for (search = 0; !bFound && pFinanzaemter != NULL && pFinanzaemter->iFA != 0; ++pFinanzaemter) {
        if (pFinanzaemter->iFA == iBufa) {
            bFound = 1;
        }
        // Wenn gefunden aber nicht zulässig, Finanzamt ignorieren
        if ((isAnmeldungssteuern == 1) && (pFinanzaemter->StDUEV == 'N')) {
            bFound = 0;
        }
    }
    return (bFound ? 0 : -1);
}

/* Pruefe_BUFA_Nr */
int DLL_EXP Pruefe_BUFA_Nr(
  char *cSteuernummer) {
    // neue Version aufrufen mit Unterfallart 10 (ESt), da hier alle Finanzämter freigegeben
    return Pruefe_BUFA_Nr_V2(cSteuernummer, 10);
}

/* Pruefe_Steuernummer */
// Implementierung Funktion Pruefe_Steuernummer
extern "C" short Pruefe_Steuernummer(
  const char *ELSTERSteuernummer,
  char *Pruefziffer_vor_Steuernummerumstellung,
  char *Pruefziffer_nach_Steuernummerumstellung) {
    /* Steuernummer auf Formale Richtigkeit pruefen */
    int Stelle_b;
    int iPruefziffer_vor_Steuernummerumstellung = -1;
    int iPruefziffer_nach_Steuernummerumstellung = -1;

    // Berechnete Pruefziffer
    int Berechnete_Pruefziffer = 0;

    // Ziffern der Steuernummer (Uebergabe)
    struct_ELSTER_Steuernummer ELSTER_Steuernummer_Ziffern;

    // Ziffern der Steuernummer nach Maskierung
    struct_ELSTER_Steuernummer ZIFFERN;

    // Aktuelle Multiplikatoren und Summanden
    struct_Multiplikatoren_Summanden MULTIS;

    // BUFA-Nummer
    char BUFA_NUMMER[4];
    int iLand;
    int iBUFA_NUMMER;
    char ELSTER_Steuernummer[14];   // Interne Kopie da an der Nummer teilweise etwas verändert wird

    /* Pruefung: Steuernummer 13 Stellen */
    if ((strlen(ELSTERSteuernummer) != 13)) {
        strcpy(Pruefziffer_vor_Steuernummerumstellung, " Steuernummer ist nicht 13 Stellen lang !");
        strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
        return Steuernummer_Falsch;
    }
    /* Pruefung: Steuernummer enthaelt nicht nur Ziffern */
    for (Stelle_b = 0; Stelle_b < 13; Stelle_b++) {
        if (isdigit(ELSTERSteuernummer[Stelle_b]) == 0) {
            strcpy(Pruefziffer_vor_Steuernummerumstellung, "Noch nicht ermittelt. Steuernummer enthaelt nicht nur Ziffern !");
            strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
            return Steuernummer_Falsch;
        }
    }
    /* Pruefung: 5. Stelle in Steuernummer = 0 */
    if (ELSTERSteuernummer[4] != '0') {
        strcpy(Pruefziffer_vor_Steuernummerumstellung, "Noch nicht ermittelt.  5. Stelle der Steuernummer ist nicht 0 !");
        strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
        return Steuernummer_Falsch;
    }
    // Umkopieren in einen internen String
    strcpy(ELSTER_Steuernummer, ELSTERSteuernummer);
    /* Initialisieren der Rückgabewerte */
    strcpy(Pruefziffer_vor_Steuernummerumstellung, "Noch nicht ermittelt.");
    strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
   
    /* Benoetigte Variablen vereinbaren und initialisieren */
    /* Struktur ELSTER_Steuernummer_Ziffern belegen */
    // 1. Stelle
    ELSTER_Steuernummer_Ziffern.F1 = atoin(ELSTER_Steuernummer + 0, 1);
    // 2. Stelle
    ELSTER_Steuernummer_Ziffern.F2 = atoin(ELSTER_Steuernummer + 1, 1);
    // 3. Stelle
    ELSTER_Steuernummer_Ziffern.F3 = atoin(ELSTER_Steuernummer + 2, 1);
    // 4. Stelle
    ELSTER_Steuernummer_Ziffern.F4 = atoin(ELSTER_Steuernummer + 3, 1);
    // 5. Stelle
    ELSTER_Steuernummer_Ziffern.B1 = atoin(ELSTER_Steuernummer + 4, 1);
    // 6. Stelle
    ELSTER_Steuernummer_Ziffern.B2 = atoin(ELSTER_Steuernummer + 5, 1);
    // 7. Stelle
    ELSTER_Steuernummer_Ziffern.B3 = atoin(ELSTER_Steuernummer + 6, 1);
    // 8. Stelle
    ELSTER_Steuernummer_Ziffern.B4 = atoin(ELSTER_Steuernummer + 7, 1);
    // 9. Stelle
    ELSTER_Steuernummer_Ziffern.U1 = atoin(ELSTER_Steuernummer + 8, 1);
    // 10. Stelle
    ELSTER_Steuernummer_Ziffern.U2 = atoin(ELSTER_Steuernummer + 9, 1);
    // 11. Stelle
    ELSTER_Steuernummer_Ziffern.U3 = atoin(ELSTER_Steuernummer + 10, 1);
    // 12. Stelle
    ELSTER_Steuernummer_Ziffern.U4 = atoin(ELSTER_Steuernummer + 11, 1);
    // 13. Stelle
    ELSTER_Steuernummer_Ziffern.PZ = atoin(ELSTER_Steuernummer + 12, 1);
    /* Struktur ZIFFERN belegen mit Defaulteinstellung (d.h. Maske 1) */
    ZIFFERN.F1 = 0;
    ZIFFERN.F2 = ELSTER_Steuernummer_Ziffern.F2;
    ZIFFERN.F3 = ELSTER_Steuernummer_Ziffern.F3;
    ZIFFERN.F4 = ELSTER_Steuernummer_Ziffern.F4;
    ZIFFERN.B1 = 0;
    ZIFFERN.B2 = ELSTER_Steuernummer_Ziffern.B2;
    ZIFFERN.B3 = ELSTER_Steuernummer_Ziffern.B3;
    ZIFFERN.B4 = ELSTER_Steuernummer_Ziffern.B4;
    ZIFFERN.U1 = ELSTER_Steuernummer_Ziffern.U1;
    ZIFFERN.U2 = ELSTER_Steuernummer_Ziffern.U2;
    ZIFFERN.U3 = ELSTER_Steuernummer_Ziffern.U3;
    ZIFFERN.U4 = ELSTER_Steuernummer_Ziffern.U4;
    ZIFFERN.PZ = ELSTER_Steuernummer_Ziffern.PZ;
    strncpy(BUFA_NUMMER, ELSTER_Steuernummer, 4);
    /* Test auf gueltige BUFA-Nr */
    if (Pruefe_BUFA_Nr(ELSTER_Steuernummer) != 0) {
        // Nicht erlaubte BUFA_Nr
        strcpy(Pruefziffer_vor_Steuernummerumstellung, "Noch nicht ermittelt.  Ungueltige Bundesfinanzamtsnummer !");
        strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
        return Steuernummer_Falsch;
    }
    /* Test auf Bezirk letzte 3 Stellen = 000 (NRW letzte 4 Stellen = 0000) */
    if (BUFA_NUMMER[0] != '5') {
        if ((ELSTER_Steuernummer_Ziffern.B2 == 0) && (ELSTER_Steuernummer_Ziffern.B3 == 0) && (ELSTER_Steuernummer_Ziffern.B4 == 0)) {
            strcpy(Pruefziffer_vor_Steuernummerumstellung, "Noch nicht ermittelt.  Ungueltiger Bezirk 0 !");
            strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
            return Steuernummer_Falsch;
        }
    } else {
        // NRW
        if ((ELSTER_Steuernummer_Ziffern.B2 == 0) && (ELSTER_Steuernummer_Ziffern.B3 == 0) && (ELSTER_Steuernummer_Ziffern.B4 == 0)
          && (ELSTER_Steuernummer_Ziffern.U1 == 0)) {
            strcpy(Pruefziffer_vor_Steuernummerumstellung, "Noch nicht ermittelt.  Ungueltiger Bezirk 0 !");
            strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
            return Steuernummer_Falsch;
        }
    }
    /* Test auf 'Dummysteuernummer' (Bezirk letzte 3 Stellen = 999 und Unterscheidungsnummer 9999 und Prüfziffer 9) nur für Bayern */
    if (BUFA_NUMMER[0] == '9') {
        if ((ELSTER_Steuernummer_Ziffern.B2 == 9)
          && (ELSTER_Steuernummer_Ziffern.B3 == 9)
          && (ELSTER_Steuernummer_Ziffern.B4 == 9)
          && (ELSTER_Steuernummer_Ziffern.U1 == 9)
          && (ELSTER_Steuernummer_Ziffern.U2 == 9) && (ELSTER_Steuernummer_Ziffern.U3 == 9) && (ELSTER_Steuernummer_Ziffern.U4 == 9)
          && (ELSTER_Steuernummer_Ziffern.PZ == 9)) {
            strcpy(Pruefziffer_vor_Steuernummerumstellung, "Steuernummer mit Aufbau 9xxx099999999 ist in Bayern ungültig.");
            strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
            return Steuernummer_Falsch;
        }
    }
    /* Test auf Bezirk unter 100 */
    iBUFA_NUMMER = atoin(BUFA_NUMMER, 4);
    iLand = iBUFA_NUMMER / 100;
    switch (iLand) {
      case BAYERN_MUC:
          // Bayern BayLfSt -Dienststelle München-
      case BAYERN_NBG:
          // Bayern BayLfSt -Dienststelle Nürnberg-
      case BRANDENBURG:
          // Brandenburg
      case MECKLENBURG_VORPOMMERN:
          // Mecklenburg Vorpommern
      case SAARLAND:
          // Saarland
      case SACHSEN:
          // Sachsen
      case SACHSEN_ANHALT:
          // Sachsen Anhalt
      case THUERINGEN:
          // Thüringen
          /* Test auf Bezirk unter 100 */
          if (ELSTER_Steuernummer_Ziffern.B2 == 0   // Hunderterstelle
            ) {
              strcpy(Pruefziffer_vor_Steuernummerumstellung, "Noch nicht ermittelt.  Ungueltiger Bezirk (Bezirk < 100) !");
              strcpy(Pruefziffer_nach_Steuernummerumstellung, Pruefziffer_vor_Steuernummerumstellung);
              return Steuernummer_Falsch;
          }
    }
    /* Land suchen und Uebertragen der Multiplikatoren */
    switch (iLand) {
      case BAYERN_MUC:
          /* EasyCODE = */
      case BAYERN_NBG:
          // *** BAYERN ***
          BAYERN_A_ODER_B(ELSTER_Steuernummer_Ziffern, iBUFA_NUMMER, &MULTIS,   // Wird gesetzt
            Multis_BAYERN_A, Multis_BAYERN_N);
          break;
      case BRANDENBURG:
          /* EasyCODE = */
      case SACHSEN_ANHALT:
          /* EasyCODE = */
      case SACHSEN:
          // *** BRANDENBURG, ETC ***
          // BRANDEN-SACHSEN
          MULTIS = Multis_BRANDEN_SACHSEN;
          break;
      case MECKLENBURG_VORPOMMERN:
          /* EasyCODE = */
      case THUERINGEN:
          // *** MECKLENBURG, THUERINGEN ***
          // MECKLEN-THUERING
          MULTIS = Multis_MECKLEN_THUERING;
          break;
      case NORDRHEIN_WESTFALEN_ID1:
          /* EasyCODE = */
      case NORDRHEIN_WESTFALEN_ID2:
          /* EasyCODE = */
      case NORDRHEIN_WESTFALEN_ID3:
          /* EasyCODE = */
      case 54:
          // Testfinanzaemter NRW
      case 55:
          // Testfinanzaemter NRW
      case 56:
          // Testfinanzaemter NRW
          // *** NRW ***
          // NRW
          MULTIS = Multis_NRW;
          /* Maske 2 setzen: NRW */
          // Maske 2 ist gueltig; (NRW)
          // Defaultmaske (Maske 1) ueberschreiben
          ZIFFERN.F1 = 0;
          ZIFFERN.F2 = ELSTER_Steuernummer_Ziffern.F2;
          ZIFFERN.F3 = ELSTER_Steuernummer_Ziffern.F3;
          ZIFFERN.F4 = ELSTER_Steuernummer_Ziffern.F4;
          ZIFFERN.B1 = ELSTER_Steuernummer_Ziffern.B2;
          ZIFFERN.B2 = ELSTER_Steuernummer_Ziffern.B3;
          ZIFFERN.B3 = ELSTER_Steuernummer_Ziffern.B4;
          ZIFFERN.B4 = ELSTER_Steuernummer_Ziffern.U1;
          ZIFFERN.U1 = 0;
          ZIFFERN.U2 = ELSTER_Steuernummer_Ziffern.U2;
          ZIFFERN.U3 = ELSTER_Steuernummer_Ziffern.U3;
          ZIFFERN.U4 = ELSTER_Steuernummer_Ziffern.U4;
          ZIFFERN.PZ = ELSTER_Steuernummer_Ziffern.PZ;
          break;
      case BADEN_WUERTTEMBERG:
          // *** BADEN-WUERTTEMBERG ***
          // BADEN-W
          MULTIS = Multis_BADEN_W;
          break;
      case BERLIN:
          // *** BERLIN ***
          BERLIN_A_ODER_B(ELSTER_Steuernummer_Ziffern, iBUFA_NUMMER, &MULTIS, Multis_BERLIN_A, Multis_BERLIN_B);
          break;
      case BREMEN:
          // *** BREMEN ***
          // BREMEN
          MULTIS = Multis_BREMEN;
          break;
      case HAMBURG:
          // *** HAMBURG ***
          // HAMBURG
          MULTIS = Multis_HAMBURG;
          break;
      case HESSEN:
          // *** HESSEN ***
          // HESSEN
          MULTIS = Multis_HESSEN;
          break;
      case NIEDERSACHSEN:
          // *** NIEDERSACHSEN ***
          // NIEDERSACHSEN
          MULTIS = Multis_NIEDERSACHSEN;
          break;
      case RHEINLAND_PFALZ:
          // *** RHEINLAND-PFALZ ***
          // RHEINLAND-PFALZ
          MULTIS = Multis_RHEINLAND_PFALZ;
          break;
      case SAARLAND:
          // *** SAARLAND ***
		  // seit "Übernahme Bayern"
		  // nur noch neue Berechnung nach 11er-Verfahren Bayern (BAYERN_N)
		  MULTIS = Multis_BAYERN_N;
          break;
      case SCHLESWIG_HOLSTEIN:
          // *** SCHLESWIG-HOLSTEIN ***
          // SCHLESWIG-H
          MULTIS = Multis_SCHLESWIG_H;
          break;
      default:
          // FEHLER
          return Steuernummer_Falsch;
          break;
    }
    /* Auswahl des Pruefverfahren Verfahren steht in MULTIS.VERFAHREN */
    switch (MULTIS.VERFAHREN) {
      case V_MODI_11ER_VERF:
          Berechnete_Pruefziffer = MODI_11ER_VERF(ZIFFERN, MULTIS);
          break;
      case V_2ER_VERF:
          Berechnete_Pruefziffer = ZWEIER_VERF(ZIFFERN, MULTIS);
          break;
      case V_11ER_VERF:
          Berechnete_Pruefziffer = ELFER_VERF(ZIFFERN, MULTIS);
          break;
    }
    /* Berechnete Pruefziffer festhalten */
    /* Test Pruefziffer mehrstellig */
    if (Berechnete_Pruefziffer > 9) {
        iPruefziffer_vor_Steuernummerumstellung = -1;
        strcpy(Pruefziffer_vor_Steuernummerumstellung, "Nicht berechenbar.  Ungueltige Steuernummer fuehrt zu mehrstelliger Pruefziffer.");
    } else {
        iPruefziffer_vor_Steuernummerumstellung = Berechnete_Pruefziffer;
        itoa(Berechnete_Pruefziffer, Pruefziffer_vor_Steuernummerumstellung, 10);
    }
   
    // Pruefziffernberechnungen Steuernummerumstellung 
	// für umgestellte Finanzämter, die auch zusätzlich noch 
	// nach 2er - Verfahren berechnet werden 
	// weil die großrechnerseitigen Umsteigelisten noch aktiv sind
    /* BAYERN */
    switch (iBUFA_NUMMER) 
	{
      case 9143: // Finanzamt München f. Körpersch. -Körperschaftsteuer-
	  case 9163: // Traunstein
	  case 9217: // Forchheim
      case 9220: // Gunzenhausen
      case 9244: // Regensburg
	  case 9247: // Schwabach
	  case 9249: // Schweinfurt 
	  case 9252: // Uffenheim
      case 9144: // Finanzamt München I     *ab 21./22. September 2007*
      case 9145: // Finanzamt München III   *ab 21./22. September 2007*
	  case 9148: // Finanzamt München V     *ab 21./22. September 2007* 
      case 9146: // Finanzamt München IV    *ab 26./27. April 2008*
      case 9147: // Finanzamt München II    *ab 26./27. April 2008* 
          MULTIS = Multis_BAYERN_N; // Muliplikatoren fuer 11er Verfahren in Bayern
          Berechnete_Pruefziffer = ELFER_VERF(ZIFFERN, MULTIS);
          if (Berechnete_Pruefziffer > 9) {
              iPruefziffer_nach_Steuernummerumstellung = -1;
              strcpy(Pruefziffer_nach_Steuernummerumstellung, "Nicht berechenbar.  Ungueltige Steuernummer fuehrt zu mehrstelliger Pruefziffer.");
          } else {
              iPruefziffer_nach_Steuernummerumstellung = Berechnete_Pruefziffer;
              itoa(Berechnete_Pruefziffer, Pruefziffer_nach_Steuernummerumstellung, 10);
          }
          break;
      default:
          iPruefziffer_nach_Steuernummerumstellung = -1;
          strcpy(Pruefziffer_nach_Steuernummerumstellung, "Keine Steuernummerumstellung bei Finanzamt mit dieser BUFA-Nr.");
    }


    /* Test berechnete Pruefziffer = Pruefziffer in Steuernummer; sowie Steuernummerumstellung */
    if (
        // Prüfung der Prüfziffer 
		// Steuernummerumstellung Bayern berücksichtigen
           (iPruefziffer_vor_Steuernummerumstellung  == ELSTER_Steuernummer_Ziffern.PZ) 
		|| (iPruefziffer_nach_Steuernummerumstellung == ELSTER_Steuernummer_Ziffern.PZ)
       )
       {
         return 0;
	   }
	   else 
	   {
         return Steuernummer_Falsch;
	   }
}
