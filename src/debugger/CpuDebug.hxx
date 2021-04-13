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
// $Id: CpuDebug.hxx,v 1.8 2005/07/21 19:29:59 stephena Exp $
//============================================================================

#ifndef CPU_DEBUG_HXX
#define CPU_DEBUG_HXX

class EquateList;

#include "Array.hxx"
#include "System.hxx"
#include "DebuggerSystem.hxx"

// pointer types for CpuDebug instance methods
typedef int (CpuDebug::*CPUDEBUG_INT_METHOD)();

// call the pointed-to method on the (global) CPU debugger object.
#define CALL_CPUDEBUG_METHOD(method) ( ( Debugger::debugger().cpuDebug().*method)() )

class CpuState : public DebuggerState
{
  public:
    int PC, SP, PS, A, X, Y;
    BoolArray PSbits;
};

class CpuDebug : public DebuggerSystem
{
  public:
    CpuDebug(Debugger* dbg, Console* console);

    DebuggerState& getState();
    DebuggerState& getOldState() { return myOldState; }

    void saveOldState();

	 // I know, we ain't supposed to do this...
    M6502 &m6502() { return mySystem->m6502(); }

    int disassemble(int address, char* buffer, EquateList* equateList);
    int dPeek(int address);
    int getBank();

    int pc() { return mySystem->m6502().PC; }
    int sp() { return mySystem->m6502().SP; }
    int a() { return mySystem->m6502().A; }
    int x() { return mySystem->m6502().X; }
    int y() { return mySystem->m6502().Y; }

	 // these return int, not boolean!
    int n() { return mySystem->m6502().N; }
    int v() { return mySystem->m6502().V; }
    int b() { return mySystem->m6502().B; }
    int d() { return mySystem->m6502().D; }
    int i() { return mySystem->m6502().I; }
    int z() { return !mySystem->m6502().notZ; }
    int c() { return mySystem->m6502().C; }

    void setPC(int pc);
    void setSP(int sp);
    void setPS(int ps);
    void setA(int a);
    void setX(int x);
    void setY(int y);

    void setN(bool on);
    void setV(bool on);
    void setB(bool on);
    void setD(bool on);
    void setI(bool on);
    void setZ(bool on);
    void setC(bool on);

    void toggleN();
    void toggleV();
    void toggleB();
    void toggleD();
    void toggleI();
    void toggleZ();
    void toggleC();

  private:
    static unsigned char set_bit(unsigned char input, int bit, bool on)
    {
      if(on)
        return input | (1 << bit);
      else
        return input & (~(1 << bit));
    }

    static int dpeek(System* system, int address)
    {
      return system->peek(address) | (system->peek(address + 1) << 8);
    }

  private:
    CpuState myState;
    CpuState myOldState;

    System* mySystem;
};

#endif
