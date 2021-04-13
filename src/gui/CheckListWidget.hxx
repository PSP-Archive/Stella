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
// $Id: CheckListWidget.hxx,v 1.6 2005/08/26 16:44:17 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef CHECK_LIST_WIDGET_HXX
#define CHECK_LIST_WIDGET_HXX

class CheckboxWidget;

#include "ListWidget.hxx"

// Some special commands
enum {
  kListItemChecked = 'LIct'  // checkbox toggled on current line
};

enum CheckStyle {
  kXFill,
  kSolidFill
};

typedef GUI::Array<CheckboxWidget*> CheckboxArray;


/** CheckListWidget */
class CheckListWidget : public ListWidget
{
  public:
    CheckListWidget(GuiObject* boss, const GUI::Font& font,
                    int x, int y, int w, int h);
    virtual ~CheckListWidget();

    void setStyle(CheckStyle style);
    void setList(const StringList& list, const BoolArray& state);
    void setLine(int line, const string& str, const bool& state);

    bool getState(int line);

    void handleCommand(CommandSender* sender, int cmd, int data, int id);

  protected:
    void drawWidget(bool hilite);
    GUI::Rect getEditRect() const;

  protected:
    BoolArray     _stateList;
    CheckboxArray _checkList;
};

#endif
