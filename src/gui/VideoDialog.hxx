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
// $Id: VideoDialog.hxx,v 1.10 2005/10/18 18:49:46 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef VIDEO_DIALOG_HXX
#define VIDEO_DIALOG_HXX

class CommandSender;
class DialogContainer;
class PopUpWidget;
class SliderWidget;
class StaticTextWidget;
class CheckboxWidget;

#include "OSystem.hxx"
#include "Dialog.hxx"
#include "bspf.hxx"

enum {
  kRendererChanged    = 'VDrd',
  kAspectRatioChanged = 'VDar',
  kFrameRateChanged   = 'VDfr',
  kZoomChanged        = 'VDzm'
};

class VideoDialog : public Dialog
{
  public:
    VideoDialog(OSystem* osystem, DialogContainer* parent,
                int x, int y, int w, int h);
    ~VideoDialog();

  protected:
    PopUpWidget*      myDirtyPopup;
    PopUpWidget*      myRendererPopup;
    PopUpWidget*      myFilterPopup;
    SliderWidget*     myAspectRatioSlider;
    StaticTextWidget* myAspectRatioLabel;
    PopUpWidget*      myPalettePopup;
    SliderWidget*     myFrameRateSlider;
    StaticTextWidget* myFrameRateLabel;
    SliderWidget*     myZoomSlider;
    StaticTextWidget* myZoomLabel;
    CheckboxWidget*   myFullscreenCheckbox;
    CheckboxWidget*   myUseDeskResCheckbox;

  private:
    void loadConfig();
    void saveConfig();
    void setDefaults();

    void handleRendererChange(int item);
    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id);
};

#endif
