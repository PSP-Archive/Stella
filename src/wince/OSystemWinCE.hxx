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
// Windows CE Port by Kostas Nakos
//============================================================================

#ifndef OSYSTEM_WINCE_HXX
#define OSYSTEM_WINCE_HXX

#include "bspf.hxx"
#include "OSystem.hxx"

class OSystemWinCE : public OSystem
{
  public:
    OSystemWinCE();
    virtual ~OSystemWinCE();

  public:
    virtual void mainLoop();
	virtual uInt32 getTicks(void);
    virtual void setFramerate(uInt32 framerate);
	inline const GUI::Font& font() const;
};

#endif
