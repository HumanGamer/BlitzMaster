/////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/statline.h
// Purpose:     wxStaticLine class for wxUniversal
// Author:      Vadim Zeitlin
// Created:     28.06.99
// Version:     $Id: statline.h,v 1.9.2.1 2005/09/25 20:46:45 MW Exp $
// Copyright:   (c) 1999 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_STATLINE_H_
#define _WX_UNIV_STATLINE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "univstatline.h"
#endif

class WXDLLEXPORT wxStaticLine : public wxStaticLineBase
{
public:
    // constructors and pseudo-constructors
    wxStaticLine() { }

    wxStaticLine(wxWindow *parent,
                 const wxPoint &pos,
                 wxCoord length,
                 long style = wxLI_HORIZONTAL)
    {
        Create(parent, wxID_ANY, pos,
               style & wxLI_VERTICAL ? wxSize(wxDefaultCoord, length)
                                     : wxSize(length, wxDefaultCoord),
               style);
    }

    wxStaticLine(wxWindow *parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxLI_HORIZONTAL,
                 const wxString &name = wxStaticTextNameStr )
    {
        Create(parent, id, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = wxLI_HORIZONTAL,
                const wxString &name = wxStaticTextNameStr );

protected:
    virtual void DoDraw(wxControlRenderer *renderer);

private:
    DECLARE_DYNAMIC_CLASS(wxStaticLine)
};

#endif // _WX_UNIV_STATLINE_H_

