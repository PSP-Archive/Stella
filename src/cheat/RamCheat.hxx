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
// $Id: RamCheat.hxx,v 1.1 2005/11/11 21:44:18 stephena Exp $
//============================================================================

#ifndef RAM_CHEAT_HXX
#define RAM_CHEAT_HXX

#include "Cheat.hxx"

class RamCheat : public Cheat
{
  public:
    RamCheat(OSystem* os, const string& name, const string& code);
    ~RamCheat();

    virtual bool enable();
    virtual bool disable();

    virtual void evaluate();

  private:
    uInt16 address;
    uInt8  value;
};

#endif
