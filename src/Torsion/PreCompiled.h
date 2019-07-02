#pragma once

#ifdef _DEBUG 
   #include <crtdbg.h> 
   #define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__) 
#else 
   #define DEBUG_NEW new 
#endif 

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/process.h>
#include <wx/socket.h>
#include <wx/dynarray.h>
#include <wx/docview.h>

#ifdef __WXMSW__
   #pragma warning( disable : 4018 ) // warning C4018: '<' : signed/unsigned mismatch
   #pragma warning( disable : 4267 ) // warning C4267: 'return' : conversion from 'size_t' to 'int', possible loss of data
#endif

#define APP_NAME "BlitzMaster"
#define VENDOR_NAME "Blitz Community"

#define INTERNAL_NAME "BlitzMaster"
#define INST_NAME "BlitzMaster.BlitzCommunity"
