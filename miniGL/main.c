#include "lib/mine.h"

#ifndef SDL
  #include "lib/miniGL.h"
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <SDL/SDL.h>

  #include <unistd.h> //sleep

  #include <math.h>
  #define DEG2RAD 3.14159/180.0
  void miniGL_init(void){
   int xresolution = 144;
   int yresolution = 144;

   SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD);
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0 );
   SDL_SetVideoMode(xresolution, yresolution, 0, SDL_OPENGL);

   SDL_ShowCursor( 0 );
  }
#endif

#include <stdio.h>
#include <stdlib.h>


#include <stdio.h>
unsigned char* load_model() {
  FILE* modelfile = fopen("model1.stl","rb");
  if(modelfile == NULL){
    printf("couldn't open modelfile\n");
    return NULL;
  }
  fseek(modelfile,0,SEEK_END);
  int modelfile_bytes = ftell(modelfile);
  fseek(modelfile,0,SEEK_SET);
  unsigned char* model = (unsigned char*)malloc(modelfile_bytes);
  fread(model,1,modelfile_bytes,modelfile);
  return model;
}

#ifdef SDL
  uint8_t framebuffer[144*144/2];
#else
  extern uint8_t framebuffer[144*144/2];
#endif

void save_image(const char* filename, unsigned char* data, int len) {
  FILE* fp = fopen(filename, "wb");
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


int main(int argc, char* argv[]){

  int xpos = 0;
  int ypos = 0;
  int width = 144;
  int height = 144;

  miniGL_init();

  gl_init();


  //glTranslatef( -(xpos+width/2.0f), -(ypos+height/2.0f),0.0f );

  unsigned char* model = load_model();

  int rcount = 2;
#ifdef SDL
  rcount = 50;
#endif

for(int i = 0; i < rcount ; i ++){
  glRotatef(10,0,1,0);
  gl_drawframe(model);

  //lets dump the framebuffer to file
  static int framecount = 0;
  static int filecount = 0;
  char filename[32];
  sprintf(filename,"fb_%03d.gray4bit",filecount);
  save_image(filename, framebuffer, sizeof(framebuffer));
  filecount++;
  framecount++;

#ifdef SDL
  sleep(1);
#endif
}

  return 0;
}
