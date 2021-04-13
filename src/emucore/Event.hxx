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
// $Id: Event.hxx,v 1.13 2005/08/29 18:36:41 stephena Exp $
//============================================================================

#ifndef EVENT_HXX
#define EVENT_HXX

class Event;

#include "bspf.hxx"

/**
  @author  Bradford W. Mott
  @version $Id: Event.hxx,v 1.13 2005/08/29 18:36:41 stephena Exp $
*/
class Event
{
  public:
    /**
      Enumeration of all possible events in Stella, including both
      console and controller event types as well as events that aren't
      technically part of the core
    */
    enum Type
    {
      NoType,
      ConsoleOn, ConsoleOff, ConsoleColor, ConsoleBlackWhite,
      ConsoleLeftDifficultyA, ConsoleLeftDifficultyB,
      ConsoleRightDifficultyA, ConsoleRightDifficultyB,
      ConsoleSelect, ConsoleReset,

      JoystickZeroUp, JoystickZeroDown, JoystickZeroLeft,
      JoystickZeroRight, JoystickZeroFire,
      JoystickOneUp, JoystickOneDown, JoystickOneLeft,
      JoystickOneRight, JoystickOneFire,

      BoosterGripZeroTrigger, BoosterGripZeroBooster,
      BoosterGripOneTrigger, BoosterGripOneBooster,

      PaddleZeroResistance, PaddleZeroFire,
      PaddleOneResistance, PaddleOneFire,
      PaddleTwoResistance, PaddleTwoFire,
      PaddleThreeResistance, PaddleThreeFire,

      KeyboardZero1, KeyboardZero2, KeyboardZero3,
      KeyboardZero4, KeyboardZero5, KeyboardZero6,
      KeyboardZero7, KeyboardZero8, KeyboardZero9,
      KeyboardZeroStar, KeyboardZero0, KeyboardZeroPound,

      KeyboardOne1, KeyboardOne2, KeyboardOne3,
      KeyboardOne4, KeyboardOne5, KeyboardOne6,
      KeyboardOne7, KeyboardOne8, KeyboardOne9,
      KeyboardOneStar, KeyboardOne0, KeyboardOnePound,

      DrivingZeroClockwise, DrivingZeroCounterClockwise, DrivingZeroValue, 
	  DrivingZeroFire,
      DrivingOneClockwise, DrivingOneCounterClockwise, DrivingOneValue,
	  DrivingOneFire,
	  
      ChangeState, LoadState, SaveState, TakeSnapshot, Pause, Quit,
      MenuMode, CmdMenuMode, DebuggerMode, LauncherMode, Fry,

      LastType
    };

  public:
    /**
      Create a new event object
    */
    Event();
 
    /**
      Destructor
    */
    virtual ~Event();

  public:
    /**
      Get the value associated with the event of the specified type
    */
    virtual Int32 get(Type type) const;

    /**
      Set the value associated with the event of the specified type
    */
    virtual void set(Type type, Int32 value);

    /**
      Clears the event array (resets to initial state)
    */
    virtual void clear();

  protected:
    // Number of event types there are
    const Int32 myNumberOfTypes;

    // Array of values associated with each event type
    Int32 myValues[LastType];
};

#endif
