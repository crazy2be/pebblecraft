/**
 * miniGL_lib.c
 *
 * miniGL for the Palm Computing platform
 * Michael Sherman <msherman@dsbox.com>
 * (c) 2000 Digital Sandbox, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <SysAll.h>
#include "miniGL.h"
#include "miniGL_main.h"
#include "miniGL_lib.h"

/**
 * This is the entry point into the library code.
 */
ULong start(UInt refNum, SysLibTblEntryPtr entryP) {
	Err ret;

	asm("
	movel %%fp@(10),%%sp@-
	movew %%fp@(8),%%sp@-
	jsr install_dispatcher(%%pc)
	movel %%d0,%0
	jmp out(%%pc)
gettable:
	lea jmptable(%%pc),%%a0
	rts
jmptable:
	dc.w 260
        dc.w 88
        dc.w 92
        dc.w 96
        dc.w 100
        dc.w 104
        dc.w 108
        dc.w 112
        dc.w 116
        dc.w 120
        dc.w 124
        dc.w 128
        dc.w 132
        dc.w 136
        dc.w 140
        dc.w 144
        dc.w 148
        dc.w 152
        dc.w 156
        dc.w 160
        dc.w 164
        dc.w 168
        dc.w 172
        dc.w 176
        dc.w 180
        dc.w 184
        dc.w 188
        dc.w 192
        dc.w 196
        dc.w 200
        dc.w 204
        dc.w 208
        dc.w 212
        dc.w 216
        dc.w 220
        dc.w 224
        dc.w 228
        dc.w 232
        dc.w 236
        dc.w 240
        dc.w 244
        dc.w 248
        dc.w 252
        dc.w 256
        jmp minigllib_open(%%pc)
        jmp minigllib_close(%%pc)
        jmp minigllib_sleep(%%pc)
        jmp minigllib_wake(%%pc)
        jmp minigllib_glmultmatrixf(%%pc)
        jmp minigllib_glloadmatrixf(%%pc)
        jmp minigllib_glloadidentity(%%pc)
        jmp minigllib_glclearcolor(%%pc)
        jmp minigllib_glclear(%%pc)
        jmp minigllib_glcolor3f(%%pc)
        jmp minigllib_glcolor4f(%%pc)
        jmp minigllib_glortho(%%pc)
        jmp minigllib_gluortho2d(%%pc)
        jmp minigllib_glviewport(%%pc)
        jmp minigllib_gluperspective(%%pc)
        jmp minigllib_glbegin(%%pc)
        jmp minigllib_glend(%%pc)
        jmp minigllib_glflush(%%pc)
        jmp minigllib_glvertex2f(%%pc)
        jmp minigllib_glvertex3f(%%pc)
        jmp minigllib_glshademodel(%%pc)
        jmp minigllib_glrotatef(%%pc)
        jmp minigllib_glscalef(%%pc)
        jmp minigllib_gltranslatef(%%pc)
        jmp minigllib_glpopmatrix(%%pc)
        jmp minigllib_glpushmatrix(%%pc)
        jmp minigllib_glmatrixmode(%%pc)
        jmp minigllib_glnormal3f(%%pc)
        jmp minigllib_glenable(%%pc)
        jmp minigllib_gldisable(%%pc)
        jmp minigllib_gllightfv(%%pc)
        jmp minigllib_glgetlightfv(%%pc)
        jmp minigllib_glpolygonmode(%%pc)
        jmp minigllib_glmap2f(%%pc)
        jmp minigllib_glmapgrid2f(%%pc)
        jmp minigllib_glevalmesh2(%%pc)
        jmp minigllib_glpushattrib(%%pc)
        jmp minigllib_glpopattrib(%%pc)
        jmp minigllib_gldrawpixels(%%pc)
        jmp minigllib_glbitmap(%%pc)
        jmp minigllib_winsetglarea(%%pc)
        jmp minigllib_winremoveglarea(%%pc)
        jmp minigllib_glutswapbuffers(%%pc)
	.asciz \"miniGLLib\"
.even
out:
	" : "=r" (ret) :);
        return ret;
}

/**
 * Sets the dispatch table pointer to the above assembly block.
 */
ULong install_dispatcher(UInt refNum, SysLibTblEntryPtr entryP) {
	Ptr *table = gettable();
	entryP->dispatchTblP = table;
	entryP->globalsP = 0;

	return 0;
}

/**
 * The routine to open the shared library.  If some other program
 * already has it open, it increments the use counter.  Else it
 * allocates and initializes all global variables.
 */
Err minigllib_open(UInt refNum) {
	miniGLLib_globalsPtr gP = NULL;
	Err err;
	int i;

	ErrFatalDisplayIf(sysInvalidRefNum == refNum, "Invalid refNum in open.");

	gP = miniGLLockGlobals(refNum);

	if (!gP) {
		gP = miniGLAllocGlobals(refNum);
		if (!gP)
			return miniGLErrNoGlobals;

		/** Initialize globals here */
		gP->refcount = 1;
		gP->modv_level = 0;
		gP->proj_level = 0;
		for (i=0;i<16;i++) {
			gP->cur_matrix[i] = 0.0;
			gP->per_matrix[i] = 0.0;
			gP->scr_matrix[i] = 0.0;
		}
		gP->num_vertices = 0;
		gP->two_created = 0;
		gP->lighting = 1;
		gP->wireframe = 1;

	} else {
		gP->refcount++;
	}

	err = miniGLUnlockGlobals(gP);
	ErrFatalDisplayIf(err,"Unable to unlock lib globals in open.");

	return miniGLErrNone;
}

/**
 * Function to close the library.  If there are still more programs
 * referencing the library, it merely decrements the use counter.
 * Else if closes the library and de-allocates the global vars.
 */
Err minigllib_close(UInt refNum, UIntPtr numappsP) {
	Err err;
	miniGLLib_globalsPtr gP = NULL;

	ErrFatalDisplayIf(sysInvalidRefNum == refNum, "Invalid refNum in open.");

	if (!numappsP)
		return miniGLErrParam;

	gP = miniGLLockGlobals(refNum);
	if (!gP)
		return miniGLErrNoGlobals;

	gP->refcount--;
	ErrNonFatalDisplayIf(gP->refcount<0, "Lib globals underlock.");

	*numappsP = gP->refcount;

	miniGLUnlockGlobals(gP);

	if (*numappsP <= 0) {
		err = miniGLFreeGlobals(refNum);
		ErrFatalDisplayIf(err, "Unable to free lib globals.");
	}

	return miniGLErrNone;
}

/**
 * Function called when the Palm device is powered off.
 */
Err minigllib_sleep(UInt refNum) {
	return miniGLErrNone;
}

/**
 * Function called when the Palm device is turned on.
 */
Err minigllib_wake(UInt refNum) {
	return miniGLErrNone;
}

/**
 * Allocates the memory for the global variable struct.
 */
miniGLLib_globalsPtr miniGLAllocGlobals(UInt refNum) {
	miniGLLib_globalsPtr gP = NULL;
	SysLibTblEntryPtr sysLibEntryP;
	VoidHand gH = NULL;

	ErrFatalDisplayIf(sysInvalidRefNum == refNum, "Invalid refNum.");

	sysLibEntryP = SysLibTblEntry(refNum);
	ErrFatalDisplayIf(NULL == sysLibEntryP, "Invalid refNum.");
	ErrFatalDisplayIf(sysLibEntryP->globalsP, "Lib globals ptr already exists.");

	gH = MemHandleNew(sizeof(miniGLLib_globals));
	if (!gH) 
		return(NULL);

	sysLibEntryP->globalsP = (void*)gH;
	gP = (miniGLLib_globalsPtr) miniGLLockGlobals(refNum);
	ErrFatalDisplayIf(!gP, "Unable to lock lib globals ptr.");

	MemPtrSetOwner(gP, 0);

	MemSet(gP, sizeof(miniGLLib_globals), 0);

	return (gP);
}

/**
 * Grabs and locks the reference to the global struct pointer.
 */
miniGLLib_globalsPtr miniGLLockGlobals(UInt refNum) {
	miniGLLib_globalsPtr gP = NULL;
	SysLibTblEntryPtr sysLibEntryP = NULL;
	VoidHand gH = NULL;
	
	ErrFatalDisplayIf(sysInvalidRefNum == refNum, "Invalid refNum.");

	sysLibEntryP = SysLibTblEntry(refNum);
	ErrFatalDisplayIf(NULL == sysLibEntryP, "Invalid refNum.");

	gH = (VoidHand) (sysLibEntryP->globalsP);
	if (gH) 
		gP = (miniGLLib_globalsPtr)MemHandleLock(gH);

	return gP;
}

/**
 * Frees the memory block used for the global struct.
 */
Err miniGLFreeGlobals(UInt refNum) {
	SysLibTblEntryPtr sysLibEntryP;
	VoidHand gH = NULL;

	ErrFatalDisplayIf(sysInvalidRefNum == refNum, "Invalid refNum.");

	sysLibEntryP= SysLibTblEntry(refNum);
	ErrFatalDisplayIf(NULL == sysLibEntryP, "Invalid refNum.");

	gH = (VoidHand) (sysLibEntryP->globalsP);
	ErrFatalDisplayIf(!gH, "Lib globals ptr does not exist.");

	sysLibEntryP->globalsP = NULL;
	return (MemHandleFree(gH));
}

/**
 * Unlocks the hold on the global struct pointer.
 */
Err miniGLUnlockGlobals(miniGLLib_globalsPtr gP) {
	return(MemPtrUnlock(gP));
}

/**
 * End of miniGL_lib.c 
 */
