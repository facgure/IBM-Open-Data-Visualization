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
#include <math.h>
#include <string.h>

#include "dxf.h"
#include "entity.h"

/*
 * The routines in this file read in entire entities and return
 * void pointers to such.  These void pointers are attached to
 * the Entities 'data' pointer.
 */

/* This handles read-aheads */
static int need_group = TRUE;
static int default_polyline_start_width = 0.0;
static int default_polyline_end_width = 0.0;

void rewind_file()
{
  need_group = FALSE;
}

/*
 * Scan file for next non-comment group.  Return TRUE (i.e. done) if
 * EOF is reached, otherwise return FALSE and fill in data.  If
 * 'required' is TRUE and an entire group is not found then we exit with
 * message.
 */
int get_next_group(FILE *fp, int required,
		   char *s, char *value, int *group, int *type)
{
  static int line=0;
  int comment = TRUE;

  /* This handles the case where we've already read-ahead */
  if (!need_group) {
    need_group = TRUE;
    return FALSE;
  }
  need_group = TRUE;

  while (1) {
    /* look for group code... */
    if (fgets(s,256,fp)) {
      line++;
      /* Blow away CR/LF's */
      if (strchr(s,'\n')) *strchr(s,'\n') = '\0';
      if (strchr(s,0x0D)) *strchr(s,0x0D) = '\0';
      *group = strtoul(s,NULL,0);
      /* blow off comments */
      if (*group == G_COMMENT) {
	if (!fgets(s,256,fp)) {
	  ErrorExit("Invalid comment");
	}
	line++;
	continue;
      }
      *type = get_type_from_group(*group);
      if (fgets(value,256,fp)) {
	/* Blow away CR/LF's */
	if (strchr(value,'\n')) *strchr(value,'\n') = '\0';
	if (strchr(value,0x0D)) *strchr(value,0x0D) = '\0';
	line++;
	return FALSE;
      } else {
	/* if a group code is found a value is required */
	fprintf(stderr,"Found group code: %d but not value pair at line: %d\n",
		*group, line);
	exit(0);
      }
    }
      /* if fgets reads no data we are at EOF so return 'TRUE' */
    if (!required) {
      return TRUE;
    } else {
      fprintf(stderr,"Group not found at line %d\n",line);
      exit(0);
    }
  }
}

int get_type_from_group(int group)
{
  if (group > 0 && group < 10) return T_STRING;
  if (group > 9 && group < 60) return T_FLOAT;
  if (group > 59 && group < 80) return T_INT;
  if (group > 209 && group < 240) return T_FLOAT;
  if (group == 999) return T_COMMENT;
  return T_UNKNOWN;
}


void *snag_block(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Block *item;
  int i;

  item = (Block *)malloc(sizeof(Block));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->processed = FALSE;
  init_list(&item->entities);
  
  while (TRUE) {
    /*
     * Assume that we can read to the next entity group
     */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* base point */
    case G_X_COOR1: item->loc[0] = atof(value); break;
    case G_Y_COOR1: item->loc[1] = atof(value); break;
    case G_Z_COOR1: item->loc[2] = atof(value); break;
      /* flags */
    case G_INT1:    item->flags = strtol(value,NULL,0); break;
      /* name */
    case G_NAME:    strncpy(item->name,value,25); break;
      /* external reference -- v. 11 and later */
    case G_TEXT_STRING:    strncpy(item->external_ref,value,50); break;
      /* other text -- v. 11 and later */
    case G_OTHER_TEXT1:    strncpy(item->other_text,value,25); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in BLOCK", *group, value, IGNORE);
    }
  }
  
}

void *snag_end_block(FILE *fpin, char *s, char *value, int *group, int *type)
{
  while (TRUE) {
    /* No groups except maybe the common defaults -- so just read them
       in and safely ignore them */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      continue;
    }
    switch (*group) {
    case G_FILE_SEP:
      rewind_file();
      return NULL;
    default:
      print_group("Unknown group in ENDBLOCK", *group, value, IGNORE);
    }
  }
}

void *snag_point(FILE *fpin, char *s, char *value, int *group, int *type)
{
  PointEntity *item;

  item = (PointEntity *)malloc(sizeof(PointEntity));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->angle = 0;
  
  while (TRUE) {
    /*
     * Assume that we can read to the next entity group
     */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* angle */
    case G_ANGLE1:  item->angle = atof(value); break;
      /* PointEntity */
    case G_X_COOR1: item->pt[0] = atof(value); break;
    case G_Y_COOR1: item->pt[1] = atof(value); break;
    case G_Z_COOR1: item->pt[2] = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in POINT", *group, value, IGNORE);
    }
  }
}

void *snag_shape(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Shape *item;

  item = (Shape *)malloc(sizeof(Shape));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->rot = 0.0;
  item->x_scale = 1.0;
  item->oblique = 0.0;
  
  while (TRUE) {
    /*
     * Assume that we can read to the next entity group
     */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* val */
    case G_NAME:
      strncpy(item->name,value,10);
      break;
      /* size */
    case G_FLOAT1:	item->size = atof(value); break;
      /* x_scale */
    case G_FLOAT2:	item->x_scale = atof(value); break;
      /* rot */
    case G_ANGLE1:	item->rot = atof(value);     break;
      /* oblique */
    case G_ANGLE2:	item->oblique = atof(value); break;
      /* point */
    case G_X_COOR1: 	item->pt[0] = atof(value);   break;
    case G_Y_COOR1: 	item->pt[1] = atof(value);   break;
    case G_Z_COOR1: 	item->pt[2] = atof(value);   break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in SHAPE", *group, value, IGNORE);
    }
  }
  
}

void *snag_text(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Text *item;

  item = (Text *)malloc(sizeof(Text));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->rot = 0.0;
  item->x_scale = 1.0;
  item->oblique = 0.0;
  strncpy(item->style,"STANDARD",10);
  item->flags = 0x0;
  item->just = 0.0;
  item->align_pt[0] = item->align_pt[1] = item->align_pt[2] = 0.0;
  
  while (TRUE) {
    /*
     * Assume that we can read to the next entity group
     */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* val */
    case G_TEXT_STRING:
      strncpy(item->val,value,25);
      break;
      /* height */
    case G_FLOAT1:	item->height = atof(value);  break;
      /* x_scale */
    case G_FLOAT2:	item->x_scale = atof(value); break;
      /* rot */
    case G_ANGLE1:	item->rot = atof(value);     break;
      /* oblique */
    case G_ANGLE2:	item->oblique = atof(value); break;
      /* style */
    case G_TEXT_STYLE:
      strncpy(item->style,value,10);
      break;
      /* flags */
    case G_INT2:	item->flags = strtol(value,NULL,0); break;
      /* just */
    case G_INT3:	item->just = strtol(value,NULL,0);  break;
      /* point */
    case G_X_COOR1: 	item->pt[0] = atof(value);   break;
    case G_Y_COOR1: 	item->pt[1] = atof(value);   break;
    case G_Z_COOR1: 	item->pt[2] = atof(value);   break;
      /* align_pt */
    case G_X_COOR2: 	item->align_pt[0] = atof(value); break;
    case G_Y_COOR2: 	item->align_pt[1] = atof(value); break;
    case G_Z_COOR2: 	item->align_pt[2] = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in TEXT", *group, value, IGNORE);
    }
  }
  
}

/* Also does line_3d */
void *snag_line(FILE *fpin, char *s, char *value, int *group, int *type)
{
  LineEntity *item;

  item = (LineEntity *)malloc(sizeof(LineEntity));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  /* none */
  
  while (TRUE) {
    /*
     * Assume that we can read to the next entity group
     */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* start */
    case G_X_COOR1: item->start[0] = atof(value); break;
    case G_Y_COOR1: item->start[1] = atof(value); break;
    case G_Z_COOR1: item->start[2] = atof(value); break;
      /* end */
    case G_X_COOR2: item->end[0]   = atof(value); break;
    case G_Y_COOR2: item->end[1]   = atof(value); break;
    case G_Z_COOR2: item->end[2]   = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in LINE", *group, value, IGNORE);
    }
  }
  
}

void *snag_3dface(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Face3d *item;

  item = (Face3d *)malloc(sizeof(Face3d));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->edge_vis_flag = 0;
  
  /*
   * Assume that we can read to the next entity group
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* edge visibility flag */
    case G_INT1: item->edge_vis_flag = strtol(value,NULL,0); break;
      /* pt1 */
    case G_X_COOR1: item->pt1[0] = atof(value); break;
    case G_Y_COOR1: item->pt1[1] = atof(value); break;
    case G_Z_COOR1: item->pt1[2] = atof(value); break;
      /* pt2 */
    case G_X_COOR2: item->pt2[0] = atof(value); break;
    case G_Y_COOR2: item->pt2[1] = atof(value); break;
    case G_Z_COOR2: item->pt2[2] = atof(value); break;
      /* pt3 */
    case G_X_COOR3: item->pt3[0] = atof(value); break;
    case G_Y_COOR3: item->pt3[1] = atof(value); break;
    case G_Z_COOR3: item->pt3[2] = atof(value); break;
      /* pt2 */
    case G_X_COOR4: item->pt4[0] = atof(value); break;
    case G_Y_COOR4: item->pt4[1] = atof(value); break;
    case G_Z_COOR4: item->pt4[2] = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in FACE", *group, value, IGNORE);
    }
  }
}

void *snag_solid(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Solid *item;

  item = (Solid *)malloc(sizeof(Solid));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  /* none */
  
  /*
   * Assume that we can read to the next entity group
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* pt1 */
    case G_X_COOR1: item->pt1[0] = atof(value); break;
    case G_Y_COOR1: item->pt1[1] = atof(value); break;
    case G_Z_COOR1: item->pt1[2] = atof(value); break;
      /* pt2 */
    case G_X_COOR2: item->pt2[0] = atof(value); break;
    case G_Y_COOR2: item->pt2[1] = atof(value); break;
    case G_Z_COOR2: item->pt2[2] = atof(value); break;
      /* pt3 */
    case G_X_COOR3: item->pt3[0] = atof(value); break;
    case G_Y_COOR3: item->pt3[1] = atof(value); break;
    case G_Z_COOR3: item->pt3[2] = atof(value); break;
      /* pt2 */
    case G_X_COOR4: item->pt4[0] = atof(value); break;
    case G_Y_COOR4: item->pt4[1] = atof(value); break;
    case G_Z_COOR4: item->pt4[2] = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in SOLID", *group, value, IGNORE);
    }
  }
}

void *snag_trace(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Trace *item;

  item = (Trace *)malloc(sizeof(Trace));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  /* none */
  
  /*
   * Assume that we can read to the next entity group
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* pt1 */
    case G_X_COOR1: item->pt1[0] = atof(value); break;
    case G_Y_COOR1: item->pt1[1] = atof(value); break;
    case G_Z_COOR1: item->pt1[2] = atof(value); break;
      /* pt2 */
    case G_X_COOR2: item->pt2[0] = atof(value); break;
    case G_Y_COOR2: item->pt2[1] = atof(value); break;
    case G_Z_COOR2: item->pt2[2] = atof(value); break;
      /* pt3 */
    case G_X_COOR3: item->pt3[0] = atof(value); break;
    case G_Y_COOR3: item->pt3[1] = atof(value); break;
    case G_Z_COOR3: item->pt3[2] = atof(value); break;
      /* pt2 */
    case G_X_COOR4: item->pt4[0] = atof(value); break;
    case G_Y_COOR4: item->pt4[1] = atof(value); break;
    case G_Z_COOR4: item->pt4[2] = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in TRACE", *group, value, IGNORE);
    }
  }
}

void *snag_arc(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Arc *item;

  item = (Arc *)malloc(sizeof(Arc));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  /* none */
  
  /*
   * Assume that we can read to the next entity group
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* center */
    case G_X_COOR1: item->center[0]   = atof(value); break;
    case G_Y_COOR1: item->center[1]   = atof(value); break;
    case G_Z_COOR1: item->center[2]   = atof(value); break;
      /* radius */
    case G_FLOAT1:  item->radius      = atof(value); break;
      /* start and stop angles */
    case G_ANGLE1:  item->start_angle = atof(value); break;
    case G_ANGLE2:  item->end_angle   = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in ARC", *group, value, IGNORE);
    }
  }
}

void *snag_vertex(FILE *fpin, char *s, char *value, int *group, int *type)
{
  VertexEntity *item;

  item = (VertexEntity *)malloc(sizeof(VertexEntity));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->start_width = default_polyline_start_width;
  item->end_width = default_polyline_end_width;
  item->bulge = 0.0;
  item->vert_flags = 0x0;
  item->tangent = 0.0;
  item->mesh_indicies[0] = item->mesh_indicies[1] =
    item->mesh_indicies[2] = item->mesh_indicies[3] = 0;
  
  /*
   * Assume that we can read to the next entity group
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* location */
    case G_X_COOR1: item->loc[0]     = atof(value); break;
    case G_Y_COOR1: item->loc[1]     = atof(value); break;
    case G_Z_COOR1: item->loc[2]     = atof(value); break;
      /* start_wdith */
    case G_FLOAT1: item->start_width = atof(value); break;
      /* end width */
    case G_FLOAT2: item->end_width   = atof(value); break;
      /* bulge */
    case G_FLOAT3: item->bulge       = atof(value); break;
      /* vert_flag */
    case G_INT1:   item->vert_flags  = strtol(value,NULL,0); break;
      /* curve fit tangent */
    case G_ANGLE1: item->tangent     = atof(value); break;
      /* vertex indicies (for mesh) -- ignore edge invisble flag -- i.e. -1 */
    case G_INT2: item->mesh_indicies[0] = abs(strtol(value,NULL,0)); break;
    case G_INT3: item->mesh_indicies[1] = abs(strtol(value,NULL,0)); break;
    case G_INT4: item->mesh_indicies[2] = abs(strtol(value,NULL,0)); break;
    case G_INT5: item->mesh_indicies[3] = abs(strtol(value,NULL,0)); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in VERTEX", *group, value, IGNORE);
    }
  }
}
void *snag_polyline(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Polyline *item;

  item = (Polyline *)malloc(sizeof(Polyline));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->verts_follow = 0x0;
  item->elev[0] = item->elev[1] = item->elev[2] = 0.0;
  item->polyline_flags = 0x0;
  item->start_width = 0.0;
  item->end_width = 0.0;
  item->mesh_m = 0;
  item->mesh_n = 0;
  item->smooth_m = 0;
  item->smooth_n = 0;
  item->smooth_type = 0;
  
  /*
   * Assume that we can read to the next entity group
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* position of polyline -- not in spec!!! */
    case G_X_COOR1:     item->elev[0]  = atof(value); break;
    case G_Y_COOR1:     item->elev[1]  = atof(value); break;
    case G_Z_COOR1:     item->elev[2]  = atof(value); break;
      /* verts follow flag */
    case G_ENTS_FOLLOW: item->verts_follow = strtol(value,NULL,0); break;
      /* polyline flags */
    case G_INT1:        item->polyline_flags = strtol(value,NULL,0); break;
      /* start width */
    case G_FLOAT1:
      item->start_width = atof(value);
      default_polyline_start_width = item->start_width;
      break;
      /* start width */
    case G_FLOAT2:
      item->end_width = atof(value);
      default_polyline_end_width = item->start_width;
      break;
      /* mesh_m */
    case G_INT2:        item->mesh_m = strtol(value,NULL,0); break;
      /* mesh_n */
    case G_INT3:        item->mesh_n = strtol(value,NULL,0); break;
      /* smooth m */
    case G_INT4:        item->smooth_m = strtol(value,NULL,0); break;
      /* smooth n */
    case G_INT5:        item->smooth_n = strtol(value,NULL,0); break;
      /* smooth type */
    case G_INT6:        item->smooth_type = strtol(value,NULL,0); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in POLYLINE", *group, value, IGNORE);
    }
  }
}

void *snag_insert(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Insert *item;

  item = (Insert *)malloc(sizeof(Insert));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->attrs_follow = 0x0;
  item->translate[0] = item->translate[1] = item->translate[1] = 0.0;
  item->scale[0] = item->scale[1] = item->scale[2] = 1.0;
  item->rot = 0.0;
  item->col_cnt = 1;
  item->row_cnt = 1;
  item->col_space = 0.0;
  item->row_space = 0.0;
  
  /*
   * Assume that we can read to the next entity group
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* position of insertion */
    case G_X_COOR1: item->translate[0]  = atof(value); break;
    case G_Y_COOR1: item->translate[1]  = atof(value); break;
    case G_Z_COOR1: item->translate[2]  = atof(value); break;
      /* attributes follow */
    case G_ENTS_FOLLOW: item->attrs_follow = strtol(value,NULL,0); break;
      /* block name to insert */
    case G_NAME:    strncpy(item->block_name,value,25); break;
      /* scales */
    case G_FLOAT2: item->scale[0] = atof(value); break;
    case G_FLOAT3: item->scale[1] = atof(value); break;
    case G_FLOAT4: item->scale[2] = atof(value); break;
      /* rotation angle */
    case G_ANGLE1: item->rot = atof(value); break;
      /* column a row counts */
    case G_INT1: item->col_cnt = strtol(value, NULL, 0); break;
    case G_INT2: item->row_cnt = strtol(value, NULL, 0); break;
      /* column a row spacing */
    case G_FLOAT5: item->col_space = atof(value); break;
    case G_FLOAT6: item->row_space = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in INSERT", *group, value, IGNORE);
    }
  }
}

void *snag_attdef(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Attdef *item;

  item = (Attdef *)malloc(sizeof(Attdef));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->pt[0] = item->pt[1] = item->pt[2] = 0.0;
  item->text_height = 0;
  strcpy(item->value,"");
  strcpy(item->prompt, "");
  strcpy(item->tag,"");
  item->attribute_flags = 0x0;
  item->field_length = 0;
  item->text_rotation = 0.0;
  item->rel_x_scale = 0.0;
  item->obliquing_angle = 0.0;
  strcpy(item->text_style,"");
  item->text_generation_flags = 0x0;
  item->horz_just = 0x0;
  item->vert_just = 0x0;
  item->align_pt[0] = item->align_pt[1] = item->align_pt[2] = 0.0;

  while (TRUE) {
    /* No groups except maybe the common defaults -- so just read them
       in and safely ignore them */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      continue;
    }
    switch(*group) {
      /* text start */
    case G_X_COOR1: item->pt[0] = atof(value); break;
    case G_Y_COOR1: item->pt[1] = atof(value); break;
    case G_Z_COOR1: item->pt[2] = atof(value); break;
      /* alignment point */
    case G_X_COOR2: item->align_pt[0] = atof(value); break;
    case G_Y_COOR2: item->align_pt[1] = atof(value); break;
    case G_Z_COOR2: item->align_pt[2] = atof(value); break;
      /* default value */
    case G_TEXT_STRING: strncpy(item->value, value, 25); break;
      /* prompt string */
    case G_OTHER_TEXT1: strncpy(item->prompt, value, 25); break;
      /* text style */
    case G_TEXT_STYLE: strncpy(item->text_style, value, 10); break;
      /* tag */
    case G_NAME: strncpy(item->tag,value, 25); break;
      /* attribute flags */
    case G_INT1: item->attribute_flags = strtol(value, NULL, 0); break;
      /* text generation flag */
    case G_INT2: item->text_generation_flags = strtol(value, NULL, 0); break;
      /* horiz text justification */
    case G_INT3: item->horz_just = strtol(value, NULL, 0); break;
      /* field length */
    case G_INT4: item->field_length = strtol(value, NULL, 0); break;
      /* vert text just */
    case G_INT5: item->vert_just = strtol(value, NULL, 0); break;
      /* rotation angle */
    case G_ANGLE1: item->text_rotation = atof(value); break;
      /* oblique angle */
    case G_ANGLE2: item->obliquing_angle = atof(value); break;
      /* text height */
    case G_FLOAT1: item->text_height = strtol(value,NULL,0); break;
      /* x scale */
    case G_FLOAT2: item->rel_x_scale = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return NULL;
    default:
      print_group("Unknown group in ENDBLOCK", *group, value, IGNORE);
    }
  }
}

void *snag_attrib(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Attdef *item;

  item = (Attdef *)malloc(sizeof(Attdef));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  item->pt[0] = item->pt[1] = item->pt[2] = 0.0;
  item->text_height = 0;
  strcpy(item->value,"");
  item->attribute_flags = 0x0;
  item->field_length = 0;
  item->text_rotation = 0.0;
  item->rel_x_scale = 0.0;
  item->obliquing_angle = 0.0;
  strcpy(item->text_style,"");
  item->text_generation_flags = 0x0;
  item->horz_just = 0x0;
  item->vert_just = 0x0;
  item->align_pt[0] = item->align_pt[1] = item->align_pt[2] = 0.0;

  while (TRUE) {
    /* No groups except maybe the common defaults -- so just read them
       in and safely ignore them */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      continue;
    }
    switch(*group) {
      /* text start */
    case G_X_COOR1: item->pt[0]  = atof(value); break;
    case G_Y_COOR1: item->pt[1]  = atof(value); break;
    case G_Z_COOR1: item->pt[2]  = atof(value); break;
      /* alignment point */
    case G_X_COOR2: item->align_pt[0]  = atof(value); break;
    case G_Y_COOR2: item->align_pt[1]  = atof(value); break;
    case G_Z_COOR2: item->align_pt[2]  = atof(value); break;
      /* default value */
    case G_TEXT_STRING: strncpy(item->value, value, 25); break;
      /* text style */
    case G_TEXT_STYLE: strncpy(item->text_style, value, 10); break;
      /* attribute flags */
    case G_INT1: item->attribute_flags = strtol(value, NULL, 0); break;
      /* text generation flag */
    case G_INT2: item->text_generation_flags = strtol(value, NULL, 0); break;
      /* horiz text justification */
    case G_INT3: item->horz_just = strtol(value, NULL, 0); break;
      /* field length */
    case G_INT4: item->field_length = strtol(value, NULL, 0); break;
      /* vert text just */
    case G_INT5: item->vert_just = strtol(value, NULL, 0); break;
      /* rotation angle */
    case G_ANGLE1: item->text_rotation = atof(value); break;
      /* oblique angle */
    case G_ANGLE2: item->obliquing_angle = atof(value); break;
      /* text height */
    case G_FLOAT1: item->text_height = strtol(value,NULL,0); break;
      /* x scale */
    case G_FLOAT2: item->rel_x_scale = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return NULL;
    default:
      print_group("Unknown group in ENDBLOCK", *group, value, IGNORE);
    }
  }
}

void *snag_seqend(FILE *fpin, char *s, char *value, int *group, int *type)
{
  default_polyline_start_width = 0.0;
  default_polyline_end_width = 0.0;

  while (TRUE) {
    /* No groups except maybe the common defaults -- so just read them
       in and safely ignore them */
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      continue;
    }
    switch (*group) {
    case G_FILE_SEP:
      rewind_file();
      return NULL;
    default:
      print_group("Unknown group in ENDSEQ", *group, value, IGNORE);
    }
  }
}


void *snag_circle(FILE *fpin, char *s, char *value, int *group, int *type)
{
  Circle *item;

  item = (Circle *)malloc(sizeof(Circle));
  if (!item) {
    printf("malloc failed\n");
    return NULL;
  }

  /* common defaults */
  common_defaults((void *)item);
  /* entity defaults */
  /* none */
  
  /*
   * Assume that we can read to the next entity group
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (is_common(*group)) {
      /* common stuff */
      handle_common((void *)item, *group, value);
      continue;
    }
    switch (*group) {
      /* center */
    case G_X_COOR1: item->center[0]  = atof(value); break;
    case G_Y_COOR1: item->center[1]  = atof(value); break;
    case G_Z_COOR1: item->center[2]  = atof(value); break;
      /* radius */
    case G_FLOAT1:  item->radius     = atof(value); break;
      /* next group */
    case G_FILE_SEP:
      rewind_file();
      return (void *)item;
    default:
      print_group("Unknown group in CIRCLE", *group, value, IGNORE);
    }
  }
}

