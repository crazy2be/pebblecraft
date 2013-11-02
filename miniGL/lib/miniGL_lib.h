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

#pragma once

// The global structure for this library
extern uint32_t refcount;

extern float modv_matrix[MAX_MAT_STACK_MODV][16];
extern char  modv_level;
extern float proj_matrix[MAX_MAT_STACK_PROJ][16];
extern char  proj_level;

extern GLfloat cur_matrix[16];
extern GLfloat per_matrix[16];
extern GLfloat scr_matrix[16];
extern GLfloat cur_color[4];
extern GLfloat cur_normal[4];
extern GLfloat vertices_color[MAX_VERTICES][4];
extern GLfloat vertices_normal[MAX_VERTICES][4];
extern GLfloat vertices[MAX_VERTICES][4];
extern GLfloat scr_vertices[MAX_VERTICES][3];
extern int num_vertices;
extern int screen_width;
extern int screen_height;
extern int culling;
extern int screen_startx;
extern int screen_starty;
extern int two_created;
extern int lighting;
extern int wireframe;
extern int greyscale_mode;
extern GLenum cur_mode;
extern GLenum matrix_mode;
extern Light lights[8];
