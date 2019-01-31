/////////////////////////////////////////////////////////////////////////////
// Name:        dc.cpp
// Purpose:     wxDC class
// Author:      Stefan Csomor
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: dccg.cpp,v 1.26.2.2 2005/12/19 11:18:12 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "dc.h"
#endif

#include "wx/wxprec.h"

#include "wx/dc.h"

#if wxMAC_USE_CORE_GRAPHICS

#include "wx/app.h"
#include "wx/mac/uma.h"
#include "wx/dcmemory.h"
#include "wx/dcprint.h"
#include "wx/region.h"
#include "wx/image.h"
#include "wx/log.h"


#if __MSL__ >= 0x6000
#include "math.h"
    // in case our functions were defined outside std, we make it known all the same
    namespace std { }  
    using namespace std ;
#endif

#include "wx/mac/private.h"

IMPLEMENT_ABSTRACT_CLASS(wxDC, wxObject)

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------

#if !defined( __DARWIN__ ) || defined(__MWERKS__)
#ifndef M_PI
const double M_PI = 3.14159265358979 ;
#endif
#endif
const double RAD2DEG  = 180.0 / M_PI;
const short kEmulatedMode = -1 ;
const short kUnsupportedMode = -2 ;

extern TECObjectRef s_TECNativeCToUnicode ;

// TODO Update
// The text ctrl implementation still needs that for the non hiview implementation

wxMacWindowClipper::wxMacWindowClipper( const wxWindow* win ) :
    wxMacPortSaver( (GrafPtr) GetWindowPort((WindowRef) win->MacGetTopLevelWindowRef()) )
{
    m_newPort =(GrafPtr) GetWindowPort((WindowRef) win->MacGetTopLevelWindowRef()) ;
    m_formerClip = NewRgn() ;
    m_newClip = NewRgn() ;
    GetClip( m_formerClip ) ;
    
    if ( win )
    {
        // guard against half constructed objects, this just leads to a empty clip
        if( win->GetPeer() )
        {
            int x = 0 , y = 0;
            win->MacWindowToRootWindow( &x,&y ) ;
            // get area including focus rect
            CopyRgn( (RgnHandle) ((wxWindow*)win)->MacGetVisibleRegion(true).GetWXHRGN() , m_newClip ) ;
            if ( !EmptyRgn( m_newClip ) )
                OffsetRgn( m_newClip , x , y ) ;
        }

        SetClip( m_newClip ) ;
    }
}

wxMacWindowClipper::~wxMacWindowClipper() 
{
    SetPort( m_newPort ) ;
    SetClip( m_formerClip ) ;
    DisposeRgn( m_newClip ) ;
    DisposeRgn( m_formerClip ) ;
}

wxMacWindowStateSaver::wxMacWindowStateSaver( const wxWindow* win ) :
    wxMacWindowClipper( win )
{
    // the port is already set at this point
    m_newPort =(GrafPtr) GetWindowPort((WindowRef) win->MacGetTopLevelWindowRef()) ;
    GetThemeDrawingState( &m_themeDrawingState ) ;
}

wxMacWindowStateSaver::~wxMacWindowStateSaver() 
{
    SetPort( m_newPort ) ;
    SetThemeDrawingState( m_themeDrawingState , true ) ;
}

// minimal implementation only used for appearance drawing < 10.3

wxMacPortSetter::wxMacPortSetter( const wxDC* dc ) :
    m_ph( (GrafPtr) dc->m_macPort )
{
    wxASSERT( dc->Ok() ) ;
    m_dc = dc ;
//    dc->MacSetupPort(&m_ph) ;
}
wxMacPortSetter::~wxMacPortSetter()
{
//    m_dc->MacCleanupPort(&m_ph) ;
}

//-----------------------------------------------------------------------------
// Local functions
//-----------------------------------------------------------------------------

static inline double dmin(double a, double b) { return a < b ? a : b; }
static inline double dmax(double a, double b) { return a > b ? a : b; }
static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }

//-----------------------------------------------------------------------------
// device context implementation
//
// more and more of the dc functionality should be implemented by calling
// the appropricate wxMacCGContext, but we will have to do that step by step
// also coordinate conversions should be moved to native matrix ops
//-----------------------------------------------------------------------------

// we always stock two context states, one at entry, to be able to preserve the
// state we were called with, the other one after changing to HI Graphics orientation
// (this one is used for getting back clippings etc)

wxMacCGPath::wxMacCGPath()
{
    m_path = CGPathCreateMutable() ;
}

wxMacCGPath::~wxMacCGPath()
{
    CGPathRelease( m_path ) ;
}

//  Starts a new subpath at
void wxMacCGPath::MoveToPoint( wxCoord x1 , wxCoord y1 )
{
    CGPathMoveToPoint( m_path , NULL , x1 , y1 ) ;
}

void wxMacCGPath::AddLineToPoint( wxCoord x1 , wxCoord y1 ) 
{
    CGPathAddLineToPoint( m_path , NULL , x1 , y1 ) ;
}

void wxMacCGPath::AddQuadCurveToPoint( wxCoord cx1, wxCoord cy1, wxCoord x1, wxCoord y1 )
{
    CGPathAddQuadCurveToPoint( m_path , NULL , cx1 , cy1 , x1 , y1 );
}

void wxMacCGPath::AddRectangle( wxCoord x, wxCoord y, wxCoord w, wxCoord h )
{
    CGRect cgRect = { { x , y } , { w , h } } ;
    CGPathAddRect( m_path , NULL , cgRect ) ;
}

void wxMacCGPath::AddCircle( wxCoord x, wxCoord y , wxCoord r )
{
    CGPathAddArc( m_path , NULL , x , y , r , 0.0 , 2 * M_PI , true ) ;
}

// closes the current subpath
void wxMacCGPath::CloseSubpath()
{
    CGPathCloseSubpath( m_path ) ;
}

CGPathRef wxMacCGPath::GetPath() const 
{
    return m_path ;
}

wxMacCGContext::wxMacCGContext( CGrafPtr port ) 
{
    m_qdPort = port ;
    m_cgContext = NULL ;
}

wxMacCGContext::wxMacCGContext( CGContextRef cgcontext )
{
    m_qdPort = NULL ;
    m_cgContext = cgcontext ;
    CGContextSaveGState( m_cgContext ) ;
    CGContextSaveGState( m_cgContext ) ;
}

wxMacCGContext::wxMacCGContext()
{
    m_qdPort = NULL ;
    m_cgContext = NULL ;
}

wxMacCGContext::~wxMacCGContext() 
{
    if ( m_cgContext )
    {
        CGContextSynchronize( m_cgContext ) ;
        CGContextRestoreGState( m_cgContext ) ;
        CGContextRestoreGState( m_cgContext ) ;
    }
    if ( m_qdPort )
        CGContextRelease( m_cgContext ) ;
}


void wxMacCGContext::Clip( const wxRegion &region )
{
//        ClipCGContextToRegion ( m_cgContext, &bounds , (RgnHandle) dc->m_macCurrentClipRgn ) ;
}

void wxMacCGContext::StrokePath( const wxGraphicPath *p ) 
{
    const wxMacCGPath* path = dynamic_cast< const wxMacCGPath*>( p ) ;
    CGContextAddPath( m_cgContext , path->GetPath() ) ;
    CGContextStrokePath( m_cgContext ) ;
}

void wxMacCGContext::DrawPath( const wxGraphicPath *p , int fillStyle ) 
{
    const wxMacCGPath* path = dynamic_cast< const wxMacCGPath*>( p ) ;
    CGPathDrawingMode mode = m_mode ;
    if ( fillStyle == wxODDEVEN_RULE )
    {
        if ( mode == kCGPathFill )
            mode = kCGPathEOFill ;
        else if ( mode == kCGPathFillStroke )
            mode = kCGPathEOFillStroke ;
    }
    CGContextAddPath( m_cgContext , path->GetPath() ) ;
    CGContextDrawPath( m_cgContext , mode ) ;
}

void wxMacCGContext::FillPath( const wxGraphicPath *p , const wxColor &fillColor , int fillStyle ) 
{
    const wxMacCGPath* path = dynamic_cast< const wxMacCGPath*>( p ) ;
    CGContextSaveGState( m_cgContext ) ;
    
    RGBColor col = MAC_WXCOLORREF( fillColor.GetPixel() ) ;
    CGContextSetRGBFillColor( m_cgContext , col.red / 65536.0 , col.green / 65536.0 , col.blue / 65536.0 , 1.0 ) ;
    CGPathDrawingMode mode = kCGPathFill ;

    if ( fillStyle == wxODDEVEN_RULE )
        mode = kCGPathEOFill ;
               
    CGContextBeginPath( m_cgContext ) ;
    CGContextAddPath( m_cgContext , path->GetPath() ) ;
    CGContextClosePath( m_cgContext ) ;
    CGContextDrawPath( m_cgContext , mode ) ;
    
    CGContextRestoreGState( m_cgContext ) ;
}
    
wxGraphicPath* wxMacCGContext::CreatePath() 
{ 
    // make sure that we now have a real cgref, before doing
    // anything with paths
    CGContextRef cg = GetNativeContext() ;
    cg = NULL ;
    return new wxMacCGPath() ; 
}

// in case we only got a QDPort only create a cgref now

CGContextRef wxMacCGContext::GetNativeContext() 
{ 
    if( m_cgContext == NULL )
    {
        Rect bounds ;
        GetPortBounds( (CGrafPtr) m_qdPort , &bounds ) ;
        OSStatus status = CreateCGContextForPort((CGrafPtr) m_qdPort , &m_cgContext) ;
        CGContextSaveGState( m_cgContext ) ;

        wxASSERT_MSG( status == noErr , wxT("Cannot nest wxDCs on the same window") ) ;
        CGContextTranslateCTM( m_cgContext , 0 , bounds.bottom - bounds.top ) ;
        CGContextScaleCTM( m_cgContext , 1 , -1 ) ;
        
        CGContextSaveGState( m_cgContext ) ;
        SetPen( m_pen ) ;
        SetBrush( m_brush ) ;
    }
    return m_cgContext ; 
}

void wxMacCGContext::SetNativeContext( CGContextRef cg ) 
{ 
    // we allow either setting or clearing but not replacing
    wxASSERT( m_cgContext == NULL || cg == NULL ) ;
    if ( cg )
        CGContextSaveGState( cg ) ;
    m_cgContext = cg ; 
}

#pragma mark -

// wrapper class for a CGPattern, always allocate on heap, never call destructor

class wxMacCGPattern
{
public :
    wxMacCGPattern()
    {
    }
    
    // is guaranteed to be called only with a non-Null CGContextRef
    virtual void Render( CGContextRef	ctxRef ) = 0 ;

    operator CGPatternRef() const { return m_patternRef ; }
protected :
    virtual ~wxMacCGPattern()
    {
        // as this is called only when our m_patternRef is been released, don't release
        // it again
    }
    
    static void _Render( void *info, CGContextRef ctxRef )
    {
        wxMacCGPattern* self = (wxMacCGPattern*) info ;
        if ( self && ctxRef )
            self->Render( ctxRef ) ;
    }
    
    static void _Dispose( void *info )
    {
        wxMacCGPattern* self = (wxMacCGPattern*) info ;
        delete self ;
    }
    
    CGPatternRef m_patternRef ;

    static const CGPatternCallbacks ms_Callbacks ;
} ;

const CGPatternCallbacks wxMacCGPattern::ms_Callbacks = { 0, &wxMacCGPattern::_Render, &wxMacCGPattern::_Dispose };

class ImagePattern : public wxMacCGPattern
{
public :
    ImagePattern( const wxBitmap* bmp  , CGAffineTransform transform )
    {
        wxASSERT( bmp && bmp->Ok() ) ;
        Init( (CGImageRef) bmp->CGImageCreate() , transform ) ;
    }
    
    // ImagePattern takes ownership of CGImageRef passed in
    ImagePattern( CGImageRef image , CGAffineTransform transform )
    {
        if ( image )
        {
            CFRetain( image ) ;
        }
        Init( image , transform ) ;
    }
    
    virtual void Render( CGContextRef	ctxRef )
    {
        if (m_image != NULL)
            HIViewDrawCGImage( ctxRef, &m_imageBounds, m_image );
    }

protected :
    void Init( CGImageRef image , CGAffineTransform transform )
    {
        m_image = image ;
        if ( m_image )
        {
            m_imageBounds = CGRectMake( 0.0, 0.0, (float)CGImageGetWidth( m_image  ), (float)CGImageGetHeight( m_image ) ) ;
            m_patternRef = CGPatternCreate( this , m_imageBounds, transform ,
                                            m_imageBounds.size.width, m_imageBounds.size.height,
                                            kCGPatternTilingNoDistortion, true , &wxMacCGPattern::ms_Callbacks ) ;
        }
    }
    
    ~ImagePattern()
    {
        if ( m_image )
            CGImageRelease( m_image ) ;
    }
    CGImageRef  m_image ;
    CGRect      m_imageBounds ;
} ;

class HatchPattern : public wxMacCGPattern
{
public :
    HatchPattern( int hatchstyle  , CGAffineTransform transform )
    {
        m_hatch = hatchstyle ;
        m_imageBounds = CGRectMake( 0.0, 0.0, 8.0 , 8.0 ) ;
        m_patternRef = CGPatternCreate( this , m_imageBounds, transform ,
                                            m_imageBounds.size.width, m_imageBounds.size.height,
                                            kCGPatternTilingNoDistortion, false , &wxMacCGPattern::ms_Callbacks ) ;
    }
        
    void StrokeLineSegments( CGContextRef ctxRef , const CGPoint pts[] , size_t count )
    {
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
        if ( UMAGetSystemVersion() >= 0x1040 )
        {
            CGContextStrokeLineSegments( ctxRef , pts , count ) ;
        }
        else
#endif
        {
            CGContextBeginPath (ctxRef);
            for (size_t i = 0; i < count; i += 2) {
                CGContextMoveToPoint(ctxRef, pts[i].x, pts[i].y);
                CGContextAddLineToPoint(ctxRef, pts[i+1].x, pts[i+1].y);
            }
            CGContextStrokePath(ctxRef);
        }
    }
    
    virtual void Render( CGContextRef	ctxRef )
    {
        switch( m_hatch )
        {
            case wxBDIAGONAL_HATCH :
                {
                    CGPoint pts[] = { 
                    { 8.0 , 0.0 } , { 0.0 , 8.0 } 
                    };
                    StrokeLineSegments( ctxRef , pts , 2 ) ;
                }
                break ;
            case wxCROSSDIAG_HATCH :
                {
                    CGPoint pts[] = { 
                        { 0.0 , 0.0 } , { 8.0 , 8.0 } , 
                        { 8.0 , 0.0 } , { 0.0 , 8.0 } 
                    };
                    StrokeLineSegments( ctxRef , pts , 4 ) ;
                }
                break ;
            case wxFDIAGONAL_HATCH :
                {
                    CGPoint pts[] = { 
                    { 0.0 , 0.0 } , { 8.0 , 8.0 } 
                    };
                    StrokeLineSegments( ctxRef , pts , 2 ) ;
                }
                break ;
            case wxCROSS_HATCH :
                {
                    CGPoint pts[] = { 
                    { 0.0 , 4.0 } , { 8.0 , 4.0 } , 
                    { 4.0 , 0.0 } , { 4.0 , 8.0 } , 
                    };
                    StrokeLineSegments( ctxRef , pts , 4 ) ;
                }
                break ;
            case wxHORIZONTAL_HATCH :
                {
                    CGPoint pts[] = { 
                    { 0.0 , 4.0 } , { 8.0 , 4.0 } , 
                    };
                    StrokeLineSegments( ctxRef , pts , 2 ) ;
                }
                break ;
            case wxVERTICAL_HATCH :
                {
                    CGPoint pts[] = { 
                    { 4.0 , 0.0 } , { 4.0 , 8.0 } , 
                    };
                    StrokeLineSegments( ctxRef , pts , 2 ) ;
                }
                break ;
        }
    }
    
protected :
    ~HatchPattern()
    {
    }
    int         m_hatch ;
    CGRect      m_imageBounds ;
} ;

void wxMacCGContext::SetPen( const wxPen &pen )
{
    m_pen = pen ;
    if ( m_cgContext == NULL )
        return ;
    bool fill = m_brush.GetStyle() != wxTRANSPARENT ;
    bool stroke = pen.GetStyle() != wxTRANSPARENT ;
    
#if 0
    // we can benchmark performance, should go into a setting later
    CGContextSetShouldAntialias( m_cgContext , false ) ;
#endif
    if ( fill | stroke )
    {
        // setup brushes
        m_mode = kCGPathFill ; // just a default

        if ( fill )
        {
            m_mode = kCGPathFill ;
        }
        if ( stroke )
        {
            RGBColor col = MAC_WXCOLORREF( pen.GetColour().GetPixel() ) ;
            CGContextSetRGBStrokeColor( m_cgContext , col.red / 65536.0 , col.green / 65536.0 , col.blue / 65536.0 , 1.0 ) ;

            /* TODO * m_dc->m_scaleX */
            float penWidth = pen.GetWidth();
            if (penWidth <= 0.0)
                penWidth = 0.1;
            CGContextSetLineWidth( m_cgContext , penWidth ) ;
            
            CGLineCap cap ;
            switch( pen.GetCap() )
            {
                case wxCAP_ROUND :
                    cap = kCGLineCapRound ;
                    break ;
                case wxCAP_PROJECTING :
                    cap = kCGLineCapSquare ;
                    break ;
                case wxCAP_BUTT :
                    cap = kCGLineCapButt ;
                    break ;
                default :
                    cap = kCGLineCapButt ;
                    break ;
            }

            CGLineJoin join ;
            switch( pen.GetJoin() )
            {
                case wxJOIN_BEVEL :
                    join = kCGLineJoinBevel ;
                    break ;
                case wxJOIN_MITER :
                    join = kCGLineJoinMiter ;
                    break ;
                case wxJOIN_ROUND :
                    join = kCGLineJoinRound ;
                    break ;
                default :
                    join = kCGLineJoinMiter ;
                    break; 
            }
            CGContextSetLineJoin( m_cgContext , join ) ;

            m_mode = kCGPathStroke ;
            int count = 0 ;
            
            const float *lengths = NULL ;
            float *userLengths = NULL ;

            const float dashUnit = ( penWidth < 1.0 ) ? 1.0 : penWidth;
  
            const float dotted[] = { dashUnit  , dashUnit + 2.0 };
            const float short_dashed[] = { 9.0 , 6.0 };
            const float dashed[] = { 19.0 , 9.0 };
            const float dotted_dashed[] = { 9.0 , 6.0 , 3.0 , 3.0 };


            switch( pen.GetStyle() )
            {
                case wxSOLID :
                    break ;
                case wxDOT :
                    lengths = dotted ;
                    count = WXSIZEOF(dotted);
                    break ;
                case wxLONG_DASH :
                    lengths = dashed ;
                    count = WXSIZEOF(dashed) ;
                    break ;
                case wxSHORT_DASH :
                    lengths = short_dashed ;
                    count = WXSIZEOF(short_dashed) ;
                    break ;
                case wxDOT_DASH :
                    lengths = dotted_dashed ;
                    count = WXSIZEOF(dotted_dashed);
                    break ;
                case wxUSER_DASH :
                    wxDash *dashes ;
                    count = pen.GetDashes( &dashes ) ;
                    if ((dashes != NULL) && (count > 0))
                    {
                        userLengths = new float[count] ;
                        for( int i = 0 ; i < count ; ++i )
                        {
                            userLengths[i] = dashes[i] * dashUnit ;

                            if ( i % 2 == 1 && userLengths[i] < dashUnit + 2.0 )
                                userLengths[i] = dashUnit + 2.0 ;
                            else if ( i % 2 == 0 && userLengths[i] < dashUnit )
                                userLengths[i] = dashUnit ;
                        }
                    }
                    lengths = userLengths ;
                    break ;
                case wxSTIPPLE :
                    {
                        float  alphaArray[1] = { 1.0 } ;
                        wxBitmap* bmp = pen.GetStipple() ;
                        if ( bmp && bmp->Ok() ) 
                        {
                            wxMacCFRefHolder<CGColorSpaceRef> patternSpace( CGColorSpaceCreatePattern( NULL ) ) ;
                            CGContextSetStrokeColorSpace( m_cgContext , patternSpace ) ;
                            wxMacCFRefHolder<CGPatternRef> pattern( *( new ImagePattern( bmp , CGContextGetCTM( m_cgContext ) ) ) );
                            CGContextSetStrokePattern( m_cgContext, pattern , alphaArray ) ;
                        }
                    }
                    break ;
                default :
                    {
                        wxMacCFRefHolder<CGColorSpaceRef> patternSpace( CGColorSpaceCreatePattern( wxMacGetGenericRGBColorSpace() ) ) ;
                        CGContextSetStrokeColorSpace( m_cgContext , patternSpace ) ;
                        wxMacCFRefHolder<CGPatternRef> pattern( *( new HatchPattern( pen.GetStyle() , CGContextGetCTM( m_cgContext ) ) ) );
                        
                        RGBColor col = MAC_WXCOLORREF( pen.GetColour().GetPixel() ) ;
                        float  colorArray[4] = { col.red / 65536.0 , col.green / 65536.0 , col.blue / 65536.0 , 1.0 } ;
                        
                        CGContextSetStrokePattern( m_cgContext, pattern , colorArray ) ;
                    }
                    break ; 
            }

            if ((lengths != NULL) && (count > 0))
            {
                CGContextSetLineDash( m_cgContext , 0 , lengths , count ) ;
                // force the line cap, otherwise we get artifacts (overlaps) and just solid lines
                cap = kCGLineCapButt ;
            }
            else 
            {
               CGContextSetLineDash( m_cgContext , 0 , NULL , 0 ) ;
            }
            CGContextSetLineCap( m_cgContext , cap ) ;

            delete[] userLengths ;
        }
        if ( fill && stroke )
        {
            m_mode = kCGPathFillStroke ;
        }
    }
}

void wxMacCGContext::SetBrush( const wxBrush &brush )
{
    m_brush = brush ;
    if ( m_cgContext == NULL )
        return ;

    bool fill = brush.GetStyle() != wxTRANSPARENT ;
    bool stroke = m_pen.GetStyle() != wxTRANSPARENT ;

#if 0
    // we can benchmark performance, should go into a setting later
    CGContextSetShouldAntialias( m_cgContext , false ) ;
#endif

    if ( fill | stroke )
    {
        // setup brushes
        m_mode = kCGPathFill ; // just a default

        if ( fill )
        {
            if ( brush.GetStyle() == wxSOLID )
            {
                RGBColor col = MAC_WXCOLORREF( brush.GetColour().GetPixel() ) ;
                CGContextSetRGBFillColor( m_cgContext , col.red / 65536.0 , col.green / 65536.0 , col.blue / 65536.0 , 1.0 ) ;
            }
            else if ( brush.IsHatch() )
            {
                wxMacCFRefHolder<CGColorSpaceRef> patternSpace( CGColorSpaceCreatePattern( wxMacGetGenericRGBColorSpace() ) ) ;
                CGContextSetFillColorSpace( m_cgContext , patternSpace ) ;
                wxMacCFRefHolder<CGPatternRef> pattern( *( new HatchPattern( brush.GetStyle() , CGContextGetCTM( m_cgContext ) ) ) );

                RGBColor col = MAC_WXCOLORREF( brush.GetColour().GetPixel() ) ;
                float  colorArray[4] = { col.red / 65536.0 , col.green / 65536.0 , col.blue / 65536.0 , 1.0 } ;
                
                CGContextSetFillPattern( m_cgContext, pattern , colorArray ) ;
            }
            else
            {
                // now brush is a bitmap
                float  alphaArray[1] = { 1.0 } ;
                wxBitmap* bmp = brush.GetStipple() ;
                if ( bmp && bmp->Ok() ) 
                {
                    wxMacCFRefHolder<CGColorSpaceRef> patternSpace( CGColorSpaceCreatePattern( NULL ) ) ;
                    CGContextSetFillColorSpace( m_cgContext , patternSpace ) ;
                    wxMacCFRefHolder<CGPatternRef> pattern( *( new ImagePattern( bmp , CGContextGetCTM( m_cgContext ) ) ) );
                    CGContextSetFillPattern( m_cgContext, pattern , alphaArray ) ;
                }
            }
            m_mode = kCGPathFill ;
        }
        if ( stroke )
        {
            m_mode = kCGPathStroke ;
        }
        if ( fill && stroke )
        {
            m_mode = kCGPathFillStroke ;
        }
    }
}

void AddEllipticArcToPath(CGContextRef c, CGPoint center, float a, float b, float fromDegree , float toDegree )
{
    CGContextSaveGState(c);
    CGContextTranslateCTM(c, center.x, center.y);
    CGContextScaleCTM(c, a, b);
    CGContextMoveToPoint(c, 1, 0);
    CGContextAddArc(c, 0, 0, 1, DegToRad(fromDegree), DegToRad(toDegree), 0);
    CGContextClosePath(c);
    CGContextRestoreGState(c);
} 

void AddRoundedRectToPath(CGContextRef c, CGRect rect, float ovalWidth,
      float ovalHeight)
{
    float fw, fh;
    if (ovalWidth == 0 || ovalHeight == 0) 
    {
        CGContextAddRect(c, rect);
        return;
    }
    CGContextSaveGState(c);
    CGContextTranslateCTM(c, CGRectGetMinX(rect), CGRectGetMinY(rect));
    CGContextScaleCTM(c, ovalWidth, ovalHeight);
    fw = CGRectGetWidth(rect) / ovalWidth;
    fh = CGRectGetHeight(rect) / ovalHeight;
    CGContextMoveToPoint(c, fw, fh/2);
    CGContextAddArcToPoint(c, fw, fh, fw/2, fh, 1);
    CGContextAddArcToPoint(c, 0, fh, 0, fh/2, 1);
    CGContextAddArcToPoint(c, 0, 0, fw/2, 0, 1);
    CGContextAddArcToPoint(c, fw, 0, fw, fh/2, 1);
    CGContextClosePath(c);
    CGContextRestoreGState(c);
} 

wxDC::wxDC()
{
    m_ok = false;
    m_colour = true;
    m_mm_to_pix_x = mm2pt;
    m_mm_to_pix_y = mm2pt;

    m_externalDeviceOriginX = 0;
    m_externalDeviceOriginY = 0;
    m_logicalScaleX = 1.0;
    m_logicalScaleY = 1.0;
    m_userScaleX = 1.0;
    m_userScaleY = 1.0;
    m_scaleX = 1.0;
    m_scaleY = 1.0;
    m_needComputeScaleX = false;
    m_needComputeScaleY = false;

    m_ok = false ;
    m_macPort = 0 ;
    m_macLocalOrigin.x = m_macLocalOrigin.y = 0 ;

    m_pen = *wxBLACK_PEN;
    m_font = *wxNORMAL_FONT;
    m_brush = *wxWHITE_BRUSH;

    m_macATSUIStyle = NULL ;

    m_graphicContext = NULL ;
}

wxDC::~wxDC(void)
{
    if( m_macATSUIStyle )
    {
        ::ATSUDisposeStyle((ATSUStyle)m_macATSUIStyle);
        m_macATSUIStyle = NULL ;
    }

    delete m_graphicContext ;  
}

void wxDC::DoDrawBitmap( const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask )
{
    wxCHECK_RET( Ok(), wxT("invalid window dc") );
    wxCHECK_RET( bmp.Ok(), wxT("invalid bitmap") );
    wxCoord xx = XLOG2DEVMAC(x);
    wxCoord yy = YLOG2DEVMAC(y);
    wxCoord w = bmp.GetWidth();
    wxCoord h = bmp.GetHeight();
    wxCoord ww = XLOG2DEVREL(w);
    wxCoord hh = YLOG2DEVREL(h);

    CGContextRef cg = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
    CGImageRef image = (CGImageRef)( bmp.CGImageCreate() ) ;
    HIRect r = CGRectMake( xx , yy , ww , hh ) ;
    HIViewDrawCGImage( cg , &r , image ) ;
    CGImageRelease( image ) ;
}

void wxDC::DoDrawIcon( const wxIcon &icon, wxCoord x, wxCoord y )
{
    wxCHECK_RET(Ok(), wxT("Invalid dc  wxDC::DoDrawIcon"));
    wxCHECK_RET(icon.Ok(), wxT("Invalid icon wxDC::DoDrawIcon"));

    wxCoord xx = XLOG2DEVMAC(x);
    wxCoord yy = YLOG2DEVMAC(y);
    wxCoord w = icon.GetWidth();
    wxCoord h = icon.GetHeight();
    wxCoord ww = XLOG2DEVREL(w);
    wxCoord hh = YLOG2DEVREL(h);

    CGContextRef cg = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
    CGRect r = CGRectMake( 00 , 00 , ww , hh ) ;
    CGContextSaveGState(cg);    
    CGContextTranslateCTM(cg, xx , yy + hh );
    CGContextScaleCTM(cg, 1, -1);
    PlotIconRefInContext( cg , &r , kAlignNone , kTransformNone ,
        NULL , kPlotIconRefNormalFlags , MAC_WXHICON( icon.GetHICON() ) ) ;
    CGContextRestoreGState( cg ) ;
}

void wxDC::DoSetClippingRegion( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{
    wxCHECK_RET(Ok(), wxT("wxDC::DoSetClippingRegion  Invalid DC"));
    wxCoord xx, yy, ww, hh;
    xx = XLOG2DEVMAC(x);
    yy = YLOG2DEVMAC(y);
    ww = XLOG2DEVREL(width);
    hh = YLOG2DEVREL(height);

    CGContextRef cgContext = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
    CGRect clipRect = CGRectMake( xx ,yy , ww, hh ) ;
    CGContextClipToRect( cgContext , clipRect ) ;

//    SetRectRgn( (RgnHandle) m_macCurrentClipRgn , xx , yy , xx + ww , yy + hh ) ;
//    SectRgn( (RgnHandle) m_macCurrentClipRgn , (RgnHandle) m_macBoundaryClipRgn , (RgnHandle) m_macCurrentClipRgn ) ;
    if( m_clipping )
    {
        m_clipX1 = wxMax( m_clipX1 , xx );
        m_clipY1 = wxMax( m_clipY1 , yy );
        m_clipX2 = wxMin( m_clipX2, (xx + ww));
        m_clipY2 = wxMin( m_clipY2, (yy + hh));
    }
    else
    {
        m_clipping = true;
        m_clipX1 = xx;
        m_clipY1 = yy;
        m_clipX2 = xx + ww;
        m_clipY2 = yy + hh;
    }
    // TODO as soon as we don't reset the context for each operation anymore
    // we have to update the context as well
}

void wxDC::DoSetClippingRegionAsRegion( const wxRegion &region  )
{
    wxCHECK_RET( Ok(), wxT("invalid window dc") ) ;
    if (region.Empty())
    {
        DestroyClippingRegion();
        return;
    }
    wxCoord x, y, w, h;
    region.GetBox( x, y, w, h );
    wxCoord xx, yy, ww, hh;
    xx = XLOG2DEVMAC(x);
    yy = YLOG2DEVMAC(y);
    ww = XLOG2DEVREL(w);
    hh = YLOG2DEVREL(h);
    // if we have a scaling that we cannot map onto native regions
    // we must use the box
    if ( ww != w || hh != h )
    {
        wxDC::DoSetClippingRegion( x, y, w, h );
    }
    else
    {
        /*
        CopyRgn( (RgnHandle) region.GetWXHRGN() , (RgnHandle) m_macCurrentClipRgn ) ;
        if ( xx != x || yy != y )
        {
            OffsetRgn( (RgnHandle) m_macCurrentClipRgn , xx - x , yy - y ) ;
        }
        SectRgn( (RgnHandle) m_macCurrentClipRgn , (RgnHandle) m_macBoundaryClipRgn , (RgnHandle) m_macCurrentClipRgn ) ;
        */
        if( m_clipping )
        {
            m_clipX1 = wxMax( m_clipX1 , xx );
            m_clipY1 = wxMax( m_clipY1 , yy );
            m_clipX2 = wxMin( m_clipX2, (xx + ww));
            m_clipY2 = wxMin( m_clipY2, (yy + hh));
        }
        else
        {
            m_clipping = true;
            m_clipX1 = xx;
            m_clipY1 = yy;
            m_clipX2 = xx + ww;
            m_clipY2 = yy + hh;
        }
    }
}

void wxDC::DestroyClippingRegion()
{
//    CopyRgn( (RgnHandle) m_macBoundaryClipRgn , (RgnHandle) m_macCurrentClipRgn ) ;
    CGContextRef cgContext = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
    CGContextRestoreGState( cgContext );    
    CGContextSaveGState( cgContext );    
    m_graphicContext->SetPen( m_pen ) ;
    m_graphicContext->SetBrush( m_brush ) ;
    m_clipping = false;
}

void wxDC::DoGetSizeMM( int* width, int* height ) const
{
    int w = 0;
    int h = 0;
    GetSize( &w, &h );
    *width = long( double(w) / (m_scaleX*m_mm_to_pix_x) );
    *height = long( double(h) / (m_scaleY*m_mm_to_pix_y) );
}

void wxDC::SetTextForeground( const wxColour &col )
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));
    if ( col != m_textForegroundColour )
    {
        m_textForegroundColour = col;
        MacInstallFont() ;
    }
}

void wxDC::SetTextBackground( const wxColour &col )
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));
    m_textBackgroundColour = col;
}

void wxDC::SetMapMode( int mode )
{
    switch (mode)
    {
    case wxMM_TWIPS:
        SetLogicalScale( twips2mm*m_mm_to_pix_x, twips2mm*m_mm_to_pix_y );
        break;
    case wxMM_POINTS:
        SetLogicalScale( pt2mm*m_mm_to_pix_x, pt2mm*m_mm_to_pix_y );
        break;
    case wxMM_METRIC:
        SetLogicalScale( m_mm_to_pix_x, m_mm_to_pix_y );
        break;
    case wxMM_LOMETRIC:
        SetLogicalScale( m_mm_to_pix_x/10.0, m_mm_to_pix_y/10.0 );
        break;
    default:
    case wxMM_TEXT:
        SetLogicalScale( 1.0, 1.0 );
        break;
    }
    if (mode != wxMM_TEXT)
    {
        m_needComputeScaleX = true;
        m_needComputeScaleY = true;
    }
}

void wxDC::SetUserScale( double x, double y )
{
    // allow negative ? -> no
    m_userScaleX = x;
    m_userScaleY = y;
    ComputeScaleAndOrigin();
}

void wxDC::SetLogicalScale( double x, double y )
{
    // allow negative ?
    m_logicalScaleX = x;
    m_logicalScaleY = y;
    ComputeScaleAndOrigin();
}

void wxDC::SetLogicalOrigin( wxCoord x, wxCoord y )
{
    m_logicalOriginX = x * m_signX;   // is this still correct ?
    m_logicalOriginY = y * m_signY;
    ComputeScaleAndOrigin();
}

void wxDC::SetDeviceOrigin( wxCoord x, wxCoord y )
{
    m_externalDeviceOriginX = x;
    m_externalDeviceOriginY = y;
    ComputeScaleAndOrigin();
}

void wxDC::SetAxisOrientation( bool xLeftRight, bool yBottomUp )
{
    m_signX = (xLeftRight ?  1 : -1);
    m_signY = (yBottomUp  ? -1 :  1);
    ComputeScaleAndOrigin();
}

wxSize wxDC::GetPPI() const
{
    return wxSize(72, 72);
}

int wxDC::GetDepth() const
{
    return 32 ;
}

void wxDC::ComputeScaleAndOrigin()
{
    // CMB: copy scale to see if it changes
    double origScaleX = m_scaleX;
    double origScaleY = m_scaleY;
    m_scaleX = m_logicalScaleX * m_userScaleX;
    m_scaleY = m_logicalScaleY * m_userScaleY;
    m_deviceOriginX = m_externalDeviceOriginX;
    m_deviceOriginY = m_externalDeviceOriginY;
    // CMB: if scale has changed call SetPen to recalulate the line width
    if (m_scaleX != origScaleX || m_scaleY != origScaleY)
    {
        // this is a bit artificial, but we need to force wxDC to think
        // the pen has changed
        wxPen pen(GetPen());
        m_pen = wxNullPen;
        SetPen(pen);
    }
}

void  wxDC::SetPalette( const wxPalette& palette )
{
}

void  wxDC::SetBackgroundMode( int mode )
{
    m_backgroundMode = mode ;
}

void  wxDC::SetFont( const wxFont &font )
{
    m_font = font;
    MacInstallFont() ;
}

void  wxDC::SetPen( const wxPen &pen )
{
    if ( m_pen == pen )
        return ;
    m_pen = pen;
    if ( m_graphicContext )
    {
        if ( m_pen.GetStyle() == wxSOLID || m_pen.GetStyle() == wxTRANSPARENT )
        {
            m_graphicContext->SetPen( m_pen ) ;
        }
        else
        {
            // we have to compensate for moved device origins etc. otherwise patterned pens are standing still
            // eg when using a wxScrollWindow and scrolling around
            CGContextRef cgContext = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
            int origX = XLOG2DEVMAC(0) ;
            int origY = YLOG2DEVMAC(0) ;
            CGContextTranslateCTM (cgContext,origX,origY);
            m_graphicContext->SetPen( m_pen ) ;
            CGContextTranslateCTM (cgContext,-origX,-origY);
        }
    }
}

void  wxDC::SetBrush( const wxBrush &brush )
{
    if (m_brush == brush)
        return;
    m_brush = brush;
    if ( m_graphicContext )
    {
        if ( brush.GetStyle() == wxSOLID || brush.GetStyle() == wxTRANSPARENT )
        {
            m_graphicContext->SetBrush( m_brush ) ;
        }
        else
        {
            // we have to compensate for moved device origins etc. otherwise patterned brushes are standing still
            // eg when using a wxScrollWindow and scrolling around
            CGContextRef cgContext = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
            int origX = XLOG2DEVMAC(0) ;
            int origY = YLOG2DEVMAC(0) ;
            CGContextTranslateCTM (cgContext,origX,origY);
            m_graphicContext->SetBrush( m_brush ) ;
            CGContextTranslateCTM (cgContext,-origX,-origY);
        }
    }
}

void  wxDC::SetBackground( const wxBrush &brush )
{
    if (m_backgroundBrush == brush)
        return;
    m_backgroundBrush = brush;
    if (!m_backgroundBrush.Ok())
        return;
}

void  wxDC::SetLogicalFunction( int function )
{
    if (m_logicalFunction == function)
        return;
    m_logicalFunction = function ;
}

extern bool wxDoFloodFill(wxDC *dc, wxCoord x, wxCoord y,
                          const wxColour & col, int style);

bool wxDC::DoFloodFill(wxCoord x, wxCoord y,
                       const wxColour& col, int style)
{
    return wxDoFloodFill(this, x, y, col, style);
}

bool  wxDC::DoGetPixel( wxCoord x, wxCoord y, wxColour *col ) const
{
    wxCHECK_MSG( Ok(), false, wxT("wxDC::DoGetPixel  Invalid DC") );
    wxCHECK_MSG( Ok(), false, wxT("wxDC::DoGetPixel  Invalid DC") );
    wxMacPortSaver helper((CGrafPtr)m_macPort) ;
    RGBColor colour;
    GetCPixel( 
        XLOG2DEVMAC(x) + m_macLocalOriginInPort.x - m_macLocalOrigin.x, 
        YLOG2DEVMAC(y) + m_macLocalOriginInPort.y - m_macLocalOrigin.y, &colour );
    // Convert from Mac colour to wx
    col->Set( colour.red   >> 8,
        colour.green >> 8,
        colour.blue  >> 8);
    return true ;
}

void  wxDC::DoDrawLine( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2 )
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));
    
    if ( m_logicalFunction != wxCOPY )
        return ;
    
    wxCoord xx1 = XLOG2DEVMAC(x1) ;
    wxCoord yy1 = YLOG2DEVMAC(y1) ;
    wxCoord xx2 = XLOG2DEVMAC(x2) ;
    wxCoord yy2 = YLOG2DEVMAC(y2) ;

    wxGraphicPath* path = m_graphicContext->CreatePath() ;
    path->MoveToPoint( xx1 , yy1 ) ;
    path->AddLineToPoint( xx2 , yy2 ) ;
    path->CloseSubpath() ;
    m_graphicContext->StrokePath( path ) ;
    delete path ;

    CalcBoundingBox(x1, y1);
    CalcBoundingBox(x2, y2);
}

void  wxDC::DoCrossHair( wxCoord x, wxCoord y )
{
    wxCHECK_RET( Ok(), wxT("wxDC::DoCrossHair  Invalid window dc") );

    if ( m_logicalFunction != wxCOPY )
        return ;
    
    int w = 0;
    int h = 0;
    GetSize( &w, &h );
    wxCoord xx = XLOG2DEVMAC(x);
    wxCoord yy = YLOG2DEVMAC(y);

    wxGraphicPath* path = m_graphicContext->CreatePath() ;
    path->MoveToPoint( XLOG2DEVMAC(0), yy  ) ;
    path->AddLineToPoint( XLOG2DEVMAC(w), yy ) ;
    path->CloseSubpath() ;
    path->MoveToPoint( xx, YLOG2DEVMAC(0) ) ;
    path->AddLineToPoint( xx, YLOG2DEVMAC(h) ) ;
    path->CloseSubpath() ;
    m_graphicContext->StrokePath( path ) ;
    delete path ;

    CalcBoundingBox(x, y);
    CalcBoundingBox(x+w, y+h);
}

void  wxDC::DoDrawArc( wxCoord x1, wxCoord y1,
                      wxCoord x2, wxCoord y2,
                      wxCoord xc, wxCoord yc )
{
    wxCHECK_RET(Ok(), wxT("wxDC::DoDrawArc  Invalid DC"));

    if ( m_logicalFunction != wxCOPY )
        return ;

    wxCoord xx1 = XLOG2DEVMAC(x1);
    wxCoord yy1 = YLOG2DEVMAC(y1);
    wxCoord xx2 = XLOG2DEVMAC(x2);
    wxCoord yy2 = YLOG2DEVMAC(y2);
    wxCoord xxc = XLOG2DEVMAC(xc);
    wxCoord yyc = YLOG2DEVMAC(yc);
    double dx = xx1 - xxc;
    double dy = yy1 - yyc;
    double radius = sqrt((double)(dx*dx+dy*dy));
    wxCoord rad   = (wxCoord)radius;
    double sa, ea;
    if (xx1 == xx2 && yy1 == yy2)
    {
        sa = 0.0;
        ea = 360.0;
    }
    else if (radius == 0.0)
    {
        sa = ea = 0.0;
    }
    else
    {
        sa = (xx1 - xxc == 0) ?
            (yy1 - yyc < 0) ? 90.0 : -90.0 :
        -atan2(double(yy1-yyc), double(xx1-xxc)) * RAD2DEG;
        ea = (xx2 - xxc == 0) ?
            (yy2 - yyc < 0) ? 90.0 : -90.0 :
        -atan2(double(yy2-yyc), double(xx2-xxc)) * RAD2DEG;
    }

    bool fill = m_brush.GetStyle() != wxTRANSPARENT ;
    wxMacCGContext* mctx = ((wxMacCGContext*) m_graphicContext) ;
    CGContextRef ctx = mctx->GetNativeContext() ;
    CGContextSaveGState( ctx ) ;
    CGContextTranslateCTM( ctx, xxc , yyc );
    CGContextScaleCTM( ctx , 1 , -1 ) ;
    if ( fill )
        CGContextMoveToPoint( ctx , 0 , 0 ) ;
    CGContextAddArc( ctx, 0, 0 , rad , DegToRad(sa), DegToRad(ea), 0);
    if ( fill )
        CGContextAddLineToPoint( ctx , 0 , 0 ) ;
    CGContextRestoreGState( ctx ) ;
    CGContextDrawPath( ctx , mctx->GetDrawingMode() ) ;
}

void  wxDC::DoDrawEllipticArc( wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                              double sa, double ea )
{
    wxCHECK_RET(Ok(), wxT("wxDC::DoDrawEllepticArc  Invalid DC"));

    if ( m_logicalFunction != wxCOPY )
        return ;

    wxCoord xx = XLOG2DEVMAC(x);
    wxCoord yy = YLOG2DEVMAC(y);
    wxCoord ww = m_signX * XLOG2DEVREL(w);
    wxCoord hh = m_signY * YLOG2DEVREL(h);
    // handle -ve width and/or height
    if (ww < 0) { ww = -ww; xx = xx - ww; }
    if (hh < 0) { hh = -hh; yy = yy - hh; }

    bool fill = m_brush.GetStyle() != wxTRANSPARENT ;

    wxMacCGContext* mctx = ((wxMacCGContext*) m_graphicContext) ;
    CGContextRef ctx = mctx->GetNativeContext() ;

    CGContextSaveGState( ctx ) ;
    CGContextTranslateCTM( ctx, xx + ww / 2, yy + hh / 2);
    CGContextScaleCTM( ctx , 1 * ww / 2 , -1 * hh / 2 ) ;
    if ( fill )
        CGContextMoveToPoint( ctx , 0 , 0 ) ;
    CGContextAddArc( ctx, 0, 0, 1, DegToRad(sa), DegToRad(ea), 0);
    if ( fill )
        CGContextAddLineToPoint( ctx , 0 , 0 ) ;
    CGContextRestoreGState( ctx ) ;
    CGContextDrawPath( ctx , mctx->GetDrawingMode() ) ;    
}

void  wxDC::DoDrawPoint( wxCoord x, wxCoord y )
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));
    DoDrawLine( x , y , x + 1 , y + 1 ) ;
}

void  wxDC::DoDrawLines(int n, wxPoint points[],
                        wxCoord xoffset, wxCoord yoffset)
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));
    
    if ( m_logicalFunction != wxCOPY )
        return ;

    wxCoord x1, x2 , y1 , y2 ;
    x1 = XLOG2DEVMAC(points[0].x + xoffset);
    y1 = YLOG2DEVMAC(points[0].y + yoffset);
    wxGraphicPath* path = m_graphicContext->CreatePath() ;
    path->MoveToPoint( x1 , y1 ) ;
    for (int i = 1; i < n; i++)
    {
        x2 = XLOG2DEVMAC(points[i].x + xoffset);
        y2 = YLOG2DEVMAC(points[i].y + yoffset);

        path->AddLineToPoint( x2 , y2 ) ;
    }
    m_graphicContext->StrokePath( path ) ;
    delete path ;
}

#if wxUSE_SPLINES
void wxDC::DoDrawSpline(wxList *points)
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));
    
    if ( m_logicalFunction != wxCOPY )
        return ;
    
    wxGraphicPath* path = m_graphicContext->CreatePath() ;
    
    wxList::compatibility_iterator node = points->GetFirst();
    if (node == wxList::compatibility_iterator())
        // empty list
        return;
    
    wxPoint *p = (wxPoint *)node->GetData();
    
    wxCoord x1 = p->x;
    wxCoord y1 = p->y;
    
    node = node->GetNext();
    p = (wxPoint *)node->GetData();
    
    wxCoord x2 = p->x;
    wxCoord y2 = p->y;
    wxCoord cx1 = ( x1 + x2 ) / 2;
    wxCoord cy1 = ( y1 + y2 ) / 2;
    
    path->MoveToPoint( XLOG2DEVMAC( x1 ) , XLOG2DEVMAC( y1 ) ) ;
    path->AddLineToPoint( XLOG2DEVMAC( cx1 ) , XLOG2DEVMAC( cy1 ) ) ;
    
#if !wxUSE_STL
    while ((node = node->GetNext()) != NULL)
#else
    while ((node = node->GetNext()))
#endif // !wxUSE_STL
    {
        p = (wxPoint *)node->GetData();
        x1 = x2;
        y1 = y2;
        x2 = p->x;
        y2 = p->y;
        wxCoord cx4 = (x1 + x2) / 2;
        wxCoord cy4 = (y1 + y2) / 2;
        
        path->AddQuadCurveToPoint( XLOG2DEVMAC( x1 ) , XLOG2DEVMAC( y1 ) , 
                                   XLOG2DEVMAC( cx4 ) , XLOG2DEVMAC( cy4 ) ) ;
        
        cx1 = cx4;
        cy1 = cy4;
    }
            
    path->AddLineToPoint( XLOG2DEVMAC( x2 ) , XLOG2DEVMAC( y2 ) ) ;
    
    m_graphicContext->StrokePath( path ) ;
    delete path ;
}
#endif

void  wxDC::DoDrawPolygon(int n, wxPoint points[],
                          wxCoord xoffset, wxCoord yoffset,
                          int fillStyle )
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));
        wxCoord x1, x2 , y1 , y2 ;
    if ( n== 0 || (m_brush.GetStyle() == wxTRANSPARENT && m_pen.GetStyle() == wxTRANSPARENT ) )
        return ;
        
    if ( m_logicalFunction != wxCOPY )
        return ;

    x2 = x1 = XLOG2DEVMAC(points[0].x + xoffset);
    y2 = y1 = YLOG2DEVMAC(points[0].y + yoffset);
    
    wxGraphicPath* path = m_graphicContext->CreatePath() ;
    path->MoveToPoint( x1 , y1 ) ;
    for (int i = 1; i < n; i++)
    {
        x2 = XLOG2DEVMAC(points[i].x + xoffset);
        y2 = YLOG2DEVMAC(points[i].y + yoffset);

        path->AddLineToPoint( x2 , y2 ) ;
    }
    if ( x1 != x2 || y1 != y2 )
    {
        path->AddLineToPoint( x1,y1 ) ;
    }
    path->CloseSubpath() ;
    m_graphicContext->DrawPath( path , fillStyle ) ;
    delete path ;
}

void wxDC::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));

    if ( m_logicalFunction != wxCOPY )
        return ;

    wxCoord xx = XLOG2DEVMAC(x);
    wxCoord yy = YLOG2DEVMAC(y);
    wxCoord ww = m_signX * XLOG2DEVREL(width);
    wxCoord hh = m_signY * YLOG2DEVREL(height);
    // CMB: draw nothing if transformed w or h is 0
    if (ww == 0 || hh == 0)
        return;
    // CMB: handle -ve width and/or height
    if (ww < 0)
    {
        ww = -ww;
        xx = xx - ww;
    }
    if (hh < 0)
    {
        hh = -hh;
        yy = yy - hh;
    }
    wxGraphicPath* path = m_graphicContext->CreatePath() ;
    path->AddRectangle( xx , yy , ww , hh ) ;
    m_graphicContext->DrawPath( path ) ;
    delete path ;
}

void  wxDC::DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                   wxCoord width, wxCoord height,
                                   double radius)
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));

    if ( m_logicalFunction != wxCOPY )
        return ;


    if (radius < 0.0)
        radius = - radius * ((width < height) ? width : height);
    wxCoord xx = XLOG2DEVMAC(x);
    wxCoord yy = YLOG2DEVMAC(y);
    wxCoord ww = m_signX * XLOG2DEVREL(width);
    wxCoord hh = m_signY * YLOG2DEVREL(height);
    // CMB: draw nothing if transformed w or h is 0
    if (ww == 0 || hh == 0)
        return;
    // CMB: handle -ve width and/or height
    if (ww < 0)
    {
        ww = -ww;
        xx = xx - ww;
    }
    if (hh < 0)
    {
        hh = -hh;
        yy = yy - hh;
    }
    wxMacCGContext* mctx = ((wxMacCGContext*) m_graphicContext) ;
    CGContextRef ctx = mctx->GetNativeContext() ;
    AddRoundedRectToPath( ctx  , CGRectMake( xx , yy , ww , hh ) , 16 ,16  ) ;
    CGContextDrawPath( ctx , mctx->GetDrawingMode() ) ;
}

void  wxDC::DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));

    if ( m_logicalFunction != wxCOPY )
        return ;

    wxCoord xx = XLOG2DEVMAC(x);
    wxCoord yy = YLOG2DEVMAC(y);
    wxCoord ww = m_signX * XLOG2DEVREL(width);
    wxCoord hh = m_signY * YLOG2DEVREL(height);
    // CMB: draw nothing if transformed w or h is 0
    if (ww == 0 || hh == 0)
        return;
    // CMB: handle -ve width and/or height
    if (ww < 0)
    {
        ww = -ww;
        xx = xx - ww;
    }
    if (hh < 0)
    {
        hh = -hh;
        yy = yy - hh;
    }

    wxMacCGContext* mctx = ((wxMacCGContext*) m_graphicContext) ;
    CGContextRef ctx = mctx->GetNativeContext() ;
    CGContextSaveGState( ctx ) ;
    CGContextTranslateCTM( ctx, xx + ww / 2, yy + hh / 2);
    CGContextScaleCTM( ctx , ww / 2 , hh / 2 ) ;
    CGContextAddArc( ctx, 0, 0, 1,  0 , 2*M_PI , 0);
    CGContextRestoreGState( ctx ) ;
    CGContextDrawPath( ctx , mctx->GetDrawingMode() ) ;
}

bool  wxDC::CanDrawBitmap(void) const
{
    return true ;
}

bool  wxDC::DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                   wxDC *source, wxCoord xsrc, wxCoord ysrc, int logical_func , bool useMask,
                   wxCoord xsrcMask,  wxCoord ysrcMask )
{
    wxCHECK_MSG(Ok(), false, wxT("wxDC::DoBlit Illegal dc"));
    wxCHECK_MSG(source->Ok(), false, wxT("wxDC::DoBlit  Illegal source DC"));
    if ( logical_func == wxNO_OP )
        return true ;
    if (xsrcMask == -1 && ysrcMask == -1)
    {
        xsrcMask = xsrc; ysrcMask = ysrc;
    }

    wxCoord yysrc = source->YLOG2DEVMAC(ysrc) ;
    wxCoord xxsrc = source->XLOG2DEVMAC(xsrc)  ;
    wxCoord wwsrc = source->XLOG2DEVREL(width ) ;
    wxCoord hhsrc = source->YLOG2DEVREL(height) ;
    
    wxCoord yydest = YLOG2DEVMAC(ydest) ;
    wxCoord xxdest = XLOG2DEVMAC(xdest) ;
    wxCoord wwdest = XLOG2DEVREL(width ) ;
    wxCoord hhdest = YLOG2DEVREL(height) ;
    
    wxMemoryDC* memdc = dynamic_cast<wxMemoryDC*>(source) ;
    if ( memdc && logical_func == wxCOPY )
    {
        wxBitmap blit = memdc->GetSelectedObject() ;
        wxASSERT_MSG( blit.Ok() , wxT("Invalid bitmap for blitting") ) ;

        wxCoord bmpwidth = blit.GetWidth();
        wxCoord bmpheight = blit.GetHeight();
        
        if ( xxsrc != 0 || yysrc != 0 || bmpwidth != wwsrc || bmpheight != hhsrc )
        {
            wwsrc = wxMin( wwsrc , bmpwidth - xxsrc ) ;
            hhsrc = wxMin( hhsrc , bmpheight - yysrc ) ;
            if ( wwsrc > 0 && hhsrc > 0 )
            {
                if ( xxsrc >= 0 && yysrc >= 0 )
                {
                    wxRect subrect( xxsrc, yysrc, wwsrc , hhsrc ) ;
                    blit = blit.GetSubBitmap( subrect ) ;
                }
                else
                {
                    // in this case we'd probably have to adjust the different coordinates, but
                    // we have to find out proper contract first
                    blit = wxNullBitmap ;
                }
            }
            else
            {
                blit = wxNullBitmap ;
            }
        }
        if ( blit.Ok() )
        {
            CGContextRef cg = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
            CGImageRef image = (CGImageRef)( blit.CGImageCreate() ) ;
            HIRect r = CGRectMake( xxdest , yydest , wwdest , hhdest ) ;
            HIViewDrawCGImage( cg , &r , image ) ;
            CGImageRelease( image ) ;
        }
           
    }
    else
    {
    /*
        CGContextRef cg = (wxMacCGContext*)(source->GetGraphicContext())->GetNativeContext() ;
        void *data = CGBitmapContextGetData( cg ) ;
    */
        return false ; // wxFAIL_MSG( wxT("Blitting is only supported from bitmap contexts") ) ;
    }
    return true;
}

void  wxDC::DoDrawRotatedText(const wxString& str, wxCoord x, wxCoord y,
                              double angle)
{
    wxCHECK_RET( Ok(), wxT("wxDC::DoDrawRotatedText  Invalid window dc") );

    if ( str.Length() == 0 )
        return ;
    
    if ( m_logicalFunction != wxCOPY )
        return ;

    wxCHECK_RET( m_macATSUIStyle != NULL , wxT("No valid font set") ) ;
    
    OSStatus status = noErr ;
    ATSUTextLayout atsuLayout ;
    UniCharCount chars = str.Length() ;
    UniChar* ubuf = NULL ;
#if SIZEOF_WCHAR_T == 4
	wxMBConvUTF16 converter ;
#if wxUSE_UNICODE
	size_t unicharlen = converter.WC2MB( NULL , str.wc_str() , 0 ) ;
	ubuf = (UniChar*) malloc( unicharlen + 2 ) ;
	converter.WC2MB( (char*) ubuf , str.wc_str(), unicharlen + 2 ) ;
#else
    const wxWCharBuffer wchar = str.wc_str( wxConvLocal ) ;
	size_t unicharlen = converter.WC2MB( NULL , wchar.data()  , 0 ) ;
	ubuf = (UniChar*) malloc( unicharlen + 2 ) ;
	converter.WC2MB( (char*) ubuf , wchar.data() , unicharlen + 2 ) ;
#endif
    chars = unicharlen / 2 ;
#else
#if wxUSE_UNICODE
    ubuf = (UniChar*) str.wc_str() ;
#else
    wxWCharBuffer wchar = str.wc_str( wxConvLocal ) ;
    chars = wxWcslen( wchar.data() ) ;
    ubuf = (UniChar*) wchar.data() ;
#endif
#endif

    int drawX = XLOG2DEVMAC(x) ;
    int drawY = YLOG2DEVMAC(y) ;

    status = ::ATSUCreateTextLayoutWithTextPtr( (UniCharArrayPtr) ubuf , 0 , chars , chars , 1 ,
        &chars , (ATSUStyle*) &m_macATSUIStyle , &atsuLayout ) ;

    wxASSERT_MSG( status == noErr , wxT("couldn't create the layout of the rotated text") );
    
    status = ::ATSUSetTransientFontMatching( atsuLayout , true ) ;
    wxASSERT_MSG( status == noErr , wxT("couldn't setup transient font matching") );
    
    int iAngle = int( angle );
    if ( abs(iAngle) > 0 )
    {
        Fixed atsuAngle = IntToFixed( iAngle ) ;
        ATSUAttributeTag atsuTags[] =
        {
            kATSULineRotationTag ,
        } ;
        ByteCount atsuSizes[sizeof(atsuTags)/sizeof(ATSUAttributeTag)] =
        {
            sizeof( Fixed ) ,
        } ;
        ATSUAttributeValuePtr    atsuValues[sizeof(atsuTags)/sizeof(ATSUAttributeTag)] =
        {
            &atsuAngle ,
        } ;
        status = ::ATSUSetLayoutControls(atsuLayout , sizeof(atsuTags)/sizeof(ATSUAttributeTag),
            atsuTags, atsuSizes, atsuValues ) ;
    }
    {
        CGContextRef cgContext = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
        ATSUAttributeTag atsuTags[] =
        {
            kATSUCGContextTag ,
        } ;
        ByteCount atsuSizes[sizeof(atsuTags)/sizeof(ATSUAttributeTag)] =
        {
            sizeof( CGContextRef ) ,
        } ;
        ATSUAttributeValuePtr    atsuValues[sizeof(atsuTags)/sizeof(ATSUAttributeTag)] =
        {
            &cgContext ,
        } ;
        status = ::ATSUSetLayoutControls(atsuLayout , sizeof(atsuTags)/sizeof(ATSUAttributeTag),
            atsuTags, atsuSizes, atsuValues ) ;
    }

    ATSUTextMeasurement textBefore ;
    ATSUTextMeasurement textAfter ;
    ATSUTextMeasurement ascent ;
    ATSUTextMeasurement descent ;

    status = ::ATSUGetUnjustifiedBounds( atsuLayout, kATSUFromTextBeginning, kATSUToTextEnd,
        &textBefore , &textAfter, &ascent , &descent );
    wxASSERT_MSG( status == noErr , wxT("couldn't measure the rotated text") );
    
    Rect rect ;
    
    if ( m_backgroundMode == wxSOLID )
    {
        wxGraphicPath* path = m_graphicContext->CreatePath() ;
        path->MoveToPoint( 
            drawX , 
            drawY ) ;
        path->AddLineToPoint( 
            (int) (drawX + sin(angle/RAD2DEG) * FixedToInt(ascent + descent)) , 
            (int) (drawY + cos(angle/RAD2DEG) * FixedToInt(ascent + descent)) ) ;
        path->AddLineToPoint( 
            (int) (drawX + sin(angle/RAD2DEG) * FixedToInt(ascent + descent ) + cos(angle/RAD2DEG) * FixedToInt(textAfter)) ,
            (int) (drawY + cos(angle/RAD2DEG) * FixedToInt(ascent + descent) - sin(angle/RAD2DEG) * FixedToInt(textAfter)) ) ;
        path->AddLineToPoint( 
            (int) (drawX + cos(angle/RAD2DEG) * FixedToInt(textAfter)) , 
            (int) (drawY - sin(angle/RAD2DEG) * FixedToInt(textAfter)) ) ;
            
        m_graphicContext->FillPath( path , m_textBackgroundColour ) ;
        delete path ;
    }

    drawX += (int)(sin(angle/RAD2DEG) * FixedToInt(ascent));
    drawY += (int)(cos(angle/RAD2DEG) * FixedToInt(ascent));

    status = ::ATSUMeasureTextImage( atsuLayout, kATSUFromTextBeginning, kATSUToTextEnd,
        IntToFixed(drawX) , IntToFixed(drawY) , &rect );
    wxASSERT_MSG( status == noErr , wxT("couldn't measure the rotated text") );

    CGContextSaveGState(((wxMacCGContext*)(m_graphicContext))->GetNativeContext());    
    CGContextTranslateCTM(((wxMacCGContext*)(m_graphicContext))->GetNativeContext(), drawX, drawY);
    CGContextScaleCTM(((wxMacCGContext*)(m_graphicContext))->GetNativeContext(), 1, -1);
    status = ::ATSUDrawText( atsuLayout, kATSUFromTextBeginning, kATSUToTextEnd,
        IntToFixed(0) , IntToFixed(0) );
    wxASSERT_MSG( status == noErr , wxT("couldn't draw the rotated text") );
    CGContextRestoreGState( ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ) ;

    CalcBoundingBox(XDEV2LOG(rect.left), YDEV2LOG(rect.top) );
    CalcBoundingBox(XDEV2LOG(rect.right), YDEV2LOG(rect.bottom) );

    ::ATSUDisposeTextLayout(atsuLayout);
#if SIZEOF_WCHAR_T == 4
    free( ubuf ) ;
#endif
}

void  wxDC::DoDrawText(const wxString& strtext, wxCoord x, wxCoord y)
{
    wxCHECK_RET(Ok(), wxT("wxDC::DoDrawText  Invalid DC"));
    DoDrawRotatedText( strtext , x , y , 0.0 ) ;
}

bool  wxDC::CanGetTextExtent() const
{
    wxCHECK_MSG(Ok(), false, wxT("Invalid DC"));
    return true ;
}

void  wxDC::DoGetTextExtent( const wxString &str, wxCoord *width, wxCoord *height,
                            wxCoord *descent, wxCoord *externalLeading ,
                            wxFont *theFont ) const
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));
    wxFont formerFont = m_font ;
    if ( theFont )
    {
        // work around the constness
        *((wxFont*)(&m_font)) = *theFont ;
        MacInstallFont() ;   
    }

    if ( str.Length() == 0 )
        return ;
    
    wxCHECK_RET( m_macATSUIStyle != NULL , wxT("No valid font set") ) ;
    
    OSStatus status = noErr ;
    ATSUTextLayout atsuLayout ;
    UniCharCount chars = str.Length() ;
    UniChar* ubuf = NULL ;
#if SIZEOF_WCHAR_T == 4
	wxMBConvUTF16 converter ;
#if wxUSE_UNICODE
	size_t unicharlen = converter.WC2MB( NULL , str.wc_str() , 0 ) ;
	ubuf = (UniChar*) malloc( unicharlen + 2 ) ;
	converter.WC2MB( (char*) ubuf , str.wc_str(), unicharlen + 2 ) ;
#else
    const wxWCharBuffer wchar = str.wc_str( wxConvLocal ) ;
	size_t unicharlen = converter.WC2MB( NULL , wchar.data()  , 0 ) ;
	ubuf = (UniChar*) malloc( unicharlen + 2 ) ;
	converter.WC2MB( (char*) ubuf , wchar.data() , unicharlen + 2 ) ;
#endif
    chars = unicharlen / 2 ;
#else
#if wxUSE_UNICODE
    ubuf = (UniChar*) str.wc_str() ;
#else
    wxWCharBuffer wchar = str.wc_str( wxConvLocal ) ;
    chars = wxWcslen( wchar.data() ) ;
    ubuf = (UniChar*) wchar.data() ;
#endif
#endif


    status = ::ATSUCreateTextLayoutWithTextPtr( (UniCharArrayPtr) ubuf , 0 , chars , chars , 1 ,
        &chars , (ATSUStyle*) &m_macATSUIStyle , &atsuLayout ) ;

    wxASSERT_MSG( status == noErr , wxT("couldn't create the layout of the text") );
    
    ATSUTextMeasurement textBefore ;
    ATSUTextMeasurement textAfter ;
    ATSUTextMeasurement textAscent ;
    ATSUTextMeasurement textDescent ;

    status = ::ATSUGetUnjustifiedBounds( atsuLayout, kATSUFromTextBeginning, kATSUToTextEnd,
        &textBefore , &textAfter, &textAscent , &textDescent );
        
    if ( height )
        *height = YDEV2LOGREL( FixedToInt(textAscent + textDescent) ) ;
    if ( descent )
        *descent =YDEV2LOGREL( FixedToInt(textDescent) );
    if ( externalLeading )
        *externalLeading = 0 ;
    if ( width )
        *width = XDEV2LOGREL( FixedToInt(textAfter - textBefore) ) ;
    
    ::ATSUDisposeTextLayout(atsuLayout);
#if SIZEOF_WCHAR_T == 4
    free( ubuf ) ;
#endif
    if ( theFont )
    {
        // work around the constness
        *((wxFont*)(&m_font)) = formerFont ;
        MacInstallFont() ;
    }
}


bool wxDC::DoGetPartialTextExtents(const wxString& text, wxArrayInt& widths) const
{
    wxCHECK_MSG(Ok(), false, wxT("Invalid DC"));

    widths.Empty();
    widths.Add(0, text.Length());

    if (text.Length() == 0)
        return false;
    
    ATSUTextLayout atsuLayout ;
    UniCharCount chars = text.Length() ;
    UniChar* ubuf = NULL ;
#if SIZEOF_WCHAR_T == 4
    wxMBConvUTF16 converter ;
#if wxUSE_UNICODE
    size_t unicharlen = converter.WC2MB( NULL , text.wc_str() , 0 ) ;
    ubuf = (UniChar*) malloc( unicharlen + 2 ) ;
    converter.WC2MB( (char*) ubuf , text.wc_str(), unicharlen + 2 ) ;
#else
    const wxWCharBuffer wchar = text.wc_str( wxConvLocal ) ;
    size_t unicharlen = converter.WC2MB( NULL , wchar.data()  , 0 ) ;
    ubuf = (UniChar*) malloc( unicharlen + 2 ) ;
    converter.WC2MB( (char*) ubuf , wchar.data() , unicharlen + 2 ) ;
#endif
    chars = unicharlen / 2 ;
#else
#if wxUSE_UNICODE
    ubuf = (UniChar*) text.wc_str() ;
#else
    wxWCharBuffer wchar = text.wc_str( wxConvLocal ) ;
    chars = wxWcslen( wchar.data() ) ;
    ubuf = (UniChar*) wchar.data() ;
#endif
#endif

	OSStatus status;
    status = ::ATSUCreateTextLayoutWithTextPtr( (UniCharArrayPtr) ubuf , 0 , chars , chars , 1 ,
        &chars , (ATSUStyle*) &m_macATSUIStyle , &atsuLayout ) ;
        
    	for ( int pos = 0; pos < (int)chars; pos ++ ) {
			unsigned long actualNumberOfBounds = 0;
			ATSTrapezoid glyphBounds;

			// We get a single bound, since the text should only require one. If it requires more, there is an issue
			OSStatus result; 
			result = ATSUGetGlyphBounds( atsuLayout, 0, 0, kATSUFromTextBeginning, pos + 1, kATSUseDeviceOrigins, 1, &glyphBounds, &actualNumberOfBounds );
			if (result != noErr || actualNumberOfBounds != 1 )
			{
				return false;
			}

			widths[pos] = XDEV2LOGREL(FixedToInt( glyphBounds.upperRight.x - glyphBounds.upperLeft.x ));
			//unsigned char uch = s[i];
			
    	}
    ::ATSUDisposeTextLayout(atsuLayout);
    return true;
}

wxCoord   wxDC::GetCharWidth(void) const
{
    wxCoord width ;
    DoGetTextExtent(wxT("g") , &width , NULL , NULL , NULL , NULL ) ;
    return width ;
}

wxCoord   wxDC::GetCharHeight(void) const
{
    wxCoord height ;
    DoGetTextExtent(wxT("g") , NULL , &height , NULL , NULL , NULL ) ;
    return height ;
}

void  wxDC::Clear(void)
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));

    if ( m_backgroundBrush.Ok() && m_backgroundBrush.GetStyle() != wxTRANSPARENT)
    {      
        HIRect rect = CGRectMake( -10000 , -10000 , 20000 , 20000 ) ;
        CGContextRef cg = ((wxMacCGContext*)(m_graphicContext))->GetNativeContext() ;
        switch( m_backgroundBrush.MacGetBrushKind() )
        {
            case kwxMacBrushTheme :
                {
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
                    if ( HIThemeSetFill != 0 )
                    {
                        HIThemeSetFill( m_backgroundBrush.MacGetTheme(), NULL, cg, kHIThemeOrientationNormal );
                        CGContextFillRect(cg, rect);

                    }
                    else
#endif
                    {
        				RGBColor	color;
                        GetThemeBrushAsColor( m_backgroundBrush.MacGetTheme(), 32, true, &color );
                        CGContextSetRGBFillColor( cg, (float) color.red / 65536,
        						(float) color.green / 65536, (float) color.blue / 65536, 1 );
        				CGContextFillRect( cg, rect );
                    }

                    // reset to normal value
                    RGBColor col = MAC_WXCOLORREF( GetBrush().GetColour().GetPixel() ) ;
                    CGContextSetRGBFillColor( cg, col.red / 65536.0, col.green / 65536.0, col.blue / 65536.0, 1.0 );
                }
            break ;
            case kwxMacBrushThemeBackground :
                {
                    wxFAIL_MSG( wxT("There shouldn't be theme backgrounds under Quartz") ) ;
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3
                    if ( UMAGetSystemVersion() >= 0x1030 )
                    {
                        HIThemeBackgroundDrawInfo drawInfo ;
                        drawInfo.version = 0 ;
                        drawInfo.state = kThemeStateActive ;
                        drawInfo.kind = m_backgroundBrush.MacGetThemeBackground(NULL) ;
                        if ( drawInfo.kind == kThemeBackgroundMetal )
                            HIThemeDrawBackground( &rect , &drawInfo, cg ,
                                kHIThemeOrientationNormal) ;
                            HIThemeApplyBackground( &rect , &drawInfo, cg ,
                                kHIThemeOrientationNormal) ;
                    }
                    else
#endif
                    {
                    }
                }
            break ;
            case kwxMacBrushColour :
            {
                RGBColor col = MAC_WXCOLORREF( m_backgroundBrush.GetColour().GetPixel()) ;
                CGContextSetRGBFillColor( cg , col.red / 65536.0 , col.green / 65536.0 , col.blue / 65536.0 , 1.0 ) ;
                CGContextFillRect(cg, rect);

                // reset to normal value
                col = MAC_WXCOLORREF( GetBrush().GetColour().GetPixel() ) ;
                CGContextSetRGBFillColor( cg , col.red / 65536.0 , col.green / 65536.0 , col.blue / 65536.0 , 1.0 ) ;
            }
            break ;

            default :
                wxFAIL_MSG( wxT("unknown brush kind") ) ;
                break ;
        }
    }
}

void wxDC::MacInstallFont() const
{
    wxCHECK_RET(Ok(), wxT("Invalid DC"));

    if( m_macATSUIStyle )
    {
        ::ATSUDisposeStyle((ATSUStyle)m_macATSUIStyle);
        m_macATSUIStyle = NULL ;
    }

    if ( m_font.Ok() )
    {
        OSStatus status = noErr ;
        status = ATSUCreateAndCopyStyle( (ATSUStyle) m_font.MacGetATSUStyle() , (ATSUStyle*) &m_macATSUIStyle ) ;
        wxASSERT_MSG( status == noErr , wxT("couldn't set create ATSU style") ) ;

        Fixed atsuSize = IntToFixed( int(m_scaleY * m_font.MacGetFontSize()) ) ;
        RGBColor atsuColor = MAC_WXCOLORREF( m_textForegroundColour.GetPixel() ) ;
        ATSUAttributeTag atsuTags[] =
        {
                kATSUSizeTag ,
                kATSUColorTag ,
        } ;
        ByteCount atsuSizes[sizeof(atsuTags)/sizeof(ATSUAttributeTag)] =
        {
                sizeof( Fixed ) ,
                sizeof( RGBColor ) ,
        } ;
    //    Boolean kTrue = true ;
    //    Boolean kFalse = false ;

    //    ATSUVerticalCharacterType kHorizontal = kATSUStronglyHorizontal;
        ATSUAttributeValuePtr    atsuValues[sizeof(atsuTags)/sizeof(ATSUAttributeTag)] =
        {
                &atsuSize ,
                &atsuColor ,
        } ;
        status = ::ATSUSetAttributes((ATSUStyle)m_macATSUIStyle, sizeof(atsuTags)/sizeof(ATSUAttributeTag) ,
            atsuTags, atsuSizes, atsuValues);

        wxASSERT_MSG( status == noErr , wxT("couldn't Modify ATSU style") ) ;
    }
}

// ---------------------------------------------------------------------------
// coordinates transformations
// ---------------------------------------------------------------------------

wxCoord wxDCBase::DeviceToLogicalX(wxCoord x) const
{
    return ((wxDC *)this)->XDEV2LOG(x);
}

wxCoord wxDCBase::DeviceToLogicalY(wxCoord y) const
{
    return ((wxDC *)this)->YDEV2LOG(y);
}

wxCoord wxDCBase::DeviceToLogicalXRel(wxCoord x) const
{
    return ((wxDC *)this)->XDEV2LOGREL(x);
}

wxCoord wxDCBase::DeviceToLogicalYRel(wxCoord y) const
{
    return ((wxDC *)this)->YDEV2LOGREL(y);
}

wxCoord wxDCBase::LogicalToDeviceX(wxCoord x) const
{
    return ((wxDC *)this)->XLOG2DEV(x);
}

wxCoord wxDCBase::LogicalToDeviceY(wxCoord y) const
{
    return ((wxDC *)this)->YLOG2DEV(y);
}

wxCoord wxDCBase::LogicalToDeviceXRel(wxCoord x) const
{
    return ((wxDC *)this)->XLOG2DEVREL(x);
}

wxCoord wxDCBase::LogicalToDeviceYRel(wxCoord y) const
{
    return ((wxDC *)this)->YLOG2DEVREL(y);
}

#endif // wxMAC_USE_CORE_GRAPHICS

