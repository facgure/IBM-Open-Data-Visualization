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


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "dxf.h"
#include "entity.h"

void print_layer_usage(Tree *tree)
{
  int i;
  Block *b;
  int cnt=0;
  Entity *e;
  Tree *t = (Tree *)tree->subtrees.first_item;
  
  printf("\n");
  for (i=0;i<tree->level;i++)
    printf("\t");
  printf("*** tree %s (%d)\n", tree->name, tree->level);

  b = (Block *)tree->block_ent->data;
  printf("Block: %s\n", b->name);
  e = b->entities.first_item;
  while (e) {
    Common *c = (Common *)&((PointEntity *)e->data)->common;
    printf("%-10.10s ", c->layer);
    cnt++;
    if (!(cnt%4)) printf("\n");
    e = e->next;
  }
  printf("\n");
  
  while (t) {
    print_layer_usage(t);
    t = (Tree *)t->next;
  }
}

void print_tree(Tree *tree)
{
  int i;
  Block *b;
  Tree *t = (Tree *)tree->subtrees.first_item;

  printf("\n");
  for (i=0;i<tree->level;i++)
    printf("\t");
  printf("*** tree %s (%d)\n", tree->name, tree->level);

  print_transform(tree->transform);

  b = (Block *)tree->block_ent->data;
  if (!b->processed) {
    output_block(tree->block_ent,NULL);
    print_entities(&b->entities);
    b->processed = TRUE;
  } else
    printf("Block: %s\n", b->name);
  printf("\n");
  
  while (t) {
    print_tree(t);
    t = (Tree *)t->next;
  }
}

void print_transform(float mat[4][4])
{
  int i, j;

  printf("Tree transform:\n");

  for (i=0;i<4;i++) {
    if (i == 0)
      printf("\t[[");
    else
      printf("\t [");
    for (j=0;j<4;j++) {
      printf("% 5.5f, ",mat[i][j]);
    }
    if (i != 3)
      printf("]\n");
    else
      printf("]]\n");
  }
  
}

Entity *find_block_entity(List *b_list, char *name)
{
  Entity *b_ent = b_list->first_item;

  while (b_ent) {
    if (strstr(((Block *)b_ent->data)->name,name)) {
      /* only return exact matches */
      if (strstr(name, ((Block *)b_ent->data)->name,name)) {
	return (b_ent);
      }
    }
    b_ent = b_ent->next;
  }
  return NULL;
}

void init_list(List *list)
{
  int i;

  list->first_item = list->last_item = NULL;
  list->nItems = 0;
  for (i=0;i<MAX_ENTITIES;i++) {
    list->histogram[i] = 0;
  }
}

void identity(float mat[4][4])
{
  int i;
  int j;

  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      if (i == j && i != 3) {
	mat[i][j] = 1.0;
      } else {
	mat[i][j] = 0.0;
      }
    }
  }
}

void scale(float mat[4][4], float vec[3])
{
  mat[0][0] *= vec[0];
  mat[1][1] *= vec[1];
  mat[2][2] *= vec[2];
}

void translate(float mat[4][4], float vec[3])
{
  mat[3][0] += vec[0];
  mat[3][1] += vec[1];
  mat[3][2] += vec[2];
}

void rotate(float mat[4][4], float angle)
{
#if 0
  /* Assume it's about the z axis for no real reason */
  mat[0][0] *= sin(angle);
  mat[0][1] *= -cos(angle);
  mat[1][0] *= cos(angle);
  mat[1][1] *= -sin(angle);
#endif
}

Tree *create_new_tree(char *name, int level, Entity *block_ent)
{
  Tree *newTree = (Tree *)malloc(sizeof(Tree));

  if (newTree) {
    newTree->next = NULL;
    newTree->level = level;
    strcpy(newTree->name,name);
    identity(&newTree->transform[0][0]);
    newTree->block_ent = block_ent;
    newTree->xform = NULL;
    newTree->group = NULL;
    newTree->processed = FALSE;
    init_list(&newTree->subtrees);
  }
  return newTree;
}

void transform_tree(Tree *tree, Insert *ins)
{
  identity(tree->transform);

  /* Accumulate the scale, transform and rotate into the matrix */
  scale(tree->transform, ins->scale);
  translate(tree->transform, ins->translate);
  rotate(tree->transform, ins->rot);
}


Tree *build_tree(Tree *tree, List *b_list, List *e_list, char *layer)
{
  Entity *b_ent;
  Entity *e = e_list->first_item;
  List *blockEntityList;
  Tree *newTree;
  int i;

  while (e) {
    if (e->type == E_INSERT) {
      char *name = &((Insert *)e->data)->block_name[0];
      b_ent = find_block_entity(b_list, name);
      if (b_ent) {
	blockEntityList = &((Block *)b_ent->data)->entities;
	newTree = create_new_tree(name, tree->level+1, b_ent);
	if (newTree) {
	  transform_tree(newTree, (Insert *)e->data);
	  build_tree(newTree, b_list, blockEntityList, layer);
	  add_item_to_list(&tree->subtrees, (Entity *)newTree);
	} else {
	  fprintf(stderr,"malloc failed\n");
	  exit(0);
	}
      } else {
	fprintf(stderr,"Can't find block named %s\n",
		((Insert *)e->data)->block_name);
	return;
      }
    }
    e = e->next;
  }
}
