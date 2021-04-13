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
// $Id: LauncherOptionsDialog.hxx,v 1.7 2005/08/01 22:33:15 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef LAUNCHER_OPTIONS_DIALOG_HXX
#define LAUNCHER_OPTIONS_DIALOG_HXX

class OSystem;
class GuiObject;
class DialogContainer;
class BrowserDialog;
class CheckboxWidget;
class PopUpWidget;
class StaticTextWidget;
class TabWidget;

#include "Dialog.hxx"
#include "Command.hxx"

class LauncherOptionsDialog : public Dialog, public CommandSender
{
  public:
    LauncherOptionsDialog(OSystem* osystem, DialogContainer* parent,
                          GuiObject* boss,
                          int x, int y, int w, int h);
    ~LauncherOptionsDialog();

    virtual void loadConfig();
    virtual void saveConfig();

    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id);

  protected:
    BrowserDialog* myBrowser;
    TabWidget* myTab;

    // Rom path controls
    StaticTextWidget* myRomPath;

    // Snapshot controls
    StaticTextWidget* mySnapPath;
    PopUpWidget*      mySnapTypePopup;
    CheckboxWidget*   mySnapSingleCheckbox;

  private:
    void openRomBrowser();
    void openSnapBrowser();
};

#endif
