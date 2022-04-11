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
// $Id: ColorWidget.cxx,v 1.2 2005/09/06 22:25:40 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#include "OSystem.hxx"
#include "FrameBuffer.hxx"
#include "Command.hxx"
#include "GuiObject.hxx"
#include "bspf.hxx"
#include "GuiUtils.hxx"
#include "ColorWidget.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ColorWidget::ColorWidget(GuiObject* boss, int x, int y, int w, int h, int cmd)
  : Widget(boss, x, y, w, h),
    CommandSender(boss),
    _color(0),
    _cmd(cmd)
{
  _flags = WIDGET_ENABLED | WIDGET_CLEARBG | WIDGET_RETAIN_FOCUS;
  _type = kColorWidget;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ColorWidget::~ColorWidget()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ColorWidget::setColor(int color)
{
  _color = color;
  setDirty(); draw();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ColorWidget::handleMouseDown(int x, int y, int button, int clickCount)
{
// TODO - add ColorDialog, which will show all 256 colors in the
//         TIA palette
//  if(isEnabled())
//    parent()->addDialog(myColorDialog);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ColorWidget::drawWidget(bool hilite)
{
  FrameBuffer& fb = instance()->frameBuffer();

  // Draw a thin frame around us.
  fb.hLine(_x, _y, _x + _w - 1, kColor);
  fb.hLine(_x, _y +_h, _x + _w - 1, kShadowColor);
  fb.vLine(_x, _y, _y+_h, kColor);
  fb.vLine(_x + _w - 1, _y, _y +_h - 1, kShadowColor);

  // Show the currently selected color
  fb.fillRect(_x+1, _y+1, _w-2, _h-1, (OverlayColor) _color);
}
