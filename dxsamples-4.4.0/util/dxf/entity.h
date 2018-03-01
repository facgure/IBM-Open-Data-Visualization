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


#ifndef _H_ENTITY
#define _H_ENTITY

/* Entity types */
#define E_POINT 	0
#define E_LINE		1
#define E_CIRCLE 	2
#define E_ARC 		3
#define E_SOLID 	4
#define E_TRACE 	5
#define E_TEXT 		6
#define E_SHAPE 	7
#define E_BLOCK 	8
#define E_END_BLOCK 	9
#define E_INSERT 	10
#define E_ATTDEF	11
#define E_ATTRIB 	12
#define E_POLYLINE 	13 
#define E_VERTEX 	14
#define E_SEQ_END 	15
#define E_LINE_3D 	16
#define E_FACE_3D 	17
#define E_DIMENSION 	18
#define E_ENDSEC	19
#define E_VIEWPORT	20	/* v. 11 and later */
			
/* Snag functions */
void *snag_block     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_end_block (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_point     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_shape     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_text	     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_line	     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_3dface    (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_solid     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_trace     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_arc	     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_vertex    (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_polyline  (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_insert    (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_seqend    (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_circle    (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_attdef    (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_attrib    (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_string    (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_float     (FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_integer   (FILE *fpin, char *s, char *value, int *group, int *type);

/* Print functions */
void output_block	(Entity *e, Collection *c);
void output_end_block	(Entity *e, Collection *c);
void output_insert	(Entity *e, Collection *c);
void output_polyline	(Entity *e, Collection *c);
void output_vertex	(Entity *e, Collection *c);
void output_seqend	(Entity *e, Collection *c);
void output_trace	(Entity *e, Collection *c);
void output_circle	(Entity *e, Collection *c);
void output_arc		(Entity *e, Collection *c);
void output_solid	(Entity *e, Collection *c);
void output_3dface	(Entity *e, Collection *c);
void output_3dline	(Entity *e, Collection *c);
void output_line	(Entity *e, Collection *c);
void output_point	(Entity *e, Collection *c);
void output_text	(Entity *e, Collection *c);
void output_shape	(Entity *e, Collection *c);

#endif
