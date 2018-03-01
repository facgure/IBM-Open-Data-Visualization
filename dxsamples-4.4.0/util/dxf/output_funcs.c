/*
 * (C) COPYRIGHT International Business Machines Corp. 1995
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of IBM not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dxf.h"
#include "entity.h"

/* Vertex hash functions */
static PseudoKey VertexHash(Key key);
static int VertexCmp(Key k0, Key k1);
int add_point(Entity *e, Collection *c, Point *pt);

typedef struct _indexed_point {
  Point p;
  int index;
} IndexedPoint;

void print_blocks(List *b_list)
{
  Entity *b = b_list->first_item;

  if (debug)
    printf("***Processing Blocks\n");

  while (b) {
    output_block(b,NULL);
    print_entities(&((Block *)b->data)->entities);
    printf("\n");
    b = b->next;
  }
}

void print_entities(List *e_list)
{
  Entity *e = e_list->first_item;

  if (debug)
    print_histogram(e_list);

  do3dFaces(e,NULL);
  doLines(e,NULL);
  doPolyLines(e,NULL);
  doTheRest(e,NULL);
}

void print_histogram(List *e_list)
{
  int i;
  char *name;

  printf("Histogram ----------------------------------------\n");
  printf("    Entity Name        Count\n");
  printf("--------------------------------------------------\n");

  for (i=0;i<MAX_ENTITIES;i++) {
    name = get_entity_name(i);
    if (name) printf("\t%-20.20s\t%d\n",name,e_list->histogram[i]);
  }
}

void do3dFaces(Entity *e, Collection *c)
{

  if (debug) {
    printf("***Processing 3d Faces\n");
  } else {
    /*
     * When adding points to the DX object we do not want to add duplicate
     * points.  We create a hash table to accomplish this.  We don't
     * worry about duplicate verts between primitve types like lines and faces.
     * That's why we create and destroy the hash for each type.
     */
    c->pnts = DXCreateHash(sizeof(IndexedPoint), VertexHash, VertexCmp);
    if (!c->pnts) {
      ErrorExit("Could not create 'points' hash table");
    }
  }

  while (e) {
    if (e->type == E_FACE_3D) {
      if (debug)
	output_entity(e,c);
      else
	output_3dface(e,c);
      e->processed = TRUE;
    }
    e = e->next;
  }
  if (debug)
    printf("\n");

  if (!debug) DXDestroyHash(c->pnts);
}

void doLines(Entity *e, Collection *c)
{
  if (debug) {
    printf("***Processing Lines\n");
  } else {
    /*
     *	 When adding points to the DX object we do not want to add duplicate
   * points.  We create a hash table to accomplish this.
   */
    c->pnts = DXCreateHash(sizeof(IndexedPoint), VertexHash, VertexCmp);
    if (!c->pnts) {
      ErrorExit("Could not create 'points' hash table");
    }
  }

  while (e) {
    if (e->type == E_LINE || e->type == E_LINE_3D) {
      if (debug)
	output_entity(e,c);
      else
	output_3dline(e, c);
      e->processed = TRUE;
    }
    e = e->next;
  }
  if (debug)
    printf("\n");

  if (!debug) DXDestroyHash(c->pnts);
}	

void doPolyLines(Entity *e, Collection *c)
{
  int cnt, first, last;

  if (debug) {
    printf("***Processing Polyline\n");
  } else {
    /*
     * When adding points to the DX object we do not want to add duplicate
     * points.  We create a hash table to accomplish this.
     */
    c->pnts = DXCreateHash(sizeof(IndexedPoint), VertexHash, VertexCmp);
    if (!c->pnts) {
      ErrorExit("Could not create 'points' hash table");
    }
  }

  while (e) {
    if (e->type == E_POLYLINE) {
      if (!debug) {
	output_polyline(e,c);
      } else {
	/* print the polyline entity */
	output_entity(e,c);
	e->processed = TRUE;
	e = e->next;
	first = -1;
	cnt=0;
	while (e->type != E_SEQ_END) {
	  /* print all verticies */
	  if (!debug_detail) {
	    if (first == -1)
	      first = e->index;
	    last = e->index;
	    cnt++;
	  } else {
	    output_entity(e,c);
	  }
	  e->processed = TRUE;
	  e = e->next;
	}
	if (!debug_detail)
	  printf("  %d verticies (%d -- %d) ",cnt, first, last);
	/* print the end seqence */
	output_entity(e,c);
	e->processed = TRUE;
	printf("\n");
      }
    }
    /* find the next polyline */
    e = e->next;
  }
  if (debug)
    printf("\n");

  if (!debug) DXDestroyHash(c->pnts);
}

int should_process(Entity *e, Collection *c)
{
  switch (c->flags) {
  case GET_MESHES_ONLY:
    switch (e->type) {
    case E_CIRCLE:
    case E_ARC:
      return TRUE;
    default:
      return FALSE;
    }
  case GET_LINES_ONLY:
    switch (e->type) {
    case E_LINE:
    case E_LINE_3D:
      return TRUE;
    default:
      return FALSE;
    }
  default:
    return TRUE;
  }
}

void doTheRest(Entity *e, Collection *c)
{
  if (debug) {
    printf("\n***Processing the Rest\n");
  } else {
    /*
     * When adding points to the DX object we do not want to add duplicate
     * points.  We create a hash table to accomplish this.
     */
    c->pnts = DXCreateHash(sizeof(IndexedPoint), VertexHash, VertexCmp);
    if (!c->pnts) {
      ErrorExit("Could not create 'points' hash table");
    }
  }

  while (e) {
    if (should_process(e, c)) {
      if (!debug) {
	output_entity(e,c);
      } else {
	if (!e->processed) {
	  output_entity(e,c);
	}
      }
    }
    e = e->next;
  }
  if (debug)
    printf("\n");

  if (!debug) DXDestroyHash(c->pnts);
}

void print_common(Entity *e)
{
  /* This only works because common is the data field in all entities */
  Common *common = (Common *)(e->data);
  RGBColor col;

  find_color(e, &col);
  
  printf("\t\tlayer: %s  handle: %s  color: %d   line_type: %d\n"
	 "\t\telev:  % 3.5f thickness: % 3.5f model_space: %d\n"
	 "\t\textrude: [% 3.5f,% 3.5f,% 3.5f]\n"
	 "\t\tcolorval: [% 3.5f,% 3.5f,% 3.5f]\n",
	 common->layer,
	 common->handle,
	 common->color,
	 common->line_type,
	 common->elevation,
	 common->thickness,
	 common->model_space,
	 common->extrusion[0],
	 common->extrusion[1],
	 common->extrusion[2],
	 col.r,
	 col.g,
	 col.b);
}

int add_point(Entity *e, Collection *c, Point *pt)
{
  IndexedPoint *dup, ipt;
  RGBColor col;
  int ret;

  ipt.p = *pt;
  ipt.index = -1;
  
  /* Look for the vertex in the hash table first before adding it */
  dup = (IndexedPoint *)DXQueryHashElement(c->pnts, (IndexedPoint *)&ipt);
  if (dup) {
    return dup->index;
  } else {
    ipt.index = c->nPnts;
    /* Add the indexed point to the hash table */
    DXInsertHashElement(c->pnts, (Element)&ipt);
    /* Add the point to the field */
    find_color(e, &col);
    DXAddColor(c->f, c->nPnts, col);
    DXAddPoint(c->f, c->nPnts++, *pt);
  }
  return (c->nPnts-1);
}

/************************************************************************/
/* Output routines 							*/
/************************************************************************/
void output_3dface(Entity *e, Collection *c)
{
  Face3d *data = (Face3d *)e->data;
  Triangle t;

  if (debug) {
    int isSame = isSamePoint(data->pt3, data->pt4);

    if (isSame) {
      printf("Triangle:");
    } else {
      printf("Quad: ");
    }
  
    printf("(%d)\t[% 3.5f,% 3.5f,% 3.5f][% 3.5f,% 3.5f,% 3.5f]\n"
	   "\t\t[% 3.5f,% 3.5f,% 3.5f]",
	   e->index,
	   data->pt1[0],
	   data->pt1[1],
	   data->pt1[2],
	   data->pt2[0],
	   data->pt2[1],
	   data->pt2[2],
	   data->pt3[0],
	   data->pt3[1],
	   data->pt3[2]);

    if (isSame) {
      printf("\n");
    } else {
      printf("[% 3.5f,% 3.5f,% 3.5f]\n",
	     data->pt4[0],
	     data->pt4[1],
	     data->pt4[2]);
    }
    printf("\t\tvis flag: 0x%x\n",
	   data->edge_vis_flag);
    print_block_name(e);
    print_common(e);

  } else {
    
    t.p = add_point(e, c, (Point *)&data->pt1);
    t.q = add_point(e, c, (Point *)&data->pt2);
    t.r = add_point(e, c, (Point *)&data->pt3);
    DXAddTriangle(c->f, c->nConns++, t);
    
    if (! isSamePoint(data->pt3, data->pt4)) {
      t.q = t.r;
      t.r = add_point(e, c, (Point *)&data->pt4);
      DXAddTriangle(c->f, c->nConns++, t);
    }
  }
}

void output_line(Entity *e, Collection *c)
{
  output_3dline(e,c);
}

void output_3dline(Entity *e, Collection *c)
{
  LineEntity *data = (LineEntity *)e->data;
  Line l;

  if (debug) {
    printf("Line: (%d)\t[% 3.5f,% 3.5f,% 3.5f][% 3.5f,% 3.5f,% 3.5f]\n",
	   e->index,
	   data->start[0],
	   data->start[1],
	   data->start[2],
	   data->end[0],
	   data->end[1],
	   data->end[2]);
    print_block_name(e);
    print_common(e);

  } else {

    l.p = add_point(e, c, (Point *)data->start);
    l.q = add_point(e, c, (Point *)data->end);
    DXAddLine(c->f, c->nConns++, l);

  }
}

void generate_disc(Collection *c,
		   Entity *e, float thickness,
		   float start, float stop)
{
  /*
   * To create a disc:
   *
   * Generate each point around the circle, storing it in a temp buffer.
   *
   *	After accumulating all the points, add the points to the growing
   *	DX object and for every three points add a triangle connection -- this
   *	handles the first side of the disc
   *
   * if thickness != 0
   *
   *   Add the points for the other side by spinning through the buffer
   *   of points, translating 'z' by thickness and adding these points.
   *   Spin through the buffer in reverse order and for every three points
   *   add a new triangle.  Going in reverse allows for clockwisedness.
   *
   * if thickness != 0
   *
   *	Spin through list one more time to create the sides of the disc.  This
   *	time we go around from start to finish and add two triangle for
   *	each four points.  We yank the connections for these things from
   *	the connection buffer that we've been building as we added the other
   *	points.
   */
  Point pivot;
  Point points[MAX_CIRCLE_SLICES+1];
  int connects[2][MAX_CIRCLE_SLICES+2];
  Point pt1, pt2;
  Triangle t;
  float p, c1, s1, dx, dy;
  int nSlices = (fabs(stop-start)/360.0)*slices;
  int i;
  int n=0;
  Circle *circ = (Circle *)e->data;
  
#define TWO_PI 6.28312

  /* We use slices here (not nSlices) so we get only partial arcs if asked */
  p = TWO_PI/(slices-1);
  c1 = cos(p);
  s1 = sin(p);

  /* Pivot off the center point */
  points[0] = pivot = *(Point *)circ->center;

  /* move the initial point out 'radius' units along the x axis */
  points[0].x = pivot.x + circ->radius;

  /* Fill in the points array */
  for (i=0;i<nSlices;i++) {
    dx = points[i].x - pivot.x;
    dy = points[i].y - pivot.y;
    points[i+1].x = pivot.x + dx*c1 - dy*s1;
    points[i+1].y = pivot.y + dx*s1 + dy*c1;
    points[i+1].z = pivot.z;
  }

  /* Add the pivot point */
  t.p = add_point(e, c, &pivot);
  connects[0][n++] = t.p;

  /* Add the initial point */
  t.r = add_point(e, c, &points[0]);
  connects[0][n++] = t.r;

  for (i=0;i<nSlices;i++) {
    t.q = add_point(e, c, &points[i+1]);
    DXAddTriangle(c->f, c->nConns++, t);
    connects[0][n++] = t.q;
    t.r = t.q;
  }

  if (thickness != 0.0) {
    /* Spin through points backwards and add them */
    pivot.z += thickness;
    t.p = add_point(e, c, &pivot);
    n--;
    connects[1][n--] = t.p;

    points[nSlices].z += thickness;
    /* Add the last point */
    t.r = add_point(e, c, &points[nSlices]);
    connects[1][n--] = t.r;

    for (i=nSlices;i>0;i--) {
      points[i-1].z += thickness;
      t.q = add_point(e, c, &points[i-1]);
      DXAddTriangle(c->f, c->nConns++, t);
      connects[1][n--] = t.q;
      t.r = t.q;
    }

    /* Finally, add the sides */
    t.p = connects[0][0];
    t.q = connects[1][0];

    for (i=0;i<nSlices;i++) {
      t.r = connects[0][i+1];
      DXAddTriangle(c->f, c->nConns++, t);
      t.p = connects[0][i+1];
      t.r = connects[1][i+1];
      DXAddTriangle(c->f, c->nConns++, t);
      t.q = t.r;
    }

    /* and if not a complete circle then add the closing sections on the arc */
    if ((fabs(start-stop) < 360.0)) {
      printf("This is an arc and needs to be closed\n");
    }
  }
}
    
void output_circle(Entity *e, Collection *c)
{
  Circle *data = (Circle *)e->data;
    
  if (c->flags != GET_MESHES_ONLY)
    return;

  if (debug) {
    printf("Circle: (%d)\t[% 3.5f,% 3.5f,% 3.5f] radius: % 3.5f\n",
	   e->index,
	   data->center[0],
	   data->center[1],
	   data->center[2],
	   data->radius);
    print_block_name(e);
    print_common(e);

  } else {
    generate_disc(c, e,
		  data->common.thickness,
		  0.0, 360.0);
  }
}

void output_arc(Entity *e, Collection *c)
{
  Arc *data = (Arc *)e->data;
  
  if (debug) {
    printf("Arc: (%d)\t[% 3.5f,% 3.5f,% 3.5f] radius: % 3.5f\n"
	   "\t\tstart: % 3.5f end: % 3.5f\n",
	   e->index,
	   data->center[0],
	   data->center[1],
	   data->center[2],
	   data->radius,
	   data->start_angle,
	   data->end_angle);
    print_block_name(e);
    print_common(e);

  } else {
    generate_disc(c, e,
		  data->common.thickness,
		  data->start_angle, data->end_angle);
  }
}

void output_polyline(Entity *e, Collection *c)
{
  Polyline *data = (Polyline *)e->data;
  Triangle t;
  Line l;
  VertexEntity *vertex;
  Point pt1, pt2, pt3;
  Entity *ent;
	

  if (debug) {
    printf("Poly: (%d)\tverts_follow: 0x%x flags: 0x%x\n"
	   "\t\telev: [% 3.5f,% 3.5f,% 3.5f]\n"
	   "\t\ts_w: %3.5f e_w: % 3.5f\n"
	   "\t\tmesh [%dx%d] smooth_type: 0x%x  smooth [% 3.5fx%3.5f]\n",
	   e->index,
	   data->verts_follow,
	   data->polyline_flags,
	   data->elev[0],
	   data->elev[1],
	   data->elev[2],
	   data->start_width,
	   data->end_width,
	   data->mesh_m,
	   data->mesh_n,
	   data->smooth_m,
	   data->smooth_n,
	   data->smooth_type);
    print_block_name(e);
    print_common(e);

  } else {

    switch (c->flags) {
    case GET_MESHES_ONLY:
      if (data->polyline_flags & PL_POLYFACE_MESH) {
	int i;
	int *pnts;
	Point first;
	/*
	 * Polyface meshes are lists of 'm' points followed by
	 * a list of 'n' connections so just read the mother's in,
	 * save the DX indicies for the points and read in the
	 * connections and create triangles
	 *
	 * Reserve space for the index tranlation table first
	 */

	pnts = (int *)malloc(sizeof(int)*data->mesh_m);
	if (!pnts) {
	  fprintf(stderr,"malloc failed\n");
	  return;
	}

	ent = e->next;

	/*
	 * Add all the 'm' points to the DX database, save
	 * indicies.  AutoCAD points start at 1
	 */
	for (i=1;i<=data->mesh_m;i++) {
	  vertex = (VertexEntity *)ent->data;
	  pnts[i] = add_point(ent, c, (Point *)vertex->loc);
	  
	  ent = ent->next;
	  if (!ent) {
	    fprintf(stderr,"Missing some verticies in polyface entity\n");
	    free(pnts);
	    return;
	  }
	}

	/*
	 * Read and add the 'n' connections (break quads into tris)
	 */
	for (i=0;i<data->mesh_n;i++) {
	  int n, m, o, p;
	  vertex = (VertexEntity *)ent->data;
	  n = vertex->mesh_indicies[0];
	  m = vertex->mesh_indicies[1];
	  o = vertex->mesh_indicies[2];
	  p = vertex->mesh_indicies[3];
	  t.p = pnts[n];
	  t.q = pnts[m];
	  t.r = pnts[o];
	  DXAddTriangle(c->f, c->nConns++, t);
	  if (p) {
	    t.p = t.q; t.q = t.r;
	    t.r = pnts[p];
	    DXAddTriangle(c->f, c->nConns++, t);
	  }
	  ent = ent->next;
	  if (!ent) {
	    fprintf(stderr,"Missing vertex indicies in polyface entity\n");
	    free(pnts);
	    return;
	  }
	}
	free(pnts);
      } else if (data->polyline_flags & PL_3D_MESH) {
	/*
	 * Read in the first row of points, if the mesh is closed in
	 * 'm' then save away the first row for later use
	 *
	 * For each of m-1 rows.
	 *	read in a row
	 *	create triangles using previous row and current row
	 *	copy current row to previous row
	 */
	int i, j;
	Entity *ent;
	int *first_buff=NULL;
	int *buff[2];

	/* one buffer each for current and previous rows */
	buff[0] = (int *)malloc(sizeof(int)*data->mesh_n);
	if (!buff[0]) {
	  fprintf(stderr,"malloc failed\n");
	  return;
	}
	buff[1] = (int *)malloc(sizeof(int)*data->mesh_n);
	if (!buff[1]) {
	  fprintf(stderr,"malloc failed\n");
	  return;
	}
	if (data->polyline_flags & PL_CLOSED_IN_M) {
	  first_buff = (int *)malloc(sizeof(int)*data->mesh_n);
	  if (!first_buff) {
	    fprintf(stderr,"malloc failed\n");
	    return;
	  }
	}

	/* Go the the first vertex */
	ent = e->next;

	/* get the first row, save it if necassary */
	for (i=0;i<data->mesh_n;i++) {
	  vertex = (VertexEntity *)ent->data;
	  buff[0][i] = add_point(ent, c, (Point *)vertex->loc);
	  if (first_buff) {
	    first_buff[i] = buff[0][i];
	  }
	  ent = ent->next;
	}

	/*
	 * For each of the remaining rows, grab the whole
	 * row, create triangles and copy the row to the previous row
	 */
	for (i=1;i<data->mesh_m;i++) {
	  for (j=0;j<data->mesh_n;j++) {
	    vertex = (VertexEntity *)ent->data;
	    buff[1][j] = add_point(ent, c, (Point *)vertex->loc);
	    ent = ent->next;
	  }
	  t.p = buff[0][0];
	  t.q = buff[0][1];
	  for (j=1;j<data->mesh_n;j++) {
	    t.r = buff[1][j-1];
	    DXAddTriangle(c->f, c->nConns++, t);
	    t.p = t.q;
	    t.q = buff[1][j];
	    DXAddTriangle(c->f, c->nConns++, t);
	    t.q = buff[0][j+1];
	  }
	  for (j=0;j<data->mesh_n;j++) {
	    buff[0][j] = buff[1][j];
	  }
	}
	if (first_buff) {
	  t.p = buff[0][0];
	  t.q = buff[0][1];
	  for (j=1;j<data->mesh_n;j++) {
	    t.r = first_buff[j-1];
	    DXAddTriangle(c->f, c->nConns++, t);
	    t.p = t.q;
	    t.q = first_buff[j];
	    DXAddTriangle(c->f, c->nConns++, t);
	    t.q = buff[0][j+1];
	  }
	}
	free(buff[0]);
	free(buff[1]);
	if (first_buff)
	  free(first_buff);
      }
      break;
    case GET_LINES_ONLY:
      if ((data->polyline_flags & PL_POLYFACE_MESH) ||
	  (data->polyline_flags & PL_3D_MESH)) {
	/* Ignore it becuase it is a triangle connection */
      } else {
	/* Handle 3-d poly lines */
	Entity *ent;
	int first;
	

	/* Assume at least two points */
	ent = e->next;
	vertex = (VertexEntity *)ent->data;
	l.p = add_point(ent, c, (Point *)vertex->loc);
	if (data->polyline_flags & PL_CLOSED_IN_M) {
	  first = l.p;
	}
	
	ent = ent->next;
	while (ent->type != E_SEQ_END) {
	  vertex = (VertexEntity *)ent->data;
	  l.q = add_point(ent, c, (Point *)vertex->loc);
	  DXAddLine(c->f, c->nConns++, l);
	  l.p = l.q;
	  ent = ent->next;
	}
	if (data->polyline_flags & PL_CLOSED_IN_M) {
	  l.q = first;
	  DXAddLine(c->f, c->nConns++, l);
	}
      }
      break;
    default:
      fprintf(stderr,
	      "Implementaion error: Don't know what type of "
	      "polyjunk to extract\n");
      exit(0);
    }
  }
}

void output_vertex(Entity *e, Collection *c)
{
  VertexEntity *data = (VertexEntity *)e->data;

  if (debug) {
    printf("Vertex: (%d)\t[% 3.5f,% 3.5f,% 3.5f]\n"
	   "\t\tmesh indicies: [%d %d %d %d]\n"
	   "\t\ts_w: % 3.5f e_w: % 3.5f\n"
	   "\t\tbulge: % 3.5f flags 0x%x tangent % 3.5f\n",
	   e->index,
	   data->loc[0],
	   data->loc[1],
	   data->loc[2],
	   data->mesh_indicies[0],
	   data->mesh_indicies[1],
	   data->mesh_indicies[2],
	   data->start_width,
	   data->end_width,
	   data->bulge,
	   data->vert_flags,
	   data->tangent);
    print_block_name(e);
    print_common(e);
    
  } else {
  }
}

void output_trace(Entity *e, Collection *c)
{
  Trace *data = (Trace *)e->data;
  
  if (debug) {
    printf("Trace: (%d)\t[% 3.5f,% 3.5f,% 3.5f][% 3.5f,% 3.5f,% 3.5f]\n"
	   "\t\t[% 3.5f,% 3.5f,% 3.5f][% 3.5f,% 3.5f,% 3.5f]\n",
	   e->index,
	   data->pt1[0],
	   data->pt1[1],
	   data->pt1[2],
	   data->pt2[0],
	   data->pt2[1],
	   data->pt2[2],
	   data->pt3[0],
	   data->pt3[1],
	   data->pt3[2],
	   data->pt4[0],
	   data->pt4[1],
	   data->pt4[2]);
    print_block_name(e);
    print_common(e);

  } else {
  }  
}

void output_solid(Entity *e, Collection *c)
{
  Solid *data = (Solid *)e->data;
  
  if (debug) {
    printf("Solid: (%d)\t[% 3.5f,% 3.5f,% 3.5f][% 3.5f,% 3.5f,% 3.5f]\n"
	   "\t\t[% 3.5f,% 3.5f,% 3.5f][% 3.5f,% 3.5f,% 3.5f]\n",
	   e->index,
	   data->pt1[0],
	   data->pt1[1],
	   data->pt1[2],
	   data->pt2[0],
	   data->pt2[1],
	   data->pt2[2],
	   data->pt3[0],
	   data->pt3[1],
	   data->pt3[2],
	   data->pt4[0],
	   data->pt4[1],
	   data->pt4[2]);
    print_block_name(e);
    print_common(e);

  } else {
  }
  
}

void output_point(Entity *e, Collection *c)
{
  PointEntity *data = (PointEntity *)e->data;
  
  if (debug) {
    printf("2D Point: (%d)\t[% 3.5f,% 3.5f,% 3.5f] angle: % 3.5f\n",
	   e->index,
	   data->pt[0],
	   data->pt[1],
	   data->pt[2],
	   data->angle);
    print_block_name(e);
    print_common(e);

  } else {
  }
}

void output_text(Entity *e, Collection *c)
{
  Text *data = (Text *)e->data;
  
  if (debug) {
    printf("Text: (%d)\t[% 3.5f,% 3.5f,% 3.5f] height: % 3.5f\n"
	   "\t\tstring: %s\n"
	   "\t\trot: % 3.5f x_scale: % 3.5f oblique: % 3.5f\n"
	   "\t\tstyle: %s flags: 0x%x just: %d\n"
	   "\t\talign: [% 3.5f,% 3.5f,% 3.5f]\n",
	   e->index,
	   data->pt[0],
	   data->pt[1],
	   data->pt[2],
	   data->height,
	   data->val,
	   data->rot,
	   data->x_scale,
	   data->oblique,
	   data->style,
	   data->flags,
	   data->just,
	   data->align_pt[0],
	   data->align_pt[1],
	   data->align_pt[2]);
    print_block_name(e);
    print_common(e);

  } else {

    /*  DXAddArrayData(a, nPnts++, 1, &((Text *)data)->pt); */
  }
}

void output_shape(Entity *e, Collection *c)
{
  Shape *data = (Shape *)e->data;
  
  if (debug) {
    printf("Shape: (%d)\t[% 3.5f,% 3.5f,% 3.5f] size: % 3.5f\n"
	   "\t\tname: %s\n"
	   "\t\trot: % 3.5f x_scale: % 3.5f oblique: % 3.5f\n",
	   e->index,
	   data->pt[0],
	   data->pt[1],
	   data->pt[2],
	   data->size,
	   data->name,
	   data->rot,
	   data->x_scale,
	   data->oblique);
    print_block_name(e);
    print_common(e);

  } else {
  }
}

/************************************************************************/
/* The following generate no DX data					*/
/************************************************************************/
void output_block(Entity *e, Collection *c)
{
  /* This generates no DX geometry per se.  So only do anything on DEBUG */
  Block *data = (Block *)e->data;
  
  if (debug) {
    printf("Block: (0x%x)name: %s\n"
	   "\t\tother text: %s\n"
	   "\t\tflags: 0x%x  loc: [% 3.5f,% 3.5f,% 3.5f]\n",
	   e->data,
	   data->name,
	   data->other_text,
	   data->flags,
	   data->loc[0],
	   data->loc[1],
	   data->loc[2]);
    printf("\t\tnEntities: %d\n", 
	   data->entities.nItems);
    print_common(e);
  }
}

void output_end_block(Entity *e, Collection *c)
{
  /* This generates no DX geometry per se.  So only do anything on DEBUG */
  if (debug) {
    printf("Block End: (%d)\n",e->index);
  }
}

void output_insert(Entity *e, Collection *c)
{
  /* This generates no DX geometry per se.  So only do anything on DEBUG */
  Insert *data = (Insert *)e->data;

  if (debug) {
    printf("Insert: (%d)\tattrs follow 0x%x  block name: %s\n"
	   "\t\tloc: [% 3.5f,% 3.5f,% 3.5f]\n"
	   "\t\tscale: [% 3.5f,% 3.5f,% 3.5f]\n"
	   "\t\trot: % 3.5f\n"
	   "\t\tcols [%d, % 3.5f] rows [%d % 3.5f]\n",
	   e->index,
	   data->attrs_follow,
	   data->block_name,
	   data->translate[0],
	   data->translate[1],
	   data->translate[2],
	   data->scale[0],
	   data->scale[1],
	   data->scale[2],
	   data->rot,
	   data->col_cnt,
	   data->col_space,
	   data->row_cnt,
	   data->row_space);
    print_block_name(e);
    print_common(e);
  }
}

void output_seqend(Entity *e, Collection *c)
{
  /* This generates no DX geometry per se.  So only do anything on DEBUG */
  if (debug) {
    printf("\nEndseq: (%d)\n",e->index);
    print_block_name(e);
  }
}

/************************************************************************/
/* Utils								*/
/************************************************************************/
int isSamePoint(float pt1[3], float pt2[3])
{
  return
    ((pt1[0] == pt2[0]) &&
     (pt1[1] == pt2[1]) &&
     (pt1[2] == pt2[2]));
}

/*
 * Called whenever a group is read from the file and not recofgnized.
 * Records ignored groups and optionally prints a warning.
 */
void print_group(char *s, int group, char *value, int ignored)
{
  if (ignored)
    add_to_ignored_list(group);

  if (debug_detail) {
    if (group == 0)
      fprintf(stderr,"\n");
    fprintf(stderr, "%s [group: %4d  value: %s]\n", s, group, value);
  }
}

void print_entity_name(Entity *e)
{
  char *name;

  name = get_entity_name(e->type);
  printf("%-7.7s (%4d) ",name,e->index);
}

void output_entity(Entity *e, Collection *c)
{
  static int i;
  PFV output_func = NULL;

  output_func = get_output_func(e->type);

  if (!debug) {
    if (output_func)
      (*output_func)(e,c);
  } else {
    if (e->processed) return;
    e->processed = TRUE;
    if (!debug_detail) { 
      i++;
      if (!(i%5)) printf("\n");
      print_entity_name(e);
    } else {
      if (output_func)
	(*output_func)(e,c);
    }
  }
}

void print_block_name(Entity *e)
{
  Block *b;

  if (debug) {
    if (e->block) {
      b = (Block *)e->block->data;
      printf("\t\tblock: %s\n", b->name);
    } else {
      printf("\t\tblock: NULL\n");
    }
  }
}


static float primes[] =
    { 143669821.0, 45497241659.0, 1455799321.0};

static PseudoKey
VertexHash(Key key)
{
  register int i;
  register long l;
             float j;
  register long k;
  register float *keyPtr;
  register float *primesPtr;

  l = 0;
  keyPtr = (float *)key;
  primesPtr = primes;
  for (i = 0; i < 3; i++) {
    j = *primesPtr++ * *keyPtr++;
    k = (*(int *)&j);
    k = (((k ^ (k >> 8))) ^ (k >> 16)) ^ (k >> 24);
    l = l + k;
  }

  return (PseudoKey)l;
}

static int
VertexCmp(Key k0, Key k1)
{
  register int   i;
  register float *p0, *p1;

  p0 = (float *)k0;
  p1 = (float *)k1;
  
  for (i = 0; i < 3; i++)
    if (*p0++ != *p1++) return 1;
    
  return 0;
}

