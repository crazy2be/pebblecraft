/**
 * miniGL.c
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

#include <SysAll.h>
#include "miniGL.h"

/** Library reference returned by SysLibFind() or SysLibLoad() */
UInt miniGLLibRef;

void glMultMatrixf(const GLfloat *m) {
	MultMatrixf(miniGLLibRef, m);
}

void glLoadMatrixf(const GLfloat *m) {
	LoadMatrixf(miniGLLibRef, m);
}

void glLoadIdentity() {
	LoadIdentity(miniGLLibRef);
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
	ClearColor(miniGLLibRef, red, green, blue, alpha);
}

void glClear(GLbitfield mask) {
	Clear(miniGLLibRef, mask);
}

void glColor3f(GLfloat r, GLfloat g, GLfloat b) {
	Color3f(miniGLLibRef, r, g, b);
}

void glColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	Color4f(miniGLLibRef, r, g, b, a);
}
 
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
        GLdouble near, GLdouble distant) {
	Ortho(miniGLLibRef, left, right, bottom, top, near, distant);
}

void gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top) {
	Ortho2D(miniGLLibRef, left, right, bottom, top);
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
	Viewport(miniGLLibRef, x, y, width, height);
}

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble near,
        GLdouble distant) {
	Perspective(miniGLLibRef, fovy, aspect, near, distant);
}

void glBegin(GLenum mode) {
	Begin(miniGLLibRef, mode);
}

void glEnd() {
	End(miniGLLibRef);
}

void glFlush() {
	Flush(miniGLLibRef);
}

void glVertex2f(GLfloat x, GLfloat y) {
	Vertex2f(miniGLLibRef, x, y);
}

void glVertex3f(GLfloat x, GLfloat y, GLfloat z) {
	Vertex3f(miniGLLibRef, x, y, z);
}

void glShadeModel(GLenum mode) {
	ShadeModel(miniGLLibRef, mode);
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
	Rotatef(miniGLLibRef, angle, x, y, z);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z) {
	Scalef(miniGLLibRef, x, y, z);
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z) {
	Translatef(miniGLLibRef, x, y, z);
}

void glPopMatrix() {
	PopMatrix(miniGLLibRef);
}

void glPushMatrix() {
	PushMatrix(miniGLLibRef);
}

void glMatrixMode(GLenum mode) {
	MatrixMode(miniGLLibRef, mode);
}

void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz) {
	Normal3f(miniGLLibRef, nx, ny, nz);
}

void glEnable(GLenum cap) {
	Enable(miniGLLibRef, cap);
}

void glDisable(GLenum cap) {
	Disable(miniGLLibRef, cap);
}

void glLightfv(GLenum light, GLenum pname, const GLfloat *params) {
	Lightfv(miniGLLibRef, light, pname, params);
}

void glGetLightfv(GLenum light, GLenum pname, GLfloat *params) {
	GetLightfv(miniGLLibRef, light, pname, params);
}

void glPolygonMode(GLenum face, GLenum mode) {
	PolygonMode(miniGLLibRef, face, mode);
}

void glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride,
	GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder,
       	const GLfloat *points) {

	Map2f(miniGLLibRef, target, u1, u2, ustride, uorder, v1, v2,
		vstride, vorder, points);
}

void glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1,
		GLfloat v2) {
	MapGrid2f(miniGLLibRef, un, u1, u2, vn, v1, v2);
}

void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) {
	EvalMesh2(miniGLLibRef, mode, i1, i2, j1, j2);
}

void glPushAttrib(GLbitfield mask) {
	PushAttrib(miniGLLibRef, mask);
}

void glPopAttrib() {
	PopAttrib(miniGLLibRef);
}

void glDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid *pixels) {
	DrawPixels(miniGLLibRef, width, height, format, type, pixels);
}

void glBitmap(GLsizei width, GLsizei height, GLfloat xbo, GLfloat ybo,
		GLfloat xbi, GLfloat ybi, const GLubyte *bitmap) {
	Bitmap(miniGLLibRef, width, height, xbo, ybo, xbi, ybi, bitmap);
}

void WinSetGLArea(int x1, int y1, int x2, int y2) {
	SetGLArea(miniGLLibRef, x1, y1, x2, y2);
}

void WinRemoveGLArea() {
	RemoveGLArea(miniGLLibRef);
}

void glutSwapBuffers() {
	SwapBuffers(miniGLLibRef);
}

/**
 * End of miniGL.c 
 */
