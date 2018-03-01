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

#include "dxf.h"

/*
 * Every entity has certian common elements: color, layer, line_type
 * etc.  The routines in this file handle these common items
 */
int is_common(int group)
{
 switch (group) {
 case G_LAYER:
 case G_COLOR:
 case G_LINE_TYPE:
 case G_Z_ELEVATION:
 case G_THICKNESS:
 case G_HANDLE:
 case G_X_EXTRUSION:
 case G_Y_EXTRUSION:
 case G_Z_EXTRUSION:
 case G_MODEL_SPACE: /* v. 11 and later */
   return TRUE;
 default:
   return FALSE;
 }
}

void handle_common(void *data, int group, char *value)
{
  int ret;
  Common *common = (Common *)data;

  switch (group) {
  case G_LAYER:
    strncpy(common->layer,value,25); 	     return;
  case G_HANDLE:
    strncpy(common->handle,value,25); 	     return;
  case G_COLOR:
    common->color = strtol(value,NULL,0);    return;
  case G_LINE_TYPE:
    common->line_type = strtol(value,NULL,0);return;
  case G_Z_ELEVATION:
    common->elevation = atof(value); 	     return;
  case G_THICKNESS:
    common->thickness = atof(value); 	     return;
  case G_X_EXTRUSION:
    common->extrusion[0] = atof(value);	     return;
  case G_Y_EXTRUSION:
    common->extrusion[1] = atof(value);	     return;
  case G_Z_EXTRUSION:
    common->extrusion[2] = atof(value);	     return;
  case G_MODEL_SPACE:
    common->model_space = strtoul(value, NULL, 0); return;
  default:
    DEBUG_ERROR("Unknown common type");
    return;
  }
}

void common_defaults(void *data)
{
  /*
   * Note: This only works because common is the first data field in every
   * entity.  Do not move the common data fields.
   */
  Common *common = (Common *)data;

  strcpy(common->layer,"");
  strcpy(common->handle,"");
  common->color = 256;		/* Assume BYLAYER coloring */
  common->line_type = 0;
  common->elevation = 0.0;
  common->thickness = 0.0;
  common->model_space = TRUE;
  common->colorval.r = common->colorval.g = common->colorval.b = 1.0; 
  common->extrusion[0] = common->extrusion[1] = common->extrusion[2] = 0.0;
  return;
}


