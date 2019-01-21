// Liste der Finanzamtsnummern und Finanzamtsnamen
// Version 12.07.2004
//
#define BUNDESAMT_FUER_FINANZEN 5
#define BADEN_WUERTTEMBERG      28
#define BAYERN_MUC              91
#define BAYERN_NBG              92
#define BERLIN                  11
#define BRANDENBURG             30
#define BREMEN                  24
#define HAMBURG                 22
#define HESSEN                  26
#define MECKLENBURG_VORPOMMERN  40
#define NIEDERSACHSEN           23
#define NORDRHEIN_WESTFALEN_ID1 51
#define NORDRHEIN_WESTFALEN_ID2 52
#define NORDRHEIN_WESTFALEN_ID3 53
#define RHEINLAND_PFALZ         27
#define SAARLAND                10
#define SACHSEN                 32
#define SACHSEN_ANHALT          31
#define SCHLESWIG_HOLSTEIN      21
#define THUERINGEN              41

/****************************************************************/

/* Beteiligte Länder am ELSTER-Projekt                          */

/****************************************************************/

/****************************************************************/
typedef struct _FAListe {
    unsigned short iFA;
    const TCHAR *szFA;
    const TCHAR StDUEV;
} FAListe;

#ifdef __cplusplus
extern "C" {
#endif
    extern const FAListe ListOfLand[];
    extern const FAListe ListOf10[];
    extern const FAListe ListOf11[];
    extern const FAListe ListOf21[];
    extern const FAListe ListOf22[];
    extern const FAListe ListOf23[];
    extern const FAListe ListOf24[];
    extern const FAListe ListOf26[];
    extern const FAListe ListOf27[];
    extern const FAListe ListOf28[];
    extern const FAListe ListOf30[];
    extern const FAListe ListOf31[];
    extern const FAListe ListOf32[];
    extern const FAListe ListOf40[];
    extern const FAListe ListOf41[];
    extern const FAListe ListOf51[];
    extern const FAListe ListOf52[];
    extern const FAListe ListOf53[];
    extern const FAListe ListOf91[];
    extern const FAListe ListOf92[];
    extern "C" const FAListe ListOfTestfinanzaemter[];
#ifdef __cplusplus
}
#endif

extern "C" const FAListe ListOfFinanzaemter[];