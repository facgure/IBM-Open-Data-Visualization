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

#include "dxf.h"

static List ignore_list;

void init_ignored_list(void)
{
  /* because it's a file static list */
  init_list(&ignore_list);
}


void add_to_ignored_list(int group)
{
  int found = FALSE;
  Ignored *ignored = find_ignored(group);

  if (!ignored) {
    /* If this group has not yet been ignored
       create a new ignored item for the list */
    ignored = (Ignored *)malloc(sizeof(Ignored));
    ignored->group = group;
    ignored->count = 0;
    if (!ignored) {
      fprintf(stderr,"malloc failed\n");
      exit(0);
    }
  } else {
    found = TRUE;
  }

  ignored->count++;

  if (!found) {
    add_item_to_list(&ignore_list, (Entity *)ignored);
  }
}

void print_ignored(void)
{
  Ignored *ig = (Ignored *)ignore_list.first_item;

  printf("Ignored groups---------------------\n");
  printf("Group		Count\n");
  printf("------------------------------------\n");
    
  while (ig) {
    printf(" %d\t\t %d\n", ig->group, ig->count);
    ig = ig->next;
  }

  printf("\n\n");
  return;
}

Ignored *find_ignored(int group)
{
  Ignored *ig = (Ignored *)ignore_list.first_item;

  while (ig) {
    if (ig->group == group) return ig;
    ig = ig->next;
  }

  return NULL;
}

