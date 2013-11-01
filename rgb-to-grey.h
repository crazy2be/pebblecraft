
#define RENDER_WIDTH 144
#define RENDER_HEIGHT 144

static inline unit16_t min(uint16_t a, uint16_t b) {
  return a < b ? a : b;
}

static inline uint8_t luma(uint8_t r, uint8_t g, uint8_t b) {
  return (r+r+b+g+g+g)/6;
}
// Grey buf is two pixels per byte. All lengths refer to length
// in bytes.
void copy_from_rgb_to_grey(uint8_t* rgb_data, uint16_t rgb_len,
                           uint8_t* grey_buf, uint16_t grey_len) {
  uint8_t* rgb = rgb_data;
  uint16_t len = min(rgb_len, grey_len);
  for (uint16_t grey_idx = 0; grey_idx < len; grey_idx++) {
    uint8_t grey_byte = 0;
    uint16_t i = grey_idx * 6;
    grey_byte |= luma(rgb[i  ], rgb[i+1], rgb[i+2]) & 0xF0;
    grey_byte |= luma(rgb[i+3], rgb[i+4], rgb[i+5]) >> 4;
    grey_buf[grey_idx] = grey_byte;
  }
}
