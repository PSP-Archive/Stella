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
// $Id: Widget.cxx,v 1.34 2005/08/31 19:15:10 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#include "OSystem.hxx"
#include "FrameBuffer.hxx"
#include "Font.hxx"
#include "Dialog.hxx"
#include "Command.hxx"
#include "GuiObject.hxx"
#include "bspf.hxx"
#include "GuiUtils.hxx"
#include "Widget.hxx"

//static int COUNT = 0;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Widget::Widget(GuiObject* boss, int x, int y, int w, int h)
    : GuiObject(boss->instance(), boss->parent(), x, y, w, h),
      _type(0),
      _boss(boss),
      _id(-1),
      _flags(0),
      _hasFocus(false),
      _color(kTextColor)
{
  // Insert into the widget list of the boss
  _next = _boss->_firstWidget;
  _boss->_firstWidget = this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Widget::~Widget()
{
  delete _next;
  _next = NULL;

  _focusList.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Widget::draw()
{
  if(!_dirty)
    return;

  _dirty = false;
  
  FrameBuffer& fb = _boss->instance()->frameBuffer();

  if(!isVisible() || !_boss->isVisible())
    return;

  int oldX = _x, oldY = _y, oldW = _w, oldH = _h;

  // Account for our relative position in the dialog
  _x = getAbsX();
  _y = getAbsY();

  // Clear background (unless alpha blending is enabled)
  if(_flags & WIDGET_CLEARBG)
    fb.fillRect(_x, _y, _w, _h, kBGColor);

  // Draw border
  if(_flags & WIDGET_BORDER) {
    OverlayColor colorA = kColor;
    OverlayColor colorB = kShadowColor;
    if((_flags & WIDGET_INV_BORDER) == WIDGET_INV_BORDER)
      SWAP(colorA, colorB);
    fb.box(_x, _y, _w, _h, colorA, colorB);
    _x += 4;
    _y += 4;
    _w -= 8;
    _h -= 8;
  }

  // Now perform the actual widget draw
  drawWidget((_flags & WIDGET_HILITED) ? true : false);

  // Restore x/y
  if (_flags & WIDGET_BORDER) {
    _x -= 4;
    _y -= 4;
    _w += 8;
    _h += 8;
  }

  _x = oldX;
  _y = oldY;

  // Draw all children
  Widget* w = _firstWidget;
  while(w)
  {
    w->draw();
    w = w->_next;
  }

  // Tell the framebuffer this area is dirty
  fb.addDirtyRect(getAbsX(), getAbsY(), oldW, oldH);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Widget::receivedFocus()
{
  if(_hasFocus)
    return;

  _hasFocus = true;
  receivedFocusWidget();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Widget::lostFocus()
{
  if(!_hasFocus)
    return;

  _hasFocus = false;
  lostFocusWidget();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Widget::setEnabled(bool e)
{
  if(e)
    setFlags(WIDGET_ENABLED);
  else
    clearFlags(WIDGET_ENABLED);

  setDirty(); draw();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Widget* Widget::findWidgetInChain(Widget *w, int x, int y)
{
  while(w)
  {
    // Stop as soon as we find a widget that contains the point (x,y)
    if(x >= w->_x && x < w->_x + w->_w && y >= w->_y && y < w->_y + w->_h)
      break;
    w = w->_next;
  }

  if(w)
    w = w->findWidget(x - w->_x, y - w->_y);

  return w;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Widget::isWidgetInChain(Widget* w, Widget* find)
{
  bool found = false;

  while(w)
  {
    // Stop as soon as we find the widget
    if(w == find)
    {
      found = true;
      break;
    }
    w = w->_next;
  }

  return found;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Widget::isWidgetInChain(WidgetArray& list, Widget* find)
{
  bool found = false;

  for(int i = 0; i < (int)list.size(); ++i)
  {
    if(list[i] == find)
    {
      found = true;
      break;
    }
  }

  return found;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Widget* Widget::setFocusForChain(GuiObject* boss, WidgetArray& arr,
                                 Widget* wid, int direction)
{
  FrameBuffer& fb = boss->instance()->frameBuffer();
  int size = arr.size(), pos = -1;
  Widget* tmp;
  for(int i = 0; i < size; ++i)
  {
    tmp = arr[i];

    // Determine position of widget 'w'
    if(wid == tmp)
      pos = i;

    int x = tmp->getAbsX() - 1,  y = tmp->getAbsY() - 1,
        w = tmp->getWidth() + 2, h = tmp->getHeight() + 2;

    // First clear area surrounding all widgets
    if(tmp->_hasFocus)
    {
      tmp->lostFocus();
      if(!(tmp->_flags & WIDGET_NODRAW_FOCUS))
        fb.frameRect(x, y, w, h, kBGColor);
      tmp->setDirty(); tmp->draw();
      fb.addDirtyRect(x, y, w, h);
    }
  }

  // Figure out which which should be active
  if(pos == -1)
    return 0;
  else
  {
    switch(direction)
    {
      case -1:  // previous widget
        pos--;
        if(pos < 0)
          pos = size - 1;
        break;

      case +1:  // next widget
        pos++;
        if(pos >= size)
          pos = 0;
        break;

      default:
        // pos already set
        break;
    }
  }

  // Now highlight the active widget
  tmp = arr[pos];
  int x = tmp->getAbsX() - 1,  y = tmp->getAbsY() - 1,
      w = tmp->getWidth() + 2, h = tmp->getHeight() + 2;

  tmp->receivedFocus();
  if(!(tmp->_flags & WIDGET_NODRAW_FOCUS))
    fb.frameRect(x, y, w, h, kTextColorEm, kDashLine);

  tmp->setDirty(); tmp->draw();
  fb.addDirtyRect(x, y, w, h);

  return tmp;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Widget::setDirtyInChain(Widget* start)
{
  while(start)
  {
    start->setDirty();
    start = start->_next;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
StaticTextWidget::StaticTextWidget(GuiObject *boss, int x, int y, int w, int h,
                                   const string& text, TextAlignment align)
    : Widget(boss, x, y, w, h),
      _align(align)
{
  _flags = WIDGET_ENABLED | WIDGET_CLEARBG;
  _type = kStaticTextWidget;
  _label = text;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void StaticTextWidget::setValue(int value)
{
  char buf[256];
  sprintf(buf, "%d", value);
  _label = buf;

  setDirty(); draw();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void StaticTextWidget::setLabel(const string& label)
{
  _label = label;
  setDirty(); draw();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void StaticTextWidget::drawWidget(bool hilite)
{
  FrameBuffer& fb = _boss->instance()->frameBuffer();
  fb.drawString(_font, _label, _x, _y, _w,
                isEnabled() ? _color : kColor, _align);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ButtonWidget::ButtonWidget(GuiObject *boss, int x, int y, int w, int h,
                           const string& label, int cmd, uInt8 hotkey)
    : StaticTextWidget(boss, x, y, w, h, label, kTextAlignCenter),
      CommandSender(boss),
	  _cmd(cmd),
      _hotkey(hotkey)
{
  _flags = WIDGET_ENABLED | WIDGET_BORDER | WIDGET_CLEARBG;
  _type = kButtonWidget;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ButtonWidget::handleMouseEntered(int button)
{
  setFlags(WIDGET_HILITED);
  setDirty(); draw();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ButtonWidget::handleMouseLeft(int button)
{
  clearFlags(WIDGET_HILITED);
  setDirty(); draw();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ButtonWidget::handleMouseUp(int x, int y, int button, int clickCount)
{
  if(isEnabled() && x >= 0 && x < _w && y >= 0 && y < _h)
  {
    clearFlags(WIDGET_HILITED);
    sendCommand(_cmd, 0, _id);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ButtonWidget::drawWidget(bool hilite)
{
  FrameBuffer& fb = _boss->instance()->frameBuffer();
  fb.drawString(_font, _label, _x, _y + (_h - kLineHeight)/2 + 1, _w,
                !isEnabled() ? kColor : hilite ? kTextColorHi : _color, _align);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/* 8x8 checkbox bitmap */
static unsigned int checked_img[8] =
{
	0x00000000,
	0x01000010,
	0x00100100,
	0x00011000,
	0x00011000,
	0x00100100,
	0x01000010,
	0x00000000,
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CheckboxWidget::CheckboxWidget(GuiObject *boss, const GUI::Font& font,
                               int x, int y, const string& label,
                               int cmd)
  : ButtonWidget(boss, x, y, 16, 16, label, cmd, 0),
    _state(false),
    _editable(true),
    _holdFocus(true),
    _fillRect(false),
    _drawBox(true),
    _fillColor(kColor)
{
  _flags = WIDGET_ENABLED | WIDGET_RETAIN_FOCUS;
  _type = kCheckboxWidget;

  setFont(font);
  if(label == "")
    _w = 14;
  else
    _w = font.getStringWidth(label) + 20;
  _h = font.getFontHeight() < 14 ? 14 : font.getFontHeight();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheckboxWidget::handleMouseUp(int x, int y, int button, int clickCount)
{
  if(isEnabled() && _editable && x >= 0 && x < _w && y >= 0 && y < _h)
  {
    toggleState();

    // We only send a command when the widget has been changed interactively
    sendCommand(_cmd, _state, _id);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CheckboxWidget::handleKeyDown(int ascii, int keycode, int modifiers)
{
  bool handled = false;

  // (De)activate with space or return
  switch(keycode)
  {
    case '\n':  // enter/return
    case '\r':
    case ' ' :  // space
      // Simulate mouse event
      handleMouseUp(0, 0, 1, 0);
      handled = true;
      break;
  }

  return handled;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CheckboxWidget::wantsFocus()
{
  if(!_holdFocus)
    return false;
  else
    return _editable;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheckboxWidget::setEditable(bool editable)
{
  _holdFocus = _editable = editable;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheckboxWidget::setState(bool state)
{
  if(_state != state)
  {
    _state = state;
    _flags ^= WIDGET_INV_BORDER;
    setDirty(); draw();
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheckboxWidget::drawWidget(bool hilite)
{
  FrameBuffer& fb = _boss->instance()->frameBuffer();

  // Depending on font size, either the font or box will need to be 
  // centered vertically
  int box_yoff = 0, text_yoff = 0;
  if(_h > 14)  // center box
    box_yoff = (_h - 14) / 2;
  else         // center text
    text_yoff = (14 - _font->getFontHeight()) / 2;

  // Draw the box
  if(_drawBox)
    fb.box(_x, _y + box_yoff, 14, 14, kColor, kShadowColor);

  // If checked, draw cross inside the box
  if(_state)
  {
    if(_fillRect)
      fb.fillRect(_x + 2, _y + box_yoff + 2, 10, 10,
                  isEnabled() ? _color : kColor);
    else
      fb.drawBitmap(checked_img, _x + 3, _y + box_yoff + 3,
                    isEnabled() ? _color : kColor);
  }
  else
    fb.fillRect(_x + 2, _y + box_yoff + 2, 10, 10, kBGColor);

  // Finally draw the label
  fb.drawString(_font, _label, _x + 20, _y + text_yoff, _w,
                isEnabled() ? _color : kColor);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
SliderWidget::SliderWidget(GuiObject *boss, int x, int y, int w, int h,
                           const string& label, int labelWidth, int cmd, uInt8 hotkey)
  : ButtonWidget(boss, x, y, w, h, label, cmd, hotkey),
    _value(0),
    _oldValue(0),
    _valueMin(0),
    _valueMax(100),
    _isDragging(false),
    _labelWidth(labelWidth)
{
  _flags = WIDGET_ENABLED | WIDGET_TRACK_MOUSE | WIDGET_CLEARBG;
  _type = kSliderWidget;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SliderWidget::setValue(int value)
{
  _value = value;
  setDirty(); draw();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SliderWidget::handleMouseMoved(int x, int y, int button)
{
  // TODO: when the mouse is dragged outside the widget, the slider should
  // snap back to the old value.
  if(isEnabled() && _isDragging && x >= (int)_labelWidth)
  {
    int newValue = posToValue(x - _labelWidth);

    if(newValue < _valueMin)
      newValue = _valueMin;
    else if (newValue > _valueMax)
      newValue = _valueMax;

    if(newValue != _value)
    {
      _value = newValue; 
      setDirty(); draw();
      sendCommand(_cmd, _value, _id);
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SliderWidget::handleMouseDown(int x, int y, int button, int clickCount)
{
  if(isEnabled())
  {
    _isDragging = true;
    handleMouseMoved(x, y, button);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SliderWidget::handleMouseUp(int x, int y, int button, int clickCount)
{
  if(isEnabled() && _isDragging)
    sendCommand(_cmd, _value, _id);

  _isDragging = false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SliderWidget::drawWidget(bool hilite)
{
  FrameBuffer& fb = _boss->instance()->frameBuffer();

  // Draw the label, if any
  if(_labelWidth > 0)
    fb.drawString(_font, _label, _x, _y + 2, _labelWidth,
                  isEnabled() ? _color : kColor, kTextAlignRight);

  // Draw the box
  fb.box(_x + _labelWidth, _y, _w - _labelWidth, _h, kColor, kShadowColor);

  // Draw the 'bar'
  fb.fillRect(_x + _labelWidth + 2, _y + 2, valueToPos(_value), _h - 4,
              !isEnabled() ? kColor :
              hilite ? kTextColorHi : _color);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int SliderWidget::valueToPos(int value)
{
  return ((_w - _labelWidth - 4) * (value - _valueMin) / (_valueMax - _valueMin));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int SliderWidget::posToValue(int pos)
{
  return (pos) * (_valueMax - _valueMin) / (_w - _labelWidth - 4) + _valueMin;
}
