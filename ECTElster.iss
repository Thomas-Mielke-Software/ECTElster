; Script generated by the Inno Setup Script Wizard.    Version 2 !!!
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=Elster-Export Plugin f�r EasyCash&Tax
AppVerName=Elster-Export 2.13
DiskSpanning=no
AppPublisher=tm
AppPublisherURL=http://www.easyct.de
AppSupportURL=http://www.easyct.de
AppUpdatesURL=http://www.easyct.de
DefaultDirName={pf}\EasyCash&Tax\Plugins\Elster-Export
DefaultGroupName=EasyCash
OutputBaseFilename=ECTElsterSetup
OutputDir=D:\Work\ECTElster\Setup
MinVersion=5.0
; MFC9 greift auf GetFileSizeEx zu, das Win 98 nicht in der kernel.dll hat.
Compression=bzip   
SignTool=winsdk81sha1   ; dual sign the 
SignTool=winsdk81sha256 ; installer

[Files]                                                                                                    
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\VC_redist.x86.exe"; DestDir: {tmp}; Flags: dontcopy    
;Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\msvcp140.dll"; DestDir: {app}; Flags: ignoreversion
;Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\msvcr140.dll"; DestDir: {app}; Flags: ignoreversion 

Source: D:\Work\ECTElster\Release\ECTElster.ocx; DestDir: {app}; Flags: regserver ignoreversion
Source: D:\Work\ECTElster\Elster.gif; DestDir: {app}; Flags: ignoreversion

Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\ericapi.dll"; DestDir: {app}; Flags: ignoreversion 
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\ericxerces.dll"; DestDir: {app}; Flags: ignoreversion
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\eSigner.dll"; DestDir: {app}; Flags: ignoreversion

Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\plugins2\checkUStVA_2014.dll"; DestDir: {app}\plugins2; Flags: ignoreversion   
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\plugins2\checkUStVA_2015.dll"; DestDir: {app}\plugins2; Flags: ignoreversion     
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\plugins2\checkUStVA_2016.dll"; DestDir: {app}\plugins2; Flags: ignoreversion     
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\plugins2\checkUStVA_2017.dll"; DestDir: {app}\plugins2; Flags: ignoreversion     
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\plugins2\checkUStVA_2018.dll"; DestDir: {app}\plugins2; Flags: ignoreversion     
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\plugins2\checkUStVA_2019.dll"; DestDir: {app}\plugins2; Flags: ignoreversion     
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\plugins2\checkUStVA_2020.dll"; DestDir: {app}\plugins2; Flags: ignoreversion
Source: "C:\Program Files (x86)\Elster\ERiC-31.4.2.0\Windows-x86\dll\plugins2\commonData.dll"; DestDir: {app}\plugins2; Flags: ignoreversion

Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-1.png; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-2.png; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-01.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-02.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-03.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-04.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-05.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-06.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-07.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-08.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-09.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-10.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-11.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-12.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-q1.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-q2.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-q3.ecf; DestDir: {app}\..\..; Flags: ignoreversion
Source: D:\Work\EasyCash\UVA\D-2019\uva-d-2019-q4.ecf; DestDir: {app}\..\..; Flags: ignoreversion

[Registry]
Root: HKLM; Subkey: Software\Tools; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: Software\Tools\EasyCash; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: Software\Tools\EasyCash\Plugins; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: Software\Tools\EasyCash\Plugins\Elster-Export; Flags: uninsdeletekey
Root: HKLM; Subkey: Software\Tools\EasyCash\Plugins\Elster-Export; ValueType: string; ValueName: Aufruf; ValueData: ECTELSTER.ECTElsterCtrl.1
Root: HKLM; Subkey: Software\Tools\EasyCash\Plugins\Elster-Export; ValueType: string; ValueName: Beschreibung; ValueData: �bertragung der Umsatzsteuervoranmeldung an Elster
Root: HKLM; Subkey: Software\Tools\EasyCash\Plugins\Elster-Export; ValueType: string; ValueName: Bitmap; ValueData: {app}\Elster.gif
Root: HKLM; Subkey: Software\Tools\EasyCash\Plugins\Elster-Export; ValueType: string; ValueName: Name; ValueData: Elster-Export
Root: HKLM; Subkey: Software\Tools\EasyCash\Plugins\Elster-Export; ValueType: string; ValueName: Typ; ValueData: noscroll

[Languages]
Name: default; MessagesFile: compiler:Languages\German.isl

;[Run]
;Filename: {tmp}\vcredist_x86.exe; Parameters: "/passive /norestart";

[Code]
function CheckProcessRunning( aProcName,
                              aProcDesc: string ): boolean;
var
  ShellResult: boolean;
  ResultCode: integer;
  cmd: string;
  sl: TStringList;
  f: string;
  d: string;
begin
  cmd := 'for /f "delims=," %%i ' + 
         'in (''tasklist /FI "IMAGENAME eq ' + aProcName + '" /FO CSV'') ' + 
         'do if "%%~i"=="' + aProcName + '" exit 1'; 
  f := 'CheckProc.cmd';
  d := AddBackSlash( ExpandConstant( '{tmp}' ));
  sl := TStringList.Create;
  sl.Add( cmd );
  sl.Add( 'exit /0' );
  sl.SaveToFile( d + f );
  sl.Free;
  Result := true;
  while ( Result ) do
  begin
    ResultCode := 1;
    ShellResult := Exec( f,
                         '',
                         d, 
                         SW_HIDE, 
                         ewWaitUntilTerminated, 
                         ResultCode );
    Result := ResultCode > 0;
    if Result and 
       ( MsgBox( aProcDesc + ' ist noch aktiv. Das Programm muss beendet werden, um fortzufahren.'#13#10#13#10 + 
                 'Bitte wechseln Sie zu dem Programm, schlie�en Sie es und dr�cken auf OK. Im Zweifelsfall den Windows Task Manager starten (�ber die Tastaturkombination Strg-Alt-Entf) und dort den Prozess "' + aProcName + '" beenden. ', 
                 mbConfirmation, 
                 MB_OKCANCEL ) <> IDOK ) then
      Break;
  end;
  DeleteFile( d + f );
end;

// Perform some initializations.  Return False to abort setup
function InitializeSetup: Boolean;
begin
  // Do not use any user defined vars in here such as {app}
  Result := not ( CheckProcessRunning( 'EasyCT.exe',      'EasyCash&Tax' ));
end;


function InitializeUninstall: Boolean;
begin
  Result := not ( CheckProcessRunning( 'EasyCT.exe',      'EasyCash&Tax' ));
end;

function PrepareToInstall(var NeedsRestart: Boolean): string;
var
  ExitCode: Integer;  
begin
  // a better way to do this would be like here: http://stackoverflow.com/questions/32474205/visual-studio-2015-link-against-mfc42/32524036#32524036
  ExtractTemporaryFile('VC_redist.x86.exe');
  // then run the redist from the temp folder; if that fails, return from this handler the error text
  if not Exec(ExpandConstant('{tmp}\VC_redist.x86.exe'), '/passive /norestart', '', SW_SHOW, ewWaitUntilTerminated, ExitCode) then
  begin
    // return the error text
    Result := 'Setup konnte die VC++ Laufzeitbibliothek nicht installieren. Fehlercode: ' + IntToStr(ExitCode);
    // exit this function; 
    Exit;
  end;
end; 