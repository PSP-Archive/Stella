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
// $Id: StringList.hxx,v 1.3 2005/06/16 00:56:00 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef STRING_LIST_HXX
#define STRING_LIST_HXX

#include "Array.hxx"
#include "bspf.hxx"


class StringList : public GUI::Array<string>
{
  public:
    void push_back(const char *str)
    {
      ensureCapacity(_size + 1);
      _data[_size++] = str;
    }

    void push_back(const string& str)
    {
      ensureCapacity(_size + 1);
      _data[_size++] = str;
    }
};

#endif
