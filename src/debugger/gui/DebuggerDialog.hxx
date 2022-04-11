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
// $Id: DebuggerDialog.hxx,v 1.4 2005/09/20 19:09:10 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef DEBUGGER_DIALOG_HXX
#define DEBUGGER_DIALOG_HXX

class Debugger;
class OSystem;
class DialogContainer;
class CpuWidget;
class RamWidget;
class RomWidget;
class TabWidget;
class EditTextWidget;
class TiaInfoWidget;
class TiaOutputWidget;
class TiaZoomWidget;

#include "Dialog.hxx"
#include "PromptWidget.hxx"

class DebuggerDialog : public Dialog
{
  public:
    DebuggerDialog(OSystem* osystem, DialogContainer* parent,
                          int x, int y, int w, int h);
    ~DebuggerDialog();

    PromptWidget* prompt()       { return myPrompt;     }
    TiaInfoWidget* tiaInfo()     { return myTiaInfo;    }
    TiaOutputWidget* tiaOutput() { return myTiaOutput;  }
    TiaZoomWidget* tiaZoom()     { return myTiaZoom;    }
    RomWidget* rom()             { return myRom;        }
    EditTextWidget* message()    { return myMessageBox; }

    virtual void loadConfig();
    virtual void handleKeyDown(int ascii, int keycode, int modifiers);
    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id);

  private:
    TabWidget* myTab;

    PromptWidget*    myPrompt;
    TiaInfoWidget*   myTiaInfo;
    TiaOutputWidget* myTiaOutput;
    TiaZoomWidget*   myTiaZoom;
    CpuWidget*       myCpu;
    RamWidget*       myRam;
    RomWidget*       myRom;
    EditTextWidget*  myMessageBox;

  private:
    void addTiaArea();
    void addTabArea();
    void addStatusArea();
    void addRomArea();

    void doStep();
    void doTrace();
    void doScanlineAdvance();
    void doAdvance();
    void doExit();
};

#endif
