/////////////////////////////////////////////////////////////////////////////
// Name:        dcmemory.h
// Purpose:     wxMemoryDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: dcmemory.h,v 1.11 2005/08/03 00:53:08 MW Exp $
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
    DECLARE_DYNAMIC_CLASS(wxMemoryDC)
        
public:
    wxMemoryDC();
    wxMemoryDC( wxDC *dc ); // Create compatible DC
    ~wxMemoryDC();
    
    virtual void SelectObject( const wxBitmap& bitmap );
    
    void DoGetSize( int *width, int *height ) const;
    
    wxBitmap& GetBitmap() const { return (wxBitmap&) m_bitmap; }
    
private:
    friend class wxPaintDC;
    
    wxBitmap  m_bitmap;
};

#endif
// _WX_DCMEMORY_H_
