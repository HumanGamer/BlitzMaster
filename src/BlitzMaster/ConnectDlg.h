#pragma once

#include <wx/dialog.h>


class ConnectDlg : public wxDialog
{
   protected:

      wxSocketClient**   m_Server;
      wxIPV4address*    m_Address;
      wxString          m_Password;
      bool              m_Connecting;

   public:

      ConnectDlg( wxWindow* parent, wxSocketClient** server, wxIPV4address* address, const wxString& password, bool connectNow );

      enum {
         ID_CONNECT = wxID_HIGHEST,
         ID_SOCKET_EVENT,
      };

      void OnInitDialog( wxInitDialogEvent& event );
      void OnConnect( wxCommandEvent& event );
      void OnCancel( wxCommandEvent& event );

      void OnSocketEvent( wxSocketEvent& event );

      const wxString& GetPassword() const { return m_Password; }

   protected:

      bool                 m_ConnectNow;
      wxTextCtrl*          m_AddressCtrl;
      wxTextCtrl*          m_PortCtrl;
      wxTextCtrl*          m_PasswordCtrl;

      DECLARE_EVENT_TABLE()
};
