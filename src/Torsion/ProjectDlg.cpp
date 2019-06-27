// Generated by DialogBlocks (unregistered), 31/01/2006 21:22:37

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ProjectDlg.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "AboutDlg.h"
////@end includes

#include "PreCompiled.h"
#include "ProjectDlg.h"
#include "ConfigDlg.h"

////@begin XPM images
////@end XPM images

/*!
 * ProjectDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ProjectDlg, wxDialog )

/*!
 * ProjectDlg event table definition
 */

BEGIN_EVENT_TABLE( ProjectDlg, wxDialog )

////@begin ProjectDlg event table entries
   EVT_LISTBOX_DCLICK( pdID_CONFIGLIST, ProjectDlg::OnConfigDoubleClicked )

   EVT_BUTTON( pdID_EDIT, ProjectDlg::OnEditClick )
   EVT_UPDATE_UI( pdID_EDIT, ProjectDlg::OnConfigSelUpdate )

   EVT_BUTTON( pdID_NEW, ProjectDlg::OnNewClick )

   EVT_BUTTON( pdID_DELETE, ProjectDlg::OnDeleteClick )
   EVT_UPDATE_UI( pdID_DELETE, ProjectDlg::OnConfigSelUpdate )

   EVT_BUTTON( pdID_COPY, ProjectDlg::OnCopyClick )
   EVT_UPDATE_UI( pdID_COPY, ProjectDlg::OnConfigSelUpdate )

   EVT_BUTTON( wxID_OK, ProjectDlg::OnOkClick )

////@end ProjectDlg event table entries

END_EVENT_TABLE()

/*!
 * ProjectDlg constructors
 */

ProjectDlg::ProjectDlg( )
{
}

ProjectDlg::ProjectDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * ProjectDlg creator
 */

bool ProjectDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ProjectDlg member initialisation
   m_PropGrid = NULL;
   m_ConfigList = NULL;
////@end ProjectDlg member initialisation

////@begin ProjectDlg creation
   SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
   wxDialog::Create( parent, id, caption, pos, size, style );

   CreateControls();
   Centre();
////@end ProjectDlg creation
    return true;
}

/*!
 * Control creation for ProjectDlg
 */

void ProjectDlg::CreateControls()
{    
////@begin ProjectDlg content construction
   ProjectDlg* itemDialog1 = this;

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
   itemDialog1->SetSizer(itemBoxSizer2);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
   itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 7);

   wxNotebook* itemNotebook4 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_TOP );

   wxPanel* itemPanel5 = new wxPanel( itemNotebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
   wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
   itemPanel5->SetSizer(itemBoxSizer6);

   wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
   itemBoxSizer6->Add(itemBoxSizer7, 1, wxGROW|wxALL, 7);
   m_PropGrid = new wxPropertyGridManager( itemPanel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED | wxPG_DESCRIPTION | wxPG_TOOLTIPS | wxTAB_TRAVERSAL | wxPG_STATIC_LAYOUT );
   itemBoxSizer7->Add(m_PropGrid, 1, wxGROW, 0);

   itemNotebook4->AddPage(itemPanel5, _("&Project"));

   wxPanel* itemPanel9 = new wxPanel( itemNotebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
   wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
   itemPanel9->SetSizer(itemBoxSizer10);

   wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
   itemBoxSizer10->Add(itemBoxSizer11, 1, wxGROW|wxALL, 7);
   wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer11->Add(itemBoxSizer12, 1, wxGROW, 7);
   wxString* m_ConfigListStrings = NULL;
   m_ConfigList = new wxListBox( itemPanel9, pdID_CONFIGLIST, wxDefaultPosition, wxDefaultSize, 0, m_ConfigListStrings, wxLB_SINGLE );
   itemBoxSizer12->Add(m_ConfigList, 1, wxGROW, 5);

   wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxVERTICAL);
   itemBoxSizer12->Add(itemBoxSizer14, 0, wxALIGN_TOP|wxLEFT, 7);
   wxButton* itemButton15 = new wxButton( itemPanel9, pdID_EDIT, _("&Edit..."), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(itemButton15, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 4);

   wxButton* itemButton16 = new wxButton( itemPanel9, pdID_NEW, _("&New..."), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(itemButton16, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 4);

   wxButton* itemButton17 = new wxButton( itemPanel9, pdID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(itemButton17, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 4);

   wxButton* itemButton18 = new wxButton( itemPanel9, pdID_COPY, _("&Copy"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(itemButton18, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 4);

   itemNotebook4->AddPage(itemPanel9, _("&Configurations"));

   itemBoxSizer3->Add(itemNotebook4, 1, wxGROW, 0);

   wxStdDialogButtonSizer* itemStdDialogButtonSizer19 = new wxStdDialogButtonSizer;

   itemBoxSizer3->Add(itemStdDialogButtonSizer19, 0, wxALIGN_RIGHT|wxTOP, 7);
   wxButton* itemButton20 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
   itemButton20->SetDefault();
   itemStdDialogButtonSizer19->AddButton(itemButton20);

   wxButton* itemButton21 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer19->AddButton(itemButton21);

   itemStdDialogButtonSizer19->Realize();

////@end ProjectDlg content construction

   wxASSERT( m_PropGrid );
   wxASSERT( m_PropGrid->GetGrid() );

   // TODO: Fix to refresh colors on system color change!
   wxColor active( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE  ) );
   wxFont font( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Tahoma" );
   m_PropGrid->GetGrid()->SetFont( font );
   m_PropGrid->GetGrid()->SetMarginColour( active );
   m_PropGrid->GetGrid()->SetLineColour( active );
   m_PropGrid->GetGrid()->SetCaptionBackgroundColour( active );
   m_PropGrid->GetGrid()->SetCaptionForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
   m_PropGrid->SetDescBoxHeight( 60 );

   //m_PropGrid->SetOwnBackgroundColour( itemNotebook4->GetThemeBackgroundColour() );
}

/*!
 * Should we show tooltips?
 */

bool ProjectDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ProjectDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ProjectDlg bitmap retrieval
   wxUnusedVar(name);
   return wxNullBitmap;
////@end ProjectDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ProjectDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ProjectDlg icon retrieval
   wxUnusedVar(name);
   return wxNullIcon;
////@end ProjectDlg icon retrieval
}

int ProjectDlg::ShowModal( ProjectDoc* projectDoc )
{
   wxASSERT( projectDoc );

   m_Project = projectDoc;

   m_ConfigModified = false;
   m_RebuildExports = false;
   m_Configs = m_Project->GetConfigs();

   // Fill all the controls.
   wxASSERT( m_PropGrid );
   m_PropGrid->AddPage();

   // Add the general properties.
   m_PropGrid->Append( wxPropertyCategory(wxT("General")) );
   wxPGId id = m_PropGrid->Append( wxStringProperty(wxT("Name"), wxPG_LABEL, m_Project->GetName()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The defaul internal project name and default project file name.") );

   id = m_PropGrid->Append( wxDirProperty(wxT("Base Directory"), wxPG_LABEL, m_Project->GetWorkingDir()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("Typically the folder where the executable and entry script are located.") );
   id = m_PropGrid->Append( wxStringProperty(wxT("Entry Script"), wxPG_LABEL, m_Project->GetEntryScript()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The first script loaded by the executable.  Normally this is main.cs.") );
   id = m_PropGrid->Append( wxStringProperty(wxT("Script Scanner Extensions"), wxPG_LABEL, m_Project->GetScannerExtsString()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("A semicolon separated list of script file extensions the code completion scanner should process when generating the ScriptSense database.") );
   id = m_PropGrid->Append( wxStringProperty(wxT("Mods"), wxPG_LABEL, m_Project->GetModsString()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("An optional semicolon separated list of mods to include in the project tree and scan for inclusion in the ScriptSense database.") );

   m_PropGrid->Append( wxPropertyCategory(wxT("Debugging")) );
   id = m_PropGrid->Append( wxStringProperty(wxT("Address"), wxPG_LABEL, m_Project->GetAddress()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The IP address or DNS name of the debug target machine.  You normally use 127.0.0.1 for debugging on the local machine.") );
   id = m_PropGrid->Append( wxStringProperty(wxT("Password"), wxPG_LABEL, m_Project->GetPassword()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The password to use when connecting to the debug target.") );
   id = m_PropGrid->Append( wxIntProperty(wxT("Port"), wxPG_LABEL, m_Project->GetPort()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The port on the debug target machine (6060 is the default).") );
   id = m_PropGrid->Append( wxBoolProperty(wxT("Reload Modified Scripts"), wxPG_LABEL, m_Project->ExecModifiedScripts()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("When enabled and supported by the engine you will be automatically prompted to reload changed scripts after continuing from a breakpoint.") );
   id = m_PropGrid->Append( wxStringProperty(wxT("Hook Code"), wxPG_LABEL, m_Project->GetDebugHook()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The debugger startup hook code that will be injected into the entry script.  The strings #port# and #password# will be replaced with the correct values from the connection dialog.") );

   m_PropGrid->Append( wxPropertyCategory(wxT("Elixir")) );
   id = m_PropGrid->Append( wxStringProperty(wxT("Search URL"), wxPG_LABEL, m_Project->GetSearchUrl()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The url to the Elixir search script.") );
   id = m_PropGrid->Append( wxStringProperty(wxT("Product"), wxPG_LABEL, m_Project->GetSearchProduct()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The Elixir product string.  This is typically TGE, TGEA, or TGB.") );
   id = m_PropGrid->Append( wxStringProperty(wxT("Version"), wxPG_LABEL, m_Project->GetSearchVersion()) );
   m_PropGrid->SetPropertyHelpString( id, wxT("The product version.  Use HEAD for the latest product documentation.") );
   
   // Let's use checkboxes instead of a choice list.
   m_PropGrid->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,(long)1);

   wxASSERT( m_PropGrid->GetGrid() );
   m_PropGrid->GetGrid()->SetSplitterLeft();
   m_PropGrid->SelectProperty( wxT("General.Name") );

   UpdateConfigList();

   return wxDialog::ShowModal();
}

void ProjectDlg::UpdateConfigList()
{
   wxASSERT( m_ConfigList );
   wxASSERT( m_Project );

   int sel = m_ConfigList->GetSelection();
   m_ConfigList->Clear();

   for ( int i=0; i < m_Configs.GetCount(); i++ ) 
      m_ConfigList->Append( m_Configs[i].GetName(), (void*)i );

   if ( sel > (m_ConfigList->GetCount()-1) )
      sel = m_ConfigList->GetCount() - 1;
   if ( sel < 0 && m_ConfigList->GetCount() > 0 )
      sel = 0;

   if ( sel >= 0 )
      m_ConfigList->SetSelection( sel );
}

/*!
 * wxEVT_UPDATE_UI event handler for pdID_EDIT
 */

void ProjectDlg::OnConfigSelUpdate( wxUpdateUIEvent& event )
{
   wxASSERT( m_ConfigList );
   int sel = m_ConfigList->GetSelection();
   event.Enable( sel > -1 );
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void ProjectDlg::OnOkClick( wxCommandEvent& event )
{
   wxASSERT( m_PropGrid );

   // If nothing in the grid changed then
   // return canceled.
   if ( !m_PropGrid->IsAnyModified() && !m_ConfigModified )
   {
      EndModal( wxID_CANCEL );
      return;
   }

   wxString name     = m_PropGrid->GetPropertyValue( "General.Name" );
   wxString baseDir  = m_PropGrid->GetPropertyValue( "General.Base Directory" );

   // Verify we got everything.
   if ( name.IsEmpty() ) {
      wxMessageDialog dlg( this, "You must enter a name for your project!", GetTitle(), wxOK );
      dlg.ShowModal();
      return;
   }
   if ( baseDir.IsEmpty() ) {
      wxMessageDialog dlg( this, "You must enter a base directory for your project!", GetTitle(), wxOK );
      dlg.ShowModal();
      return;
   }
   if ( !wxFileName::DirExists( baseDir ) )
   {
      wxMessageDialog dlg( this, "The base directory must exist!", GetTitle(), wxOK );
      dlg.ShowModal();
      return;
   }
   if ( m_Configs.IsEmpty() ) {
      wxMessageDialog dlg( this, "You must have at least one configuration for your project!", GetTitle(), wxOK );
      dlg.ShowModal();
      return;
   }

   // Apply all the settings now.
   wxASSERT( m_Project );
   m_Project->SetName( name );
   m_Project->SetWorkingDir( baseDir );
   m_Project->SetEntryScript( m_PropGrid->GetPropertyValue( "General.Entry Script" ) );
   m_Project->SetScannerExtsString( m_PropGrid->GetPropertyValue( "General.Script Scanner Extensions" ) );
   m_Project->SetModsString( m_PropGrid->GetPropertyValue( "General.Mods" ) );

   m_Project->SetAddress( m_PropGrid->GetPropertyValue( "Debugging.Address" ) );
   m_Project->SetPort( m_PropGrid->GetPropertyValue( "Debugging.Port" ).GetLong() );
   m_Project->SetPassword( m_PropGrid->GetPropertyValue( "Debugging.Password" ) );
   m_Project->SetExecModifiedScripts( m_PropGrid->GetPropertyValueAsBool( "Debugging.Reload Modified Scripts" ) );
   m_Project->SetDebugHook( m_PropGrid->GetPropertyValue( "Debugging.Hook Code" ) );
   
   m_Project->SetSearchUrl( m_PropGrid->GetPropertyValue( "Elixir.Search URL" ) );
   m_Project->SetSearchProduct( m_PropGrid->GetPropertyValue( "Elixir.Product" ) );
   m_Project->SetSearchVersion( m_PropGrid->GetPropertyValue( "Elixir.Version" ) );
   m_Project->SetConfigs( m_Configs );

   wxDialog::OnOK( event );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for pdID_EDIT
 */

void ProjectDlg::OnEditClick( wxCommandEvent& event )
{
   wxASSERT( m_ConfigList );
   int sel = m_ConfigList->GetSelection();
   wxASSERT( sel != -1 );
   sel = (int)m_ConfigList->GetClientData( sel );

   wxASSERT( sel >= 0 && sel < m_Configs.GetCount() );
   ProjectConfig* config = &m_Configs[sel];

   wxString name = config->GetName();
   bool hasExports = config->HasExports();
   wxString exe = config->GetExe();

   // Spawn the new config dialog.
   ConfigDlg dlg( this );
   if ( dlg.ShowModal( 
      config, 
      &m_Configs, 
      m_PropGrid->GetPropertyValue( "General.Base Directory" ) ) == wxID_OK ) 
   {
      m_ConfigModified = true;
      m_RebuildExports =   config->GetName() != name ||
                           config->HasExports() != hasExports ||
                           config->GetExe() != exe;
      UpdateConfigList();
   }
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for pdID_NEW
 */

void ProjectDlg::OnNewClick( wxCommandEvent& event )
{
   // Spawn the new config dialog.
   ProjectConfig config;

   wxASSERT( m_PropGrid );

   ConfigDlg dlg( this );
   if ( dlg.ShowModal(  
      &config, 
      &m_Configs, 
      m_PropGrid->GetPropertyValue( "General.Base Directory" ) ) == wxID_OK ) 
   {
      m_Configs.Add( config );
      m_ConfigModified = true;
      m_RebuildExports = true;
      UpdateConfigList();
   }
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for pdID_DELETE
 */

void ProjectDlg::OnDeleteClick( wxCommandEvent& event )
{
   wxASSERT( m_ConfigList );
   int sel = m_ConfigList->GetSelection();
   wxASSERT( sel != -1 );
   sel = (int)m_ConfigList->GetClientData( sel );

   wxASSERT( sel >= 0 && sel < m_Configs.GetCount() );
   m_Configs.RemoveAt( sel );

   m_ConfigModified = true;
   m_RebuildExports = true;
   UpdateConfigList();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for pdID_COPY
 */

void ProjectDlg::OnCopyClick( wxCommandEvent& event )
{
   wxASSERT( m_ConfigList );
   int sel = m_ConfigList->GetSelection();
   wxASSERT( sel != -1 );
   sel = (int)m_ConfigList->GetClientData( sel );

   wxASSERT( sel >= 0 && sel < m_Configs.GetCount() );
   ProjectConfig config = m_Configs[sel];
   config.SetName( "" );

   // Spawn the new config dialog.
   wxASSERT( m_PropGrid );
   ConfigDlg dlg( this );
   if ( dlg.ShowModal(  
      &config, 
      &m_Configs, 
      m_PropGrid->GetPropertyValue( "General.Base Directory" ) ) == wxID_OK ) 
   {
      m_Configs.Add( config );
      m_ConfigModified = true;
      m_RebuildExports = true;
      UpdateConfigList();
   }
}


/*!
 * wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event handler for wxID_ANY
 */

void ProjectDlg::OnConfigDoubleClicked( wxCommandEvent& event )
{
   OnEditClick( event );
}


