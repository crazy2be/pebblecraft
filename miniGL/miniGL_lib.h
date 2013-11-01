/**
 * miniGL_lib.h
 *
 * miniGL for the Palm Computing platform
 * Michael Sherman <msherman@dsbox.com>
 * (c) 2000 Digital Sandbox, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef ___MINIGL_LIB_H___
#define ___MINIGL_LIB_H___

#include "fastgraph.h"

/**
 * The global structure for this library
 */
UInt refcount;

float modv_matrix[MAX_MAT_STACK_MODV][16]; 
char  modv_level;
float proj_matrix[MAX_MAT_STACK_PROJ][16]; 
char  proj_level;

GLfloat cur_matrix[16];
GLfloat per_matrix[16];
GLfloat scr_matrix[16]; 
GLfloat cur_color[4];
GLfloat cur_normal[4];
GLfloat vertices_color[MAX_VERTICES][4];
GLfloat vertices_normal[MAX_VERTICES][4];
GLfloat vertices[MAX_VERTICES][4];
GLfloat scr_vertices[MAX_VERTICES][3];
int num_vertices;
int screen_width;
int screen_height;
int culling;
int screen_startx;
int screen_starty;
int two_created;
int lighting;
int wireframe;
int greyscale_mode;
GLenum cur_mode;
GLenum matrix_mode;
Light lights[8];



#endif

/**
 * End of miniGL_lib.h
 */
