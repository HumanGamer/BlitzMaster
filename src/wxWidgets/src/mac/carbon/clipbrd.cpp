/////////////////////////////////////////////////////////////////////////////
// Name:        clipbrd.cpp
// Purpose:     Clipboard functionality
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: clipbrd.cpp,v 1.38.2.1 2005/12/19 08:50:11 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "clipbrd.h"
#endif

#include "wx/wxprec.h"

#if wxUSE_CLIPBOARD

#include "wx/app.h"
#include "wx/frame.h"
#include "wx/bitmap.h"
#include "wx/utils.h"
#include "wx/metafile.h"
#include "wx/clipbrd.h"
#include "wx/intl.h"
#include "wx/log.h"

#ifndef __DARWIN__
#include <Scrap.h>
#endif
#include "wx/mac/uma.h"

#define wxUSE_DATAOBJ 1

#include <string.h>

// the trace mask we use with wxLogTrace() - call
// wxLog::AddTraceMask(TRACE_CLIPBOARD) to enable the trace messages from here
// (there will be a *lot* of them!)
static const wxChar *TRACE_CLIPBOARD = _T("clipboard");

void *wxGetClipboardData(wxDataFormat dataFormat, long *len)
{
#if !TARGET_CARBON
    OSErr err = noErr ;
#else
    OSStatus err = noErr ;
#endif
    void * data = NULL ;
    Size byteCount;

    switch (dataFormat.GetType())
    {
    case wxDF_OEMTEXT:
        dataFormat = wxDF_TEXT;
        // fall through

    case wxDF_TEXT:
        break;
    case wxDF_UNICODETEXT:
        break;
    case wxDF_BITMAP :
    case wxDF_METAFILE :
        break ;
    default:
        // custom data type
        break ;
    }

#if TARGET_CARBON
    ScrapRef scrapRef;

    err = GetCurrentScrap( &scrapRef );
    if ( err != noTypeErr && err != memFullErr )
    {
        ScrapFlavorFlags    flavorFlags;

        if (( err = GetScrapFlavorFlags( scrapRef, dataFormat.GetFormatId(), &flavorFlags )) == noErr)
        {
            if (( err = GetScrapFlavorSize( scrapRef, dataFormat.GetFormatId(), &byteCount )) == noErr)
            {
                Size allocSize = byteCount ;
                if ( dataFormat.GetType() == wxDF_TEXT )
                    allocSize += 1 ;
                else if ( dataFormat.GetType() == wxDF_UNICODETEXT )
                    allocSize += 2 ;

                data = new char[ allocSize ] ;

                if (( err = GetScrapFlavorData( scrapRef, dataFormat.GetFormatId(), &byteCount , data )) == noErr )
                {
                    *len = allocSize ;
                    if ( dataFormat.GetType() == wxDF_TEXT )
                        ((char*)data)[byteCount] = 0 ;
                    if ( dataFormat.GetType() == wxDF_UNICODETEXT )
                    {
                        // "data" format is UTF16, so 2 bytes = one character
                        // wxChar size depends on platform, so just clear last 2 bytes
                        ((char*)data)[byteCount]   = 0;
                        ((char*)data)[byteCount+1] = 0;
                    }
                }
                else
                {
                    delete[] ((char *)data) ;
                    data = NULL ;
                }
            }
        }
    }

#else
    long offset ;
    Handle datahandle = NewHandle(0) ;
    HLock( datahandle ) ;
    GetScrap( datahandle , dataFormat.GetFormatId() , &offset ) ;
    HUnlock( datahandle ) ;
    if ( GetHandleSize( datahandle ) > 0 )
    {
        byteCount = GetHandleSize( datahandle ) ;
        Size allocSize = byteCount ;
        if ( dataFormat.GetType() == wxDF_TEXT )
            allocSize += 1 ;
        else if ( dataFormat.GetType() == wxDF_UNICODETEXT )
            allocSize += 2 ;

        data = new char[ allocSize ] ;

        memcpy( (char*) data , (char*) *datahandle , byteCount ) ;
        if ( dataFormat.GetType() == wxDF_TEXT )
            ((char*)data)[byteCount] = 0 ;
        if ( dataFormat.GetType() == wxDF_UNICODETEXT )
            ((wxChar*)data)[byteCount/2] = 0 ;
        *len = byteCount ;
    }
    DisposeHandle( datahandle ) ;
#endif
    if ( err )
    {
        wxLogSysError(_("Failed to get clipboard data."));

        return NULL ;
    }

    if ( dataFormat.GetType() == wxDF_TEXT )
    {
        wxMacConvertNewlines10To13( (char*) data ) ;
    }

    return data;
}


/*
 * Generalized clipboard implementation by Matthew Flatt
 */

IMPLEMENT_DYNAMIC_CLASS(wxClipboard, wxObject)

wxClipboard::wxClipboard()
{
    m_open = false ;
    m_data = NULL ;
}

wxClipboard::~wxClipboard()
{
    if (m_data)
    {
        delete m_data;
        m_data = (wxDataObject*) NULL;
    }
}

void wxClipboard::Clear()
{
    if (m_data)
    {
        delete m_data;
        m_data = (wxDataObject*) NULL;
    }
#if TARGET_CARBON
    OSStatus err ;
    err = ClearCurrentScrap( );
#else
    OSErr err ;
    err = ZeroScrap( );
#endif
    if ( err )
    {
        wxLogSysError(_("Failed to empty the clipboard."));
    }
}

bool wxClipboard::Flush()
{
    return false;
}

bool wxClipboard::Open()
{
    wxCHECK_MSG( !m_open, false, wxT("clipboard already open") );
    m_open = true ;
    return true ;
}

bool wxClipboard::IsOpened() const
{
    return m_open;
}

bool wxClipboard::SetData( wxDataObject *data )
{
    wxCHECK_MSG( m_open, false, wxT("clipboard not open") );

    wxCHECK_MSG( data, false, wxT("data is invalid") );

    Clear();
    // as we can only store one wxDataObject, this is the same in this
    // implementation
    return AddData( data );
}

bool wxClipboard::AddData( wxDataObject *data )
{
    wxCHECK_MSG( m_open, false, wxT("clipboard not open") );

    wxCHECK_MSG( data, false, wxT("data is invalid") );

    // we can only store one wxDataObject
    Clear();

    m_data = data;

    // get formats from wxDataObjects
    wxDataFormat *array = new wxDataFormat[ m_data->GetFormatCount() ];
    m_data->GetAllFormats( array );

    for (size_t i = 0; i < m_data->GetFormatCount(); i++)
    {
        wxLogTrace( TRACE_CLIPBOARD,
                    wxT("wxClipboard now supports atom %s"),
                    array[i].GetId().c_str() );

        size_t sz = data->GetDataSize( array[i] ) ;
        void* buf = malloc( sz + 1 ) ;
        if ( buf )
        {
            // empty the buffer because in some case GetDataHere does not fill buf
            memset(buf, 0, sz+1);
            data->GetDataHere( array[i] , buf ) ;
            OSType mactype = 0 ;
            switch ( array[i].GetType() )
            {
               case wxDF_TEXT:
               case wxDF_OEMTEXT:
                    mactype = kScrapFlavorTypeText ;
                    sz -= 1;
               break ;
    #if wxUSE_UNICODE
               case wxDF_UNICODETEXT :
                    mactype = kScrapFlavorTypeUnicode ;
                    sz -= 2;
               break ;
    #endif
    #if wxUSE_DRAG_AND_DROP
            case wxDF_METAFILE:
                    mactype = kScrapFlavorTypePicture ;
               break ;
    #endif
               case wxDF_BITMAP:
               case wxDF_DIB:
                    mactype = kScrapFlavorTypePicture ;
                    break ;
               default:
                    mactype = (OSType)(array[i].GetFormatId());
                    break ;
            }
            UMAPutScrap( sz , mactype , buf ) ;
            free( buf ) ;
        }
    }

    delete[] array;

    return true ;
}

void wxClipboard::Close()
{
    wxCHECK_RET( m_open, wxT("clipboard not open") );

    m_open = false ;

    // Get rid of cached object.  If this is not done copying from another application will
    // only work once
    if (m_data)
    {
        delete m_data;
        m_data = (wxDataObject*) NULL;
    }
}

bool wxClipboard::IsSupported( const wxDataFormat &dataFormat )
{
    if ( m_data )
    {
        return m_data->IsSupported( dataFormat ) ;
    }
#if TARGET_CARBON
    OSStatus err = noErr;
    ScrapRef scrapRef;

    err = GetCurrentScrap( &scrapRef );
    if ( err != noTypeErr && err != memFullErr )
    {
        ScrapFlavorFlags    flavorFlags;
        Size                byteCount;

        if (( err = GetScrapFlavorFlags( scrapRef, dataFormat.GetFormatId(), &flavorFlags )) == noErr)
        {
            if (( err = GetScrapFlavorSize( scrapRef, dataFormat.GetFormatId(), &byteCount )) == noErr)
            {
                return true ;
            }
        }
    }
    return false;

#else
    long offset ;
    Handle datahandle = NewHandle(0) ;
    HLock( datahandle ) ;
    GetScrap( datahandle , dataFormat.GetFormatId() , &offset ) ;
    HUnlock( datahandle ) ;
    bool hasData = GetHandleSize( datahandle ) > 0 ;
    DisposeHandle( datahandle ) ;
    return hasData ;
#endif
}

bool wxClipboard::GetData( wxDataObject& data )
{
    wxCHECK_MSG( m_open, false, wxT("clipboard not open") );

    size_t formatcount = data.GetFormatCount() + 1 ;
    wxDataFormat *array = new wxDataFormat[ formatcount  ];
    array[0] = data.GetPreferredFormat();
    data.GetAllFormats( &array[1] );

    bool transferred = false ;

    if ( m_data )
    {
      for (size_t i = 0; !transferred && i < formatcount ; i++)
      {
          wxDataFormat format = array[i] ;
          if ( m_data->IsSupported( format ) )
          {
            int size = m_data->GetDataSize( format );
            transferred = true ;

            if (size == 0)
            {
              data.SetData(format , 0 , 0 ) ;
            }
            else
            {
              char *d = new char[size];
              m_data->GetDataHere( format , (void*) d );
              data.SetData( format , size , d ) ;
              delete[] d ;
            }
          }
       }
    }

    // get formats from wxDataObjects
    if ( !transferred )
    {
      for (size_t i = 0; !transferred && i < formatcount ; i++)
      {
          wxDataFormat format = array[i] ;

          switch ( format.GetType() )
          {
              // NOTE: this is usable for all data types
              case wxDF_TEXT :
              case wxDF_UNICODETEXT:
              case wxDF_OEMTEXT :
              case wxDF_BITMAP :
              case wxDF_METAFILE :
              default :
              {
                  long len ;
                  char* s = (char*)wxGetClipboardData(format, &len );
                  if ( s )
                  {
                    data.SetData( format , len , s ) ;
                    delete [] s;

                    transferred = true ;
                  }
              }
              break ;
          }
       }
    }

    delete[] array ;
    return transferred ;
}

#endif
