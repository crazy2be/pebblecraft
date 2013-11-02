
#ifndef SDL
  #include "miniGL.h"
  #define GL_COLOR_BUFFER_BIT GL_COLOR
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

  glViewport(0,0,144,144);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  
  glOrtho(-74, 74, -74, 74, -144, 30.0);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

#if 1
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);

  //gluPerspective(40.0, 1.4, -100.0, 0.0);

  
	GLfloat amb[4] = {0.3, 0.3, 0.3, 0};
	GLfloat dif[4] = {1,1,1,0};
  GLfloat lightpos[] = {30.0, 64.0, -34.0, 1};
  glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  
#else
  //gluPerspective(40, 1.0, 20.0, 100.0);
  glDisable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);
#endif


  //glTranslatef( -(xpos+width/2.0f), -(ypos+height/2.0f),0.0f );

  FILE* modelfile = fopen("model1.stl","rb");
  if(modelfile == NULL){
    printf("couldn't open modelfile\n");
    return 1;
  }
  fseek(modelfile,0,SEEK_END);
  int modelfile_bytes = ftell(modelfile);
  fseek(modelfile,0,SEEK_SET);
  char* model = (char*)malloc(modelfile_bytes);
  fread(model,1,modelfile_bytes,modelfile);

  int triangle_count = *(int*)&model[80];
  printf("triangle_count = %d\n",triangle_count);

#pragma pack(push,1) 
  struct stl_data {
    float normal[3];
    float vertex1[3];
    float vertex2[3];
    float vertex3[3];
    uint16_t color;
  };
#pragma pack(pop)

  float red = 0;
  float green = 0;
  float blue = 0;

  int rcount = 1;
#ifdef SDL
  rcount = 5;
#endif

for(int i = 0; i < rcount ; i ++){
  glRotatef(10,0,1,0);
  glClearColor(0.0,0.0,0.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < triangle_count; i++){
    struct stl_data stl = 
      *(struct stl_data*)&model[80 + 4 + i*sizeof(struct stl_data)];
    red   = (   stl.color & 0x001F )        / 31.0;
    green = ( ( stl.color & 0x03E0 ) >> 5 ) / 31.0;
    blue  = ( ( stl.color & 0x7C00 ) >> 10) / 31.0;

    printf("stl[%i]: red:%f green:%f blue:%f \n",i,red,green,blue);
    printf("normal( %f, %f, %f )\n",
      stl.normal[0], stl.normal[1], stl.normal[2]);
    printf(
      "vertex1( %f ,%f ,%f )\n"
      "vertex2( %f ,%f ,%f )\n"
      "vertex3( %f ,%f ,%f )\n",
      stl.vertex1[0],stl.vertex1[1],stl.vertex1[2],
      stl.vertex2[0],stl.vertex2[1],stl.vertex2[2],
      stl.vertex3[0],stl.vertex3[1],stl.vertex3[2]);

    //glColor3f(red,green,blue);
    glColor3f(0.9,0.9,0.9);
    glBegin(GL_POLYGON);
    glNormal3f(stl.normal[0],stl.normal[1],stl.normal[2]);
    glVertex3f(stl.vertex1[0], stl.vertex1[1], stl.vertex1[2]);
    glVertex3f(stl.vertex2[0], stl.vertex2[1], stl.vertex2[2]);
    glVertex3f(stl.vertex3[0], stl.vertex3[1], stl.vertex3[2]);
    glEnd();
  }
  glFlush();
  glClear(GL_COLOR_BUFFER_BIT);
#ifdef SDL
  sleep(1);
#endif
}

  return 0;
}
