#pragma once

#include <wx/laywin.h>


class tsSashLayoutWindow : public wxSashLayoutWindow
{
public:

   tsSashLayoutWindow(  wxWindow *parent, 
                        wxWindowID id = wxID_ANY, 
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize, 
                        long style = wxSW_3D|wxCLIP_CHILDREN, 
                        const wxString& name = wxT("layoutWindow") )
   {
      m_HasPainted = false;
      Create(parent, id, pos, size, style, name);
   }

protected:

   void OnPaint( wxPaintEvent& event );
   void OnEraseBackground( wxEraseEvent& event );

   bool m_HasPainted;

   DECLARE_EVENT_TABLE()
};
