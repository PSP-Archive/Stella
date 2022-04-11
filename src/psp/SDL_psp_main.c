/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/*  
    PSP port contributed by:
    Marcus R. Brown <mrbrown@ocgnet.org>
    Jim Paris <jim@jtan.com>
    Matthew H <matthewh@webone.com.au>
*/

#include "SDL_main.h"
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <pspthreadman.h>
#include <stdlib.h>
#include <stdio.h>

#define STDERR_FILE	"ms0:/stderr.txt"
#define STDOUT_FILE	"ms0:/stdout.txt"




/* If application's main() is redefined as SDL_main, and libSDLmain is
   linked, then this file will create the standard exit callback,
   define the PSP_* macros, add an exception handler, nullify device 
   checks and exit back to the browser when the program is finished. */

extern int SDL_main(int argc, char *argv[]);

PSP_MODULE_INFO("SDL App", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int sdl_psp_exit_callback(int arg1, int arg2, void *common)
{
	sceKernelExitGame();
	return 0;
}

int sdl_psp_callback_thread(SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", 
				       sdl_psp_exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int sdl_psp_setup_callbacks(void)
{
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", 
				     sdl_psp_callback_thread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);
	return thid;
}

#ifndef NO_STDIO_REDIRECT
int sdl_psp_stderr_handler(const char *data, int len)
{
	FILE *stderr_fp;

	stderr_fp = fopen(STDERR_FILE, "a");
	fwrite (data, len, 1, stderr_fp);
	fclose(stderr_fp);
	return 0;
}

int sdl_psp_stdout_handler(const char *data, int len)
{
	FILE *stdout_fp;

	stdout_fp = fopen(STDOUT_FILE, "a");
	fwrite (data, len, 1, stdout_fp);
	fclose(stdout_fp);
	return 0;
}
#endif

void sdl_psp_exception_handler(PspDebugRegBlock *regs)
{
	pspDebugScreenInit();

	pspDebugScreenSetBackColor(0x00FF0000);
	pspDebugScreenSetTextColor(0xFFFFFFFF);
	pspDebugScreenClear();

	pspDebugScreenPrintf("I regret to inform you your psp has just crashed\n\n");
	pspDebugScreenPrintf("Exception Details:\n");
	pspDebugDumpException(regs);
	pspDebugScreenPrintf("\nThe offending routine may be identified with:\n\n"
		"\tpsp-addr2line -e target.elf -f -C 0x%x 0x%x 0x%x\n",
		regs->epc, regs->badvaddr, regs->r[31]);
}

__attribute__ ((constructor))
void loaderInit()
{
	pspKernelSetKernelPC();
	pspSdkInstallNoDeviceCheckPatch();
	pspDebugInstallErrorHandler(sdl_psp_exception_handler);
#ifndef NO_STDIO_REDIRECT
	pspDebugInstallStdoutHandler(sdl_psp_stdout_handler);
	pspDebugInstallStderrHandler(sdl_psp_stderr_handler);
#endif
}

#undef main 

int main(int argc, char *argv[]) {
	pspDebugScreenInit();
	sdl_psp_setup_callbacks();

	(void)SDL_main(argc, argv);

	sceKernelDelayThread(2500000);
	sceKernelExitGame();
	return 0;
}
