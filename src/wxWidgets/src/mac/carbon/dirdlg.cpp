/////////////////////////////////////////////////////////////////////////////
// Name:        dirdlg.cpp
// Purpose:     wxDirDialog
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: dirdlg.cpp,v 1.22.2.1 2006/02/04 16:23:46 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "dirdlg.h"
#endif

#include "wx/wxprec.h"

#if wxUSE_DIRDLG

#include "wx/utils.h"
#include "wx/dialog.h"
#include "wx/dirdlg.h"

#include "wx/cmndata.h"
#include "wx/filename.h"

#include "wx/mac/private.h"

#ifdef __DARWIN__
  #include <Carbon/Carbon.h>
#else
  #include <Navigation.h>
#endif

IMPLEMENT_CLASS(wxDirDialog, wxDialog)

static pascal void NavEventProc(
                                NavEventCallbackMessage inSelector,
                                NavCBRecPtr ioParams,
                                NavCallBackUserData ioUserData );

static NavEventUPP sStandardNavEventFilter = NewNavEventUPP(NavEventProc);

static pascal void NavEventProc(
                                NavEventCallbackMessage inSelector,
                                NavCBRecPtr ioParams,
                                NavCallBackUserData ioUserData )
{
    wxDirDialog * data = ( wxDirDialog *) ioUserData ;
    if ( inSelector == kNavCBStart )
    {
        if (data && !data->GetPath().IsEmpty() )
        {
            // Set default location for the modern Navigation APIs
            // Apple Technical Q&A 1151
            FSSpec theFSSpec;
            wxMacFilename2FSSpec(data->GetPath(), &theFSSpec);
            AEDesc theLocation = { typeNull, NULL };
            if (noErr == ::AECreateDesc(typeFSS, &theFSSpec, sizeof(FSSpec), &theLocation))
                ::NavCustomControl(ioParams->context, kNavCtlSetLocation, (void *) &theLocation);
        }
    }
}

wxDirDialog::wxDirDialog(wxWindow *parent,
                         const wxString& message,
                         const wxString& defaultPath,
                         long style,
                         const wxPoint& WXUNUSED(pos),
                         const wxSize& WXUNUSED(size),
                         const wxString& WXUNUSED(name))
{
    wxASSERT_MSG( NavServicesAvailable() , wxT("Navigation Services are not running") ) ;
    m_message = message;
    m_dialogStyle = style;
    m_parent = parent;
    m_path = defaultPath;
}

int wxDirDialog::ShowModal()
{
    NavDialogRef dialog;
    NavDialogCreationOptions options;
    NavReplyRecord reply ;
    bool disposeReply = false ;
    OSStatus err = noErr;
    
    err = NavGetDefaultDialogCreationOptions(&options);
    if (err == noErr) 
    {
        wxMacCFStringHolder message(m_message, m_font.GetEncoding());
        options.message = message;
        err = NavCreateChooseFolderDialog(&options, sStandardNavEventFilter , NULL,  this , &dialog);
        if (err == noErr) 
        {        
            err = NavDialogRun(dialog);
            if ( err == noErr )
            {
                err = NavDialogGetReply(dialog, &reply);
                disposeReply = true ;
            }
        }
    }
    
    if ( err == noErr ) 
    { 
        if ( reply.validRecord )
        {
            FSRef folderInfo;
            AEDesc specDesc ;
            
            OSErr err = ::AECoerceDesc( &reply.selection , typeFSRef, &specDesc);
            if ( err != noErr ) 
            {
                m_path = wxEmptyString ;
            }
            else
            {
                folderInfo = **(FSRef**) specDesc.dataHandle;
                m_path = wxMacFSRefToPath( &folderInfo ) ;
                if (specDesc.dataHandle != nil) 
                {
                    ::AEDisposeDesc(&specDesc);
                }            
            }
        }
        else
        {
            err = paramErr ; // could be any error, only used for giving back wxID_CANCEL
        }
    }
    
    if ( disposeReply )
        ::NavDisposeReply(&reply);
    
    // apparently cancelling shouldn't change m_path
    if ( err != noErr && err != userCanceledErr )
        m_path = wxEmptyString ;
    
    return (err == noErr) ? wxID_OK : wxID_CANCEL ;
}

#endif
