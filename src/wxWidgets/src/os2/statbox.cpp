/////////////////////////////////////////////////////////////////////////////
// Name:        statbox.cpp
// Purpose:     wxStaticBox
// Author:      David Webster
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id: statbox.cpp,v 1.20 2006/01/06 22:26:15 SN Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/window.h"
#include "wx/os2/private.h"

#ifndef WX_PRECOMP
#include "wx/app.h"
#include "wx/dcclient.h"
#endif

#include "wx/statbox.h"

IMPLEMENT_DYNAMIC_CLASS(wxStaticBox, wxControl)

bool wxStaticBox::Create(
  wxWindow*                         pParent
, wxWindowID                        vId
, const wxString&                   rsLabel
, const wxPoint&                    rPos
, const wxSize&                     rSize
, long                              lStyle
, const wxString&                   rsName
)
{
    if(!CreateControl( pParent
                      ,vId
                      ,rPos
                      ,rSize
                      ,lStyle
                      ,wxDefaultValidator
                      ,rsName
                     ))
    {
        return FALSE;
    }

    wxPoint                         vPos(0,0);
    wxSize                          vSize(0,0);

    if (!OS2CreateControl( wxT("STATIC")
                          ,SS_GROUPBOX
                          ,vPos
                          ,vSize
                          ,rsLabel
                         ))
    {
        return FALSE;
    }

    //
    // To be transparent we should have the same colour as the parent as well
    //
    SetBackgroundColour(GetParent()->GetBackgroundColour());

    wxColour                        vColour;
    vColour.Set(wxString(wxT("BLACK")));
    LONG                            lColor = (LONG)vColour.GetPixel();
    ::WinSetPresParam( m_hWnd
                      ,PP_FOREGROUNDCOLOR
                      ,sizeof(LONG)
                      ,(PVOID)&lColor
                     );

    lColor = (LONG)m_backgroundColour.GetPixel();
    ::WinSetPresParam( m_hWnd
                      ,PP_BACKGROUNDCOLOR
                      ,sizeof(LONG)
                      ,(PVOID)&lColor
                     );
    SetSize( rPos.x
            ,rPos.y
            ,rSize.x
            ,rSize.y
           );
    return TRUE;
} // end of wxStaticBox::Create

wxSize wxStaticBox::DoGetBestSize() const
{
    int                             nCx;
    int                             nCy;
    int                             wBox;

    nCx = GetCharWidth();
    nCy = GetCharHeight();
    GetTextExtent( wxGetWindowText(m_hWnd)
                  ,&wBox
                  ,NULL
                 );
    wBox += 3 * nCx;

    int                             hBox = EDIT_HEIGHT_FROM_CHAR_HEIGHT(nCy);

    return wxSize( wBox
                  ,hBox
                 );
} // end of wxStaticBox::DoGetBestSize

MRESULT wxStaticBox::OS2WindowProc(
  WXUINT                            nMsg
, WXWPARAM                          wParam
, WXLPARAM                          lParam
)
{
    return wxControl::OS2WindowProc(nMsg, wParam, lParam);
} // end of wxStaticBox::OS2WindowProc


