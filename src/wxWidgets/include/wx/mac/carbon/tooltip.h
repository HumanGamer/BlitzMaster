///////////////////////////////////////////////////////////////////////////////
// Name:        mac/tooltip.h
// Purpose:     wxToolTip class - tooltip control
// Author:      Stefan Csomor
// Modified by:
// Created:     31.01.99
// RCS-ID:      $Id: tooltip.h,v 1.4.4.1 2005/09/25 20:46:27 MW Exp $
// Copyright:   (c) 1999 Robert Roebling, Vadim Zeitlin, Stefan Csomor
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAC_TOOLTIP_H_
#define _WX_MAC_TOOLTIP_H_

class wxToolTip : public wxObject
{
public:
    // ctor & dtor
    wxToolTip(const wxString &tip);
    virtual ~wxToolTip();

    // accessors
        // tip text
    void SetTip(const wxString& tip);
    const wxString& GetTip() const { return m_text; }

        // the window we're associated with
    void SetWindow(wxWindow *win);
    wxWindow *GetWindow() const { return m_window; }

    // controlling tooltip behaviour: globally change tooltip parameters
        // enable or disable the tooltips globally
    static void Enable(bool flag);
        // set the delay after which the tooltip appears
    static void SetDelay(long milliseconds);
    static void NotifyWindowDelete( WXHWND win ) ;

    // implementation only from now on
    // -------------------------------

    // should be called in response to mouse events
    static void RelayEvent(wxWindow *win , wxMouseEvent &event);
    static void RemoveToolTips();

private:
    wxString  m_text;           // tooltip text
    wxWindow *m_window;         // window we're associated with
    DECLARE_ABSTRACT_CLASS(wxToolTip)
};

#endif // _WX_MAC_TOOLTIP_H_
