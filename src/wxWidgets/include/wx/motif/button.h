/////////////////////////////////////////////////////////////////////////////
// Name:        button.h
// Purpose:     wxButton class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: button.h,v 1.19 2005/08/28 07:51:57 MBN Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BUTTON_H_
#define _WX_BUTTON_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "button.h"
#endif

// Pushbutton
class WXDLLEXPORT wxButton: public wxButtonBase
{
    DECLARE_DYNAMIC_CLASS(wxButton)
        
public:
    wxButton() { }
    wxButton(wxWindow *parent,
        wxWindowID id,
        const wxString& label = wxEmptyString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr)
    {
        Create(parent, id, label, pos, size, style, validator, name);
    }
    
    bool Create(wxWindow *parent, wxWindowID id,
        const wxString& label = wxEmptyString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr);
    
    virtual void SetDefault();
    virtual void Command(wxCommandEvent& event);
    
    static wxSize GetDefaultSize();
    
    // Implementation
private:
    virtual wxSize DoGetBestSize() const;
    virtual wxSize GetMinSize() const;
    wxSize OldGetBestSize() const;
    wxSize OldGetMinSize() const;
    void SetDefaultShadowThicknessAndResize();
};

#endif
// _WX_BUTTON_H_
