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
// $Id: GameList.hxx,v 1.7 2005/12/09 01:16:13 stephena Exp $
//
//   Based on code from KStella - Stella frontend
//   Copyright (C) 2003-2005 Stephen Anthony
//============================================================================

#ifndef GAME_LIST_HXX
#define GAME_LIST_HXX

#include "Array.hxx"
#include "bspf.hxx"

/**
  Holds the list of game info for the ROM launcher.
*/
class GameList
{
  private:
    struct Entry {
      string _rom;
      string _name;
      string _note;
    };

    typedef Common::Array<Entry> EntryList;
    EntryList myArray;

  public:
    GameList();
    ~GameList();

    inline const string& rom(int i)  { return myArray[i]._rom; }
    inline const string& name(int i) { return myArray[i]._name; }
    inline const string& note(int i) { return myArray[i]._note; }
    inline int size() { return myArray.size(); }

    void clear() { myArray.clear(); }
    void appendGame(const string& rom, const string& name, const string& note);
    void sortByName();
};

#endif
