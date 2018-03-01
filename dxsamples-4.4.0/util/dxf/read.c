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

extern int verbose;

/*
 * Routines in this file read in the four main parts of the DXF
 * file: HEADER, TABLE, BLOCK and ENTITY.  See the DXF file format
 * documentation for more info.
 */

/************************************************************************
 *
 * Read in the 'header' section of the DXF file.
 *
 ************************************************************************/
void process_header_section(FILE *fpin)
{
  char s[256], value[256];
  int group, type;
  int done=FALSE;
  char var_name[256];
  PFVP snag_func = NULL;
  void *data;
  int cnt=0;

  if (verbose) fprintf(stderr, "processing header section...");

  while (TRUE) {
    /*
     * Assume that we should be able to read groups until we encounter
     * a ENDSEC group.  It's an ERROR otherwise
     */
    get_next_group(fpin, REQUIRED, s, value, &group, &type);
    switch (group) {
    case G_VARIABLE_NAME:
      cnt++;
      type = get_type_from_variable_table(value);
      snag_func = get_variable_snag_func(type);
      if (snag_func) {
	/* The snag_func will alter 'value' so save it for later use */
	strcpy(var_name, value);
	data = (*snag_func)(fpin, s, value, &group, &type);
	if (data) {
	  add_variable_data(var_name, data);
	}
      }
      break;
    case G_FILE_SEP:
      if (strstr(value, "ENDSEC")) {
	if (verbose) fprintf(stderr, "read %d variables\n", cnt);
	return;
      } else {
	print_group("Unknown file separator in header section: ", group,
		    value, IGNORE);
      }
    default:
      print_group("Unknown group in header section: ", group, value, IGNORE);
      /* Find the next variable */
      ignore_var(fpin, s, value, &group, &type);
    }
  }

  ErrorExit("Should never reach here\n");
}


/************************************************************************
 *
 * Read in the 'tables' section of the DXF file.
 *
 * There are 8 kinds of tables in a DXF file's table section.
 * We are only interested in the layer table for now (it contains
 * default color information for the entities.  So we'll scan
 * until we find the color table and then store it for later
 * use (see colors.c)).
 * 
 * Table types are: VPORT, LTYPE, LAYER, STYLE, VIEW,
 *		DIMSTYLE, UCS or APPID.
 *
 * We store these tables in an array of tables so as to get at them fast.
 ************************************************************************/
void process_table_section(FILE *fpin)
{
  char s[256], value[256];
  int group, type;
  int done=FALSE;
  int i;
  DXFTable *aTable;
  int cnt=0;

  if (verbose) fprintf(stderr, "processing table section...");

  InitTables();

  while (TRUE) {
    /*
     * Assume that we should be able to read groups until we encounter
     * a ENDSEC group.  It's an ERROR otherwise
     */
    get_next_group(fpin, REQUIRED, s, value, &group, &type);
    switch (group) {
    case G_FILE_SEP:
      if (strstr(value, "ENDSEC")) {
	if (verbose) fprintf(stderr, "read %d tables\n", cnt);
	return;
      } else {
	cnt++;
	/*
	 * This is the TABLE group, so snag the table and
	 * insert it in the table list
	 */
	aTable = get_table(fpin, s, value, &group, &type);
	add_table(aTable);
      }
      break;
    default:
      print_group("Table:             ", group, value, IGNORE);
    }
  }

  ErrorExit("Should never reach here\n");
}

/************************************************************************
 *
 * Read in the 'block' section of the DXF file.
 * 
 * Blocks contain their own entity list.  These entities are not part of
 * the global (i.e. top-level) entity list.
 ************************************************************************/
void process_block_section(FILE *fpin, List *b_list)
{
  char s[256], value[256];
  int group, type;
  int done=FALSE;
  int entity;
  void *data;
  PFVP snag_func = 0;
  Entity *curBlock = NULL;
  Entity *newEntity = NULL;
  List *blockEntityList = NULL;
  int cnt=0;

  if (verbose) fprintf(stderr, "processing block section...");

  /*
   * Assume that we should be able to read groups until we encounter
   * an E_ENDSEC entity.  It's an ERROR otherwise
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, &group, &type);
    switch (group) {
    case G_FILE_SEP:
      snag_func = get_snag_func(value,&entity);
      if (entity == E_ENDSEC) {
	if (verbose) fprintf(stderr, "read %d entities\n", cnt);
	return;
      } else if (snag_func) {
	cnt++;
	data = (*snag_func)(fpin,s,value,&group,&type);
	newEntity = create_new_entity(entity, data);
	if (newEntity) {
	  if (newEntity->type == E_BLOCK) {
	    /* If the entity is a block, then add it to the block list */
	    newEntity->index = add_item_to_list(b_list, newEntity);
	    /* And make it the current block */
	    curBlock = newEntity;
	    blockEntityList = &((Block *)curBlock->data)->entities;
	  } else {
	    /*
	     * Add the item to the block's entity list.  Add a pointer
	     * back to the block to the entity
	     */
	    add_block_to_entity(newEntity, curBlock);
	    newEntity->index = add_item_to_list(blockEntityList, newEntity);
	    if (newEntity->type == E_END_BLOCK) {
	      blockEntityList = NULL;
	      curBlock = NULL;
	    }
	  }
	} else {
	  ErrorExit("Malloc failed while reading BLOCK section");
	}
      } else {
	print_group("Warning: Unknown group in BLOCK section: ", group,
		    value, IGNORE);
      }
      break;
    default:
      print_group("Warning: Unknown group in BLOCK section: ", group,
		  value, IGNORE);
    }
  }

  ErrorExit("Should never reach here\n");
}

/************************************************************************
 *
 * Read in the 'block' section of the DXF file.
 *
 * Process the entity section of the file.  This section includes
 * all entities that are not part of a block.  These entities are
 * added to the global (i.e. top-level) entity list.
 ************************************************************************/
void process_entity_section(FILE *fpin, List *e_list)
{
  char s[256], value[256];
  int group, type;
  int done=FALSE;
  int entity;
  void *data;
  PFVP snag_func = 0;
  Entity *newEntity;
  int cnt=0;

  if (verbose) fprintf(stderr, "processing entity section...");

  /*
   * Assume that we should be able to read groups until we encounter
   * an E_ENDSEC entity.  It's an ERROR otherwise.
   */
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, &group, &type);
    switch (group) {
    case G_FILE_SEP:
      snag_func = get_snag_func(value,&entity);
      if (entity == E_ENDSEC) {
	if (verbose) fprintf(stderr, "read %d entities\n", cnt);
	return;
      } else if (snag_func) {
	cnt++;
	data = (*snag_func)(fpin,s,value,&group,&type);
	newEntity = create_new_entity(entity, data);
	if (!newEntity) {
	  printf("malloc failed\n");
	  exit(0);
	}
	newEntity->index = add_item_to_list(e_list, newEntity);
      } else {
	print_group("Warning: Unknown group in ENTITIES section: ", group,
		    value, IGNORE);
      }
      break;
    default:
      print_group("Warning: Unknown group in ENTITIES section: ", group,
		  value, IGNORE);
    }
  }

  ErrorExit("Should never reach here\n");
}


