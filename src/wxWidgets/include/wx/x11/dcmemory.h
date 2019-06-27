/////////////////////////////////////////////////////////////////////////////
// Name:        dcmemory.h
// Purpose:     wxMemoryDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: dcmemory.h,v 1.7 2005/08/02 18:16:49 MW Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCMEMORY_H_
#define _WX_DCMEMORY_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dcmemory.h"
#endif

#include "wx/dcclient.h"

class WXDLLIMPEXP_CORE wxMemoryDC : public wxWindowDC
{
public:
    wxMemoryDC();
    wxMemoryDC( wxDC *dc ); // Create compatible DC
    ~wxMemoryDC();
    virtual void SelectObject( const wxBitmap& bitmap );
    void DoGetSize( int *width, int *height ) const;

    // implementation
    wxBitmap  m_selected;

private:
    DECLARE_DYNAMIC_CLASS(wxMemoryDC)
};

#endif
// _WX_DCMEMORY_H_
