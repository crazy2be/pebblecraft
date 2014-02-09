#ifndef FASTGRAPH_H
#define FASTGRAPH_H

void fgDrawLine(int x0, int y0, int x1, int y1);
void fgDrawScanLine(int x0, int y0, int x1, int y1);
void fgSetColor(int r, int g, int b);
void fgClearColor(int r, int g, int b);
void fgDrawPixel(int x0, int y0);
void fgClearWindow(int sx, int sy, int w, int h);

#endif

/* 
 * End of fastgraph.h
 */
