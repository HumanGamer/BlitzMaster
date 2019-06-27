/////////////////////////////////////////////////////////////////////////////
// Name:        wx/motif/toplevel.h
// Purpose:     wxTopLevelWindow Motif implementation
// Author:      Mattia Barbon
// Modified by:
// Created:     12/10/2002
// RCS-ID:      $Id: toplevel.h,v 1.13 2005/09/19 10:36:18 VZ Exp $
// Copyright:   (c) Mattia Barbon
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __MOTIFTOPLEVELH__
#define __MOTIFTOPLEVELH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "toplevel.h"
#endif

class WXDLLIMPEXP_CORE wxTopLevelWindowMotif : public wxTopLevelWindowBase
{
public:
    wxTopLevelWindowMotif() { Init(); }
    wxTopLevelWindowMotif( wxWindow* parent, wxWindowID id,
                           const wxString& title,
                           const wxPoint& pos = wxDefaultPosition,
                           const wxSize& size = wxDefaultSize,
                           long style = wxDEFAULT_FRAME_STYLE,
                           const wxString& name = wxFrameNameStr )
    {
        Init();

        Create( parent, id, title, pos, size, style, name );
    }

    bool Create( wxWindow* parent, wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_FRAME_STYLE,
                 const wxString& name = wxFrameNameStr );

    virtual ~wxTopLevelWindowMotif();

    virtual bool ShowFullScreen( bool show, long style = wxFULLSCREEN_ALL );
    virtual bool IsFullScreen() const;

    virtual void Maximize(bool maximize = true);
    virtual void Restore();
    virtual void Iconize(bool iconize = true);
    virtual bool IsMaximized() const;
    virtual bool IsIconized() const;

    virtual void Raise();
    virtual void Lower();

    virtual wxString GetTitle() const { return m_title; }
    virtual void SetTitle( const wxString& title ) { m_title = title; }

    virtual void DoSetSizeHints( int minW, int minH,
                               int maxW = -1, int maxH = -1,
                               int incW = -1, int incH = -1 );

    virtual bool SetShape( const wxRegion& region );

    WXWidget GetShellWidget() const;
protected:
    // common part of all constructors
    void Init();
    // common part of wxDialog/wxFrame destructors
    void PreDestroy();

    virtual void DoGetPosition(int* x, int* y) const;

private:
#if wxCHECK_VERSION(2,7,0)
    // DoDestroy() is not used anywhere else, DoCreate() should also be renamed
    // in src/motif/dialog.cpp, frame.cpp and toplevel.cp
    #error "Remove DoDestroy() and rename DoCreate() to XmDoCreateTLW(), they were only kept for binary backwards compatibility"
#endif

    // really create the Motif widget for TLW
    virtual bool DoCreate(wxWindow* parent,
                               wxWindowID id,
                               const wxString& title,
                               const wxPoint& pos,
                               const wxSize& size,
                               long style,
                               const wxString& name) = 0;

    virtual void DoDestroy() { }

    wxString m_title;
};

#endif // __MOTIFTOPLEVELH__
