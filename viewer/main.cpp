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

enum ImageFormat {
  Grey4,
  Grey1,
};

GLubyte* load_image(const char* filename) {
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("ERROR reading file %s", filename);
    return NULL;
  }
  if (fseek(fp, 0L, SEEK_END) != 0) {
    printf("ERROR seeking in file %s", filename);
    return NULL;
  }
  long bufsize = ftell(fp);
  if (bufsize == -1) {
    printf("ERROR getting file bufsize %s", filename);
    return NULL;
  }
  GLubyte* buf = (GLubyte*)malloc(sizeof(GLubyte) * bufsize + 1);
  if (buf == NULL) {
    printf("Could not allocate buffer %ld", bufsize);
    return NULL;
  }
  if (fseek(fp, 0L, SEEK_SET) != 0) {
    printf("ERORR seeking in file %s", filename);
    return NULL;
  }
  size_t readsize = fread(buf, sizeof(GLubyte), bufsize, fp);
  if (!(readsize > 0)) {
    printf("ERROR file is empty %s", filename);
    return NULL;
  }
  return buf;
}

void draw_image(ImageFormat format, GLubyte* data) {
  float index[] = {0.0, 1.0};

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

  glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 2, index);
  glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 2, index);
  glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 2, index);
  glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 2, index);

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,16,16,0,GL_COLOR_INDEX,GL_BITMAP,data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
  if (argc < 2) {
    printf("Usage: viewer <grey4|grey1> <filename>\n");
    return 1;
  }
  ImageFormat format;
  if (strcmp(argv[1], "grey4")) {
    format = Grey4;
  } else if (strcmp(argv[1], "grey1")) {
    format = Grey1;
  } else {
    printf("Invalid format %s", argv[1]);
    return 1;
  }
  char* filename = argv[2];
  GLubyte* image_data = load_image(filename);

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

    draw_image(format, image_data);

    glFlush();
    glFinish();
    //SDL_GL_SwapBuffers();
    handle_sdl_events();
  }

  return 0;
}


