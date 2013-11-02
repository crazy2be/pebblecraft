#include <inttypes.h>

void gl_init(void);

void gl_drawframe(uint8_t* model);

struct stl_data {
  float normal[3];
  float vertex1[3];
  float vertex2[3];
  float vertex3[3];
  uint16_t color;
} __attribute__ ((packed));
