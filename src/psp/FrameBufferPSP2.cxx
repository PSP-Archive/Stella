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
// $Id: FrameBufferPSP2.cxx,v 1.1 2005/09/18 14:35:59 optixx Exp $
//============================================================================




#include <SDL.h>
#include <SDL_syswm.h>
#include <sstream>

#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <sys/time.h>
#include <time.h>
#include <pspgu.h>
#include <psputils.h>


#include "Console.hxx"
#include "FrameBufferPSP2.hxx"
#include "MediaSrc.hxx"
#include "Settings.hxx"
#include "OSystem.hxx"
#include "Font.hxx"
#include "GuiUtils.hxx"


#define PSP_SLICE_SIZE  (32)
#define PSP_SLICE_SIZE2 (16)
#define PSP_LINE_SIZE (512)
#define SCREEN_WIDTH (480)
#define SCREEN_HEIGHT (272)
#define BUFFER_SIZE             (512 * PSP_LINE_SIZE * 4)
#define FB_WIDTH        256
#define FB_HEIGHT       512

static unsigned int __attribute__((aligned(16))) _list[1024];
static unsigned int *list = (unsigned int *) ((uInt32) _list | 0x40000000);
static uInt32 __attribute__((aligned(16))) palette[256];
static uInt8 __attribute__((aligned(16))) _myFrameBuffer[FB_WIDTH*FB_HEIGHT];

struct Vertex
{
    unsigned short u, v;
    unsigned short color;
    short x, y, z;
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FrameBufferPSP2::FrameBufferPSP2(OSystem* osystem)
  : FrameBufferSoft(osystem),
    myFrameBuffer((uInt8 *) ((uInt32) _myFrameBuffer | 0x40000000))

{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FrameBufferPSP2::~FrameBufferPSP2()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FrameBufferPSP2::initSubsystem()
{

#ifdef PSP_DEBUG
  fprintf(stdout, "FrameBufferPSP2::initSubsystem\n");
#endif

  // Create the screen
  if(!createScreen())
    return false;

  // Show some info
  if(myOSystem->settings().getBool("showinfo"))
    cout << "Video rendering: Software mode" << endl << endl;

  // Precompute the GUI palette
  // We abuse the concept of 'enum' by referring directly to the integer values
  for(uInt8 i = 0; i < kNumColors-256; i++)
    myPalette[i+256] = mapRGB(ourGUIColors[i][0], ourGUIColors[i][1], ourGUIColors[i][2]);

  return true;
}
static uInt32 * myScreenPixels;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FrameBufferPSP2::createScreen()
{
  myScreenDim.x = myScreenDim.y = 0;

  myScreenDim.w = myBaseDim.w;
  myScreenDim.h = myBaseDim.h;

  // In software mode, the image and screen dimensions are always the same
  myImageDim = myScreenDim;

  myScreen = SDL_SetVideoMode(myScreenDim.w, myScreenDim.h, 0, mySDLFlags);
  if(myScreen == NULL)
  {
    fprintf(stdout,"ERROR: Unable to open SDL window: %s\n",SDL_GetError());
    return false;
  }

  sceGuInit();
  sceGuStart(GU_DIRECT, list);
  sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, (void*)0, PSP_LINE_SIZE);
  sceGuDrawBuffer(GU_PSM_8888, (void *) BUFFER_SIZE, PSP_LINE_SIZE);
  sceGuDepthBuffer((void *) (BUFFER_SIZE * 2), PSP_LINE_SIZE);
  sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
  sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
  sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
  sceGuDepthRange(50000, 10000);
  sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  sceGuEnable(GU_SCISSOR_TEST);
  sceGuFrontFace(GU_CW);

  sceGuFinish();
  sceGuSync(0, 0);

  sceDisplayWaitVblankStart();
  sceGuDisplay(1);

  myScreenPixels = (uInt32 *) ((uInt32) myScreen->pixels | 0x40000000);
  memset(myFrameBuffer, 0, FB_WIDTH * FB_HEIGHT);
  memset(myScreenPixels, 0, 512 * 512 * 4);

  myOSystem->eventHandler().refreshDisplay();

  return true;
}

void FrameBufferPSP2::drawMediaSource()
{

    MediaSource& mediasrc = myOSystem->console().mediaSource();
    uInt8* currentFrame   = mediasrc.currentFrameBuffer();
    uInt32 height                         = mediasrc.height();
    uInt32 width                          = mediasrc.width();

    register uInt32 y;
    register uInt8 *buffer = myFrameBuffer;

    for(y = 0; y < height; ++y )
    {
        memcpy(buffer, currentFrame, width);
        buffer                   += FB_WIDTH;
        currentFrame += width;
    }
}

void FrameBufferPSP2::preFrameUpdate()
{
}

// void FrameBufferPSP2::clearOverLay()
// {
//     memset(myScreen->pixels, 0, BUFFER_SIZE);
// }

bool hasOverLay = false, redrawScreen = true;

/**
 * Update the screen using Gu textures and sprites.
 * Useful for scaling dimensions when using SWSURFACE.
 */
extern "C" int sceDisplayWaitVblankCB(void);

static void PSP_GuUpdate(uInt8 *frameBuffer, uInt32 w, uInt32 h, SDL_Surface *screen)
{
    unsigned int slice;
    struct Vertex *vertices;

    sceKernelDcacheWritebackAll();

    sceGuStart(GU_DIRECT,list);

    sceGuClearColor(0xff000000);
    sceGuClear(GU_COLOR_BUFFER_BIT);

    sceGuEnable(GU_TEXTURE_2D);
    sceGuClutMode(GU_PSM_8888, 0, 255, 0);
    sceGuClutLoad(32, palette);
    sceGuTexMode(GU_PSM_T8,0,0,0);
    sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
    sceGuTexFilter(GU_LINEAR, GU_LINEAR);
    sceGuTexImage(0, FB_WIDTH, FB_HEIGHT, FB_WIDTH, frameBuffer);
    sceGuTexSync();

    for (slice = 0; slice < (SCREEN_WIDTH / PSP_SLICE_SIZE); slice++) {
        vertices = (struct Vertex*)sceGuGetMemory(2 * sizeof(struct Vertex));

        vertices[0].x = slice * PSP_SLICE_SIZE;
        vertices[1].x = vertices[0].x + PSP_SLICE_SIZE;

        vertices[0].u = slice * PSP_SLICE_SIZE * w / SCREEN_WIDTH;
        vertices[1].u = (slice + 1) * PSP_SLICE_SIZE * w / SCREEN_WIDTH;

        vertices[0].v = 0;
        vertices[0].color = 0;
        vertices[0].y = 0;
        vertices[0].z = 0;

        vertices[1].v = h;
        vertices[1].color = 0;
        vertices[1].y = SCREEN_HEIGHT;
        vertices[1].z = 0;

        sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_COLOR_4444|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
    }

    if ( hasOverLay ) {
        sceGuDisable(GU_TEXTURE_2D);

                        // draw overlay...
        sceGuEnable(GU_TEXTURE_2D);
        sceGuEnable(GU_BLEND);
        sceGuTexMode(GU_PSM_8888,0,0,0);
        sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
        sceGuTexImage(0, 512, 512, 512, myScreenPixels);
        sceGuTexSync();

        sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

        for (slice = 0; slice < (SCREEN_WIDTH / PSP_SLICE_SIZE2); slice++) {

            vertices = (struct Vertex*)sceGuGetMemory(2 * sizeof(struct Vertex));

            vertices[0].x = slice * PSP_SLICE_SIZE2;
            vertices[1].x = vertices[0].x + PSP_SLICE_SIZE2;

            vertices[0].u = slice * PSP_SLICE_SIZE2;
            vertices[1].u = (slice + 1) * PSP_SLICE_SIZE2;

            vertices[0].v = 0;
            vertices[0].color = 0;
            vertices[0].y = 0;
            vertices[0].z = 0;

            vertices[1].v = SCREEN_HEIGHT;
            vertices[1].color = 0;
            vertices[1].y = SCREEN_HEIGHT;
            vertices[1].z = 0;

            sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_COLOR_4444|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
        }

        sceGuDisable(GU_BLEND);
        sceGuDisable(GU_TEXTURE_2D);
    }

    sceGuFinish();
    sceGuSync(0,0);

        //sceDisplayWaitVblankCB();

    sceGuSwapBuffers();
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBufferPSP2::postFrameUpdate()
{
    MediaSource& mediasrc = myOSystem->console().mediaSource();
    uInt32 height                         = mediasrc.height();
    uInt32 width                          = mediasrc.width();
    PSP_GuUpdate(myFrameBuffer, width, height, myScreen);

}

