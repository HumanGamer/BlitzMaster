#pragma once


template<class T> int CmpNameNoCase( T* first, T* second )
{
   wxASSERT( first );
   wxASSERT( second );
   return first->GetName().CmpNoCase( second->GetName() );
}

inline int CmpStringNoCase( const wxString& first, const wxString& second )
{
   return first.CmpNoCase( second );
}
