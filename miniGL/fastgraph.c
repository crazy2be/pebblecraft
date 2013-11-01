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

#define MAX_SCREEN_WIDTH	144
#define MAX_SCREEN_HEIGHT	144

//4-bit grayscale, only need support in framebuffer and draw_pixel
static uint8_t framebuffer[MAX_SCREEN_WIDTH * MAX_SCREEN_HEIGHT / 2];
static uint8_t current_color = 0x00; //Default to black

#define CHECK_CLIP( x1, y1 ) \
  (x1 >= 0 && x1 < MAX_SCREEN_WIDTH && y1 >= 0 && y1 < MAX_SCREEN_HEIGHT)

#define SWAP_POINTS( v0, v1 ) \
  v0 ^= v1; \
  v1 ^= v0; \
  v0 ^= v1; \

#define DRAW_PIXEL( x0, y0 ) \
  int pixelpos = (y0*MAX_SCREEN_WIDTH + x0) / 2; \
  framebuffer[ pixelpos ] = \
  (x0%2) ? \
    (current_color << 4) | (frambuffer[ pixelpos ]  & 0x0F) : \
    (frambuffer[ pixelpos ] & 0xF0) | current_color; \



/*
 * Draws a line from (x0,y0) to (x1,y1) using Bresenham's algorithm and the 
 * globally-set greyscale mode.
 */
void fgDrawLine(int x0, int y0, int x1, int y1) { 
  
  // If both points out of clipping zone
  if( !Check_Clip( x0, y0 ) && !Check_Clip( x1, y1 ) ) {
    // If both points off same side of clip
    if( x0 >= MAX_SCREEN_WIDTH && x1 >= MAX_SCREEN_WIDTH ) {
      return;
    } else if( x0 < 0 && x1 < 0 ) {
      return;
    } else if( y0 >= MAX_SCREEN_HEIGHT && y1 >= MAX_SCREEN_HEIGHT ) {
      return;
    } else if( y0 < 0 && y1 < 0 ) {
      return;
    }
  }

  if( x0 > x1 ) { // force drawing from left
    SWAP_POINTS( x0, x1 );
    SWAP_POINTS( y0, y1 );
  }

  // compute difference between start and end
  int dx = x1 - x0;
  int dy = abs( y1 - y0 );           // handle top to bottom, bottom to top
  int step_y = ( y0 > y1 )? -1: 1;   // direction of y steps (top to bottom, bottom to top)

  int delta;  // accumulated error from slope
  int x = x0; // placement points (initially start of line)
  int y = y0; // placement points (initially start of line)

  if( dx > dy ) { // find the fastest direction of traversal
    delta = dx / 2;
    while( x != x1 ) {
      delta -= dy;
      if( delta < 0 ) {
        y += step_y;
        delta += dx;
      }
      x++;
      DRAW_PIXEL( x, y );
    }
  } else {
    delta = dy / 2;
    while( y != y1 ) {
      delta -= dx;
      if( delta < 0 ) {
        x++;
        delta += dy;
      }
      y += step_y;
      DRAW_PIXEL( x, y );
    }
  }
}

/*
 * Sets the color closest to the (r,g,b) value given in the parameters.  Will
 * use the global greyscale mode to determine how to best map and set the 
 * color.
 */
void fgSetColor(int r, int g, int b) {
  //current_color = (r+r+r+b+g+g+g+g)>>3; //Fast Luminosity 8-bit
  current_color = (r+r+r+b+g+g+g+g)>>4; //Fast Luminosity 4-bit
}

/*
 * Using the current greyscale mode, draws a pixel on the screen using the 
 * current color.
 */
void fgDrawPixel(int x0, int y0) {
  if ( CHECK_CLIP(x0, y0) ) DRAW_PIXEL( x0, y0 );
}

/*
 *
 */
void fgClearWindow(int sx, int sy, int w, int h) {
  //fast char aligned memset
  if( (x%2) && (w%2) ){
    for(int row = y; row < y+h, row++){
      memset( framebuffer[ (y0*MAX_SCREEN_WIDTH + x0) / 2 ], 0, w/2 );
    }
  }else{ //slow hack, need to manually keep other pixel when not aligned
    //Todo
  }
}


/* 
 * End of fastgraph.c
 */