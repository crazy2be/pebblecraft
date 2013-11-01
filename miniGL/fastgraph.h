/**
 * fastgraph.h
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

#ifndef FASTGRAPH_H
#define FASTGRAPH_H

#include <Pilot.h>
#include <ScrDriver.h>
#include <MemoryPrv.h>

int OSVersion(int major, int minor);
Err SetDisplayMode(Boolean restore, DWord width, DWord height,
                DWord depth, Boolean color);

int fgChangeToGreyscale();
int fgChangeFromGreyscale();
void fgDrawLine(int x0, int y0, int x1, int y1);
void fgSetColor(int r, int g, int b);
void fgDrawPixel(int x0, int y0);
void fgClearWindow(int sx, int sy, int w, int h);

typedef struct {
        //Win2Color clr;
        //Win2Color clrBackground;
        Byte *pbGreyScreenBase;
        Byte *pbMonoScreenBase;
        short fGreyScale;
} WinStruct;    

#endif

/* 
 * End of fastgraph.h
 */
