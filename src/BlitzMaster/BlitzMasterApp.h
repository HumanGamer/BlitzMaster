#pragma once

#include "AppPrefs.h"

class DocManager;
class Debugger;
class wxFileType;
class wxSingleInstanceChecker;
class TorsionDDEServer;
class AutoCompManager;
class wxDocTemplate;
//class UpdateChecker;


// Used to send the WM_COPYDATA message to an existing app.
#define COPYDATA_OPEN 554234


class BlitzMasterApp : public wxApp  
{
public:
	BlitzMasterApp();
	virtual ~BlitzMasterApp();

	virtual bool OnInit();
   virtual void CleanUp();
   virtual int OnExit();
   virtual bool ProcessEvent( wxEvent& event );

   virtual void OnFatalException();
   //virtual bool OnExceptionInMainLoop();

	Debugger*            GetDebugger() { return m_Debugger; }
	AutoCompManager*     GetAutoCompManager() { return m_AutoCompManager; }

   const AppPrefs& GetPreferences() const { return m_Prefs; }
   AppPrefs& GetPreferences()             { return m_Prefs; }

   bool CheckForUpdate( bool noUpdateMsg );

   void RegisterScriptExts();
   void UnregisterScriptExts( const wxArrayString& exts );

   void OnKeyDown( wxKeyEvent& event );

   const wxString & GetAppPath() const { return m_AppPath; }

protected:

   wxSingleInstanceChecker* m_InstChecker;
   wxSingleInstanceChecker* m_UserInstChecker;
   TorsionDDEServer* m_DDEServer;

   //UpdateChecker*    m_Checker;

   wxString          m_AppPath;

   AppPrefs          m_Prefs;
   DocManager*       m_DocManager;
	Debugger*		   m_Debugger;
   AutoCompManager*  m_AutoCompManager;

   DECLARE_EVENT_TABLE()
};

DECLARE_APP(BlitzMasterApp);

inline AppPrefs& tsGetPrefs() { return wxGetApp().GetPreferences(); }
inline Debugger* tsGetDebugger() { return wxGetApp().GetDebugger(); }
inline AutoCompManager* tsGetAutoComp() { return wxGetApp().GetAutoCompManager(); }
