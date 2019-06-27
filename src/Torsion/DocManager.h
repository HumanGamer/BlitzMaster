#pragma once

#include <wx/docview.h>

class wxFileHistory;


class DocManager : public wxDocManager
{
   public:
      DocManager();
      virtual ~DocManager();

      virtual void AddFileToHistory(const wxString& file);

      wxFileHistory* GetProjectHistory() const { return m_ProjectHistory; }

      wxDocTemplate* GetProjectTemplate() const { return m_ProjectTemplate; }

      void UpdateScriptFileTemplates();

   protected:

      wxDocTemplate* m_ScriptTemplate;
      wxDocTemplate* m_ProjectTemplate;
      wxFileHistory* m_ProjectHistory;
};
