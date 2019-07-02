#pragma once

#include <wx/stackwalk.h>


class StackDump : protected wxStackWalker
{
public:

   StackDump();
   virtual ~StackDump();

   void Dump( wxString* address, wxString* data );

protected:

   virtual void OnStackFrame( const wxStackFrame& frame );

   wxString* m_Address;
   wxString* m_Data;
};
