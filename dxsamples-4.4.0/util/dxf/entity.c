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

static Table entity_table[] = {
  {"POINT",	E_POINT,	snag_point,	output_point}, 
  {"CIRCLE",	E_CIRCLE,	snag_circle,	output_circle}, 
  {"ARC",	E_ARC,		snag_arc,	output_arc}, 
  {"SOLID",	E_SOLID,	snag_solid,	output_solid}, 
  {"TRACE",	E_TRACE,	snag_trace,	output_trace}, 
  {"TEXT",	E_TEXT,		snag_text,	output_text}, 
  {"SHAPE",	E_SHAPE,	snag_shape,	output_shape}, 
  {"BLOCK",	E_BLOCK,	snag_block,	output_block}, 
  {"ENDBLK",	E_END_BLOCK,	snag_end_block,	output_end_block}, 
  {"INSERT",	E_INSERT,	snag_insert,	output_insert}, 
  {"ATTDEF",	E_ATTDEF,	snag_attdef,	NULL}, 
  {"ATTRIB",	E_ATTRIB,	snag_attrib,	NULL}, 
  {"POLYLINE",	E_POLYLINE,	snag_polyline,	output_polyline}, 
  {"VERTEX",	E_VERTEX,	snag_vertex,	output_vertex}, 
  {"SEQEND",	E_SEQ_END,	snag_seqend,	output_seqend}, 
  {"3DLINE",	E_LINE_3D,	snag_line,	output_3dline}, 
  {"LINE",	E_LINE,		snag_line,	output_line},
  {"3DFACE",	E_FACE_3D,	snag_3dface,	output_3dface}, 
  {"DIMENSION",	E_DIMENSION,	NULL,		NULL}, 
  {"ENDSEC",	E_ENDSEC,	NULL,		NULL},
  {"VIEWPORT",	E_VIEWPORT,	NULL,		NULL},
};
static nEntities = sizeof(entity_table)/sizeof(Table);

/*
 * Given a string representing an entity type (read from the file)
 * return a pointer to the function that will read that entity
 * into memory.  Make an assignment to the 'enitity' parameter also.
 */
PFVP get_snag_func(char *value,int *entity)
{
  int i;

  for (i=0;i<nEntities;i++) {
    if (strstr(value, entity_table[i].name)) {
      /* Only return exact matches */
      if (strstr(entity_table[i].name, value)) {
	*entity = entity_table[i].value;
	return entity_table[i].snag_func;
      }
    }
  }
  *entity = -1;
  return NULL;
}

/*
 * Given a entity type, return the function that outputs that entity.
 * This may mean creates a DX sub object, or (if debugging) prints
 * the objects values.
 */
PFV get_output_func(int entity)
{
  int i;
  
  for (i=0;i<nEntities;i++) {
    if (entity_table[i].value == entity) {
      return entity_table[i].output_func;
    }
  }
  return NULL;
}

/*
 * Given an entity's type return it's name
 */
char *get_entity_name(int entity)
{
  int i;

  for (i=0;i<nEntities;i++) {
    if (entity_table[i].value == entity) {
      return &entity_table[i].name[0];
    }
  }
  return NULL;
}

/*
 * Make an assignment to an entities block pointer.
 */
void add_block_to_entity(Entity *e, Entity *block)
{
  e->block = block;
}

/*
 * Create a new entity and set all defaults
 */
Entity *create_new_entity(int type, void *data)
{
  Entity *e = (Entity *)malloc(sizeof(Entity));
  
  if (e) {
    e->data = data;
    e->type = type;
    e->index = -1;
    e->next = NULL;
    e->block = NULL;
    e->processed = FALSE;
  } else {
    fprintf(stderr,"malloc failed\n");
    return NULL;
  }

  return (Entity *)e;
}

/*
 * Add an item to the end of a list.  The item must contain
 * a next pointer and a type pointer in known locations.
 * The item can contain any other type of data.
 */
int add_item_to_list(List *list, Entity *item)
{
  if (!(item && list)) {
    fprintf(stderr,"Implementation error\n");
    exit(0);
  }

  if (list->first_item == NULL) {
    /* First time through for this list */
    list->first_item = item;
  } else {
    list->last_item->next = item;
  }
  list->last_item = item;
  list->nItems++;
  if (item->type > -1 && item->type < MAX_ENTITIES) {
    list->histogram[item->type]++;
  }

  return (list->nItems-1);
}

