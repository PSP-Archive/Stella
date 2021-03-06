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
// Copyright (c) 1995-2005 by Bradford W. Mott
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: TIASnd.hxx,v 1.3 2005/10/09 17:31:47 stephena Exp $
//============================================================================

#ifndef TIASOUND_HXX
#define TIASOUND_HXX

#include "bspf.hxx"

/**
  This class implements a fairly accurate emulation of the TIA sound
  hardware.

  @author  Bradford W. Mott
  @version $Id: TIASnd.hxx,v 1.3 2005/10/09 17:31:47 stephena Exp $
*/
class TIASound
{
  public:
    static const int TIASoundFrequency;

  public:
    /**
      Create a new TIA Sound object using the specified output frequency
    */
    TIASound(Int32 outputFrequency = TIASoundFrequency, uInt32 channels = 1);

    /**
      Destructor
    */
    virtual ~TIASound();

  public:
    /**
      Reset the sound emulation to its power-on state
    */
    void reset();

    /**
      Set the frequency output samples should be generated at
    */
    void outputFrequency(uInt32 freq);

    /**
      Selects the number of audio channels per sample (1 = mono, 2 = stereo)
    */
    void channels(uInt32 number);

  public:
    /**
      Sets the specified sound register to the given value

      @param address Register address
      @param value Value to store in the register
    */
    void set(uInt16 address, uInt8 value);

    /**
      Gets the specified sound register's value

      @param address Register address
    */
    uInt8 get(uInt16 address);

    /**
      Create sound samples based on the current sound register settings
      in the specified buffer. NOTE: If channels is set to stereo then
      the buffer will need to be twice as long as the number of samples.

      @param buffer The location to store generated samples
      @param samples The number of samples to generate
    */
    void process(uInt8* buffer, uInt32 samples);

    /**
      Set the volume of the samples created (0-100)
    */
    void volume(uInt32 percent);

  private:
    /**
      Frequency divider class which outputs 1 after "divide-by" clocks. This
      is used to divide the main frequency by the values 1 to 32.
    */
    class FreqDiv
    {
      public:
        FreqDiv()
        {
          myDivideByValue = myCounter = 0;
        }

        void set(uInt32 divideBy)
        {
          myDivideByValue = divideBy;
        }

        bool clock()
        {
          if(++myCounter > myDivideByValue)
          {
            myCounter = 0;
            return true;
          }
          return false;
        }

      private:
        uInt32 myDivideByValue;
        uInt32 myCounter;
    };

  private:
    uInt8 myAUDC[2];
    uInt8 myAUDF[2];
    uInt8 myAUDV[2];

    FreqDiv myFreqDiv[2];    // Frequency dividers
    uInt8 myP4[2];           // 4-bit register LFSR (lower 4 bits used)
    uInt8 myP5[2];           // 5-bit register LFSR (lower 5 bits used)

    Int32  myOutputFrequency;
    uInt32 myChannels;
    Int32  myOutputCounter;
    uInt32 myVolumePercentage;
};

#endif
