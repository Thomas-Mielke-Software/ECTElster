# ECTElster - Elster-Plugin für die Finanzbuchhaltung EasyCash&Tax

## Voraussetzungen:
- VS2017 Community Edition
    - mit Visual C++ ATL für x86 und x64
    - mit Visual C++ MFC für x86 und x64
    - inkl. einer passenden Windows SDK
- aktuelle ERiC-SDK
    - downloaden unter https://www.elster.de/elsterweb/infoseite/entwickler
    - vorzugsweise in *"C:\Program Files (x86)\Elster"* installieren
    - ericapi.lib und .h Dateien in das Solution-Verzeichnis kopieren
    - siehe unten bei *ERiC Update Checkliste*


## Bauen:
- ECTElster.sln in VS2017 öffnen
- Bei den Projekteigenschaften -> Konfigurationseigenschaften -> Bibliotheksverzeichnisse sicherstellen, dass die ein geeignetes SDK für die ucrt library enthalten ist, z.B. *"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x86"*
- gleiches unter C/C++ -> Allgemein -> zusätzliche Header-Dateien, z.B. *"C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt"*
- wenn man Visual Studio nicht ständig als Admin laufen lassen will, kann es die .ocx-Datei nicht während des Build-Vorgangs mit registrieren. Das ist nicht schlimm. Man kann es einmal manuell mit regsvr32.exe ECTElster.ocx im Debug-Verzeichnis machen und dann den Fehler ignorieren.

## Debuggen:
ericapi.dll, ericxerces.dll und eSigner.dll aus dem ERiC-SDK ( z.B. C:\Program Files (x86)\Elster\ERiC-29.2.6.0\Windows-x86\dll ) sowie das gesamte plugins2-Verzeichnis in das Debug-Unterverzeichnis kopieren. Dort auch Kopien der Dateien des EC&T-Hauptprogramms ablegen (EasyCT.exe, EasyCTXP.dll, ECTIFace.dll, CrashSender1402.exe und crashrpt_lang.ini werden benötigt)

## Verpacken:
- InnoSetup herunterladen ( http://www.jrsoftware.org/isdl.php )
- InnoSetup starten
- ECTElster.iss öffnen
- ggf. sigenes Software-Signaturzertifikat bei *SignTool=* anpassen
- ggf. Pfade und *AppVerName=* anpassen

## Hinweise zum Hacken:
Die eigentliche Musik spielt in ElsterDlg.cpp. Es wird dort im Kern eine ListCtrl Abwandlung namens *QuickList* verwendet und darin der Aufbau des Formulars nachgeahmt. Wenn eine Validierung via ERiC-API erfolgreich ist, kann es mit dieser API versendet werden. 

## ERiC Update Checkliste

1) aktuelle Version des ERiC-SDK downloaden (wofür man erst einmal ein Passwort anfragen muss): https://www.elster.de/elsterweb/entwickler/infoseite/eric (als entwickler einloggen)
2) common.zip nach C:\Programme (x86)\Elster entpacken
3) windows-x86.jar mit 7zip öffnen, Setup.exe entpacken & starten (wenn vorhanden, ab v34 wohl nicht mehr)
4) Windows-x86\dll Verzeichnis mit Vorversion abgleichen (manchmal gibt es neue DLLs, die eingebunden werden müssen oder aber DLLs fallen weg)
5) Windows-x86\dll\plugins2 Verzeichnis checkUStVA_20XX.dll und commonData.dll mit Vorversion abgleichen (oft sind nur die neuen Jahres-Versionen der DLLs zu ergänzen, manchmal gibt es aber auch neue DLLs, die eingebunden werden müssen oder aber DLLs fallen weg)
6) Änderungen in ECTElster.iss anpassen und Pfad ändern (replace all "C:\Program Files (x86)\Elster\ERiC-##.#.#.#\", neue checkUStVA_20XX.dll nicht vergessen!)
7) VC_redist.x86.exe aus dem alten ERiC-Basisverzeichnis ins neue kopieren (da speichere ich es zumindest immer zu Archivzwecken, ansonsten von Microsoft herunterladen)
8) checken, ob eriapi.dll immer noch die VC2017 runtime version benutzt (steht in common\ERiC-Entwicklerhandbuch.pdf, in ERiC v31 auf Seite 23 im Abschnitt "2.4.5 Mindestanforderungen an benötigte Software"; im Zweifel Suche im PDF nach "Visual C++ Redistributable")
9) Windows-x86\lib\ericapi.lib nach ECTElster kopieren
10) Windows-x86\include\\*.h nach ECTElster kopieren
11) Version in den ECTElster-Ressouces und der .iss hochsetzen, copyright in About-Dialog aktualisieren
12) #define TESTVERBINDUNG in ElsterDlg.cpp -- Kommentarzeichen entfernen
13) ECTElster.sln mit Debug-Konfiguration neu bauen, ggf. Compilerfehler beseitigen (Abwärtskompatibilität ist nicht vorgesehen, es kann jedesmal Überraschungen geben; common\ERiC-Aenderungsdokumentation.pdf ist dann ein Blick wert)
14) Test-Datei mit aktuellem Buchungsjahr übertragen
15) // #define TESTVERBINDUNG -- wieder auskommentieren, um Plugin scharf zu schalten
16) ECTElster.sln mit Release-Konfiguration neu bauen
17) .iss bauen und installieren

## Credits:
Dank gebührt Olaf Stüben von Winston, der bei der ersten Version des Plugins behilflich war. Außerdem wird folgende Software vom Plugin verwendet:
- ERiC-API der bayrischen Finanzverwaltung
- XMLite: leichtgewichtige Klassen für XML-Parsing
- CRegExp: Klasse für reguläre Ausdrücke von Henry Spencer, CodeGuru-Post durch Zafir Anjum 
- QuickList: CListCtrl replacement von PEK
