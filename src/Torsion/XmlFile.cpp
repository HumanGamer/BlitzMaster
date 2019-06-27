#include "PreCompiled.h"
#include "XmlFile.h"


#include <wx/tokenzr.h>

#ifdef _DEBUG 
   #define new DEBUG_NEW 
#endif 

wxChar XmlFile::s_Temp[MAX_PATH];


XmlFile::XmlFile()
{
	error = doc.Parse("");
}

XmlFile::XmlFile( const wxChar* buffer )
{
	error = doc.Parse(buffer);
}

std::string XmlFile::GetDoc()
{
	XMLPrinter printer;
	doc.Print(&printer);
	return printer.CStr();
}

bool XmlFile::SaveFile(const wxString& path)
{
	error = doc.SaveFile(path);
	return error == 0;
}

wxString XmlFile::GetError()
{
	//return doc.GetErrorStr1();
	return doc.ErrorStr();
}

bool XmlFile::SetDoc(const wxChar* buffer)
{
	XMLError error = doc.Parse(buffer);
	return !error;
}

void XmlFile::ResetMainPos()
{
	currentElement = nullptr;
}

bool XmlFile::FindElem(const wxString& name = "")
{
	bool found = false;
	if (currentParentElement == nullptr && currentElement == nullptr)
	{
		currentElement = doc.RootElement();
		found = currentElement != NULL;
	}
	else if (currentElement != nullptr)
	{
		auto element = currentElement->NextSiblingElement(name);
		if (element != nullptr)
		{
			currentElement = element;
			found = currentElement != NULL;
		}
	}
	else if (currentParentElement != nullptr)
	{
		auto element = currentParentElement->FirstChildElement(name);
		if (element != nullptr)
		{
			currentElement = element;
			found = currentElement != NULL;
		}
	}

	return found;
}

bool XmlFile::IntoElem()
{
	if (currentElement != nullptr)
	{
		currentParentElement = currentElement;
		currentElement = nullptr;
	}
	return currentParentElement != nullptr;
}

bool XmlFile::OutOfElem()
{
	if (currentParentElement != nullptr)
		currentElement = currentParentElement;
	else
		currentElement = doc.RootElement();
	if (currentElement->Parent() != nullptr)
		currentParentElement = currentElement->Parent()->ToElement();
	return currentElement != doc.RootElement();
}

wxString XmlFile::GetData()
{
	return currentElement->GetText();
}

bool XmlFile::AddAttrib(const wxString& name, const wxString& value)
{
	if (currentElement != nullptr)
	{
		currentElement->SetAttribute(name, value);
		return true;
	}

	return false;
}

wxString XmlFile::GetAttrib(const wxString& name)
{
	return currentElement->Attribute(name);
}

void XmlFile::AddElem(const wxString& name, const wxString& value)
{
	if (!doc.RootElement())
	{
		currentElement = doc.NewElement(name);
		doc.InsertFirstChild(currentElement);
	}
	else
	{
		XMLElement *node = doc.NewElement(name);
		node->SetText(value);
		currentElement = (currentParentElement->InsertEndChild(node))->ToElement();
	}
}

wxString XmlFile::GetStringElem( const wxString& name, const wxString& value )
{
   wxString result = value;
   if (FindElem(name.c_str())) {
      result = GetData().c_str();
   }

   ResetMainPos();
   return result;
}

wxPoint XmlFile::GetPointElem( const wxString& name, const wxPoint& value )
{
   return StringToPoint( GetStringElem( name, PointToString( value ) ) );
}

void XmlFile::AddPointElem( const wxString& name, const wxPoint& value )
{
   AddElem( name.c_str(), PointToString( value ).c_str() );
}

bool XmlFile::GetBoolElem( const wxString& name, bool value )
{
   return StringToBool( GetStringElem( name, value ? "true" : "false" ) );
}

void XmlFile::AddBoolElem( const wxString& name, bool value )
{
   AddElem( name.c_str(), BoolToString( value ).c_str() );
}

int XmlFile::GetIntElem( const wxString& name, int value )
{
   return StringToInt( GetStringElem( name, IntToString( value ) ) );
}

void XmlFile::AddIntElem( const wxString& name, int value )
{
   AddElem( name.c_str(), IntToString( value ).c_str() );
}

int XmlFile::GetIntAttrib( const wxString& element, const wxString& attrib, int value )
{
   int result = value;

   if (FindElem(element.c_str())) {
      wxString value( GetAttrib( attrib.c_str() ).c_str() );
      if ( !value.IsEmpty() ) {
         result = StringToInt( value );
      }
   }
	ResetMainPos();
   return result;
}

wxColour XmlFile::GetColorElem( const wxString& name, const wxColour& value )
{
   return StringToColor( GetStringElem( name, ColorToString( value ) ) );
}

void XmlFile::AddColorElem( const wxString& name, const wxColour& value )
{
   AddElem( name.c_str(), ColorToString( value ).c_str() );
}

wxColour XmlFile::GetColorAttrib( const wxString& element, const wxString& attrib, const wxColour& color )
{
   wxColour result( color );
   if (FindElem(element.c_str())) {
      wxString value( GetAttrib(attrib.c_str() ).c_str() );
      if ( !value.IsEmpty() ) {
         result = StringToColor( value );
      }
   }
	ResetMainPos();
   return result;
}

wxString XmlFile::ColorToString( const wxColour& color )
{
	wxString Result;
   Result << color.Red() << ", " << color.Green() << ", " << color.Blue();
	return Result;
}

wxColour XmlFile::StringToColor( const wxString& color )
{
	wxStringTokenizer Tokenizer( color, "\t\r\n ,;", wxTOKEN_STRTOK );

	wxString Token = Tokenizer.GetNextToken();
	Token.Trim( true );
	Token.Trim( false );
	long Red = 0;
	Token.ToLong( &Red );
	Red = wxMin( 255, wxMax( 0, Red ) );
	
	Token = Tokenizer.GetNextToken();
	Token.Trim( true );
	Token.Trim( false );
	long Green = 0;
	Token.ToLong( &Green );
	Green = wxMin( 255, wxMax( 0, Green ) );

	Token = Tokenizer.GetNextToken();
	Token.Trim( true );
	Token.Trim( false );
	long Blue = 0;
	Token.ToLong( &Blue );
	Blue = wxMin( 255, wxMax( 0, Blue ) );

	return wxColour( Red, Green, Blue );
}

wxPoint XmlFile::StringToPoint( const wxString& point )
{
	wxStringTokenizer Tokenizer( point, "\t\r\n ,;", wxTOKEN_STRTOK );
	
   wxString Token = Tokenizer.GetNextToken();
	Token.Trim( true );
	Token.Trim( false );
	long x = 0;
	Token.ToLong( &x );

   Token = Tokenizer.GetNextToken();
	Token.Trim( true );
	Token.Trim( false );
	long y = 0;
	Token.ToLong( &y );

   return wxPoint( x, y );
}

wxString XmlFile::PointToString( const wxPoint& value )
{
   wxString result;
   result << value.x << " " << value.y;
   return result;
}

int XmlFile::GetArrayStringElems( wxArrayString& output, const wxString& name, const wxString& elemName )
{
   int count = -1;
   if ( FindElem(name) && IntoElem() ) {

      count = 0;
      while ( FindElem( elemName ) ) {
         output.Add( GetData().c_str() );
         ++count;
      }

      OutOfElem();
   	ResetMainPos();
   }
	ResetMainPos();

   return count;
}

void XmlFile::AddArrayStringElems( const wxString& name, const wxString& elemName, const wxArrayString& strings )
{
	AddElem( name );
	IntoElem();
   for ( size_t i=0; i < strings.GetCount(); i++ ) {
      AddElem( elemName, strings[i] );
   }
   OutOfElem();
}

