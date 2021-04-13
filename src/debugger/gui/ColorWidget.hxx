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
// $Id: ColorWidget.hxx,v 1.1 2005/08/30 17:51:26 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef COLOR_WIDGET_HXX
#define COLOR_WIDGET_HXX

class ColorDialog;
class GuiObject;

#include "Widget.hxx"
#include "Command.hxx"

#include "bspf.hxx"

/**
  Displays a color from the TIA palette.  This class will eventually
  be expanded with a TIA palette table, to set the color visually.
  
  @author  Stephen Anthony
  @version $Id: ColorWidget.hxx,v 1.1 2005/08/30 17:51:26 stephena Exp $
*/
class ColorWidget : public Widget, public CommandSender
{
  friend class ColorDialog;

  public:
    ColorWidget(GuiObject* boss, int x, int y, int w, int h, int cmd = 0);
    ~ColorWidget();

    void setColor(int color);
    int  getColor() const    { return _color;  }
	
  protected:
    void drawWidget(bool hilite);
    void handleMouseDown(int x, int y, int button, int clickCount);

  protected:
    int _color;
    int	_cmd;
};

#endif
