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
// $Id: CheatCodeDialog.hxx,v 1.1 2005/08/05 02:28:22 urchlay Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef CHEAT_CODE_DIALOG_HXX
#define CHEAT_CODE_DIALOG_HXX

class DialogContainer;
class CommandSender;
class ButtonWidget;
class StaticTextWidget;

#include "OSystem.hxx"
#include "Dialog.hxx"
#include "Widget.hxx"
#include "Cheat.hxx"
#include "Props.hxx"
#include "bspf.hxx"

class CheatCodeDialog : public Dialog
{
  public:
    CheatCodeDialog(OSystem* osystem, DialogContainer* parent,
                   int x, int y, int w, int h);
    ~CheatCodeDialog();


  protected:
    ButtonWidget *myExitButton;
    StaticTextWidget *myTitle;
	 CheckboxWidget *myEnableCheat;

  private:
    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id);
	 void loadConfig();
	 Cheat *myCheat;
};

#endif
