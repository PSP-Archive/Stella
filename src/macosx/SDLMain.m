/*   SDLMain.m - main entry point for our Cocoa-ized SDL app
       Initial Version: Darrell Walisser <dwaliss1@purdue.edu>
       Non-NIB-Code & other changes: Max Horn <max@quendi.de>

    Feel free to customize this file to suit your needs
*/
/* $Id: SDLMain.m,v 1.5 2005/08/24 05:30:00 markgrebe Exp $ */

#import "SDL.h"
#import "SDLMain.h"
#import "Preferences.h"
#import <sys/param.h> /* for MAXPATHLEN */
#import <unistd.h>

extern int stellaMain(int argc, char* argv[]);
void macOpenConsole(char *romname);

static int    gArgc;
static char  **gArgv;
BOOL   gFinderLaunch;
static BOOL   started=NO;
static char   startupFile[FILENAME_MAX];
int fileToLoad = FALSE;
char parentdir[MAXPATHLEN];

unsigned short macOSXDisplayWidth(void)
{
	return(CGDisplayPixelsWide(kCGDirectMainDisplay));
}

unsigned short macOSXDisplayHeight(void)
{
	return(CGDisplayPixelsHigh(kCGDirectMainDisplay));
}


/* A helper category for NSString */
@interface NSString (ReplaceSubString)
- (NSString *)stringByReplacingRange:(NSRange)aRange with:(NSString *)aString;
@end

@interface SDLApplication : NSApplication
@end

@implementation SDLApplication
/* Invoked from the Quit menu item */
- (void)terminate:(id)sender
{
    /* Post a SDL_QUIT event */
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}
@end


/* The main class of the application, the application's delegate */
@implementation SDLMain

static SDLMain *sharedInstance = nil;

+ (SDLMain *)sharedInstance {
    return sharedInstance;
}

/* Fix menu to contain the real app name instead of "SDL App" */
- (void)fixMenu:(NSMenu *)aMenu withAppName:(NSString *)appName
{
    NSRange aRange;
    NSEnumerator *enumerator;
    NSMenuItem *menuItem;

    aRange = [[aMenu title] rangeOfString:@"SDL App"];
    if (aRange.length != 0)
        [aMenu setTitle: [[aMenu title] stringByReplacingRange:aRange with:appName]];

    enumerator = [[aMenu itemArray] objectEnumerator];
    while ((menuItem = [enumerator nextObject]))
    {
        aRange = [[menuItem title] rangeOfString:@"SDL App"];
        if (aRange.length != 0)
            [menuItem setTitle: [[menuItem title] stringByReplacingRange:aRange with:appName]];
        if ([menuItem hasSubmenu])
            [self fixMenu:[menuItem submenu] withAppName:appName];
    }
    [ aMenu sizeToFit ];
}

char *appName = "StellaOSX";
char fileName[FILENAME_MAX];

/* Called when the internal event loop has just started running */
- (void) applicationDidFinishLaunching: (NSNotification *) note
{
    int status;
	char *args[2];
    
	started = YES;
    
    /* Set the main menu to contain the real app name instead of "SDL App" */
    [self fixMenu:[NSApp mainMenu] withAppName:[[NSProcessInfo processInfo] processName]];

    /* Hand off to main application code */
	args[0] = appName;
	if (fileToLoad)
		{
	    args[1] = startupFile;
		status = stellaMain(2,args);
		}
	else
		{
		status = stellaMain(1,args);
		}

    /* We're done, thank you for playing */
    exit(status);
}

/*------------------------------------------------------------------------------
*  application openFile - Open a file dragged to the application.
*-----------------------------------------------------------------------------*/
- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename
{
	char *cFilename;

    if (started) {
		cFilename = malloc(FILENAME_MAX);
		if (cFilename)
		  {
		  [filename getCString:cFilename];
		  macOpenConsole(cFilename);
		  }
	  }
    else {
        fileToLoad = TRUE;
        [filename getCString:startupFile];
      }

    return(FALSE);
}

@end


@implementation NSString (ReplaceSubString)

- (NSString *)stringByReplacingRange:(NSRange)aRange with:(NSString *)aString
{
    unsigned int bufferSize;
    unsigned int selfLen = [self length];
    unsigned int aStringLen = [aString length];
    unichar *buffer;
    NSRange localRange;
    NSString *result;

    bufferSize = selfLen + aStringLen - aRange.length;
    buffer = NSAllocateMemoryPages(bufferSize*sizeof(unichar));
    
    /* Get first part into buffer */
    localRange.location = 0;
    localRange.length = aRange.location;
    [self getCharacters:buffer range:localRange];
    
    /* Get middle part into buffer */
    localRange.location = 0;
    localRange.length = aStringLen;
    [aString getCharacters:(buffer+aRange.location) range:localRange];
     
    /* Get last part into buffer */
    localRange.location = aRange.location + aRange.length;
    localRange.length = selfLen - localRange.location;
    [self getCharacters:(buffer+aRange.location+aStringLen) range:localRange];
    
    /* Build output string */
    result = [NSString stringWithCharacters:buffer length:bufferSize];
    
    NSDeallocateMemoryPages(buffer, bufferSize);
    
    return result;
}

@end



#ifdef main
#  undef main
#endif

void setupParentDirectory(void)
{
    char *c;
    
    strncpy ( parentdir, gArgv[0], sizeof(parentdir) );
    c = (char*) parentdir;

    while (*c != '\0')     /* go to end */
        c++;
    
    while (*c != '/')      /* back up to parent */
        c--;
    
    *c++ = '\0';             /* cut off last part (binary name) */

}

/* Main entry point to executable - should *not* be SDL_main! */
int main (int argc, char **argv)
{
    static Preferences *myPrefs;
    /* Copy the arguments into a global variable */
    int i;
    
    /* This is passed if we are launched by double-clicking */
    if ( argc >= 2 && strncmp (argv[1], "-psn", 4) == 0 ) {
        gArgc = 1;
	gFinderLaunch = YES;
    } else {
        gArgc = argc;
	gFinderLaunch = NO;
    }
    gArgv = (char**) malloc (sizeof(*gArgv) * (gArgc+1));
    assert (gArgv != NULL);
    for (i = 0; i < gArgc; i++)
        gArgv[i] = argv[i];
    gArgv[i] = NULL;

    myPrefs = [Preferences sharedInstance];

    setupParentDirectory();

    [SDLApplication poseAsClass:[NSApplication class]];
    NSApplicationMain (argc, argv);
    return 0;
}

/* Routine to center the application window */
void centerAppWindow(void)
{
	[[[SDLApplication sharedApplication] keyWindow] center];
}
	
