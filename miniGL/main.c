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

int main(int argc, char* argv[]){

  int xpos = 0;
  int ypos = 0;
  int width = 144;
  int height = 144;

  miniGL_init();

  gl_init();


  //glTranslatef( -(xpos+width/2.0f), -(ypos+height/2.0f),0.0f );

  unsigned char* model = load_model();
  int triangle_count = *(int*)&model[80];
  printf("triangle_count = %d\n",triangle_count);

  int rcount = 1;
#ifdef SDL
  rcount = 5;
#endif

for(int i = 0; i < rcount ; i ++){
  glRotatef(10,0,1,0);
  gl_drawframe(model, triangle_count);
#ifdef SDL
  sleep(1);
#endif
}

  return 0;
}
