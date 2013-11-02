#include <stdlib.h>

#include "mine.h"
#ifdef SDL
  #include <GL/gl.h>
#else
  #include "miniGL.h"
  #define GL_COLOR_BUFFER_BIT GL_COLOR
#endif

#ifdef DESKTOP
#include <stdio.h>
#else
#define printf(...)
#endif

void gl_init() {
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
//   gluPerspective(40, 1.0, 20.0, 100.0);
  glDisable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_LINE);
#endif
}

void gl_drawframe(uint8_t* model) {
  static float vx = 0, vy = 0, vz = 0;
  int triangle_count = *(int*)&model[80];
  glClearColor(0.0,0.0,0.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  vx += (float)(rand() % 100)/10.0;
  vy += (float)(rand() % 100)/10.0;
  vz += (float)(rand() % 100)/10.0;
  glRotatef(10.0, 1.0, 1.0, 1.0);
  for (int i = 0; i < triangle_count; i++){
    struct stl_data stl = *(struct stl_data*)&model[80 + 4 + i*sizeof(stl)];
//     float red   = (   stl.color & 0x001F )        / 31.0;
//     float green = ( ( stl.color & 0x03E0 ) >> 5 ) / 31.0;
//     float blue  = ( ( stl.color & 0x7C00 ) >> 10) / 31.0;

//     printf("stl[%i]: red:%f green:%f blue:%f \n",i,red,green,blue);
//     printf("normal( %f, %f, %f )\n",
//       stl.normal[0], stl.normal[1], stl.normal[2]);
//     printf(
//       "vertex1( %f ,%f ,%f )\n"
//       "vertex2( %f ,%f ,%f )\n"
//       "vertex3( %f ,%f ,%f )\n",
//       stl.vertex1[0],stl.vertex1[1],stl.vertex1[2],
//       stl.vertex2[0],stl.vertex2[1],stl.vertex2[2],
//       stl.vertex3[0],stl.vertex3[1],stl.vertex3[2]);

    //glColor3f(red,green,blue);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glNormal3f(stl.normal[0],stl.normal[1],stl.normal[2]);
    glVertex3f(stl.vertex1[0], stl.vertex1[1], stl.vertex1[2]);
    glVertex3f(stl.vertex2[0], stl.vertex2[1], stl.vertex2[2]);
    glVertex3f(stl.vertex3[0], stl.vertex3[1], stl.vertex3[2]);
    glEnd();
  }
  glFlush();
}
