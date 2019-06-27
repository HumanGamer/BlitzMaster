#pragma once


struct IContextMenu2;


class ShellMenu : public wxMenu
{
   public:

      ShellMenu( const wxString& path );
      virtual ~ShellMenu();

      WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

   protected:

      bool PopulateMenu();

      IContextMenu2*    m_ContextHandler;
      bool              m_IsInitialized;
      wxFileName        m_File;
};
