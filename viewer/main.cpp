#include <GL/gl.h>
#include <SDL/SDL.h>

enum ImageFormat {
  Grey1,
  Grey4,
};

typedef struct {
  int width;
  int height;
  GLubyte* buf;
} ImageData;

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

GLubyte* make_texture_data(ImageFormat format, ImageData data) {
  int n = format == Grey1 ? 8 : 2;
  int len = data.width * data.height;
  GLubyte* rgb = (GLubyte*)malloc(len * 3 + 1);
  GLubyte* grey = data.buf;
  for (int i = 0; i < len; i++) {
    int s = 7 - i%n; // shift
    rgb[i*3  ] = 255 * ((grey[i / n] & (1 << s))>>s);
    rgb[i*3+1] = 255 * ((grey[i / n] & (1 << s))>>s);
    rgb[i*3+2] = 255 * ((grey[i / n] & (1 << s))>>s);
  }
  return rgb;
}

GLuint load_texture(ImageFormat format, ImageData data) {
  GLuint texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  float index2[] = {0.0, 1.0};
  float index16[] = {
    0.0 /15.0, 1.0 /15.0, 2.0 /15.0, 3.0 /15.0,
    4.0 /15.0, 5.0 /15.0, 6.0 /15.0, 7.0 /15.0,
    8.0 /15.0, 9.0 /15.0, 10.0/15.0, 11.0/15.0,
    12.0/15.0, 13.0/15.0, 14.0/15.0, 15.0/15.0,
  };

  GLubyte* texture_data = make_texture_data(format, data);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.width, data.height,
               0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  return texture;
}

void draw_texture(GLuint texture, int width, int height) {
  glBindTexture(GL_TEXTURE_2D, texture);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0  , 1.0);
  glVertex2f  (0.0  , 0.0);

  glTexCoord2f(1.0  , 1.0);
  glVertex2f  (width, 0.0);

  glTexCoord2f(1.0  , 0.0);
  glVertex2f  (width, height);

  glTexCoord2f(0.0  , 0.0);
  glVertex2f  (0.0  , height);

  glEnd();
}

static void handle_sdl_events(){
   SDL_Event sdlevent;
   if (SDL_PeepEvents(&sdlevent,1,SDL_GETEVENT,
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

static GLubyte smiley[] = /* 16x16 smiley face */
{
    0x03, 0xc0, /*       ****       */
    0x0f, 0xf0, /*     ********     */
    0x1e, 0x78, /*    ****  ****    */
    0x39, 0x9c, /*   ***  **  ***   */
    0x77, 0xee, /*  *** ****** ***  */
    0x6f, 0xf6, /*  ** ******** **  */
    0xff, 0xff, /* **************** */
    0xff, 0xff, /* **************** */
    0xff, 0xff, /* **************** */
    0xff, 0xff, /* **************** */
    0x73, 0xce, /*  ***  ****  ***  */
    0x73, 0xce, /*  ***  ****  ***  */
    0x3f, 0xfc, /*   ************   */
    0x1f, 0xf8, /*    **********    */
    0x0f, 0xf0, /*     ********     */
    0x03, 0xc0  /*       ****       */
};

static void initGL(int w, int h) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD);
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0 );
  SDL_SetVideoMode(w, h, 0, SDL_OPENGL);

  glViewport( 0, 0, w, h);

  //Orthogonality setup
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glOrtho( 0, w, 0, h, -1, 1 );

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);
}

ImageFormat read_format(const char* str) {
  ImageFormat format;
  if (strcmp(str, "grey4") == 0) {
    return Grey4;
  } else if (strcmp(str, "grey1") == 0) {
    return Grey1;
  } else {
    printf("Invalid format %s", str);
    exit(1);
  }
}

ImageData read_image(const char* name) {
  ImageData data;
  if (strcmp(name, "smile") == 0) {
    data.buf = smiley;
    data.width = 16;
    data.height = 16;
  } else {
    data.buf = load_image(name);
    data.width = 144;
    data.height = 144;
  }
  if (!data.buf) exit(1);
  return data;
}

int main ( int argc, char *argv[]){
  if (argc < 2) {
    printf("Usage: viewer <grey4|grey1> <filename>\n");
    return 1;
  }
  ImageFormat format = read_format(argv[1]);
  ImageData data = read_image(argv[2]);
  printf("Format: %d\n", format);
  fflush(stdout);

  int screen_width = 144;
  int screen_height = 144;

  initGL(screen_width, screen_height);

  GLuint texture = load_texture(format, data);

  while (1) {
    glClear( GL_COLOR_BUFFER_BIT );

    draw_texture(texture, screen_width, screen_height);

    glFlush();
    glFinish();
    handle_sdl_events();
  }

  return 0;
}
