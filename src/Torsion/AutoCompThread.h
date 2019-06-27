#pragma once

#include "AutoCompText.h"
#include "AutoCompPage.h"
#include "DirWatcher.h"
#include "ScriptScanner.h"

class AutoCompData;
class AutoCompExports;


class AutoCompThread : public wxThread
{
public:

   AutoCompThread();
   ~AutoCompThread();

   void SetPath( const wxString& path, const wxArrayString& mods, const wxArrayString& exts );
   void SetExports( AutoCompExports* exports );
   AutoCompText* AddActivePage( const wxString& path );
   void RemoveActivePage( AutoCompText* text );

protected:


   virtual void *Entry();
   virtual void OnExit();

   void AddPage( AutoCompPage* page );
   void RebuildCompData( AutoCompExports* exports );
   bool ScanPath( const wxString& path, const wxString& basePath, const wxArrayString& mods, const wxArrayString& scriptExts );
   void Clear();

   static AutoCompPage* ScanFile( const wxString& path, ScriptScanner& sc );

protected:

   wxCriticalSection    m_DataLock;

   ScriptScanner        m_Scanner;

   wxString             m_ProjectPath;
   wxArrayString        m_Mods;
   wxArrayString        m_Excluded;
   wxArrayString        m_ScriptExts;

   bool                 m_UpdateExports;
   AutoCompExports*     m_NewExports;
   AutoCompExports*     m_Exports;

   AutoCompPageArray    m_Pages;
   AutoCompTextMap      m_ActiveMap;
   DirWatcher           m_DirWatcher;

   AutoCompData*        m_Data;

};
