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
// $Id: CheetahCheat.cxx,v 1.1 2005/11/11 21:44:18 stephena Exp $
//============================================================================

#include "CheetahCheat.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CheetahCheat::CheetahCheat(OSystem* os, const string& name, const string& code)
  : Cheat(os, name, code)
{
  address = 0xf000 + unhex(code.substr(0, 3));
  value = unhex(code.substr(3, 2));
  count = unhex(code.substr(5, 1)) + 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CheetahCheat::~CheetahCheat()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CheetahCheat::enable()
{
  evaluate();
  return myEnabled;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CheetahCheat::disable()
{
  for(int i=0; i<count; i++)
    myOSystem->console().cartridge().patch(address + i, savedRom[i]);

  return myEnabled = false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CheetahCheat::evaluate()
{
  if(!myEnabled)
  {
    for(int i=0; i<count; i++)
    {
      savedRom[i] = myOSystem->console().cartridge().peek(address + i);
      myOSystem->console().cartridge().patch(address + i, value);
    }

    myEnabled = true;
  }
}
