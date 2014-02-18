#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleep
#include <math.h>

#include <SDL/SDL.h>
// Non-public but extremely useful stretch
extern int SDL_SoftStretch(
    SDL_Surface *src, SDL_Rect *srcrect,
    SDL_Surface *dst, SDL_Rect *dstrect);

#include "model_stl.h"
#include "dither.h"
#include "miniGL/miniGL.h"

unsigned char* load_model() {
  FILE* modelfile = fopen("../resources/mask.stl","r");
  if(modelfile == NULL){
    printf("Error: couldn't open modelfile\n");
    exit(EXIT_FAILURE);
  }
  fseek(modelfile,0,SEEK_END);
  int modelfile_bytes = ftell(modelfile);
  fseek(modelfile,0,SEEK_SET);
  unsigned char* model = (unsigned char*)malloc(modelfile_bytes);
  fread(model,1,modelfile_bytes,modelfile);
  return model;
}


SDL_Surface *sdl_surface;
SDL_Surface *cpy_surface; // 32-bit surface for copy buffer
SDL_Surface *img_surface; // 1-bit surface

//1-bit screen buffer
#define WORD_ALIGNED(bits) \
  ((((bits) + 31) / 32) * 32)
uint8_t screenbuffer[ WORD_ALIGNED(FRAMEBUFFER_WIDTH) * FRAMEBUFFER_HEIGHT / 8];

void sdl_setup(void) {
  SDL_Color colors[2] = {{0,0,0}, {255,255,255}};

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD);
  sdl_surface = SDL_SetVideoMode(
    FRAMEBUFFER_WIDTH * 3, FRAMEBUFFER_HEIGHT * 3, 
    32, //can't use 0 (autodetect) as next surface must match 
    SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_ShowCursor( 0 );

  //Used to convert 1-bit buffer to 32-bit buffer
  cpy_surface = SDL_CreateRGBSurface( 0,
    FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 
    32, //depth
    0, 0, 0, 0); 

  img_surface = SDL_CreateRGBSurfaceFrom( screenbuffer,
    FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 
    1, //depth
    WORD_ALIGNED(FRAMEBUFFER_WIDTH) / 8, //row_stride in bytes
    0, 0, 0, 0); 

  if (!img_surface) {
    printf("SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  //Default palette was backwards, fix here
  SDL_SetPalette(img_surface, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 2);
}

uint8_t reverse_byte(uint8_t b) {
  return ((b * 0x0202020202ULL & 0x010884422010ULL) % 1023);
}

void sdl_draw(void) {
  //fix pebble byte order (msbit) to sdl (lsbit)
  for(int i = 0; i < 160 * 144 / 8; i++){
    screenbuffer[i] = reverse_byte(screenbuffer[i]);
  }

  SDL_BlitSurface(img_surface, NULL, cpy_surface, NULL);
  SDL_SoftStretch(cpy_surface, 0, sdl_surface, 0);
  SDL_Flip(sdl_surface);
}

//provided by draw2d
extern uint8_t framebuffer[144*144/2];

int main(int argc, char* argv[]){
  bool reset = false;
  bool wireframe = false;
  uint8_t rotation = 0;

  unsigned char* model = load_model();

  sdl_setup();

  gl_init();

  while (1) {
    rotation = (rotation + 10) % 360;
    gl_drawframe(model, wireframe, rotation, reset);
    floyd_steinberg_dither(framebuffer, (uint8_t*)screenbuffer, -1);
    sdl_draw();
    usleep(200 * 1000);
  }

  return 0;
}
