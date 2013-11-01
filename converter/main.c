#include "rgb-renderbuf.h"

#define RENDER_WIDTH 144
#define RENDER_HEIGHT 144

static inline int min(int a, int b) {
  return a < b ? a : b;
}

static inline char luma(char r, char g, char b) {
  return (r+r+b+g+g+g)/6;
}
// Grey buf is two pixels per char. All lengths refer to length
// in chars.
void copy_from_rgb_to_grey(char* rgb_data, int rgb_len,
                           char* grey_buf, int grey_len) {
  char* rgb = rgb_data;
  int len = min(rgb_len, grey_len);
  for (int grey_idx = 0; grey_idx < len; grey_idx++) {
    char grey_char = 0;
    int i = grey_idx * 6;
    grey_char |= luma(rgb[i  ], rgb[i+1], rgb[i+2]) & 0xF0;
    grey_char |= luma(rgb[i+3], rgb[i+4], rgb[i+5]) >> 4;
    grey_buf[grey_idx] = grey_char;
  }
}

int main () {
  int w = gimp_image.width;
  int h = gimp_image.height;
  int rgb_len = w * h * gimp_image.bytes_per_pixel;
  char* grey_buf = malloc(w*h + 1);
  copy_from_rgb_to_grey(gimp_image.pixel_data, rgb_len,
                        grey_buf, w*h);
  
}
