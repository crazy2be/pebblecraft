/**
 * fastgraph.c
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

#include "fastgraph.h"

#define GREY_REAL               2
#define GREY_HACK               1
#define GREY_NONE               0
#define GREY_MONO               0  
#define MAX_SCREEN_WIDTH	160
#define MAX_SCREEN_HEIGHT	160

#define LSSA 			((unsigned long *)0xFFFFFA00)
#define VPW			((unsigned char *)0xFFFFFA05)
#define PICF			((unsigned char *)0xFFFFFA20)
#define CKCON			((unsigned char *)0xFFFFFA27)
#define LBAR			((unsigned char *)0xFFFFFA29)   
#define FRCM			((unsigned char *)0xFFFFFA31)
#define LGPMR			((unsigned short *)0xFFFFFA32) 

#define cbppGrey		2
#define cppbuGrey		8
#define cbGreyScreen		(MAX_SCREEN_WIDTH*cbppGrey/8*MAX_SCREEN_HEIGHT) 
#define BltUnit Word

void MidPointLine(int x0, int y0, int x1, int y1);
VoidPtr PvAllocLockedChunk(int cb);
int _AllocScreen();
void _ScreenAccess(int fAccess);
void _FreeScreen();
void _SwitchDisplayModeGrey();
void _SetShades(short sh0,short sh1,short sh2,short sh3);

/*
 * Global settings
 */
//static int greyscale_mode;
//static int r, g, b, grey_16_index, grey_4_index;
//static WinStruct win;

static const Word mpclrbuRep[] = {
        0x0000,
        0x5555,
        0xAAAA,
        0xFFFF
};  

/*
 * Returns success (1) if the OS version of the palm is equal to or greater
 * than the version passed in.  The major and minor numbers make up the version
 * number as major.minor (as in 3.1).  If the OS is earlier than the passed in
 * version numbers, failure (0) is returned.
 */
int OSVersion(int major, int minor) {
        DWord romVersion;

        FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
        if ((sysGetROMVerMajor(romVersion) < major)
                        || (sysGetROMVerMajor(romVersion) == major
                        && sysGetROMVerMinor(romVersion) < minor)) {
                return 0;
        }

        return 1;
}

/*
 * SetDisplayMode - Sets or restores the display mode
 * Params: Restore or set bit, along with the parameters to ScrDisplayMode()
 * Return: 0 if successful, otherwise an error code
 */
Err SetDisplayMode(Boolean restore, DWord width, DWord height,
                DWord depth, Boolean color) {
        Err error;
        DWord oldWidth;
        DWord oldHeight;
        DWord oldDepth;
        Boolean oldColor;

        /** Check to see if restoring the old display mode */
        if (restore) {
                error = ScrDisplayMode(scrDisplayModeSet, &oldWidth,
                                &oldHeight, &oldDepth, &oldColor);
        } else {
                /** Capture the current display mode */
                error = ScrDisplayMode(scrDisplayModeGet, &oldWidth,
                                &oldHeight, &oldDepth, &oldColor);

                /** Set the new desired mode */
                if (!error) {
                        error = ScrDisplayMode(scrDisplayModeSet, &width,
                                &height, &depth, &color);
                }
        }

        return error;
}

/*
 * Changes the mode to greyscale.  Queries the operating system to determine if
 * the mode should be set to 1-, 2-, or 4- bit depth.  Sets the global grey-
 * scale mode flag that is used by all other functions.
 */
int fgChangeToGreyscale() {
	int answer;
	int greyscale_mode;
	Err error;

	answer = OSVersion(3,3);
	if (answer) {
		greyscale_mode = GREY_REAL;
	} else {
		answer = OSVersion(2,0);
		if (answer) 
			greyscale_mode = GREY_NONE; //temp
		else
			greyscale_mode = GREY_NONE;
	}

	switch (greyscale_mode) {
		case GREY_REAL:
			error = SetDisplayMode(false, MAX_SCREEN_WIDTH, 
				MAX_SCREEN_HEIGHT, 4, false);
			/** At least one Palm model doesn't support 4-bit. Try
			  * 2-bit if it fails. */
			if (error != 0)
				SetDisplayMode(false, MAX_SCREEN_WIDTH, 
					MAX_SCREEN_HEIGHT, 2, false);
			break;

		case GREY_HACK:
			/** Do stuff here */
			//if (!_AllocScreen()) {
			//	break;
			//	greyscale_mode = GREY_NONE;
			//}
			//win.pbMonoScreenBase=(char*)*LSSA; 
			//_ScreenAccess(1);
			//MemSet(win.pbGreyScreenBase, cbGreyScreen, 0);
			//_ScreenAccess(0);
			//_SwitchDisplayModeGrey();
			//*LSSA=(long)win.pbGreyScreenBase;
			//win.fGreyScale=1;
			//break;

		case GREY_NONE:
			break;	
	}
}

/*
 * Changes back to the normal mono mode.  
 */
int fgChangeFromGreyscale() {
	int answer;
	int greyscale_mode;

	greyscale_mode = GREY_REAL;

	switch (greyscale_mode) {
		case GREY_REAL:
			SetDisplayMode(true, MAX_SCREEN_WIDTH, 
				MAX_SCREEN_HEIGHT, 1, false);
			break;
		case GREY_HACK:
			/** Do stuff here */
			break;
		case GREY_NONE:
			break;	
	}
}

void MidPointLine(int x0, int y0, int x1, int y1) {
	int dx, dy, incrE, incrNE, d, x, y;

	dx = x1 - x0;
	dy = y1 - y0;
	d = 2 * dy - dx;
	incrE = 2 * dy;
	incrNE = 2 * (dy - dx);
	x = x0;
	y = y0;

	fgDrawPixel(x,y);

	while (x < x1) {
		if (d <= 0) {
			d += incrE;
			x++;
		} else {
			d += incrNE;
			x++;
			y++;
		}
		fgDrawPixel(x,y);
	}
}

/*
 * Draws a line from (x0,y0) to (x1,y1) using Bresenham's algorithm and the 
 * globally-set greyscale mode.
 */
void fgDrawLine(int x0, int y0, int x1, int y1) { 
	int greyscale_mode = GREY_REAL;

	if (greyscale_mode == GREY_REAL || greyscale_mode == GREY_MONO) {
		/** Just use the standard Pilot function */
		WinDrawLine(x0,y0,x1,y1);
	} else {
		/** Use our own Bresenham line algorithm */
		if (x1 < x0) {
			MidPointLine(x1,y1,x0,y0);
		} else {
			MidPointLine(x0,y0,x1,y1);
		}
	}
}

/*
 * Sets the color closest to the (r,g,b) value given in the parameters.  Will
 * use the global greyscale mode to determine how to best map and set the 
 * color.
 */
void fgSetColor(int r, int g, int b) {
	RGBColorType color1, color2;
	int greyscale_mode = GREY_REAL;

	switch(greyscale_mode) { 
		case GREY_REAL: 
			/** Use standard Palm function */
			color1.r = r;
			color1.g = g;
			color1.b = b;
			WinSetForeColor(&color1, &color2);
			break;

		case GREY_HACK:
			/** Do stuff here */
			break;

		case GREY_MONO:
			/** Do nothing */
			break;
	}
}

/*
 * Using the current greyscale mode, draws a pixel on the screen using the 
 * current color.
 */
#define WriteMask(buDest,buSrc,buMask)  \
		(buDest=((buDest)&~(buMask))|((buSrc)&(buMask))) 
void fgDrawPixel(int x0, int y0) {
	BltUnit buSrc;
	BltUnit *pbuDest;
	BltUnit mskDest;
	int clr;
	int greyscale_mode = GREY_REAL;

	switch(greyscale_mode) {
		case GREY_REAL:
			WinDrawLine(x0,y0,x0,y0);
			break;

		case GREY_HACK:
			/** Do stuff here */
			//clr = 1;
			//buSrc = mpclrbuRep[clr];
			//pbuDest = ((BltUnit *)win.pbGreyScreenBase)
			//	+(MAX_SCREEN_WIDTH/cppbuGrey)*y0 
			//	+ x0/(cppbuGrey);
			//mskDest = 0x3<<((cppbuGrey-1-x0%cppbuGrey)*cbppGrey);
			//WriteMask(*pbuDest, buSrc, mskDest);
			break;

		case GREY_MONO:
			WinDrawLine(x0,y0,x0,y0);
			break;
	}
}

/*
 *
 */
void fgClearWindow(int sx, int sy, int w, int h) {
	RectangleType rect;
	int greyscale_mode = GREY_REAL;

	rect.topLeft.x = sx;
        rect.topLeft.y =sy;
        rect.extent.x = w;
        rect.extent.y = h;    

	if (greyscale_mode == GREY_REAL || greyscale_mode == GREY_NONE) {
		WinEraseRectangle(&rect,0);
	} else {
		/** Do stuff here */
	}
}

void _SwitchDisplayModeGrey() {
	*CKCON=*CKCON & 0x7F;   /* display off*/

        /*virtual page width now 40 bytes (160 pixels)*/
        *VPW=20;
        *PICF=*PICF | 0x01; /*switch to grayscale mode*/
        *LBAR=20; /*line buffer now 40 bytes*/

        /*register to control grayscale pixel oscillations*/
        *FRCM=0xB9;

        /*let the LCD get to a 2 new frames (40ms delay) */
        SysTaskDelay(4);

        /*switch LCD back on */
        *CKCON=*CKCON | 0x80;

        _SetShades(0, 3, 4, 7);     /* set palette */   
}

void _SetShades(short sh0,short sh1,short sh2,short sh3) {
        *LGPMR=(sh0<<8)+(sh1<<12)+sh2+(sh3<<4);
}  

int _AllocScreen() {
	//win.pbGreyScreenBase = (Byte *)PvAllocLockedChunk(cbGreyScreen);
        //return win.pbGreyScreenBase != NULL; 
	return 0;
}

void _ScreenAccess(int fAccess) {
        if (fAccess)
	        MemSemaphoreReserve(1);
        else
                MemSemaphoreRelease(1);
}           

void _FreeScreen() {
        //if (win.pbGreyScreenBase != NULL) {
        //        MemChunkFree(win.pbGreyScreenBase);
        //       win.pbGreyScreenBase = NULL;
        //}
}     

VoidPtr PvAllocLockedChunk(int cb) {
        int ccard;
        int icard;
        int ipass;
        VoidPtr pv;
        UInt cbFree;
        UInt cbMax;
 
        ccard = MemNumCards();
        for (ipass = 0; ipass < 2; ipass++) {
                for (icard = 0; icard < ccard; icard++) {
                        // don't bother if this card is all ROM
                        if (MemNumRAMHeaps(icard) > 0) {
                                int idHeap;
                                int cidHeap;
 
                                cidHeap = MemNumHeaps(icard);
                                for (idHeap = 0; idHeap < cidHeap; idHeap++) {
                                    // We don't want ROM or Dynamic heaps
                                   if (!(MemHeapFlags(idHeap) 
						& memHeapFlagReadOnly) 
						&& !MemHeapDynamic(idHeap)) {
 
                                          // try two -- let's try compacting
					  // the heap
                                          if (ipass == 1)
                                                MemHeapCompact(idHeap);
                                          // MemHeapFreeBytes(idHeap, &cbFree,
					//	&cbMax);
                                          pv = MemChunkNew(idHeap, cb,
						memNewChunkFlagNonMovable);
 
                                      if (pv != NULL) {
                                                return pv;
                                      }
                                   }
                                }
                        }
                }
        }
        // we couldn't find any heaps so bail
        return NULL;
}

/* 
 * End of fastgraph.c
 */
