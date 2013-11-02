#include <stdlib.h>
#include <stdio.h>

#define byte unsigned char

byte extract_grey(byte* grey, int i) {
  byte val;
  if (i%2 == 1) {
    val = (grey[i/2] & 0x0F) << 4;
  } else {
    val = (grey[i/2] & 0xF0);
  }
  return val | (val >> 4);
}

// Note: low bits are ignored.
void set_grey(byte* grey, int i, byte val) {
  if (i%2 == 1) {
    grey[i/2] = (grey[i/2] & 0xF0) | (val >> 4);
  } else {
    grey[i/2] = (grey[i/2] & 0x0F) | (val & 0xF0);
  }
}

int clamp(int n, int a, int b) {
  return n < a ? a : n > b ? b : n;
}

void add_grey(byte* grey, int i, int amount) {
  byte val = extract_grey(grey, i);
  set_grey(grey, i, clamp(val + amount, 0, 255));
}

void set_black(byte* bw, int i) {
  bw[i/8] |= 1 << (7 - i%8);
}
void set_white(byte* bw, int i) {
  bw[i/8] &= ~(1 << (7 - i%8));
}
void set_bw(byte* bw, int i, byte val) {
  if (val > 127) {
    set_black(bw, i);
  } else {
    set_white(bw, i);
  }
}

void naive_dither(byte* grey, byte* bw, int num_pixels) {
  for (int i = 0; i < num_pixels; i++) {
    set_bw(bw, i, extract_grey(grey, i));
  }
}

int index(int x, int y) {
  return x*144 + y;
}

// WARNING: Modifies grey buffer
void floyd_steinberg_dither(byte* grey, byte* bw, int num_pixels) {
  int w = 144;
  int h = 144;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int g = extract_grey(grey, index(x, y));
      int n = g > 127 ? 255 : 0;
      set_bw(bw, index(x, y), n);
      int error = g - n;
      add_grey(grey, index(x+1, y  ), (7 * error) / 16);
      add_grey(grey, index(x-1, y+1), (3 * error) / 16);
      add_grey(grey, index(x  , y+1), (5 * error) / 16);
      add_grey(grey, index(x+1, y+1), (1 * error) / 16);
    }
  }
}

byte* load_image(const char* filename) {
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("ERROR reading file %s", filename);
    return NULL;
  }
  if (fseek(fp, 0L, SEEK_END) != 0) {
    printf("ERROR seeking in file %s", filename);
    return NULL;
  }
  long bufsize = ftell(fp);
  if (bufsize == -1) {
    printf("ERROR getting file bufsize %s", filename);
    return NULL;
  }
  byte* buf = (byte*)malloc(sizeof(byte) * bufsize + 1);
  if (buf == NULL) {
    printf("ERROR Could not allocate buffer %ld", bufsize);
    return NULL;
  }
  if (fseek(fp, 0L, SEEK_SET) != 0) {
    printf("ERROR seeking in file %s", filename);
    return NULL;
  }
  size_t readsize = fread(buf, sizeof(byte), bufsize, fp);
  if (!(readsize > 0)) {
    printf("ERROR file is empty %s", filename);
    return NULL;
  }
  return buf;
}

void save_image(const char* filename, byte* data, int len) {
  FILE* fp = fopen("dithered.bin", "wb");
  if (fp == NULL) {
    printf("ERROR opening file %s\n", filename);
    return;
  }
  int written = fwrite(data, len, 1, fp);
  if (written != 1) {
    printf("ERROR writing to file %s. Wrote %d bytes of %d total\n", filename, written, len);
    return;
  }
  if (fflush(fp) != 0) {
    printf("ERROR flushing file %s\n", filename);
    return;
  }
  if (fclose(fp) != 0) {
    printf("ERROR closing file %s\n", filename);
    return;
  }
}

int main(int argc, char** argv) {
  if (argc < 1) {
    printf("Usage: ditherer.bin <grey4 format file>\n");
    return 1;
  }
  int num_pixels = 144*144;
  byte* in = load_image(argv[1]);
  byte* out = (byte*)malloc(num_pixels/8);
  floyd_steinberg_dither(in, out, num_pixels);
  save_image("dithered.bin", out, num_pixels/8);
  return 0;
}
