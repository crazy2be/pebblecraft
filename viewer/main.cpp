#include <GL/gl.h>
#include <SDL/SDL.h>

#include <math.h>
#define DEG2RAD 3.14159/180.0
static void draw_hallow_circle( float size, float x, float y ){
   float angle = 0.0f;
   float radius = size;
   float degInRad = 0.0f;
   glBegin(GL_LINE_LOOP);
   for (angle=0; angle < 360.0f; angle += 1.0f){
      degInRad = angle*DEG2RAD;
      glVertex2f(sin(degInRad)*radius + x,cos(degInRad)*radius + y);
   }
   glEnd();
}

static void handle_sdl_events(){
   SDL_Event sdlevent;
   if(SDL_PeepEvents(&sdlevent,1,SDL_GETEVENT, 
            SDL_ALLEVENTS//Handle all events, else event queue full
            ) == 1){
      switch( sdlevent.type ) {
      case SDL_KEYDOWN:
      case SDL_QUIT:
         SDL_Quit();
         exit(0);
         break;
      default:
         break;
      }
   }
}

int main ( int argc, char *argv[]){
   int xresolution = 144;
   int yresolution = 144;

   SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD);
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0 );
   SDL_SetVideoMode(xresolution, yresolution, 0, SDL_OPENGL);

   glViewport( 0, 0, xresolution, yresolution );

   //Orthogonality setup
   glMatrixMode( GL_PROJECTION );
   glPushMatrix();
   glLoadIdentity();
   glOrtho( 0, xresolution, 0, yresolution, -1, 1 );

   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
   glLoadIdentity();

   SDL_ShowCursor( 0 );

   while(1){
   glClear( GL_COLOR_BUFFER_BIT );
   
   glColor3f(1,0,0);
   draw_hallow_circle(150, xresolution/2, yresolution/2);
   glColor3f(0,1,0);
   draw_hallow_circle(100, xresolution/2, yresolution/2);
   glColor3f(0,0,1);
   draw_hallow_circle( 50, xresolution/2, yresolution/2);

   glFlush();
   glFinish();
   //SDL_GL_SwapBuffers();
   handle_sdl_events();
   }

   return 0;
}


