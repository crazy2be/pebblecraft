#include <stdlib.h>
#include <stdio.h>
#include "rgb-renderbuf.h"

#define RENDER_WIDTH 144
#define RENDER_HEIGHT 144

#define byte unsigned char

static inline int min(int a, int b) {
  return a < b ? a : b;
}

// http://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color
static inline byte luma(byte r, byte g, byte b) {
  return ((r+b+g)/3);
}
// Grey buf is two pixels per byte. All lengths refer to length
// in bytes.
void copy_from_rgb_to_grey(byte* rgb_data, int rgb_len,
                           byte* grey_buf, int grey_len) {
  byte* rgb = rgb_data;
  int len = min(rgb_len, grey_len);
  for (int grey_idx = 0; grey_idx < len; grey_idx++) {
    byte grey_byte = 0;
    int i = grey_idx * 6;
    grey_byte |= luma(rgb[i  ], rgb[i+1], rgb[i+2]) & 0xF0;
    grey_byte |= luma(rgb[i+3], rgb[i+4], rgb[i+5]) >> 4;
    grey_buf[grey_idx] = grey_byte;
  }
}

int main () {
  int w = gimp_image.width;
  int h = gimp_image.height;
  int rgb_len = w * h * gimp_image.bytes_per_pixel;
  int grey_len = w * h / 2;
  byte* grey_buf = malloc(grey_len + 1);

  copy_from_rgb_to_grey(gimp_image.pixel_data, rgb_len,
                        grey_buf, grey_len);

  FILE* pf = fopen("dump.bin", "wb");
  int written;
  if ((written = fwrite(grey_buf, grey_len, 1, pf)) != 1) {
    printf("ERROR writing to file %d %d\n", written, grey_len);
    return 1;
  }
  fflush(pf);
  if (fclose(pf) != 0) {
    printf("ERROR closing file\n");
    return 1;
  }
  printf("SUCCESS %d %d %d\n", w, h, grey_len);
  return 0;
}
