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
#include <stdint.h>
#include <string.h>

#ifdef DESKTOP
#include <stdio.h>
#else
#define printf(...)
#endif

#define MAX_SCREEN_WIDTH	144
#define MAX_SCREEN_HEIGHT	168

// 4-bit grayscale, only need support in framebuffer and draw_pixel
uint8_t framebuffer[MAX_SCREEN_WIDTH * MAX_SCREEN_HEIGHT / 2];
static uint8_t current_color = 0x00; //Default to black
static uint8_t clear_color = 0x00; //Default to black


static void draw_pixel(int x, int y) {
  int i = (y*MAX_SCREEN_WIDTH + x) / 2;
  if (x % 2 == 1) {
    framebuffer[i] = (framebuffer[i] & 0xF0) | (current_color & 0x0F);
  } else {
    framebuffer[i] = (framebuffer[i] & 0x0F) | ((current_color & 0x0F) << 4);
  }
}

static int min(int a, int b) {
  return a > b ? b : a;
}

static int max(int a, int b) {
  return a < b ? b : a;
}

static void fgDrawHorizontal(int y, int x0, int x1) {
  if (x0 > x1) {
    int tmp = x0;
    x0 = x1;
    x1 = tmp;
  }
  x0 = max(x0, 0);
  x1 = min(x1, MAX_SCREEN_WIDTH);
  for (int x = x0; x < x1; x++) {
    draw_pixel(x, y);
  }
}

static void fgDrawVertical(int x, int y0, int y1) {
  if (y0 > y1) {
    int tmp = y0;
    y0 = y1;
    y1 = tmp;
  }
  y0 = max(y0, 0);
  y1 = min(y1, MAX_SCREEN_HEIGHT);
  for (int y = y0; y < y1; y++) {
    draw_pixel(x, y);
  }
}

void fgDrawLine(int x0, int y0, int x1, int y1) {
  if (x0 == x1) {
    fgDrawVertical(x0, y0, y1);
  } else if (y0 == y1) {
    fgDrawHorizontal(y0, x0, x1);
  } else {
    printf("fgDrawLine can only do vertical or horizontal lines. %d %d %d %d\n",x0,y0,x1,y1);
  }
}

/*
 * Sets the color closest to the (r,g,b) value given in the parameters.  Will
 * use the global greyscale mode to determine how to best map and set the 
 * color.
 */
void fgSetColor(int r, int g, int b) {
  current_color = (r+r+r+b+g+g+g+g)>>6; //Fast Luminosity 4-bit
  printf("fgSetColor:%d\n",current_color);
}

/*
 * Using the current greyscale mode, draws a pixel on the screen using the 
 * current color.
 */
void fgDrawPixel(int x0, int y0) {
  printf("fgDrawPixel not implemented\n");
}

void fgClearColor(int r, int g, int b){
  clear_color = (r+r+r+b+g+g+g+g)>>6; //Fast Luminosity 4-bit
}

void fgClearWindow(int sx, int sy, int w, int h) {
  uint32_t clearvalue = clear_color << 4 | clear_color;
  memset(framebuffer, clearvalue, sizeof(framebuffer));
}
