#pragma once

#include <wx/docmdi.h>


class ClientWindow : public wxMDIClientWindow
{
   DECLARE_CLASS(ClientWindow)

   public:

      ClientWindow();
      
      bool CreateClient( wxMDIParentFrame* parent, long style );

      void OnPaint( wxPaintEvent& event );
      void OnSize( wxSizeEvent& event );

   protected:

   DECLARE_EVENT_TABLE()
};
