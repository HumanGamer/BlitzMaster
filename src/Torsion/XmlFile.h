#pragma once
#include <string>
#include "tinyxml2.h"
//#include "MarkupSTL.h"
using namespace tinyxml2;

class XmlFile
{
public:
   XmlFile();
   XmlFile( const wxChar* buffer );

   std::string GetDoc();

   bool SaveFile(const wxString & path);

   wxString GetError();

   bool SetDoc(const wxChar * buffer);

   void ResetMainPos();

   bool FindElem(const wxString & name);

   bool IntoElem();

   bool OutOfElem();

   wxString GetData();

   bool AddAttrib(const wxString & name, const wxString & value);

   wxString GetAttrib(const wxString & name);

   wxString GetStringElem( const wxString& name, const wxString& value );
  
   void AddElem(const wxString& name, const wxString& value = "");

   wxPoint GetPointElem( const wxString& name, const wxPoint& value );
   void AddPointElem( const wxString& name, const wxPoint& value );

   bool GetBoolElem( const wxString& name, bool value );
   void AddBoolElem( const wxString& name, bool value );

   int GetIntElem( const wxString& name, int value );
   void AddIntElem( const wxString& name, int value );
   int GetIntAttrib( const wxString& element, const wxString& attrib, int value );

   wxColour GetColorElem( const wxString& name, const wxColour& value );
   void AddColorElem( const wxString& name, const wxColour& value );
   wxColour GetColorAttrib( const wxString& element, const wxString& attrib, const wxColour& color );

   static wxColour StringToColor( const wxString& color );
   static bool StringToBool( const wxChar* boolean );
   static int StringToInt( const wxChar* integer );
   static wxPoint StringToPoint( const wxString& point );

   static wxString ColorToString( const wxColour& color );
   static wxString BoolToString( bool value );
   static wxString IntToString( int value );
   static wxString PointToString( const wxPoint& value );

   int GetArrayStringElems( wxArrayString& output, const wxString& name, const wxString& elemName );
   void AddArrayStringElems( const wxString& name, const wxString& elemName, const wxArrayString& strings );

protected:

   static wxChar s_Temp[MAX_PATH];
private:
	tinyxml2::XMLDocument doc;
	XMLElement* currentParentElement = nullptr;
	XMLElement* currentElement = nullptr;
	XMLError error = XMLError::XML_SUCCESS;
};

inline bool XmlFile::StringToBool( const wxChar* boolean )
{
   return stricmp( boolean, "true" ) == 0 || atoi( boolean );
}

inline int XmlFile::StringToInt( const wxChar* integer )
{
   return atoi( integer );
}

inline wxString XmlFile::BoolToString( bool value )
{
   return value ? "true" : "false";
}

inline wxString XmlFile::IntToString( int value )
{
   return itoa( value, s_Temp, 10 ); 
}
