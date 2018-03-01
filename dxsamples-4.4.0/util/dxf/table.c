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
#include "entity.h"

extern int debug = 0;
extern int debug_detail = 0;

/*
 * Routines in this file handle reading in the table section of
 * the DXF file.
 */

#define MAX_TABLES 8
DXFTable *table_list[MAX_TABLES];

int InitTables(void)
{
  int i;
  
  for (i=0;i<MAX_TABLES;i++) {
    table_list[i] = NULL;
  }
}

void add_table(DXFTable *tab)
{
  if (tab)
    if (tab->type < MAX_TABLES) {
      table_list[tab->type] = tab;
    }
}

int which_table_type(char *value)
{
  if (strstr(value, "VPORT")) return T_VPORT;
  if (strstr(value, "LTYPE")) return T_LTYPE;
  if (strstr(value, "LAYER")) return T_LAYER;
  if (strstr(value, "STYLE")) return T_STYLE;
  if (strstr(value, "VIEW")) return T_VIEW;
  if (strstr(value, "DIMSTYLE")) return T_DIMSTYLE;
  if (strstr(value, "UCS")) return T_UCS;
  if (strstr(value, "APPID")) return T_APPID;
  return -1;
}

DXFTable *get_table(FILE *fpin, char *s, char *value, int *group, int *type)
{
  LayerRecord *layers;
  DXFTable *table;
  char name[25];
  int table_type;
  int size;
  int done = 0;

  /* This better be a table name */
  get_next_group(fpin, REQUIRED, s, value, group, type);
  table_type = which_table_type(value);
  if (table_type == -1) {
    rewind_file();
    return NULL;
  }
  strncpy(name, value, 25);
  
  /* Look for the G_INT1 group.  If we do not find it before the next
     G_FILE_SEP then return error */
  *group = 1;
  while (*group != G_FILE_SEP) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    if (*group == G_INT1) {
      size = strtoul(value, NULL, 0);
      /* if size == 0 get the ENDTAB group and return */
      if (size == 0) {
	get_next_group(fpin, REQUIRED, s, value, group, type);
	return NULL;
      }
    }
  }

  if (table_type == T_LAYER) {
    /*
     * Since we are only interested in the layer table we will
     * malloc here, if we were interested in all tables we
     * would malloc above and use a switch statement.
     */
    table = (DXFTable *)malloc(sizeof(DXFTable));
    if (!table) {
      printf("malloc failed\n");
      return NULL;
    }
    strcpy(table->name, name);
    table->type = table_type;
    table->maxRecords = size;
    table->nRecords = 0;
    return get_layer_table(fpin, table);
  } else {
    ignore_table(fpin, s, value, group, type);
    return NULL;
  }
}

DXFTable *get_layer_table(FILE *fpin, DXFTable *table)
{
  LayerRecord *layers;
  char name[25];
  char s[256], value[256];
  int group, type;

  layers = (LayerRecord *)
    malloc(sizeof(LayerRecord)*table->maxRecords);

  if (!layers) {
    printf("malloc failed\n");
    free(table);
    return NULL;
  }

  /* Tabels are: 0 group with table name (for verification), 2 group
     with record name (in this case layer name) and then record
     specific data, so look for verification group, assign name and
     then read data -- this is simillar to a snag_func but there
     are possibly multiple records */
  while (TRUE) {
    /*
     * Assume that we can read to the next entity group
     */
    get_next_group(fpin, REQUIRED, s, value, &group, &type);
    switch (group) {
      /* verification group (ie. begin record marker) or table end */
    case G_FILE_SEP:
      table->nRecords++;
      if (strstr(value, "ENDTAB")) {
	table->data = (void *)layers;
	return table;
      } else {
	if (!strstr(value, "LAYER")) {
	  printf("invalid record");
	  free (table);
	  return NULL;
	}
      }
      break;
      /* name */
    case G_NAME:
      strncpy(layers[table->nRecords].layer_name,value,25);
      break;
      /* flags */
    case G_INT1:
      layers[table->nRecords].flags = strtol(value, NULL, 0);
      break;
      /* color */
    case G_COLOR:
      layers[table->nRecords].color = strtol(value, NULL, 0);
      break;
    case G_LINE_TYPE:
      strncpy(layers[table->nRecords].line_type,value,25);
      break;
    default:
      print_group("Layer table:       ", group, value, IGNORE);
    }
  }
}

void ignore_table(FILE *fpin, char *s, char *value, int *group, int *type)
{
  char name[25];

  print_group("Table:             ", *group, value, IGNORE);
  while (TRUE) {
    get_next_group(fpin, REQUIRED, s, value, group, type);
    switch (*group) {
    case G_FILE_SEP:
      if (strstr(value, "ENDTAB")) {
	return;
      }
      break;
    default:
      print_group("Table:             ", *group, value, IGNORE);
    }
  }
  return;
}

int get_int_from_table(int which, char *rec_name, int field)
{
  int i;
  DXFTable *table;
  
  switch (which) {
  case T_LAYER:
    /* look for requested layer */
    table = table_list[T_LAYER];
    if (table) {
      LayerRecord *records = ((LayerRecord *)table->data);
      for (i=0;i<table->nRecords;i++) {
	if (strstr(rec_name, records[i].layer_name)) {
	  switch (field) {
	  case VAL_COLOR:
	    return records[i].color;
	  default:
	    return -1;
	  }
	}
      }
    }
    return -1;
    break;
  default:
    return -1;
  }
}

print_tables(void)
{
  int i, j;

  for (i=0;i<MAX_TABLES;i++) {
    if (table_list[i] != NULL) {
      printf("Tables:\n");
      printf("\tname: %s\n", table_list[i]->name);
      printf("\ttype: %d\n", table_list[i]->type);
      printf("\tnRecords: %d\n", table_list[i]->nRecords);
      printf("\tmaxRecords: %d\n", table_list[i]->maxRecords);
      printf("\tdata: 0x%x\n", table_list[i]->data);
      for (j=0;j<table_list[i]->nRecords;j++) {
	printf("\t\tname: %s color %d\n",
	       ((LayerRecord *)table_list[i]->data)[j].layer_name,
	       ((LayerRecord *)table_list[i]->data)[j].color);
      }
    }
  }
}
