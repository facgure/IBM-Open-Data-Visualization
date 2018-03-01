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

/*
 * Given a hierarchically defined tree containing the DXF model
 * produce a DX object and then write it to the file.
 */
void OutputToDX(Tree *tree, char *filename)
{
  Xform xform;

  if (debug) {
    print_header();
    print_tables();
    print_ignored();
    print_tree(tree);
  } else {
    /* Build the DX object */
    DXinitdx();

    xform = build_dx_object(tree,0);
  
    DXExportDX((Object)xform, filename, "dxASCIIfollows");
  }
}

/*
 * Recursivly accumulate the tree into DX Xforms.  At any particular
 * level the growing DX object contains two optional fields and
 * any number of subobjects.  The optional fields contain triangles
 * and lines.  The sub objects are an xform that contains two optional
 * fields and any number of sub objects, and so on....
 */
Xform build_dx_object(Tree *tree, int members)
{
  Xform  xform;
  Xform  childGroup;
  Group  group = NULL;
  Matrix mat;
  Field triangles, lines;
  Block *b;
  Tree  *t;

  /*
   * If this subtree has already been processed then use the already
   * created xform and group.  Otherwise we need to create them.
   */
  if (!tree->processed) {
    group = DXNewGroup();
    if (!group) {
      ErrorExit("Could not create group");
    }
  
    b = (Block *)tree->block_ent->data;
    DXSetStringAttribute((Object)group, "name", b->name);

    triangles = grab_triangles(b->entities.first_item);
    lines     = grab_lines(b->entities.first_item);
    
    if (triangles)
      DXSetEnumeratedMember(group, members++, (Object)triangles);
    if (lines)
      DXSetEnumeratedMember(group, members++, (Object)lines);
    
    /* Get the transforma matrix for this tree node */
    mat =
      DXMat(
	    tree->transform[0][0],tree->transform[0][1],tree->transform[0][2],
	    tree->transform[1][0],tree->transform[1][1],tree->transform[1][2],
	    tree->transform[2][0],tree->transform[2][1],tree->transform[2][2],
	    tree->transform[3][0],tree->transform[3][1],tree->transform[3][2]);

    xform = DXNewXform((Object)group, mat);

    /* Assign a name (mostly for debugging) */
    DXSetStringAttribute((Object)xform, "name", b->name);

    /* So we don't recreate this object next time... */
    tree->xform = xform;
    tree->group = group;
    tree->processed = TRUE;
    
  } else {

    group = tree->group;
    xform = tree->xform;

  }
  
  /* Now process this node's children */
  t = (Tree *)tree->subtrees.first_item;
  while (t) {
    childGroup = build_dx_object(t,0);
    DXSetEnumeratedMember(group, members++, (Object)childGroup);
    t = (Tree *)t->next;
  }
  
  return xform;
}

/*
 * Grab all items that will fit into a field with
 * connection type 'triangle'.  This includes quads which are
 * triangulated.  A collection (admittidly poorly named) keeps
 * track of how many points and connections have been added
 * to the object so far.
 */
Field grab_triangles(Entity *e)
{
  Collection collection;

  collection.f = DXNewField();
  if (!collection.f) {
    ErrorExit("NewField failed in grab_triangles");
  }
  DXSetStringAttribute((Object)collection.f, "name", "Triangle Field");

  collection.nPnts = 0;
  collection.nConns = 0;
  collection.flags = 0x0;
  do3dFaces(e, &collection);

  collection.flags = GET_MESHES_ONLY;
  doPolyLines(e, &collection);
  doTheRest(e, &collection);

  if (!collection.nPnts) {
    DXDelete((Object)collection.f);
    collection.f = NULL;
  }

  return collection.f;
}

/*
 * Grab all items that will fit into a field with connection
 * type 'line'
 */
Field grab_lines(Entity *e)
{
  Collection collection;

  collection.f = DXNewField();
  if (!collection.f) {
    fprintf(stderr,"NewField failed\n");
    exit(0);
  }
  DXSetStringAttribute((Object)collection.f, "name", "Line Field");

  collection.nPnts = 0;
  collection.nConns = 0;
  collection.flags = 0x0;
  doLines(e, &collection);

  collection.flags = GET_LINES_ONLY;
  doPolyLines(e, &collection);

  if (!collection.nPnts) {
    DXDelete((Object)collection.f);
    collection.f = NULL;
  }

  return collection.f;
}

