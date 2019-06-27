///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/scrtimer.h
// Purpose:     wxScrollTimer: small helper class for wxScrollArrow/Thumb
// Author:      Vadim Zeitlin
// Modified by:
// Created:     18.02.01
// RCS-ID:      $Id: scrtimer.h,v 1.6.4.1 2005/09/25 20:46:44 MW Exp $
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_SCRTIMER_H
#define _WX_UNIV_SCRTIMER_H

// NB: this class is implemented in scrolbar.cpp

#include "wx/timer.h"

// ----------------------------------------------------------------------------
// wxScrollTimer: the timer used when the arrow or scrollbar shaft is kept
// pressed
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxScrollTimer : public wxTimer
{
public:
    // default ctor
    wxScrollTimer();

    // start generating the events
    void StartAutoScroll();

    // the base class method
    virtual void Notify();

protected:
    // to implement in derived classes: perform the scroll action and return
    // true to continue scrolling or false to stop
    virtual bool DoNotify() = 0;

    // should we skip the next timer event?
    bool m_skipNext;
};

#endif // _WX_UNIV_SCRTIMER_H
