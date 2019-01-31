/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/stabox.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: statbox.h,v 1.16 2005/08/02 22:57:58 MW Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKSTATICBOXH__
#define __GTKSTATICBOXH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

//-----------------------------------------------------------------------------
// wxStaticBox
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxStaticBox : public wxStaticBoxBase
{
public:
    wxStaticBox();
    wxStaticBox( wxWindow *parent,
                 wxWindowID id,
                 const wxString &label,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = 0,
                 const wxString &name = wxStaticBoxNameStr );
    bool Create( wxWindow *parent,
                 wxWindowID id,
                 const wxString &label,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = 0,
                 const wxString &name = wxStaticBoxNameStr );

    virtual void SetLabel( const wxString &label );

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);

    // implementation

    virtual bool IsTransparentForMouse() const { return TRUE; }

protected:
    void DoApplyWidgetStyle(GtkRcStyle *style);
    
private:
    DECLARE_DYNAMIC_CLASS(wxStaticBox)
};

#endif // __GTKSTATICBOXH__
