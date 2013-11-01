/**
 * miniGL_main.c
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

//#define PRINT_FPS 1
#define W 1
#define EPSILON 0.02

#include <SysAll.h>
#include "miniGL.h"
#include "miniGL_main.h"
#include "miniGL_lib.h"
#include "fastgraph.h"

typedef float fixed;

#define ITF(x) ((float)(x))
#define FTI(x) ((int)(x))

fixed DTF(float f) {
	return f;
}

float FTD(fixed fix) {
	return fix;
}

fixed fixmul(fixed a, fixed b) {
	return a * b;
}

fixed fixdiv(fixed a, fixed b) {
	return a / b;
}

/**
 * Mutiply a 4x4 matrix by a 4x1 vector to result in a 4x1 vector.
 *
 * m - input 4x4 matrix
 * p - input 4x1 vector
 * ret - output 4x1 matrix
 */
void MatrixMultVector(const fixed *m, const GLfloat *p, GLfloat *ret) {
	GLfloat p0,p1,p2,p3;
	
	p0 = p[0];
	p1 = p[1];
	p2 = p[2];
	p3 = p[3];
	
	ret[0] = m[0] * p0 + m[4] * p1 + m[8] * p2 + m[12] * p3;
	ret[1] = m[1] * p0 + m[5] * p1 + m[9] * p2 + m[13] * p3;
	ret[2] = m[2] * p0 + m[6] * p1 + m[10] * p2 + m[14] * p3;
	ret[3] = m[3] * p0 + m[7] * p1 + m[11] * p2 + m[15] * p3;
}

/**
 * v = p1 - p0
 */
void VectorMinusVector(const GLfloat *p1, const GLfloat *p0, GLfloat *v) {
	int i;

	for (i=0;i<4;i++) {
		v[i] = p1[i] - p0[i];
	}
}

/**
 * returns the dot product of vectors v1 and v2.
 */
GLfloat VectorDotVector(const GLfloat *v1, const GLfloat *v2) {
	GLfloat answer;
	
	answer = v1[0]*v2[0];
	answer += v1[1]*v2[1];
	answer += v1[2]*v2[2];
	answer += v1[3]*v2[3];

	return answer;
}

void VectorNormalize(GLfloat *v) {
	float l_sqr = v[0]*v[0] +
		v[1]*v[1] +
		v[2]*v[2] +
		v[3]*v[3];
	float l_inv = 1/Sqrt(l_sqr);

	v[0] = v[0]*l_inv;
	v[1] = v[1]*l_inv;
	v[2] = v[2]*l_inv;
	v[3] = v[3]*l_inv;
}

/**
 * cross = c0 x c1 (cross product).
 */
void VectorCrossVector(const GLfloat *v1, const GLfloat *v2, GLfloat *cross) {
	cross[0] = v1[1]*v2[2] - v2[1]*v1[2];
	cross[1] = v1[2]*v2[0] - v2[2]*v1[0];
	cross[2] = v1[0]*v2[1] - v2[0]*v1[1];
	cross[3] = 1;
}

/**
 * Initialize a 4x4 matrix to the identity matrix (all zeroes except ones in
 * the diagonal).
 *
 * m - input 4x4 matrix 
 */
void InitializeMatrix(fixed *m) {
	int i;

	for (i=0;i<16;i++) 
		m[i] = 0.0;

	m[0] = 1;
	m[5] = 1;
	m[10] = 1;
	m[15] = 1;
}

void MatrixToFixed(const GLfloat* d, fixed* f) {
	int i;

	for (i=0; i<16; i++)
		f[i] = DTF(d[i]);
}

void minigllib_glmultmatrixf(UInt miniGLLibRef, const GLfloat *input) {
        GLfloat result[16];
        const GLfloat *c;
        GLfloat *m;
        int i,j;
	miniGLLib_globals *gl;

        c = input;
	gl = miniGLLockGlobals(miniGLLibRef);
        m = gl->cur_matrix;

        /** 4x4 matrix multiplication */
        for (j=0;j<4;j++) {
                result[0+j] = c[0+j]*m[0] + c[4+j]*m[1]
                                        + c[8+j]*m[2] + c[12+j]*m[3];
                result[4+j] = c[0+j]*m[4] + c[4+j]*m[5]
                                        + c[8+j]*m[6] + c[12+j]*m[7];
                result[8+j] = c[0+j]*m[8] + c[4+j]*m[9]
                                        + c[8+j]*m[10] + c[12+j]*m[11];
                result[12+j] = c[0+j]*m[12] + c[4+j]*m[13]
                                        + c[8+j]*m[14] + c[12+j]*m[15];
        }

        for (i=0;i<16;i++) {
                m[i] = result[i];
        }
	miniGLUnlockGlobals(gl);
}

/**
 * Loads the input matrix m into the current matrix, overwriting any values
 * stored in the current matrix.
 *
 * m - input 4x4 matrix
 */
void minigllib_glloadmatrixf(UInt miniGLLibRef, const GLfloat *m) {
        int i;
	GLfloat *c;
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);
	c = gl->cur_matrix;

        for (i=0;i<16;i++) {
                c[i] = m[i];
        }
	miniGLUnlockGlobals(gl);
}

/**
 * The gl call to load in the identity matrix as the current geometry transform
 * matrix.  Same as InitializeMatrix(...) but uses the gl function name.
 */
void minigllib_glloadidentity(UInt miniGLLibRef) {
	miniGLLib_globals *gl;
	fixed *m;

	gl = miniGLLockGlobals(miniGLLibRef);
	m = gl->cur_matrix;

	if (m == NULL)
		return;

	InitializeMatrix(gl->cur_matrix);

	miniGLUnlockGlobals(gl);
}

/**
 * Doesn't do anything right now since there are no colors.
 */
void minigllib_glclearcolor(UInt miniGLLibRef, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
}

/**
 * Inititalize z-buffer.
 */
#ifdef ZBUFFER
void ZBufferInit() {
	int j;

	/** create z-buffer */
	/** Only use scan-line zbuffer to save heap space */
	zbuffer = (GLubyte *) MemChunkNew(MemHeapID(0, 0), 
				MAX_SCREEN_WIDTH, // * MAX_SCREEN_HEIGHT,
				memNewChunkFlagNonMovable);
				/** remember to clear this */
	
	/** initialize to deepest values */
	for (j=0;j<160;j++) {
		zbuffer[j] = 0xff; // set to largest (deepest) value
	}
}
#endif

/**
 * Clears the gl viewport as specified through the glViewport(...) call.  
 * Basically draws a "white" rectangle over the whole region to clear it.
 *
 * mask - buffer to clear 
 */
void minigllib_glclear(UInt miniGLLibRef, GLbitfield mask) {
	Err err;
	GLint i,j;
	miniGLLib_globals *gl;

	if (mask != GL_COLOR)
		return; /** anything else not supported now */

	gl = miniGLLockGlobals(miniGLLibRef);
	WinSetDrawWindow(gl->two);
	fgClearWindow(gl->screen_startx, gl->screen_starty,
			gl->screen_width, gl->screen_height);
	WinSetDrawWindow(gl->one);
	miniGLUnlockGlobals(gl);
}

/**
 * Set the current color to be the RGB value (r,g,b) with an alpha value of 1.
 *
 * r - red [0,1]
 * g - green [0,1]
 * b - blue [0,1]
 */
void minigllib_glcolor3f(UInt miniGLLibRef, GLfloat r, GLfloat g, GLfloat b) {
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);
	gl->cur_color[0] = r;
	gl->cur_color[1] = g;
	gl->cur_color[2] = b;
	gl->cur_color[3] = 1.0;
	miniGLUnlockGlobals(gl);

	fgSetColor(r,g,b);
}

/**
 * Set the current color to be the RGB value (r,g,b) with an alpha value of a.
 *
 * r - red [0,1]
 * g - green [0,1]
 * b - blue [0,1]
 * a - alpha transparency [0,1]
 */
void minigllib_glcolor4f(UInt miniGLLibRef, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);
	gl->cur_color[0] = r;
	gl->cur_color[1] = g;
	gl->cur_color[2] = b;
	gl->cur_color[3] = a;
	miniGLUnlockGlobals(gl);

	fgSetColor(r,g,b);
}

/**
 * Sets up the perspective projection matrix and screen coord translation 
 * matrix to be an orthographic projection (no perspective).
 *
 * left - world x coordinate of left edge of projection plane
 * right - world x coordinate of right edge of projection plane
 * bottom - world y coordinate of bottom edge of projection plane
 * top - world y coordinate of top edge of projection plane
 * near - world z coordinate of distance from camera to projection plane
 * distant - back clipping plane
 */
void minigllib_glortho(UInt miniGLLibRef, GLdouble left, GLdouble right,
		GLdouble bottom, GLdouble top, GLdouble near,
		GLdouble distant) {
	float d;
	miniGLLib_globals *gl;

	GLfloat width = right - left;
	GLfloat height = top - bottom;

	gl = miniGLLockGlobals(miniGLLibRef);

	d = 32000.0;
	InitializeMatrix(gl->per_matrix);
        gl->per_matrix[10] = 0;
        gl->per_matrix[11] = (1.0/d);

	InitializeMatrix(gl->scr_matrix);
	gl->scr_matrix[0] = DTF((float)(gl->screen_width) / width);
	gl->scr_matrix[5] = -DTF((float)(gl->screen_height) / height);
	gl->scr_matrix[10] = 0;
	gl->scr_matrix[12] = ITF(gl->screen_startx) + ITF(gl->screen_width)/2;
	gl->scr_matrix[13] = ITF(gl->screen_starty) + ITF(gl->screen_height)/2;

	miniGLUnlockGlobals(gl);
}

/**
 * Sets up the perspective projection matrix and screen coord translation 
 * matrix to be a 2D orthographic projection (no perspective).  Distance from
 * camera to projection plane tends to infinity.
 *
 * left - world x coordinate of left edge of projection plane
 * right - world x coordinate of right edge of projection plane
 * bottom - world y coordinate of bottom edge of projection plane
 * top - world y coordinate of top edge of projection plane
 */
void minigllib_gluortho2d(UInt miniGLLibRef, GLdouble left, GLdouble right, GLdouble bottom,
		GLdouble top) {
	minigllib_glortho(miniGLLibRef, left, right, bottom, top, -1, 1);
}

/**
 * Sets up the viewport.  Same stuff as WinSetGLArea below.
 */
void minigllib_glviewport(UInt miniGLLibRef, GLint x, GLint y, GLsizei width, GLsizei height) {
	// fill in here.
}

/**
 * Sets up the perspective projection matrix and screen coordinate transform
 * matrix to correctly render perspective projections given a field of view
 * fovy and an aspect ratio aspect (x/y).  Note:  GLU function.
 *
 * fovy - field of view in degrees [0,180]
 * aspect - aspect ratio as width/height (standard television is 4/3)
 * near - distance from camera to projection plane (world coords)
 * distant - back clipping plane world z value
 */
void minigllib_gluperspective(UInt miniGLLibRef, GLdouble fovy,
		GLdouble aspect, GLdouble near, GLdouble distant) {
	fixed *m;
	fixed tmp_mat[16];
	float half_angle;
	float f, height, width;
	miniGLLib_globals *gl;
	
	half_angle = (float)(fovy > 0?fovy:45.0)*PI/360.0;
	f = Cos(half_angle)/Sin(half_angle);
        height = 2*near/f;
	width  = height*(aspect > 0?aspect:1.0);

	gl = miniGLLockGlobals(miniGLLibRef);

	m = gl->cur_matrix;
	if (m == NULL)
		return;

	InitializeMatrix(gl->per_matrix);
        (gl->per_matrix)[10] = 0;
        (gl->per_matrix)[11] = (1.0/near);

	InitializeMatrix(gl->scr_matrix);
	(gl->scr_matrix)[0] = DTF((float)(gl->screen_width) / width);
	(gl->scr_matrix)[5] = -DTF((float)(gl->screen_height) / height);
	(gl->scr_matrix)[10] = 0;
	(gl->scr_matrix)[12] = ITF(gl->screen_startx) + ITF(gl->screen_width)/2;
	(gl->scr_matrix)[13] = ITF(gl->screen_starty) + ITF(gl->screen_height)/2;

	miniGLUnlockGlobals(gl);
}

/**
 * Begin a graphics primitive.
 */
void minigllib_glbegin(UInt miniGLLibRef, GLenum mode) {
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);

	gl->num_vertices = 0;
	gl->cur_mode = mode;

	miniGLUnlockGlobals(gl);
}

/**
 * Special function for BackFacing
 */
void MatrixMultVector2D(UInt miniGLLibRef, const fixed *m, const GLfloat *p, GLfloat *ret) {
	float p0, p1, p2, p3;

	p0 = p[0];
	p1 = p[1];
	p2 = p[2];
	p3 = p[3];

	ret[0] = FTD(fixmul(m[0], p0) + fixmul(m[4], p1) +
		fixmul(m[8], p2) + fixmul(m[12], p3));
	ret[1] = FTD(fixmul(m[1], p0) + fixmul(m[5], p1) +
		fixmul(m[9], p2) + fixmul(m[13], p3));
	ret[3] = FTD(fixmul(m[3], p0) + fixmul(m[7], p1) +
		fixmul(m[11], p2) + fixmul(m[15], p3));
}

/**
 * Check if it's facing away from the camera in order to determine whether or
 * not to draw it if culling is turned on.
 */
int BackFacing(int l, int m, int n) {
	miniGLLib_globals *gl;
	GLfloat *q0;
	GLfloat *q1;
	GLfloat *q2;
	const fixed c00;
	const fixed c01;
	const fixed c10;
	const fixed c11;

	gl =  miniGLLockGlobals(miniGLLibRef);

	q0 = (gl->scr_vertices)[l];
	q1 = (gl->scr_vertices)[m];
	q2 = (gl->scr_vertices)[n];

	/** Then use cross product to find direction it's facing */
	/*VectorMinusVector(q1, q0, c0);
	VectorMinusVector(q2, q0, c1);*/
	/** Reduce size to keep from overflowing (/8) */
	c00 = (DTF(q1[0]) - DTF(q0[0]))/8;
	c01 = (DTF(q1[1]) - DTF(q0[1]))/8;
	c10 = (DTF(q2[0]) - DTF(q0[0]))/8;
	c11 = (DTF(q2[1]) - DTF(q0[1]))/8;
	/** Partial cross product replaces VectorCrossVector */ 	
	
	miniGLUnlockGlobals(gl);

	/** Return (cross > 0) */
	return fixmul(c00, c11) > fixmul(c10, c01);
}               

void DoLightingCalc(GLfloat pos[4], GLfloat normal[4], 
				RGBColorType *color1) {
	GLfloat blaa[4];
	GLfloat cosangle;
	miniGLLib_globals *gl;
	int i;

	gl = miniGLLockGlobals(miniGLLibRef);

	if (!gl->lighting) {
		color1->r = gl->cur_color[0];
		color1->g = gl->cur_color[1];
		color1->b = gl->cur_color[2];

		miniGLUnlockGlobals(gl);

		return;
	}
	
	for (i=0;i<8;i++) { /** Go through all eight lights */
		if (gl->lights[i].enabled == 0)
			break;

		VectorMinusVector(gl->lights[i].position,
				pos, blaa);
		VectorNormalize(normal);
		VectorNormalize(blaa);
		cosangle = VectorDotVector(normal, blaa);
		if (cosangle < 0) {
			color1->r = gl->cur_color[0] 
		 		 * gl->lights[i].ambient[0];
			color1->g = gl->cur_color[1] 
				  * gl->lights[i].ambient[1];
			color1->b = gl->cur_color[2] 
				  * gl->lights[i].ambient[2];
		} else {
			color1->r = gl->cur_color[0] 
				  * (gl->lights[i].ambient[0] 
				  + (gl->lights[i].diffuse[0]
				  * cosangle));
			color1->g = gl->cur_color[1] 
				  * (gl->lights[i].ambient[1]
				  + (gl->lights[i].diffuse[1]
				  * cosangle));
			color1->b = gl->cur_color[2] 
				  * (gl->lights[i].ambient[2]
				  + (gl->lights[i].diffuse[2]
				  * cosangle));
		}
	}

	miniGLUnlockGlobals(gl);
}

/**
 * Draw a line using the z-buffer and shading.
 */
void DrawScanLine(GLfloat *start, GLfloat *end, GLfloat *startnormal,
			GLfloat *endnormal, int mode) {
	RGBColorType color1, color2, color;
	int ty, tx, tz;
	int i,j;
	GLfloat temp[4], normal[4];

#ifdef ZBUFFER
	/**
	 * Do bidness here to draw point by point; test and set z-buffer 
	 * along the way.  Put shading calcs in here, too, by interpolating
	 * the normal/intensities across the line.  Add in a beginning value
	 * and ending value to the two passed points to interpolate.  If
	 * doing Phong shading, interpolate normal (expensive!) or for now
	 * just do Gouraud shading by interpolating color values given at
	 * each end of the lines.
	 */

	temp[3] = 1;
	temp[1] = start[2];
	
	if (mode == GOURAUD) {
		DoLightingCalc(start, startnormal, &color1);
		DoLightingCalc(end, endnormal, &color2);
	}

	for (j=end[0];j<=start[0];j++) {
		if (mode == PHONG) {
			temp[0] = j;
			temp[2] = start[3]; // interpolated z

			/** Change normal for Phong shading */
			for (i=0;i<4;i++) {
				//normal[i] = normal[i];
			}

			/** Do lighting calc */
			DoLightingCalc(temp, normal, &color);
		} else {
		    /** interpolate between color1 and color2 */
		    color.r = ((j-start[0])/(end[0]-start[0]))*color2.r 
			+ (1-(j-start[0])/(end[0]-start[0]))*color1.r;
		    color.g = ((j-start[0])/(end[0]-start[0]))*color2.g 
			+ (1-(j-start[0])/(end[0]-start[0]))*color1.g;
		    color.b = ((j-start[0])/(end[0]-start[0]))*color2.b 
			+ (1-(j-start[0])/(end[0]-start[0]))*color1.b;
		}

		fgSetColor(color.r, color.g, color.b);
		// if (z bidness)
		//	do stuff

		fgDrawPixel(j,start[1]);
	}
#else
	/** Just draw the 2D scan line */
	fgDrawLine(start[0], start[1], end[0], end[1]);
#endif
}

void SetColor() {
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);

	fgSetColor(gl->cur_color[0], gl->cur_color[1], gl->cur_color[2]);

	miniGLUnlockGlobals(gl);
}

void TransformToScreen(const GLfloat* in, GLfloat* ret) {
	float *m;
	float p0, p1, p2, p3;
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);
 	m = gl->scr_matrix;
	p0 = in[0];
	p1 = in[1];
	p2 = in[2];
	p3 = in[3];

	ret[0] = FTD(fixmul(m[0], p0) + fixmul(m[4], p1) +
		fixmul(m[8], p2) + fixmul(m[12], p3));
	ret[1] = FTD(fixmul(m[1], p0) + fixmul(m[5], p1) +
		fixmul(m[9], p2) + fixmul(m[13], p3));
	ret[2] = FTD(fixmul(m[2], p0) + fixmul(m[6], p1) +
		fixmul(m[10], p2) + fixmul(m[14], p3));

	miniGLUnlockGlobals(gl);
}

/**
 * End a graphics primitive and draw to the buffer.
 */
void minigllib_glend(UInt miniGLLibRef) {
	int i, sx1, sx2, sy1, sy2, j, oldtopy, oldbottomy, oldi, oldj,
		o, n, y, tempx, tempy, q;
	float x1, y1, x2, y2, cosangle;
	GLfloat  p1[4], in1[4], out1[4], normal[4], start[4],
			end[4];
	RGBColorType color1, color2;
	Line I, II;
	miniGLLib_globals *gl;
	char foo[20], bar[20];

	gl = miniGLLockGlobals(miniGLLibRef);

	if (gl->num_vertices < 2)
		return;

	StrCopy(foo, "num_vertices = ");
	StrIToA(bar, gl->num_vertices);
	StrCat(foo, bar);
	ErrFatalDisplayIf(true, foo);

	WinSetDrawWindow(gl->two);
	WinSetActiveWindow(gl->one);

	/** Translate vertices to screen coords */
	for(i=0; i < gl->num_vertices; i++) {
		/** Geometric transform */
		//MatrixMultVector(modv_matrix[modv_level], vertices[i], p1);
		MatrixMultVector(gl->cur_matrix, gl->vertices[i], p1);

		/** Perspective transform */
		//MatrixMultVector(proj_matrix[proj_level], p1, in1);
		MatrixMultVector(gl->per_matrix, p1, in1);

		/** Go from 4D back to 3D */
		for(j=0;j<4;j++) {
			in1[j] = in1[j]/in1[3];
		}

		/** Translate into screen coords */
		//TransformToScreen(in1, scr_vertices[i]);

                MatrixMultVector(gl->scr_matrix, in1, out1);

                /** Add the point to the final list */
                for(j=0;j<4;j++) {
                        gl->scr_vertices[i][j] = out1[j];
                }

	}
	//MatrixMultVector(modv_matrix[modv_level], cur_normal, normal);
	MatrixMultVector(gl->cur_matrix, gl->cur_normal, normal);
	
	switch(gl->cur_mode) {
		case GL_TRIANGLES:
			if (gl->num_vertices < 3)
				break;

			SetColor();

			j = 0;
			while ((j+2) < gl->num_vertices) {
				if (gl->culling && !BackFacing(j,j+1,j+2)) {
					for (o=j+1;o<(j+3);o++) {
						fgDrawLine(
							gl->scr_vertices[o-1][0],
							gl->scr_vertices[o-1][1],
							gl->scr_vertices[o][0],
							gl->scr_vertices[o][1]);
					}
					fgDrawLine(
						gl->scr_vertices[o-1][0],
						gl->scr_vertices[o-1][1],
						gl->scr_vertices[j][0],
						gl->scr_vertices[j][1]);
				}
				j+=3;
			}
			break;

		case GL_TRIANGLE_STRIP:
			if (gl->num_vertices < 3)
				break;

			SetColor();

			/** Draw first line between verts 0 and 1 */
			fgDrawLine( 
				gl->scr_vertices[0][0],
				gl->scr_vertices[0][1],
				gl->scr_vertices[1][0],
				gl->scr_vertices[1][1]);

			/** Need to add culling support */

			o = 2;
			/** For each new vertex, draw two new triangle legs */
			while (o < gl->num_vertices) {
				
				fgDrawLine( 
					gl->scr_vertices[o][0],
					gl->scr_vertices[o][1],
					gl->scr_vertices[o-1][0],
					gl->scr_vertices[o-1][1]);
				fgDrawLine(
					gl->scr_vertices[o][0],
					gl->scr_vertices[o][1],
					gl->scr_vertices[o-2][0],
					gl->scr_vertices[o-2][1]);

				o++;
			}
			break;

		case GL_TRIANGLE_FAN:
			if (gl->num_vertices < 3)
				break;

			SetColor();

			for(i=1; i < gl->num_vertices; i++) {
				if (gl->culling && !BackFacing(0,i-1,i)) {
                                	fgDrawLine(gl->scr_vertices[0][0],
						gl->scr_vertices[0][1],
						gl->scr_vertices[0][2],
						gl->scr_vertices[i][1]);
					fgDrawLine(gl->scr_vertices[i-1][0],
						gl->scr_vertices[i-1][1],
						gl->scr_vertices[i][0],
						gl->scr_vertices[i][1]);
				}
			}
			break;

		case GL_QUADS:
			if (gl->num_vertices < 4)
				break;

			SetColor();

			j = 0;
			while ((j+3) < gl->num_vertices) {

				if (gl->culling && !BackFacing(j,j+1,j+2)) {

				// draw bidness with j, j+1, j+2, j+3 vertices
					for (o=j+1;o<(j+4);o++) {
						fgDrawLine(
							gl->scr_vertices[o-1][0],
							gl->scr_vertices[o-1][1],
							gl->scr_vertices[o][0],
							gl->scr_vertices[o][1]);
					}
					fgDrawLine(
						gl->scr_vertices[o-1][0],
						gl->scr_vertices[o-1][1],
						gl->scr_vertices[j][0],
						gl->scr_vertices[j][1]);
				}
				j+=4;
			}
			break;

		case GL_QUAD_STRIP:
			SetColor();

			/** Do this */
			break;

		case GL_LINE_LOOP:
			SetColor();

			if (gl->num_vertices < 1)
				break;

			for(i=1; i < gl->num_vertices; i++) {
                                fgDrawLine(gl->scr_vertices[i-1][0],
					gl->scr_vertices[i-1][1],
					gl->scr_vertices[i][0],
					gl->scr_vertices[i][1]);
			}
			/** Close the loop */
			fgDrawLine(gl->scr_vertices[i-1][0],
				gl->scr_vertices[i-1][1],
				gl->scr_vertices[0][0],
				gl->scr_vertices[0][1]);
			break;

		case GL_LINES:
			SetColor();

			if (gl->num_vertices < 1)
				break;

			for(i=1; i < gl->num_vertices; i+=2) {
                                fgDrawLine(gl->scr_vertices[i-1][0],
					gl->scr_vertices[i-1][1],
					gl->scr_vertices[i][0],
					gl->scr_vertices[i][1]);
			}
			break;

		case GL_POINTS:
			SetColor();

			for(i=0; i < gl->num_vertices; i++) {
				fgDrawLine(gl->scr_vertices[i][0],
					gl->scr_vertices[i][1],
					gl->scr_vertices[i][0],
					gl->scr_vertices[i][1]);
			}
			break;

		case GL_LINE_STRIP:
			SetColor();

			for(i=1; i < gl->num_vertices; i++) {
                                fgDrawLine(gl->scr_vertices[i-1][0],
					gl->scr_vertices[i-1][1],
					gl->scr_vertices[i][0],
					gl->scr_vertices[i][1]);
			}
			break;

		/** Right now only GL_POLYGON is floodfilled and lit */
		case GL_POLYGON:
			if (gl->num_vertices < 3)
				break;

			if (gl->culling && BackFacing(0,1,2)) {
				break;
			}
			
			/*
			 * For now, floodfill is only applied to GL_POLYGON
			 * type for testing.
			 */

			if (!gl->wireframe) {
			/** lighting calculation to get color */
			DoLightingCalc(gl->vertices[0], normal, &color1);
			fgSetColor(color1.r, color1.g, color1.b);

			/** find "highest" and "lowest" points in y dir */
			oldtopy = -1000;
			oldbottomy = 1000;
			for (i=0;i<gl->num_vertices;i++) {
				if (gl->scr_vertices[i][1] > oldtopy) {
					oldtopy = gl->scr_vertices[i][1];
					oldi = i;
				}
				if (gl->scr_vertices[i][1] < oldbottomy) {
					oldbottomy = gl->scr_vertices[i][1];
				}
			}

			/** Flood fill polygon */
			n = gl->num_vertices;

			/** Set first set of boundary lines */
			I.start = (oldi) % n;
			I.end = (oldi+1) % n;
			I.x1 = gl->scr_vertices[I.start][0];
			I.y1 = gl->scr_vertices[I.start][1];
			I.z1 = gl->scr_vertices[I.start][2];
			I.m = (GLfloat)(gl->scr_vertices[I.end][1]-I.y1) 
			    / (GLfloat)(gl->scr_vertices[I.end][0]-I.x1);
			I.mz = (GLfloat)(gl->scr_vertices[I.end][1]-I.y1) 
			    / (GLfloat)(gl->scr_vertices[I.end][2]-I.z1);

			II.start = (oldi) % n;
			II.end = (oldi+n-1) % n;
			II.x1 = gl->scr_vertices[II.start][0];
			II.y1 = gl->scr_vertices[II.start][1];
			II.z1 = gl->scr_vertices[II.start][2];
			II.m = (GLfloat)(gl->scr_vertices[II.end][1]-II.y1)
			     / (GLfloat)(gl->scr_vertices[II.end][0]-II.x1);
			II.mz = (GLfloat)(gl->scr_vertices[II.end][1]-II.y1)
			     / (GLfloat)(gl->scr_vertices[II.end][2]-II.z1);
			
			/** Loop over all y values between "top" and "bottom" */
			for (y = oldtopy-1; y > oldbottomy; y--) {
				/** Draw one scanline */
				if (I.m > -EPSILON && I.m < EPSILON) {
					start[0] = gl->scr_vertices[I.end][0];
					start[2] = gl->scr_vertices[I.end][2];
				} else {
					start[0] = I.x1 
					       + (int)((GLfloat)(y-I.y1)/I.m);
					start[2] = I.z1 
					       + (int)((GLfloat)(y-I.y1)/I.mz);
				}
				if (II.m > -EPSILON && II.m < EPSILON) {
					end[0] = gl->scr_vertices[II.end][0];
					end[2] = gl->scr_vertices[II.end][2];
				} else {
					end[0] = II.x1 
					     + (int)((GLfloat)(y-II.y1)/II.m);
					end[2] = II.z1 
					     + (int)((GLfloat)(y-II.y1)/II.mz);
				}
				start[1] = y;
				start[3] = 1;
				end[1] = y;
				end[3] = 1;
				DrawScanLine(start, end, normal, normal, 
						GOURAUD);

				/** Check if the boundary lines should change */
				tempy = gl->scr_vertices[I.end][1];
				if (y>=tempy-W && y<=tempy+W) {
					/** change I */
					oldi++;
					I.start = I.end;
					I.end = (I.end+1) % n;
					I.x1 = gl->scr_vertices[I.start][0];
					I.y1 = gl->scr_vertices[I.start][1];
					I.m = (GLfloat)
						(gl->scr_vertices[I.end][1]-I.y1)/
						(GLfloat)
						(gl->scr_vertices[I.end][0]-I.x1);
				} 

				tempy = gl->scr_vertices[II.end][1];
				if (y>=tempy-W && y<=tempy+W) {
					/** change II */
					II.start = II.end;
					II.end = (II.end+n-1) % n;
					II.x1 = gl->scr_vertices[II.start][0];
					II.y1 = gl->scr_vertices[II.start][1];
					II.m = (GLfloat) (gl->scr_vertices[II.end][1]-II.y1)/
						(GLfloat) (gl->scr_vertices[II.end][0]-II.x1);
				}
			}
			}
	
			/** Draw black edge lines */
			if (gl->wireframe) {
				fgSetColor(0,0,0);
				for (i=1;i<gl->num_vertices;i++) {
					fgDrawLine(gl->scr_vertices[i-1][0],
						gl->scr_vertices[i-1][1],
						gl->scr_vertices[i][0],
						gl->scr_vertices[i][1]);
				}
				fgDrawLine(gl->scr_vertices[i-1][0],
					gl->scr_vertices[i-1][1],
					gl->scr_vertices[0][0],
					gl->scr_vertices[0][1]);
			}
			break;

		default:
			break;
	}
	gl->num_vertices = 0;

	//fgSetColor(0,0,0);
	//fgDrawLine(0,0,160,160);

	WinSetDrawWindow(gl->one);
	WinSetActiveWindow(gl->one);

	miniGLUnlockGlobals(gl);
}

/**
 * Copies the back buffer contents into the screen bitmap area to display the
 * drawn primitives.  Only copies the area as set in WinSetGLArea(...).
 */
void minigllib_glutswapbuffers(UInt miniGLLibRef) {
	minigllib_glflush(miniGLLibRef);
}

/**
 * For now does the same as glutSwapBuffers() above.
 */
void minigllib_glflush(UInt miniGLLibRef) {
	RectangleType rect;
	RGBColorType color1, color2;
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);

#ifdef PRINT_FPS
	ULong t;
	ULong old_t;
	int count_time=0;
	int fps;
	int framecount;
	char text[30];
#endif

	rect.topLeft.x = gl->screen_startx;
	rect.topLeft.y = gl->screen_starty;
	rect.extent.x = gl->screen_width;
	rect.extent.y = gl->screen_height;

#ifdef PRINT_FPS
	/** Don't count time spent swapping buffers now. */
	t = TimGetTicks();
#endif
	if (gl->two) {
		WinCopyRectangle(gl->two, gl->one, &rect, 
				gl->screen_startx, gl->screen_starty, scrCopy);
	}


	fgSetColor(100,100,100);

#ifdef PRINT_FPS
	framecount++;
	if (t - old_t > 0 && count_time) {
		//fps = framecount;
		fps = (t - old_t)*1000/SysTicksPerSecond();
		StrIToA(text, fps);
		StrCat(text, " mspf    ");
		WinDrawChars(text, StrLen(text), 100, 0);
		framecount=0;
		old_t = t;
	} else if (!count_time) {
		old_t = t;
		count_time = 1;
	}
#endif

	fgSetColor(0,0,0);

	miniGLUnlockGlobals(gl);
}

/**
 * 
 */
void minigllib_glvertex3f(UInt miniGLLibRef, GLfloat x, GLfloat y, GLfloat z) {
	int i;
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);

	if (gl->num_vertices < MAX_VERTICES) {
		gl->vertices[gl->num_vertices][0] = x;
		gl->vertices[gl->num_vertices][1] = y;
		gl->vertices[gl->num_vertices][2] = z;
		gl->vertices[gl->num_vertices][3] = 1.0;

		for (i=0;i<4;i++) {
			gl->vertices_color[gl->num_vertices][i] 
				= gl->cur_color[i];
			gl->vertices_normal[gl->num_vertices][i] 
				= gl->cur_normal[i];
		}

		gl->num_vertices++;
	}

	miniGLUnlockGlobals(gl);
}

/**
 *
 */
void minigllib_glvertex2f(UInt miniGLLibRef, GLfloat x, GLfloat y) {
        minigllib_glvertex3f(miniGLLibRef, x, y, 0.0);
}

/**
 * 
 */
void minigllib_glshademodel(UInt miniGLLibRef, GLenum mode) {
}

/**
 * 
 */
void minigllib_glrotatef(UInt miniGLLibRef, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
        GLfloat t[16];
        double theta;

	theta = (angle/180.0) * PI;

        InitializeMatrix(t);

        if (z == 1) {
                t[0] = Cos(theta);
                t[4] = -Sin(theta);
                t[1] = Sin(theta);
                t[5] = Cos(theta);
                t[10] = 1;
                t[15] = 1;
        } else if (y == 1) {
                t[0] = Cos(theta);
                t[2] = -Sin(theta);
                t[8] = Sin(theta);
                t[10] = Cos(theta);
                t[5] = 1;
                t[15] = 1;
        } else if (x == 1) {
                t[5] = Cos(theta);
                t[6] = Sin(theta);
                t[9] = -Sin(theta);
                t[10] = Cos(theta);
                t[0] = 1;
                t[15] = 1;
        }

        minigllib_glmultmatrixf(miniGLLibRef,t);
}

/**
 * Constructs a scaling matrix from the given input parameters and multiply
 * the current geometry matrix by that scaling matrix.
 *
 * x,y,z - values to scale in the x, y, and z directions
 */
void minigllib_glscalef(UInt miniGLLibRef, GLfloat x, GLfloat y, GLfloat z) {
	fixed *m;
	fixed t[16];
	miniGLLib_globals *gl;
	
	gl = miniGLLockGlobals(miniGLLibRef);

	m = gl->cur_matrix;
	if (m == NULL)
		return;
	
	InitializeMatrix(t);
	t[0] = x;
	t[5] = y;
	t[10] = z;

	minigllib_glmultmatrixf(miniGLLibRef, t);

	miniGLUnlockGlobals(gl);
}

/** 
 * Translate the origin by x,y,z by constructing a translation 4x4 matrix and
 * multiplying the current geometry matrix by that new matrix.
 *
 * x, y, z - units to translate in 3D world space
 */
void minigllib_gltranslatef(UInt miniGLLibRef, GLfloat x, GLfloat y, GLfloat z) {
	fixed *m; 
	fixed fx, fy, fz; 
	miniGLLib_globals *gl;
	
	fx = DTF(x);
	fy = DTF(y);
	fz = DTF(z);

	gl = miniGLLockGlobals(miniGLLibRef);
	m = gl->cur_matrix;
	if (m == NULL)
		return;
	
	/** Optimized for speed */
	/** 4x4 matrix multiplication */
	m[0] += fixmul(fx, m[3]);
	m[1] += fixmul(fy, m[3]);
	m[2] += fixmul(fz, m[3]);
	m[4] += fixmul(fx, m[7]);
	m[5] += fixmul(fy, m[7]);
	m[6] += fixmul(fz, m[7]);
	m[8] += fixmul(fx, m[11]);
	m[9] += fixmul(fy, m[11]);
	m[10] += fixmul(fz, m[11]);
	m[12] += fixmul(fx, m[15]);
	m[13] += fixmul(fy, m[15]);
	m[14] += fixmul(fz, m[15]);

	miniGLUnlockGlobals(gl);
}

void minigllib_glpopmatrix(UInt miniGLLibRef) {
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);

	switch (gl->matrix_mode) {
		case GL_MODELVIEW:
			/** Error to pop an empty stack */
			if (gl->modv_level <= 0)
				return;
			gl->modv_level--;
			//cur_matrix = gl->modv_matrix[gl->modv_level];
			break;
		case GL_PROJECTION:
			/** Error to pop an empty stack */
			if (gl->proj_level <= 0)
				return;
			gl->proj_level--;
			//cur_matrix = gl->proj_matrix[gl->proj_level];
			break;
	}

	miniGLUnlockGlobals(gl);
}

void CopyMatrixFTF(const fixed* src, fixed* dst) {
	char i;

	for (i=0; i < 16; i++)
		dst[i] = src[i];
}

void minigllib_glpushmatrix(UInt miniGLLibRef) {
	miniGLLib_globals *gl;

	gl = miniGLLockGlobals(miniGLLibRef);

	switch (gl->matrix_mode) {
		case GL_MODELVIEW:
			/** Error to pop an empty stack */
			if (gl->modv_level >= (MAX_MAT_STACK_MODV - 1))
				return;
			CopyMatrixFTF(gl->modv_matrix[gl->modv_level],
				gl->modv_matrix[gl->modv_level + 1]);
			gl->modv_level++;
			//cur_matrix = gl->modv_matrix[gl->modv_level];
			break;
		case GL_PROJECTION:
			/** Error to pop an empty stack */
			if (gl->proj_level >= (MAX_MAT_STACK_PROJ - 1))
				return;
			CopyMatrixFTF(gl->proj_matrix[gl->proj_level],
				gl->proj_matrix[gl->proj_level + 1]);
			gl->proj_level++;
			//cur_matrix = gl->proj_matrix[gl->proj_level];
			break;
	}

	miniGLUnlockGlobals(gl);
}

void minigllib_glmatrixmode(UInt miniGLLibRef, GLenum mode) {
	miniGLLib_globals *gl;

        gl = miniGLLockGlobals(miniGLLibRef);

	gl->matrix_mode = mode;
	switch (gl->matrix_mode) {
		case GL_MODELVIEW:
			//cur_matrix = gl->modv_matrix[gl->modv_level];
			break;
		case GL_PROJECTION:
			//cur_matrix = gl->proj_matrix[gl->proj_level];
			break;
		default:
			//cur_matrix = NULL;
	}

	miniGLUnlockGlobals(gl);
}

void minigllib_glnormal3f(UInt miniGLLibRef, GLfloat nx, GLfloat ny, GLfloat nz) {
	miniGLLib_globals *gl;

        gl = miniGLLockGlobals(miniGLLibRef);

	gl->cur_normal[0] = nx;
	gl->cur_normal[1] = ny;
	gl->cur_normal[2] = nz;
	gl->cur_normal[3] = 1;

	miniGLUnlockGlobals(gl);
}

void minigllib_glenable(UInt miniGLLibRef, GLenum cap) {
	miniGLLib_globals *gl;

        gl = miniGLLockGlobals(miniGLLibRef);

	if (cap >= GL_LIGHT0 && cap <= GL_LIGHT7) {
		gl->lights[(cap - GL_LIGHT0)].enabled = 1;
	}

	switch (cap) {
		case GL_CULL_FACE:
			gl->culling = 1;
			break;
		case GL_LIGHTING:
			gl->lighting = 1;
			break;
		default:
			break;
	}

	miniGLUnlockGlobals(gl);
}

void minigllib_gldisable(UInt miniGLLibRef, GLenum cap) {
	miniGLLib_globals *gl;

        gl = miniGLLockGlobals(miniGLLibRef);

	if (cap >= GL_LIGHT0 && cap <= GL_LIGHT7) {
		gl->lights[(cap - GL_LIGHT0)].enabled = 0;
	}

	switch (cap) {
		case GL_CULL_FACE:
			gl->culling = 0;
			break;
		case GL_LIGHTING:
			gl->lighting = 0;
			break;
		default:
			break;
	}

	miniGLUnlockGlobals(gl);
}

/**
 * Sets an attribute of a light.  The light is identified by the GLenum light,
 * the property is selected by pname, and the values set by params.
 */
void minigllib_gllightfv(UInt miniGLLibRef, GLenum light, GLenum pname, const GLfloat *params) {
	int i;
	miniGLLib_globals *gl;

        gl = miniGLLockGlobals(miniGLLibRef);

	if (params == NULL)
		return;

	if (light > GL_LIGHT7 || light < GL_LIGHT0)
		return;

	switch(pname) {
		case GL_AMBIENT:
			for (i=0;i<4;i++) {
				gl->lights[light-GL_LIGHT0].ambient[i] = params[i];
			}
			break;
		case GL_DIFFUSE:
			for (i=0;i<4;i++) {
				gl->lights[light-GL_LIGHT0].diffuse[i] = params[i];
			}
			break;
		case GL_SPECULAR:
			for (i=0;i<4;i++) {
				gl->lights[light-GL_LIGHT0].specular[i]= params[i];
			}
			break;
		case GL_POSITION:
			for (i=0;i<4;i++) {
				gl->lights[light-GL_LIGHT0].position[i]= params[i];
			}
			break;
		default:
			break;
	}

	miniGLUnlockGlobals(gl);
}

/**
 * Sets an attribute of a light.  The light is identified by the GLenum light,
 * the property is selected by pname, and the values set by params.
 */
void minigllib_glgetlightfv(UInt miniGLLibRef, GLenum light, GLenum pname, GLfloat *params) {
	int i;
	miniGLLib_globals *gl;

        gl = miniGLLockGlobals(miniGLLibRef);

	if (params == NULL)
		return;

	if (light > GL_LIGHT7 || light < GL_LIGHT0)
		return;

	switch(pname) {
		case GL_AMBIENT:
			for (i=0;i<4;i++) {
				params[i] = gl->lights[light-GL_LIGHT0].ambient[i];
			}
			break;
		case GL_DIFFUSE:
			for (i=0;i<4;i++) {
				params[i] = gl->lights[light-GL_LIGHT0].diffuse[i];
			}
			break;
		case GL_SPECULAR:
			for (i=0;i<4;i++) {
				params[i] = gl->lights[light-GL_LIGHT0].specular[i];
			}
			break;
		case GL_POSITION:
			for (i=0;i<4;i++) {
				params[i] = gl->lights[light-GL_LIGHT0].position[i];
			}
			break;
		default:
			break;
	}

	miniGLUnlockGlobals(gl);
}

/**
 *
 */
void minigllib_glpolygonmode(UInt miniGLLibRef, GLenum face, GLenum mode) {
	miniGLLib_globals *gl;

        gl = miniGLLockGlobals(miniGLLibRef);

	if (mode == GL_FILL)
		gl->wireframe = 0;
	else if (mode == GL_LINE)
		gl->wireframe = 1;

	miniGLUnlockGlobals(gl);
}

/**
 * Sets the drawing area on the Palm screen.  Basically sets up a window into
 * which all gl primitives will be rendered according to the perspective rules
 * set up with gluPerspective(...) or glOrtho(...).
 *
 * x1, y1 - (int, int) coordinate of top left corner
 * x2, y2 - (int, int) coordinate of bottom right corner
 */
void minigllib_winsetglarea(UInt miniGLLibRef, int x1, int y1, int x2, int y2) {
	int i;
	Err err;

	miniGLLib_globals *gl = miniGLLockGlobals(miniGLLibRef);

	gl->screen_width = x2 - x1;
	gl->screen_height = y2 - y1;
	gl->screen_startx = x1;
	gl->screen_starty = y1;

	for (i=0;i<8;i++) {
		gl->lights[i].enabled = 0;
	}

	fgChangeToGreyscale();

	/** Create the second buffer if it has not been created yet */
	if (gl->two_created == 0) {

		//ZBufferInit();

		/** create offscreen bitmap */
		gl->one = WinGetActiveWindow();
		gl->two = WinCreateOffscreenWindow(gl->screen_width, 
			gl->screen_height, screenFormat, &err);
		gl->one = WinSetDrawWindow(gl->two);
		WinSetActiveWindow(gl->one);
		gl->two_created = 1;
	}

	gl->cur_matrix[15] = 0.0;

	miniGLUnlockGlobals(gl);
}

void minigllib_glmap2f(UInt miniGLLibRef, GLenum target,
                        GLfloat u1, GLfloat u2, GLint ustride, GLint uorder,
                        GLfloat v1, GLfloat v2, GLint vstride, GLint vorder,
                        const GLfloat *points ) {
	// nothing here yet.
}

void minigllib_glmapgrid2f(UInt miniGLLibRef, GLint un, GLfloat u1, GLfloat u2,
                        GLint vn, GLfloat v1, GLfloat v2 ) {
	// nothing here yet.
}

void minigllib_glevalmesh2(UInt miniGLLibRef, GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) {
	// nothing here yet.
}

void minigllib_glpushattrib(UInt miniGLLibRef, GLbitfield mask) {
	// nothing here yet.
}

void minigllib_glpopattrib(UInt miniGLLibRef) {
	// nothing here yet.
}

void minigllib_gldrawpixels(UInt miniGLLibRef, GLsizei width, GLsizei height, GLenum format,
                        GLenum type, const GLvoid *pixels) {
	// nothing here yet.
}

void minigllib_glbitmap(UInt miniGLLibRef, GLsizei width, GLsizei height, GLfloat xbo,
                        GLfloat ybo, GLfloat xbi, GLfloat ybi,
                        const GLubyte *bitmap) {
	// nothing here yet.
}

/**
 * Set back to normal mono screen rendering mode
 */
void minigllib_winremoveglarea(UInt miniGLLibRef) {
	fgChangeFromGreyscale();
}

/**
 * End of miniGL_main.c
 *
 * References:  
 *   Foley, et al.  Computer Graphics.  Addison Wesley, 1990.
 *   Woo, et al.  OpenGL Programming Reference.  Addison Wesley, 1997.
 */

