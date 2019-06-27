#pragma once

#include <wx/imaglist.h>

WX_DECLARE_STRING_HASH_MAP( int, StringToIntMap );

class FileTypeImageList : public wxImageList
{
public:

	FileTypeImageList();
   virtual ~FileTypeImageList();

public:

   int AddFileIcon( const wxString& file, wxIcon& icon );
   int AddFileIcon( const wxString& file );

public:

   StringToIntMap m_TypeToIndex;
};

