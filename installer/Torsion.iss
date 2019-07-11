; BlitzMaster IDE - Copyright (C) Sickhead Games, LLC
; This file is subject to the terms and conditions defined in
; file 'LICENSE.txt', which is part of this source code package.

[Setup]
AppName=BlitzMaster
AppVerName=BlitzMaster 1.0.0000
AppPublisher= Blitz Community
AppPublisherURL=http://www.sickheadgames.com/
AppSupportURL=https://github.com/HumanGamer/BlitzMaster/issues
AppUpdatesURL=https://github.com/HumanGamer/BlitzMaster
AppCopyright=Copyright (C) Blitz Community & Sickhead Games, LLC
AppMutex=BlitzMaster.BlitzCommunity
UsePreviousAppDir=yes
DefaultGroupName=BlitzMaster
DefaultDirName={pf}\BlitzMaster
DisableProgramGroupPage=yes
LicenseFile=..\LICENSE.txt
InfoAfterFile=PostInstall.txt
Compression=lzma/ultra
SolidCompression=yes
OutputDir=..\
OutputBaseFilename=InstallBlitzMaster
EnableDirDoesntExistWarning=no
DirExistsWarning=no
AppendDefaultDirName=no
ChangesAssociations=yes
SetupIconFile=..\src\BlitzMaster\icons\torsion.ico
WizardImageFile=installerimage.bmp
WizardSmallImageFile=installerimagesmall_a.bmp

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
;Source: "..\media\blitzmaster_p.exe"; DestName: "blitzmaster.exe";  DestDir: "{app}"; Flags: ignoreversion
Source: "..\media\blitzmaster.exe"; DestName: "blitzmaster.exe";  DestDir: "{app}"; Flags: ignoreversion
;Source: "..\media\msvcr71.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\media\msvcp71.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\media\blitzmaster.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\LICENSE.txt"; DestName: "BlitzMaster EULA.txt"; DestDir: "{app}"; Flags: ignoreversion

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[INI]
Filename: "{app}\Torsion Product Page.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.garagegames.com/products/106"
Filename: "{app}\Torsion Forums.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.garagegames.com/mg/forums/result.area.php?qa=55"

[Icons]
Name: "{group}\BlitzMaster"; Filename: "{app}\blitzmaster.exe"; WorkingDir: "{app}"; Comment: "BlitzMaster BlitzBasic IDE"
Name: "{group}\BlitzMaster Help"; Filename: "{app}\blitzmaster.chm"; WorkingDir: "{app}"; Comment: "BlitzMaster Help"
Name: "{group}\BlitzMaster Product Page"; Filename: "{app}\Torsion Product Page.url"; Comment: "BlitzMaster Product Page"
Name: "{group}\BlitzMaster Forums"; Filename: "{app}\Torsion Forums.url"; Comment: "BlitzMaster Forums"
Name: "{group}\{cm:UninstallProgram,Torsion}"; Filename: "{uninstallexe}"; Comment: "Uninstall BlitzMaster"
Name: "{userdesktop}\BlitzMaster"; Filename: "{app}\blitzmaster.exe"; Tasks: desktopicon; Comment: "BlitzMaster BlitzBasic IDE"

[Run]
Filename: "{app}\blitzmaster.exe"; Description: "{cm:LaunchProgram,BlitzMaster}"; Flags: nowait postinstall skipifsilent

; Let BlitzMaster register the script extensions itself.
Filename: "{app}\blitzmaster.exe"; WorkingDir: "{app}"; Parameters: "-exts"; Flags: waituntilterminated

[Registry]

; Add BlitzMaster to the App Paths key.
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\blitzmaster.exe"; ValueType: string; ValueData: "{app}\blitzmaster.exe"; Flags: uninsdeletevalue uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\blitzmaster.exe"; ValueName: "Path"; ValueType: string; ValueData: "{app}\"; Flags: uninsdeletevalue uninsdeletekeyifempty

; Associate .bproj to blitzmaster.exe
Root: HKCR; Subkey: ".bproj"; ValueType: string; ValueData: "BlitzMasterProject";
Root: HKCR; Subkey: "BlitzMasterProject"; ValueType: string; ValueData: "BlitzMaster Project File";
Root: HKCR; Subkey: "BlitzMasterProject\DefaultIcon"; ValueType: string; ValueData: "blitzmaster.exe,1"
Root: HKCR; Subkey: "BlitzMasterProject\shell\open\command"; ValueType: string; ValueData: "blitzmaster.exe ""%1"""

; BlitzMaster itself does the associations which point script extensions
; (.bb,.bmx,etc...) to BlitzMasterScript.
Root: HKCR; Subkey: "BlitzMasterScript"; ValueType: string; ValueData: "BlitzBasic Script";
Root: HKCR; Subkey: "BlitzMasterScript\DefaultIcon"; ValueType: string; ValueData: "blitzmaster.exe,2"
Root: HKCR; Subkey: "BlitzMasterScript\shell\open\command"; ValueType: string; ValueData: "blitzmaster.exe ""%1"""

[UninstallDelete]
Type: files; Name: "{app}\BlitzMaster Product Page.url"
Type: files; Name: "{app}\BlitzMaster Forums.url"
Type: files; Name: "{app}\preferences.xml"

[UninstallRun]
 
; Let BlitzMaster unregister the script extensions.
Filename: "{app}\blitzmaster.exe"; WorkingDir: "{app}"; Parameters: "-unexts"; Flags: waituntilterminated


