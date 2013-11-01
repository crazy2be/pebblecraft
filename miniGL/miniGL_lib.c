/**
 * miniGL_lib.c
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

#include "miniGL.h"
#include "miniGL_main.h"
#include "miniGL_lib.h"

/**
 * The routine to open the shared library.  If some other program
 * already has it open, it increments the use counter.  Else it
 * allocates and initializes all global variables.
 */
void miniGL_init(void) {
  /** Initialize globals here */
  refcount = 1;
  modv_level = 0;
  proj_level = 0;
  for (int i=0;i<16;i++) {
    cur_matrix[i] = 0.0;
    per_matrix[i] = 0.0;
    scr_matrix[i] = 0.0;
  }
  num_vertices = 0;
  two_created = 0;
  lighting = 1;
  wireframe = 1;
}

/**
 * End of miniGL_lib.c 
 */
