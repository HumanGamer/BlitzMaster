#pragma once


#include <wx/dir.h>

class FindResultsCtrl;
class FindThreadInternal;


DECLARE_EVENT_TYPE(tsEVT_FIND_STATUS, -1)
DECLARE_EVENT_TYPE(tsEVT_FIND_OUTPUT, -1)

class FindThread 
{
public:
   FindThread();

   bool Find(  wxEvtHandler* handler,
               const wxString& what,
               const wxArrayString& paths,
               const wxString& types,
               bool MatchCase,
               bool MatchWord );

   bool IsSearching();

   void Cancel();

protected:

   FindThreadInternal* m_Thread;
};
