/////////////////////////////////////////////////////////////////////////////
// Name:        gdiobj.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: gdiobj.h,v 1.10 2005/08/02 22:57:55 MW Exp $
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GDIOBJH__
#define __GDIOBJH__

#include "wx/object.h"

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

class WXDLLIMPEXP_CORE wxGDIObject : public wxObject
{
public:
    inline wxGDIObject() { m_visible = FALSE; };
    inline ~wxGDIObject() {}

    virtual bool GetVisible() { return m_visible; }
    virtual void SetVisible( bool visible ) { m_visible = visible; }

    bool IsNull() const { return (m_refData == 0); }

protected:
    bool m_visible; /* can a pointer to this object be safely taken?
                     * - only if created within FindOrCreate... */
private:
    DECLARE_DYNAMIC_CLASS(wxGDIObject)
};

#endif
    // __GDIOBJH__
