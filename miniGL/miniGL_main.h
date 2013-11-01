/**
 * miniGL_main.h
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

#ifndef __MINIGL_MAIN_H__
#define __MINIGL_MAIN_H__

void minigllib_glmultmatrixf(UInt miniGLLibRef, const GLfloat *input);
void minigllib_glloadmatrixf(UInt miniGLLibRef, const GLfloat *m);
void minigllib_glloadidentity(UInt miniGLLibRef);
void minigllib_glclearcolor(UInt miniGLLibRef, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void minigllib_glclear(UInt miniGLLibRef, GLbitfield mask);
void minigllib_glcolor3f(UInt miniGLLibRef, GLfloat r, GLfloat g, GLfloat b);
void minigllib_glcolor4f(UInt miniGLLibRef, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void minigllib_glortho(UInt miniGLLibRef, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
		GLdouble near, GLdouble distant);
void minigllib_gluortho2d(UInt miniGLLibRef, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
void minigllib_glviewport(UInt miniGLLibRef, GLint x, GLint y, GLsizei width, GLsizei height);
void minigllib_gluperspective(UInt miniGLLibRef, GLdouble fovy, GLdouble aspect, GLdouble near, 
		GLdouble distant);
void minigllib_glbegin(UInt miniGLLibRef, GLenum mode);
void minigllib_glend(UInt miniGLLibRef);
void minigllib_glutswapbuffers(UInt miniGLLibRef);
void minigllib_glflush(UInt miniGLLibRef);
void minigllib_glvertex3f(UInt miniGLLibRef, GLfloat x, GLfloat y, GLfloat z);
void minigllib_glvertex2f(UInt miniGLLibRef, GLfloat x, GLfloat y);
void minigllib_glshademodel(UInt miniGLLibRef, GLenum mode);
void minigllib_glrotatef(UInt miniGLLibRef, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void minigllib_glscalef(UInt miniGLLibRef, GLfloat x, GLfloat y, GLfloat z);
void minigllib_gltranslatef(UInt miniGLLibRef, GLfloat x, GLfloat y, GLfloat z);
void minigllib_glpopmatrix(UInt miniGLLibRef);
void minigllib_glpushmatrix(UInt miniGLLibRef);
void minigllib_glmatrixmode(UInt miniGLLibRef, GLenum mode);
void minigllib_glnormal3f(UInt miniGLLibRef, GLfloat nx, GLfloat ny, GLfloat nz);
void minigllib_glenable(UInt miniGLLibRef, GLenum cap);
void minigllib_gldisable(UInt miniGLLibRef, GLenum cap);
void minigllib_gllightfv(UInt miniGLLibRef, GLenum light, GLenum pname, const GLfloat *params);
void minigllib_glgetlightfv(UInt miniGLLibRef, GLenum light, GLenum pname, GLfloat *params);
void minigllib_glpolygonmode(UInt miniGLLibRef, GLenum face, GLenum mode);
void minigllib_winsetglArea(UInt miniGLLibRef, int x1, int y1, int x2, int y2);
void minigllib_glmap2f(UInt miniGLLibRef, GLenum target,
                GLfloat u1, GLfloat u2, GLint ustride, GLint uorder,
                GLfloat v1, GLfloat v2, GLint vstride, GLint vorder,
                const GLfloat *points);
void minigllib_glmapgrid2f(UInt miniGLLibRef, GLint un, GLfloat u1, GLfloat u2,
                GLint vn, GLfloat v1, GLfloat v2);
void minigllib_glevalmesh2(UInt miniGLLibRef, GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
void minigllib_glpushattrib(UInt miniGLLibRef, GLbitfield mask);
void minigllib_glpopattrib(UInt miniGLLibRef);
void minigllib_gldrawpixels(UInt miniGLLibRef, GLsizei width, GLsizei height, GLenum format,
                GLenum type, const GLvoid *pixels);
void minigllib_glbitmap(UInt miniGLLibRef, GLsizei width, GLsizei height, GLfloat xbo,
                GLfloat ybo, GLfloat xbi, GLfloat ybi,
                const GLubyte *bitmap);
void minigllib_winremoveglarea(UInt miniGLLibRef);

#endif

/**
 * End of miniGL_main.h
 */
