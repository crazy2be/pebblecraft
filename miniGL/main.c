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

int main(int argc, char* argv[]){

  int xpos = 0;
  int ypos = 0;
  int width = 144;
  int height = 144;

  miniGL_init();

  gl_init();


  //glTranslatef( -(xpos+width/2.0f), -(ypos+height/2.0f),0.0f );

  unsigned char* model = load_model();

  int rcount = 1;
#ifdef SDL
  rcount = 5;
#endif

for(int i = 0; i < rcount ; i ++){
  glRotatef(10,0,1,0);
  gl_drawframe(model);
#ifdef SDL
  sleep(1);
#endif
}

  return 0;
}
