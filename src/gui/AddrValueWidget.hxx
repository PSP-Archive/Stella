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
// $Id: AddrValueWidget.hxx,v 1.7 2005/08/11 19:12:39 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef ADDR_VALUE_WIDGET_HXX
#define ADDR_VALUE_WIDGET_HXX

#include "GuiObject.hxx"
#include "Widget.hxx"
#include "Command.hxx"
#include "Debugger.hxx"
#include "StringList.hxx"
#include "EditableWidget.hxx"
#include "Array.hxx"
#include "Rect.hxx"

class ScrollBarWidget;

typedef GUI::Array<int> AddrList;
typedef GUI::Array<int> ValueList;

enum {
  kAVItemDoubleClickedCmd = 'AVdb',
  kAVItemActivatedCmd     = 'AVac',
  kAVItemDataChangedCmd   = 'AVch',
  kAVSelectionChangedCmd  = 'AVsc'
};

/* AddrValueWidget */
class AddrValueWidget : public EditableWidget
{
  public:
    AddrValueWidget(GuiObject* boss, int x, int y, int w, int h,
                    int range, BaseFormat base = kBASE_DEFAULT);
    virtual ~AddrValueWidget();

    void setList(const StringList& list);
    void setList(const AddrList& alist, const ValueList& vlist);

    int getSelectedAddr() const   { return _addrList[_selectedItem]; }
    int getSelectedValue() const  { return _valueList[_selectedItem]; }

    void scrollTo(int item);
	
    virtual void handleMouseDown(int x, int y, int button, int clickCount);
    virtual void handleMouseUp(int x, int y, int button, int clickCount);
    virtual void handleMouseWheel(int x, int y, int direction);
    virtual bool handleKeyDown(int ascii, int keycode, int modifiers);
    virtual bool handleKeyUp(int ascii, int keycode, int modifiers);
    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id);

    virtual bool wantsFocus() { return true; }

    void startEditMode();
    void endEditMode();

  protected:
    void drawWidget(bool hilite);

    int findItem(int x, int y) const;
    void scrollBarRecalc();

    void abortEditMode();

    GUI::Rect getEditRect() const;

    void lostFocusWidget();
    void scrollToCurrent();

    bool tryInsertChar(char c, int pos);

  protected:
    AddrList         _addrList;
    ValueList        _valueList;
    StringList       _addrStringList;
    StringList       _valueStringList;

    int              _range;
    BaseFormat       _base;

    bool             _editMode;
    int              _currentPos;
    int              _entriesPerPage;
    int              _selectedItem;
    ScrollBarWidget* _scrollBar;
    int              _currentKeyDown;
    string           _backupString;

    string           _quickSelectStr;
    int              _quickSelectTime;
};

#endif
