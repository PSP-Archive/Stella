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
// $Id: FrameBuffer.hxx,v 1.59 2005/10/18 18:49:46 stephena Exp $
//============================================================================

#ifndef FRAMEBUFFER_HXX
#define FRAMEBUFFER_HXX

#include <SDL.h>
#include <SDL_syswm.h>

#include "bspf.hxx"
#include "Event.hxx"
#include "MediaSrc.hxx"
#include "Font.hxx"
#include "GuiUtils.hxx"

class OSystem;
class Console;

// Text alignment modes for drawString()
enum TextAlignment {
  kTextAlignLeft,
  kTextAlignCenter,
  kTextAlignRight
};

// Line types for drawing rectangular frames
enum FrameStyle {
  kSolidLine,
  kDashLine
};

/**
  This class encapsulates the MediaSource and is the basis for the video
  display in Stella.  All graphics ports should derive from this class for
  platform-specific video stuff.

  All GUI elements (ala ScummVM) are drawn here as well.

  @author  Stephen Anthony
  @version $Id: FrameBuffer.hxx,v 1.59 2005/10/18 18:49:46 stephena Exp $
*/
class FrameBuffer
{
  public:
    /**
      Creates a new Frame Buffer
    */
    FrameBuffer(OSystem* osystem);

    /**
      Destructor
    */
    virtual ~FrameBuffer();

    /**
      (Re)initializes the framebuffer display.  This must be called before any
      calls are made to derived methods.

      @param title   The title of the window
      @param width   The width of the framebuffer
      @param height  The height of the framebuffer
      @param aspect  Whether to use the aspect ratio setting
    */
    void initialize(const string& title, uInt32 width, uInt32 height,
                    bool aspect = true);

    /**
      Updates the display, which depending on the current mode could mean
      drawing the mediasource, any pending menus, etc.
    */
    void update();

    /**
      Shows a message onscreen.

      @param message  The message to be shown
    */
    void showMessage(const string& message);

    /**
      Hides any onscreen messages.
    */
    void hideMessage();

    /**
      Returns the current width of the framebuffer *before* any scaling.

      @return  The current unscaled width
    */
    inline const uInt32 baseWidth()  { return myBaseDim.w; }

    /**
      Returns the current height of the framebuffer *before* any scaling.

      @return  The current unscaled height
    */
    inline const uInt32 baseHeight() { return myBaseDim.h; }

    /**
      Returns the current width of the framebuffer image.
      Note that this will take into account the current scaling (if any).

      @return  The current width
    */
    inline const uInt32 imageWidth()  { return myImageDim.w; }

    /**
      Returns the current height of the framebuffer image.
      Note that this will take into account the current scaling (if any).

      @return  The current height
    */
    inline const uInt32 imageHeight() { return myImageDim.h; }

     /**
      Sets the pause status.  While pause is selected, the
      MediaSource will not be updated.

      @param status  Toggle pause based on status
    */
    void pause(bool status);

    /**
      Indicates that the TIA area is dirty, and certain areas need
      to be redrawn.

      @param now  Determine if the refresh should be done right away or in
                  the next frame
    */
    void refresh(bool now = false);

    /**
      Toggles between fullscreen and window mode.
      Grabmouse activated when in fullscreen mode.  
    */
    void toggleFullscreen();

    /**
      Enables/disables fullscreen mode.
      Grabmouse activated when in fullscreen mode.  

      @param enable  Set the fullscreen mode to this value
    */
    void setFullscreen(bool enable);

    /**
      This method is called when the user wants to resize the window.
      Size = 'PreviousSize' means window should decrease in size
      Size = 'NextSize' means window should increase in size
      Size = 'GivenSize' means window should be resized to quantity given in 'zoom'

      @param size  Described above
      @param zoom  The zoom level to use if size is set to 'sGiven'
    */
    void resize(Size size, Int8 zoom = 0);

    /**
      Sets the state of the cursor (hidden or grabbed) based on the
      current mode.
    */
    void setCursorState();

    /**
      Shows or hides the cursor based on the given boolean value.
    */
    void showCursor(bool show);

    /**
      Grabs or ungrabs the mouse based on the given boolean value.
    */
    void grabMouse(bool grab);

    /**
      Answers if the display is currently in fullscreen mode.
    */
    bool fullScreen();

    /**
      Calculate the maximum window size that the current screen can hold.
      Only works in X11/Win32 for now, otherwise always return 4.
    */
    uInt32 maxWindowSizeForScreen();

    /**
      Returns current zoomlevel of the framebuffer.
    */
    uInt32 zoomLevel() { return theZoomLevel; }

    /**
      Set the title for the main SDL window.
    */
    void setWindowTitle(const string& title);

    /**
      Set up the palette for a screen of any depth > 8.

      @param palette  The array of colors
    */
    virtual void setPalette(const uInt32* palette);

    /**
      This method should be called to draw a rectangular box with sides
      at the specified coordinates.

      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the box
      @param h      The height of the box
      @param colorA Lighter color for outside line.
      @param colorB Darker color for inside line.
    */
    void box(uInt32 x, uInt32 y, uInt32 w, uInt32 h,
             OverlayColor colorA, OverlayColor colorB);

    /**
      This method should be called to draw a framed rectangle.
      I'm not exactly sure what it is, so I can't explain it :)

      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the area
      @param h      The height of the area
      @param color  The color of the surrounding frame
    */
    void frameRect(uInt32 x, uInt32 y, uInt32 w, uInt32 h,
                   OverlayColor color, FrameStyle style = kSolidLine);

    /**
      This method should be called to draw the specified string.

      @param font   The font to draw the string with
      @param str    The string to draw
      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the string area
      @param h      The height of the string area
      @param color  The color of the text
      @param align  The alignment of the text in the string width area
      @param deltax 
      @param useEllipsis  Whether to use '...' when the string is too long
    */
    void drawString(const GUI::Font* font, const string& str, int x, int y, int w,
                    OverlayColor color, TextAlignment align = kTextAlignLeft,
                    int deltax = 0, bool useEllipsis = true);


  public:
    //////////////////////////////////////////////////////////////////////
    // The following methods are system-specific and must be implemented
    // in derived classes.
    //////////////////////////////////////////////////////////////////////
    /**
      This method is called to initialize the subsystem-specific video mode.
    */
    virtual bool initSubsystem() = 0;

    /**
      This method is called to set the aspect ratio of the screen.
    */
    virtual void setAspectRatio() = 0;

    /**
      This method is called whenever the screen needs to be recreated.
      It updates the global screen variable.
    */
    virtual bool createScreen() = 0;

    /**
      Switches between the filtering options in the video subsystem.
    */
    virtual void toggleFilter() = 0;

    /**
      This method should be called anytime the MediaSource needs to be redrawn
      to the screen.
    */
    virtual void drawMediaSource() = 0;

    /**
      This method is called before any drawing is done (per-frame).
    */
    virtual void preFrameUpdate() = 0;

    /**
      This method is called after any drawing is done (per-frame).
    */
    virtual void postFrameUpdate() = 0;

    /**
      This method is called to get the specified scanline data.

      @param row  The row we are looking for
      @param data The actual pixel data (in bytes)
    */
    virtual void scanline(uInt32 row, uInt8* data) = 0;

    /**
      This method is called to map a given r,g,b triple to the screen palette.

      @param r  The red component of the color.
      @param g  The green component of the color.
      @param b  The blue component of the color.
    */
    virtual Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b) = 0;

    /**
      This method should be called to draw a horizontal line.

      @param x     The first x coordinate
      @param y     The y coordinate
      @param x2    The second x coordinate
      @param color The color of the line
    */
    virtual void hLine(uInt32 x, uInt32 y, uInt32 x2, OverlayColor color) = 0;

    /**
      This method should be called to draw a vertical line.

      @param x     The x coordinate
      @param y     The first y coordinate
      @param y2    The second y coordinate
      @param color The color of the line
    */
    virtual void vLine(uInt32 x, uInt32 y, uInt32 y2, OverlayColor color) = 0;

    /**
      This method should be called to draw a filled rectangle.

      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the area
      @param h      The height of the area
      @param color  The color of the area
    */
    virtual void fillRect(uInt32 x, uInt32 y, uInt32 w, uInt32 h,
                          OverlayColor color) = 0;

    /**
      This method should be called to draw the specified character.

      @param font   The font to use to draw the character
      @param c      The character to draw
      @param x      The x coordinate
      @param y      The y coordinate
      @param color  The color of the character
    */
    virtual void drawChar(const GUI::Font* font, uInt8 c, uInt32 x, uInt32 y,
                          OverlayColor color) = 0;

    /**
      This method should be called to draw the bitmap image.

      @param bitmap The data to draw
      @param x      The x coordinate
      @param y      The y coordinate
      @param color  The color of the character
      @param h      The height of the data image
    */
    virtual void drawBitmap(uInt32* bitmap, Int32 x, Int32 y, OverlayColor color,
                            Int32 h = 8) = 0;

    /**
      This method should be called to translate the given coordinates
      to their unzoomed/unscaled equivalents.

      @param x  X coordinate to translate
      @param y  Y coordinate to translate
    */
    virtual void translateCoords(Int32* x, Int32* y) = 0;

    /**
      This method should be called to add a dirty rectangle
      (ie, an area of the screen that has changed)

      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the area
      @param h      The height of the area
    */
    virtual void addDirtyRect(uInt32 x, uInt32 y, uInt32 w, uInt32 h) = 0;

  protected:
    // The parent system for the framebuffer
    OSystem* myOSystem;

    // Dimensions of the base image, before zooming.
    // All external GUI items should refer to these dimensions,
    //  since this is the *real* size of the image.
    // The other sizes are simply scaled versions of these dimensions.
    SDL_Rect myBaseDim;

    // Dimensions of the actual image, after zooming
    SDL_Rect myImageDim;

    // Dimensions of the SDL window (not always the same as the image)
    SDL_Rect myScreenDim;

    // Dimensions of the desktop area
    SDL_Rect myDesktopDim;

    // Indicates if the TIA area should be redrawn
    bool theRedrawTIAIndicator;

    // The SDL video buffer
    SDL_Surface* myScreen;

    // SDL initialization flags
    uInt32 mySDLFlags;

    // SDL palette
    Uint32 myPalette[kNumColors];

    // Indicates the current zoom level of the SDL screen
    uInt32 theZoomLevel;

    // Indicates the maximum zoom of the SDL screen
    uInt32 theMaxZoomLevel;

    // The aspect ratio of the window
    float theAspectRatio;

    // Use dirty updates (SDL_UpdateRects instead of SDL_UpdateRect)
    bool myUseDirtyRects;

    // Table of RGB values for GUI elements
    static const uInt8 ourGUIColors[kNumColors-256][3];

  private:
    /**
      Set the icon for the main SDL window.
    */
    void setWindowIcon();

    /**
      Set the icon for the main SDL window.
    */
    void drawMessage();

  private:
    // Indicates the current framerate of the system
    uInt32 myFrameRate;

    // Indicates the current pause status
    bool myPauseStatus;

    // Message timer
    Int32 myMessageTime;

    // Message text
    string myMessageText;

    // Indicates how many times the framebuffer has been redrawn
    // Used only for debugging purposes
    uInt32 myNumRedraws;
};

#endif
