
rem  This builds the docs in %WXWIN% in a number of formats 
rem  and a clean inno setup in a second tree
rem  it uses a number of tools nmake, gnuwin32 zip, ghostscript, MS word, cvsNT
rem  cvs is in the path already from CVSNT install
rem  writes a log file in c:\

echo Building wxWidgets docs... > c:\temp.log

set WXWIN=c:\wx\wxWidgets
set DAILY=c:\daily
set PATH=%PATH%;C:\wx\wxWidg~1.3\utils\tex2rtf\src\vc_based;C:\wx\GnuWin32\bin;c:\progra~1\htmlhe~1;C:\PROGRA~1\INNOSE~1
set PATH=%PATH%;C:\Program Files\gs\gs8.51\lib;C:\Program Files\gs\gs8.51\bin
echo %PATH% >> c:\temp.log

rem update wxwidgets (holds docs) and inno (cvs wxMSW module only)
c:
cd %WXWIN%
cvs up -P -d
cd \wx\inno\wxWidgets
cvs up -P
echo CVS update  >>  c:\temp.log


rem add nmake to the path and build the docs
call  \vc6
echo %PATH% >>  c:\temp.log
SET >>  c:\temp.log
cd %WXWIN%\build\script
nmake -f makedocs.vc cleandocs
nmake -f makedocs.vc alldocs

del %DAILY%\in\*.p*
echo starting word >>  c:\temp.log
start /WAIT winword /mwx_ps


echo cvs doc up part 2 >>  c:\temp.log

rem use ghostscript ps2pdf - add extra path first
rem set PATH=%PATH%;C:\Program Files\gs\gs8.51\lib;C:\Program Files\gs\gs8.51\bin
rem set PATH=%PATH%;C:\wx\GnuWin32\bin;C:\PROGRA~1\INNOSE~1

cd %DAILY%\in
call ps2pdf wx.ps >>  c:\temp.log
call ps2pdf fl.ps >> c:\temp.log
call ps2pdf gizmos.ps >> c:\temp.log
call ps2pdf mmedia.ps >> c:\temp.log
call ps2pdf ogl.ps >> c:\temp.log
call ps2pdf svg.ps >> c:\temp.log
call ps2pdf tex2rtf.ps >> c:\temp.log

cd %WXWIN%\build\script
iscc wxwidgets.iss >> c:\temp.log


echo Zipping
cd %WXWIN%
del %DAILY%\*.zip
zip %DAILY%\wx-docs-chm.ZIP docs\htmlhelp\wx.chm
zip %DAILY%\wx-docs-pdf.ZIP %DAILY%\in\wx.pdf
zip %DAILY%\wx-docs-hlp.ZIP docs\winhelp\wx.hlp docs\winhelp\wx.cnt
zip %DAILY%\wx-docs-extra-hlp.ZIP utils/tex2rtf/docs/*.HLP utils/tex2rtf/docs/*.cnt docs/winhelp/*.hlp docs/winhelp/*.cnt -x  docs/winhelp/wx.hlp docs/winhelp/wx.*
zip %DAILY%\wx-docs-extra-chm.ZIP utils/tex2rtf/docs/*.chm docs/htmlhelp/*.chm 
cd %DAILY%\in
zip %DAILY%\wx-docs-extra-pdf.ZIP *.pdf -x wx.pdf


echo setting S
rem echo yes > net use s: /delete
net use s: \\biolpc22\bake 

copy %DAILY%\*.ZIP s:\bkl-cronjob\archives\win
copy %DAILY%\*.exe s:\bkl-cronjob\archives\win\*.EXE



