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


/* Globals */
/*----------------------------------------------------------------------*/
int verbose = 0;

int debug = 0;
int debug_detail = 0;

/* Number of slices to cut a 360 circle into */
int slices = 30;

/* File pointers and file names */
FILE *fpin = stdin;
char top_name[25];
char out_filename[25];
/*----------------------------------------------------------------------*/


/***********************************************************************
 *
 * Main routine
 *
 * Scan the file processing each of four sections.  The block list
 * contains (not surprisingly) a list of blocks.  Blocks contain
 * their own provate list of entities.  The entity list contains
 * all entities which are not part of a block.  Thus the entity
 * list contains the top-level of the model.  The hierachical
 * nature of the model is built up by "INSERT" entities in the
 * top-level which include blocks (possibly after transformation).
 *
 * Blocks are included in the model via the 'INSERT' group.
 ***********************************************************************/
void main(int argc, char *argv[])
{
  int i;
  char s[256], value[256];
  int group, type;
  int done=FALSE;
  int state = S_LOOKING_FOR_FILE_SEPARATOR;
  List e_list;			/* entity list */
  List b_list; 			/* block list */
  Tree *tree;
  Entity *top;
  Block *top_block;

  parse_args(argc, argv);

  /*
   * Initialize the ignored group list -- record keeping only
   */
  init_ignored_list();

  /* Initialize entity list */
  init_list(&e_list);
  
  /* initialize block list */
  init_list(&b_list);

  /* Add a block to the block list in anticipation of the top level data */
  top = add_top_level_block(&b_list, top_name);

  /* get memory for the model tree */
  tree = create_new_tree("__RootNode", 0, top);
  if (!tree) {
    ErrorExit("Malloc failed in main");
  }
  
  /* grab the first group */
  done = get_next_group(fpin, NOT_REQUIRED, s, value, &group, &type);
  
  /* start parsing */
  while (!done) {
    switch (state) {
    case S_LOOKING_FOR_FILE_SEPARATOR:
      if (group != G_FILE_SEP) {
	ErrorExit("Can't find file separator group 0");
      }
      if (strstr(value, "SECTION")) {
	get_next_group(fpin, REQUIRED, s, value, &group, &type);
	if (group != G_NAME) {
	  InvalidData(s);
	}
	state = which_file_section(value);
      } else {
	if (strstr(value, "EOF")) {
	  done = TRUE;
	} else {
	  ErrorExit("Found file separator group not followed "
		    "by 'SECTION' keyword");
	}
      }
      break;
    case S_HEADER_SECTION:
      process_header_section(fpin);
      done = get_next_group(fpin, NOT_REQUIRED, s, value, &group, &type);
      state = S_LOOKING_FOR_FILE_SEPARATOR;
      break;
    case S_TABLE_SECTION:
      process_table_section(fpin);
      done = get_next_group(fpin, NOT_REQUIRED, s, value, &group, &type);
      state = S_LOOKING_FOR_FILE_SEPARATOR;
      break;
    case S_BLOCK_SECTION:
      process_block_section(fpin,&b_list);
      done = get_next_group(fpin, NOT_REQUIRED, s, value, &group, &type);
      state = S_LOOKING_FOR_FILE_SEPARATOR;
      break;
    case S_ENTITY_SECTION:
      /* How about: process_entity_section(fpin, top->entity_list); */
      process_entity_section(fpin,&e_list);
      done = get_next_group(fpin, NOT_REQUIRED, s, value, &group, &type);
      state = S_LOOKING_FOR_FILE_SEPARATOR;
      break;
    default:
      ErrorExit("Implimentation error: no such state");
    }
  }


  /*
   * Entities that were not part of a block in the file have been
   * given a block name of "top_level_block".  So here we spin through
   * the entity list creating the top level block.
   */
  add_to_top_level(&b_list, &e_list, top_name);
/* Above: maybe can be removed if we send this list into the
   process_entity_section routine */


  
  /*
   * Spin through the database again and re-arrange it into
   * a 'tree' data structure.
   *
   * The last parameter is for preserving the layer structure.
   */
  top_block = (Block *)top->data;
  build_tree(tree, &b_list, &top_block->entities, FALSE);

  /*
   * Now output the data to DX
   */
  fprintf(stderr, "Outputing file to: %s\n", out_filename);

  OutputToDX(tree, out_filename);

/*   if (verbose) print_statistics(tree); */
}

/*
 * Which file section are we in?
 */
int which_file_section(char *value)
{
  if (strstr(value, "HEADER"))   return S_HEADER_SECTION;
  if (strstr(value, "TABLES"))   return S_TABLE_SECTION;
  if (strstr(value, "BLOCKS"))   return S_BLOCK_SECTION;
  if (strstr(value, "ENTITIES")) return S_ENTITY_SECTION;
  InvalidData(value);
}

/*
 * Add a block to the block list that represents the top level of
 * the model.  This block pointer will be assigned to all entities
 * that do not belong to any block (i.e. all entities read in from
 * the 'entity' section of the file (i.e. all entities in the top
 * level)).
 */
Entity *add_top_level_block(List *list, char *top_lev)
{
  Entity *block_ent;
  Block *b = (Block *)malloc(sizeof(Block));
  if (!b) {
    ErrorExit("Malloc failed in add_top_level_block");
  }

  common_defaults((void *)b);
  strcpy(b->name, top_lev);
  strcpy(b->other_text, "");
  b->flags = 0x0;
  b->processed = FALSE;
  b->loc[0] = b->loc[1] = b->loc[2] = 0.0;
  init_list(&b->entities);

  block_ent = create_new_entity(E_BLOCK, (void *)b);
  if (!block_ent) {
    ErrorExit("Malloc failed in add_top_level_block");
  }

  block_ent->index = add_item_to_list(list, block_ent);
  return block_ent;
}

/*
 * Assign the top level block pointer to each entity in the
 * top level.  Move the item from the e_list to the
 * top_level block list.  (A rightous question here would
 * be: why not do this in the first place?).
 */
void add_to_top_level(List *b_list, List *e_list, char *top_lev)
{
  Entity *e = e_list->first_item;
  Entity *block_ent;
  Block *b;

  block_ent = find_block_entity(b_list, top_lev);
  if (!block_ent) {
    fprintf(stderr, "Cannot find top level block\n");
    exit(0);
  }
  b = (Block *)block_ent->data;

  while (e) {
    add_item_to_list(&b->entities, e);
    e->block = block_ent;
    e = e->next;
  }

  /* Reset e_list for no real reason but debugging */
  init_list(e_list);
  return;
}

/*
 * As the name implies
 */
parse_args(int argc, char *argv[])
{
  int i;
  int got_input=FALSE;
  int got_output=FALSE;

  verbose = 0;
  slices = 30;
  fpin = stdin;
  strcpy(top_name, "Top Level");
  strcpy(out_filename, "dxf.dx");
  
  /* for debugging */
  if (getenv("DEBUG")) debug = getenv("DEBUG");
  if (getenv("DEBUG_DETAIL")) debug_detail = getenv("DEBUG_DETAIL");

  for (i=1;i<argc;i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'h': usage(argv[0]);
      case 'v': verbose = TRUE; break;
      case 'r':
	slices = atoi(&argv[i][2]);
	if (slices < 5) slices = 5;
	break;
      default:
	usage(argv[0]);
      }
    } else {
      if (!got_input) {
	char *s;
	fpin = fopen(argv[i], "r");
	strncpy(top_name, argv[i], 25);
	if (strstr(argv[i], ".dxf")) *strstr(argv[i], ".dxf") = '\0';
	/* blow away any directoty names before the input file name */
	s = &argv[i][strlen(argv[i])];
	while (s != &argv[i][0] && *s != '/') {
	  s--;
	}
	if (s != &argv[i][0]) s++;
	strncpy(out_filename, s, 22);
	strcat(out_filename, ".dx");
	if (!fpin) {
	  fprintf(stderr, "Cant open file %s\n",argv[i]);
	  exit(0);
	}
	got_input=TRUE;
      } else if (!got_output) {
	strcpy(out_filename, argv[i]);
	got_output=TRUE;
      } else {
	usage(argv[0]);
      }
    }
  }
}


/*
 * help
 */
usage(char *progname)
{

  fprintf(stderr,
	  "\n    Usage: %s  [-h] [-v] [-r'n'] [filein [fileout]]\n",
	  progname);
  
  fprintf(stderr,"\n\t-h:      this help message.\n");
  fprintf(stderr,"\t-v:      verbose operation.\n");
  fprintf(stderr,"\t-r'n':   resolution of circles and arcs (default 40).\n");
  fprintf(stderr,"\tfilein:  input file name (default stdin).\n");
  fprintf(stderr,"\tfileout: output file name (default dxf.dx).\n\n");

  exit(0);
}

