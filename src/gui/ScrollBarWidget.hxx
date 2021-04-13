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
// $Id: ScrollBarWidget.hxx,v 1.5 2005/08/01 22:33:16 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef SCROLL_BAR_WIDGET_HXX
#define SCROLL_BAR_WIDGET_HXX

#include "GuiObject.hxx"
#include "Widget.hxx"
#include "Command.hxx"
#include "bspf.hxx"


class ScrollBarWidget : public Widget, public CommandSender
{
  protected:
    typedef enum {
      kNoPart,
      kUpArrowPart,
      kDownArrowPart,
      kSliderPart,
      kPageUpPart,
      kPageDownPart
    } Part;

  public:
    ScrollBarWidget(GuiObject* boss, int x, int y, int w, int h);

    virtual void handleMouseDown(int x, int y, int button, int clickCount);
    virtual void handleMouseUp(int x, int y, int button, int clickCount);
    virtual void handleMouseWheel(int x, int y, int direction);
    virtual void handleMouseMoved(int x, int y, int button);
    virtual void handleMouseEntered(int button);
    virtual void handleMouseLeft(int button);

    // FIXME - this should be private, but then we also have to add accessors
    // for _numEntries, _entriesPerPage and _currentPos. This again leads to the question:
    // should these accessors force a redraw?
    void recalc();

  public:
    int _numEntries;
    int _entriesPerPage;
    int _currentPos;

  protected:
    void drawWidget(bool hilite);
    void checkBounds(int old_pos);

  protected:
    Part  _part;
    int _sliderHeight;
    int _sliderPos;
    Part  _draggingPart;
    int _sliderDeltaMouseDownPos;
};

#endif
