//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2005 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: FrameBufferPSP2.hxx,v 1.1 2005/09/18 14:35:59 optixx Exp $
//============================================================================

#ifndef FRAMEBUFFER_PSP2_HXX
#define FRAMEBUFFER_PSP2_HXX



#include "Font.hxx"
#include "bspf.hxx"
#include "GuiUtils.hxx"
#include "FrameBufferSoft.hxx"


/**
  This class implements an SDL software framebuffer.

  @author  Stephen Anthony
  @version $Id: FrameBufferPSP2.hxx,v 1.1 2005/09/18 14:35:59 optixx Exp $
*/
class FrameBufferPSP2 : public FrameBufferSoft
{
    private:
        uInt8 * myFrameBuffer;

    public:
    /**
      Creates a new software framebuffer
    */
    FrameBufferPSP2(OSystem* osystem);

    /**
      Destructor
    */
    virtual ~FrameBufferPSP2();

    //////////////////////////////////////////////////////////////////////
    // The following methods are derived from FrameBuffer.hxx
    //////////////////////////////////////////////////////////////////////
    /**
    This method is called to initialize software video mode.
    Return false if any operation fails, otherwise return true.
     */
    virtual bool initSubsystem();

    /**
    This method is called whenever the screen needs to be recreated.
    It updates the global screen variable.
     */
    virtual bool createScreen();
    /**
    This method should be called anytime the MediaSource needs to be redrawn
    to the screen.
     */
    virtual void drawMediaSource();

    /**
    This method is called before any drawing is done (per-frame).
     */
    virtual void preFrameUpdate();

    /**
    This method is called after any drawing is done (per-frame).
     */
    virtual void postFrameUpdate();

};


#endif
