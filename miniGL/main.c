
#include "miniGL.h"
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
  
  glOrtho(0, 144, 0, 144, -1, 1);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);

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

  glClear(GL_COLOR);
  glScalef(2,2,2);
  for (int i = 0; i < triangle_count; i++){
    struct stl_data stl = 
      *(struct stl_data*)&model[80 + 4 + i*sizeof(struct stl_data)];
    red   = (   stl.color & 0x001F )        / 31.0;
    green = ( ( stl.color & 0x03E0 ) >> 5 ) / 31.0;
    blue  = ( ( stl.color & 0x7C00 ) >> 10) / 31.0;

    printf("stl[%i]: red:%f green:%f blue:%f \n",i,red,green,blue);
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
    glVertex3f(stl.vertex1[0], stl.vertex1[1], stl.vertex1[2]);
    glVertex3f(stl.vertex2[0], stl.vertex2[1], stl.vertex2[2]);
    glVertex3f(stl.vertex3[0], stl.vertex3[1], stl.vertex3[2]);
    glEnd();
  }
  glClear(GL_COLOR);

  return 0;
}
