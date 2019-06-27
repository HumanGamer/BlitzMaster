#pragma once

#include <wx/stc/stc.h>
#include <wx/regex.h>
#include <vector>


struct ScriptError
{
   wxString file;
   int line;
   wxString error;
   bool warning;

   int start;
   int end;
   int row;
};

typedef std::vector<ScriptError*> ErrorArray;


class OutputCtrl : public wxStyledTextCtrl
{
   public:

      OutputCtrl( wxWindow* parent, wxWindowID id, const wxString& name, 
                  const wxPoint& pos, const wxSize& size, long style );
      virtual ~OutputCtrl();

   public:

      void AppendText( const wxString& text );
      void Clear();

      void ShowLine( int line );

      void OnDblClick( wxMouseEvent& event );
      void OnContextMenu( wxContextMenuEvent& event );

      int GetSelected() const { return m_Selected; }

      void OnGotoFile( wxCommandEvent& event );
      void OnUpdateGotoFile( wxUpdateUIEvent& event );

      void OnCopy( wxCommandEvent& event );
      void OnUpdateCopy( wxUpdateUIEvent& event );

      void  ClearErrors();
      const ErrorArray& GetErrors() const { return m_Errors; }

      void UpdatePrefs();

      /*
      void OnClearAll( wxCommandEvent& event );
      void OnUpdateClearAll( wxUpdateUIEvent& event );
      void OnCopyLines( wxCommandEvent& event );
      void OnUpdateCopyLines( wxUpdateUIEvent& event );
      void OnStopFind( wxCommandEvent& event );
      void OnUpdateStopFind( wxUpdateUIEvent& event );
      */
      
      void ActivateLine( int line, bool openFile );

   protected:

      void  AddError( ScriptError* error );

      //virtual WXDWORD MSWGetStyle(long style, WXDWORD *exstyle) const;

      ErrorArray  m_Errors;

      wxRegEx m_ErrorExpr;
      wxRegEx m_WarnExpr;
      int m_Selected;

   DECLARE_EVENT_TABLE()
};
