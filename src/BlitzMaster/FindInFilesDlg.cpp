// Generated by DialogBlocks (unregistered), 08/05/05 10:17:27

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "FindInFilesDlg.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "PreCompiled.h"


#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "FindInFilesDlg.h"

#include "BlitzMasterApp.h"
#include "MainFrame.h"
#include "ProjectDoc.h"
#include "FindThread.h"

#include <wx/tokenzr.h>


////@begin XPM images
////@end XPM images

/*!
 * FindInFilesDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FindInFilesDlg, wxDialog )

/*!
 * FindInFilesDlg event table definition
 */

BEGIN_EVENT_TABLE( FindInFilesDlg, wxDialog )

////@begin FindInFilesDlg event table entries
    EVT_BUTTON( ffID_BROWSE, FindInFilesDlg::OnBrowseClick )

    EVT_BUTTON( wxID_OK, FindInFilesDlg::OnFindClick )
    EVT_UPDATE_UI( wxID_OK, FindInFilesDlg::OnFindUpdate )

////@end FindInFilesDlg event table entries

END_EVENT_TABLE()

/*!
 * FindInFilesDlg constructors
 */

FindInFilesDlg::FindInFilesDlg( )
{
   m_FindThread = NULL;
}

FindInFilesDlg::FindInFilesDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
   m_FindThread = NULL;
   Create(parent, id, caption, pos, size, style);
}

/*!
 * FindInFilesDlg creator
 */

bool FindInFilesDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FindInFilesDlg member initialisation
    m_WhatCtrl = NULL;
    m_LookInCtrl = NULL;
    m_TypeCtrl = NULL;
    m_MatchCaseCtrl = NULL;
    m_WholeWordCtrl = NULL;
////@end FindInFilesDlg member initialisation

////@begin FindInFilesDlg creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end FindInFilesDlg creation

   return TRUE;
}

/*!
 * Control creation for FindInFilesDlg
 */

void FindInFilesDlg::CreateControls()
{    
////@begin FindInFilesDlg content construction
    FindInFilesDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_TOP|wxALL, 7);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_LEFT|wxTOP|wxBOTTOM, 2);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Fi&nd what:"), wxDefaultPosition, itemDialog1->ConvertDialogToPixels(wxSize(40, -1)), wxALIGN_LEFT );
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxADJUST_MINSIZE, 7);

    wxString* m_WhatCtrlStrings = NULL;
    m_WhatCtrl = new wxComboBox( itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, itemDialog1->ConvertDialogToPixels(wxSize(160, -1)), 0, m_WhatCtrlStrings, wxCB_DROPDOWN );
    itemBoxSizer4->Add(m_WhatCtrl, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer7, 0, wxALIGN_LEFT|wxTOP|wxBOTTOM, 2);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("&Look in:"), wxDefaultPosition, itemDialog1->ConvertDialogToPixels(wxSize(40, -1)), wxALIGN_LEFT );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxADJUST_MINSIZE, 7);

    wxString* m_LookInCtrlStrings = NULL;
    m_LookInCtrl = new wxComboBox( itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, itemDialog1->ConvertDialogToPixels(wxSize(160, -1)), 0, m_LookInCtrlStrings, wxCB_DROPDOWN );
    itemBoxSizer7->Add(m_LookInCtrl, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, ffID_BROWSE, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer7->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL, 5);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer11, 0, wxALIGN_LEFT|wxTOP|wxBOTTOM, 2);

    wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC, _("File &types:"), wxDefaultPosition, itemDialog1->ConvertDialogToPixels(wxSize(40, -1)), wxALIGN_LEFT );
    itemBoxSizer11->Add(itemStaticText12, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxADJUST_MINSIZE, 7);

    wxString* m_TypeCtrlStrings = NULL;
    m_TypeCtrl = new wxComboBox( itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, itemDialog1->ConvertDialogToPixels(wxSize(160, -1)), 0, m_TypeCtrlStrings, wxCB_DROPDOWN );
    itemBoxSizer11->Add(m_TypeCtrl, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer14, 0, wxALIGN_LEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer14->Add(itemBoxSizer15, 0, wxALIGN_CENTER_VERTICAL|wxTOP, 5);

    m_MatchCaseCtrl = new wxCheckBox( itemDialog1, ffID_MATCHCASE, _("Match &case"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
    m_MatchCaseCtrl->SetValue(false);
    itemBoxSizer15->Add(m_MatchCaseCtrl, 0, wxALIGN_LEFT|wxBOTTOM, 3);

    m_WholeWordCtrl = new wxCheckBox( itemDialog1, ffID_WHOLEWORD, _("Match &whole word"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
    m_WholeWordCtrl->SetValue(false);
    itemBoxSizer15->Add(m_WholeWordCtrl, 0, wxALIGN_RIGHT|wxTOP, 3);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer14->Add(itemBoxSizer18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer19, 0, wxALIGN_TOP|wxRIGHT|wxTOP, 7);

    wxButton* itemButton20 = new wxButton( itemDialog1, wxID_OK, _("&Find"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton20->SetDefault();
    itemBoxSizer19->Add(itemButton20, 0, wxALIGN_RIGHT|wxBOTTOM, 3);

    wxButton* itemButton21 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton21, 0, wxALIGN_RIGHT|wxALL, 0);

////@end FindInFilesDlg content construction

   // Set the types, lookin folders, and what list.
   AppPrefs::AddStringsToCombo( m_WhatCtrl, tsGetPrefs().GetFindStrings() );
   AppPrefs::AddStringsToCombo( m_LookInCtrl, tsGetPrefs().GetFindPaths() );
   AppPrefs::AddStringsToCombo( m_TypeCtrl, tsGetPrefs().GetFindTypes() );
   m_MatchCaseCtrl->SetValue( tsGetPrefs().GetFindMatchCase() );
   m_WholeWordCtrl->SetValue( tsGetPrefs().GetFindMatchWord() );
}

/*!
 * Should we show tooltips?
 */

bool FindInFilesDlg::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap FindInFilesDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin FindInFilesDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end FindInFilesDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FindInFilesDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin FindInFilesDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end FindInFilesDlg icon retrieval
}

void FindInFilesDlg::AddWhat( const wxString& what )
{
   wxASSERT( m_WhatCtrl );
   
   if ( what.IsEmpty() ) 
      return;

   // First remove it if it is already in the list.
   int Index = m_WhatCtrl->FindString( what );
   if ( Index != wxNOT_FOUND ) {
      m_WhatCtrl->Delete( Index );
   }

   // Insert it into the top of the list.
   m_WhatCtrl->Insert( what, 0 );

   // Make sure it's the active item.
   m_WhatCtrl->SetValue( what );
}

void FindInFilesDlg::AddLookIn( const wxString& path )
{
   wxASSERT( m_LookInCtrl );

   if ( path.IsEmpty() ) 
      return;

   // First remove it if it is already in the list.
   int Index = m_LookInCtrl->FindString( path );
   if ( Index != wxNOT_FOUND )
      m_LookInCtrl->Delete( Index );

   // Insert it into the top of the list.
   m_LookInCtrl->Insert( path, 0 );

   // Make sure it's the active item.
   m_LookInCtrl->SetValue( path );
}

void FindInFilesDlg::AddType( const wxString& type )
{
   wxASSERT( m_TypeCtrl );

   if ( type.IsEmpty() ) 
      return;

   // First remove it if it is already in the list.
   int Index = m_TypeCtrl->FindString( type );
   if ( Index != wxNOT_FOUND ) {
      m_TypeCtrl->Delete( Index );
   }

   // Insert it into the top of the list.
   m_TypeCtrl->Insert( type, 0 );

   // Make sure it's the active item.
   m_TypeCtrl->SetValue( type );
}

wxString FindInFilesDlg::GetWhat() const
{
   wxASSERT( m_WhatCtrl );
   return m_WhatCtrl->GetValue();
}

wxString FindInFilesDlg::GetLookIn() const
{
   wxASSERT( m_LookInCtrl );
   return m_LookInCtrl->GetValue();
}

wxString FindInFilesDlg::GetTypes() const
{
   wxASSERT( m_TypeCtrl );
   return m_TypeCtrl->GetValue();
}

wxArrayString FindInFilesDlg::GetPathsFromLookIn( const wxString& lookin ) const
{
   wxArrayString results;

   // TODO: Add <Open Files>.

   wxStringTokenizer toker( lookin, ";," );
   while ( toker.HasMoreTokens() ) 
   {
      wxString path = toker.NextToken();

      if ( path.CmpNoCase( "Project" ) == 0 ) 
      {            
         wxASSERT( tsGetMainFrame() );
         if ( !tsGetMainFrame()->GetProjectDoc() )
            continue;

         path = tsGetMainFrame()->GetProjectDoc()->GetWorkingDir();
      }
      else if ( path.CmpNoCase( "Base Directory" ) == 0 ) 
      {
         wxASSERT( tsGetMainFrame() );
         if ( !tsGetMainFrame()->GetProjectDoc() )
            continue;

         path = tsGetMainFrame()->GetProjectDoc()->GetWorkingDir();
      }

      results.Add( path );
   }

   return results;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void FindInFilesDlg::OnFindClick( wxCommandEvent& event )
{
   if ( m_FindThread->IsSearching() ) 
   {
      m_FindThread->Cancel();
      return;
   }

   // This is a hack so that we don't see
   // the flicker from the 'Add' calls
   // below... it should probably be fixed.
   Hide();

   // Add the current values back in.
   AddWhat( m_WhatCtrl->GetValue() );
   AddLookIn( m_LookInCtrl->GetValue() );
   AddType( m_TypeCtrl->GetValue() );

   // Store the new find parameters into the app prefs.
   wxArrayString strings;
   AppPrefs::GetStringsFromCombo( m_WhatCtrl, strings );
   if ( strings.GetCount() > 20 )
      strings.SetCount( 20 );
   tsGetPrefs().SetFindStrings( strings );

   strings.Empty();
   AppPrefs::GetStringsFromCombo( m_LookInCtrl, strings );
   AppPrefs::FixupFindPaths( strings );
   tsGetPrefs().SetFindPaths( strings );

   strings.Empty();
   AppPrefs::GetStringsFromCombo( m_TypeCtrl, strings );
   if ( strings.GetCount() > 20 )
      strings.SetCount( 20 );
   tsGetPrefs().SetFindTypes( strings );

   tsGetPrefs().SetFindMatchCase( m_MatchCaseCtrl->IsChecked() );
   tsGetPrefs().SetFindMatchWord( m_WholeWordCtrl->IsChecked() );

   event.Skip();
}

/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void FindInFilesDlg::OnFindUpdate( wxUpdateUIEvent& event )
{
   if ( m_FindThread->IsSearching() ) 
      event.SetText( "&Stop" );
   else
      event.SetText( "&Find" );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for tsID_BROWSE
 */

void FindInFilesDlg::OnBrowseClick( wxCommandEvent& event )
{
   wxString lookin = GetLookIn();
   wxArrayString paths = GetPathsFromLookIn( lookin );
   lookin.Empty();
   if ( !paths.IsEmpty() )
      lookin = paths[0];

   wxDirDialog dlg( this, "Select the folder to search.", lookin );
   dlg.SetTitle( "Look In" );
   if ( dlg.ShowModal() != wxID_OK )
      return;

   AddLookIn( dlg.GetPath() );
}


