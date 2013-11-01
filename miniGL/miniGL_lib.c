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

#include "miniGL.h"
#include "miniGL_main.h"
#include "miniGL_lib.h"

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
