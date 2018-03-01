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


#ifndef _H_DXF
#define _H_DXF

#include "dx/dx.h"

extern int debug, debug_detail;
extern int slices;

#define DEBUG_ERROR(s) \
  fprintf(stderr,"Error: "); \
  fprintf(stderr,s); \
  fprintf(stderr,"\n"); \
  exit(0);

#define ErrorExit(s) fprintf(stderr,s); fprintf(stderr,"\n"); exit(0);
#define InvalidData(s) fprintf(stderr,"Invalid Data: %s",value);exit(0);

/* Color definition */
#define BY_LAYER 10
#define BY_BLOCK 20

/* Handy constants */
#define TRUE 1
#define FALSE 0
#define IGNORE 1
#define REQUIRED TRUE
#define NOT_REQUIRED FALSE
#define MAX_ENTITIES 25
#define MAX_CIRCLE_SLICES 60

/* parser state defines */
#define S_LOOKING_FOR_FILE_SEPARATOR 0
#define S_HEADER_SECTION 1
#define S_BLOCK_SECTION  2
#define S_ENTITY_SECTION 3
#define S_TABLE_SECTION  4

/* data types */
#define T_COMMENT 1
#define T_FLOAT   2
#define T_INT     3
#define T_STRING  4
#define T_UNKNOWN 5

/* group types */
#define G_FILE_SEP	0
#define G_TEXT_STRING 	1
#define G_NAME 		2
#define G_OTHER_TEXT1 	3
#define G_OTHER_TEXT2 	4
#define G_HANDLE 	5
#define G_LINE_TYPE 	6
#define G_TEXT_STYLE 	7
#define G_LAYER 	8
#define G_VARIABLE_NAME 9
#define G_X_COOR1 	10
#define G_X_COOR2 	11
#define G_X_COOR3 	12
#define G_X_COOR4 	13
#define G_X_COOR5 	14
#define G_X_COOR6 	15
#define G_X_COOR7 	16
#define G_X_COOR8 	17
#define G_X_COOR9 	18
#define G_Y_COOR1 	20
#define G_Y_COOR2 	21
#define G_Y_COOR3 	22
#define G_Y_COOR4 	23
#define G_Y_COOR5 	24
#define G_Y_COOR6 	25
#define G_Y_COOR7 	26
#define G_Y_COOR8 	27
#define G_Y_COOR9 	28
#define G_Z_COOR1 	30
#define G_Z_COOR2 	31
#define G_Z_COOR3 	32
#define G_Z_COOR4 	33
#define G_Z_COOR5 	34
#define G_Z_COOR6 	35
#define G_Z_COOR7 	36
#define G_Z_COOR8 	37
#define G_Z_ELEVATION 	38
#define G_THICKNESS 	39
#define G_FLOAT1 	40
#define G_FLOAT2 	41
#define G_FLOAT3 	42
#define G_FLOAT4 	43
#define G_FLOAT5 	44
#define G_FLOAT6 	45
#define G_FLOAT7 	46
#define G_FLOAT8 	47
#define G_FLOAT9 	48
#define G_REPEATED_VAL	49
#define G_ANGLE1 	50
#define G_ANGLE2 	51
#define G_ANGLE3 	52
#define G_ANGLE4 	53
#define G_ANGLE5 	54
#define G_ANGLE6 	55
#define G_ANGLE7 	56
#define G_ANGLE8 	57
#define G_ANGLE9 	58
#define G_COLOR 	62
#define G_ENTS_FOLLOW 	66
#define G_MODEL_SPACE 	67  /* v. 11 and later */
#define G_INT1 		70
#define G_INT2 		71
#define G_INT3 		72
#define G_INT4 		73
#define G_INT5 		74
#define G_INT6 		75
#define G_INT7 		76
#define G_INT8 		77
#define G_INT9 		78
#define G_X_EXTRUSION 	210
#define G_Y_EXTRUSION 	220
#define G_Z_EXTRUSION 	230
#define G_COMMENT 	999

/* varaible types from header section */
#define V_STRING    1
#define V_2VECTOR   2
#define V_3VECTOR   3
#define V_FLOAT     4
#define V_INTEGER   5
#define V_UNKNOWN   6

/* These index into table array */
#define ENDSEC 0
#define T_VPORT    0
#define T_LTYPE    1
#define T_LAYER    2
#define T_STYLE    3
#define T_VIEW 	   4
#define T_DIMSTYLE 5
#define T_UCS      6
#define T_APPID    7

#define VAL_COLOR 1

/* pointers to functions for function tables */
typedef void * (*PFVP)();
typedef void (*PFV)();

typedef struct _dxf_table {
  char name[25];
  int type;
  int nRecords;
  int maxRecords;
  void *data;
} DXFTable;

typedef struct _layer_record {
  char layer_name[25];
  int flags;
  int color;
  char line_type[25];
} LayerRecord;

typedef struct _var_table {
  char name[15];
  int var_type;
  void *data;
  char comment[55];
} Variable, VariableTable;

typedef struct _table {
  char *name;
  int   value;
  PFVP  snag_func;
  PFV   output_func;
} Table;


/* Data common to all entities */
typedef struct _common {
  char  layer[25];
  char  handle[25];
  int 	color;
  int 	line_type;
  int 	model_space;
  float elevation;
  float thickness;
  float extrusion[3];
  RGBColor colorval;
} Common;

/* Basic building block of objects */
typedef struct _entity {
  struct _entity *next;
  int type;
  int index;
  int processed;
  struct _entity *block;
  void *data;
} Entity;

/* A list of entities and some bookkeeping info */
typedef struct _list {
  Entity *first_item;
  Entity *last_item;
  int nItems;
  int histogram[MAX_ENTITIES];
} List;

/* A list of entities and a name */
typedef struct _block {
  Common common;
  char  name[25];
  char  other_text[25];  /* v. 11 and later */
  char  external_ref[50];  /* v. 11 and later */
  /*
   * This next for telling weather or not geometry has been created from
   * this block yet
   */
  int   processed;
  int   flags;
  float loc[3];
  List  entities;
} Block;

/*
 * The hierachical model.
 *
 * A tree is, at each level, a pointer to a block (which contains geometry)
 * and a list of sub-trees.
 *
 * Note that the top level entities (those items read from
 * the 'entity' section of the DXF file) are placed into a block
 * called "Top Level Block".  This allows me to handle top
 * level items just like any other.
 */
typedef struct _tree {
  Entity *next;
  char name[25];
  int level;
  int processed;
  float transform[4][4];
  Entity *block_ent;
  Xform xform;
  Group group;
  List subtrees;
} Tree;

/* The working collection of fields, points and such */
typedef struct _collection {
  HashTable pnts;
  Field f;
  int nPnts;
  int nConns;
#define GET_MESHES_ONLY 1
#define GET_LINES_ONLY 2
  int flags;
} Collection;

/*
 * A bookkeeping mechanism for keeping track of groups that have
 * been ignored */
typedef struct _ignored {
  struct _ignored *next;
  int group;
  int count;
} Ignored;

/* Entity typedefs */
typedef struct _point {
  Common common;
  float pt[3];
  float angle;
} PointEntity;

typedef struct _line {
  Common common;
  float start[3];
  float end[3];
} LineEntity;

typedef struct _circle {
  Common common;
  float center[3];
  float radius;
} Circle;

typedef struct _arc {
  Common common;
  float center[3];
  float radius;
  float start_angle;
  float end_angle;
} Arc;

typedef struct _solid {
  Common common;
  float pt1[3];
  float pt2[3];
  float pt3[3];
  float pt4[3];
} Solid;

typedef struct _trace {
  Common common;
  float pt1[3];
  float pt2[3];
  float pt3[3];
  float pt4[3];
} Trace;

typedef struct _text {
  Common common;
  float pt[3];
  float height;
  char  val[25];
  float rot;
  float x_scale;
  float oblique;
  char  style[10];
  int   flags;
  int   just;
  float align_pt[3];
} Text;

typedef struct _shape {
  Common common;
  float pt[3];
  float size;
  char  name[10];
  float rot;
  float x_scale;
  float oblique;
} Shape;

typedef struct _endblock {
  Common common;
} EndBlock;

typedef struct _insert {
  Common common;
  int   attrs_follow;
  char  block_name[25];
  float translate[3];
  float scale[3];
  float rot;
  int   col_cnt;
  int	row_cnt;
  float col_space;
  float row_space;
} Insert;

typedef struct _attdef {
  Common common;
  float pt[3];
  float text_height;
  char  value[25];
  char  prompt[25];
  char  tag[25];
  int   attribute_flags;
  int   field_length;
  float text_rotation;
  float rel_x_scale;
  float obliquing_angle;
  char  text_style[10];
  int   text_generation_flags;
  int   horz_just;
  int   vert_just;
  float align_pt[3];
} Attdef;

typedef struct _attrib {
  Common common;
  float pt[3];
  float text_height;
  char  value[25];
  int   attribute_flags;
  int   field_length;
  float text_rotation;
  float rel_x_scale;
  float obliquing_angle;
  char  text_style[10];
  int   text_generation_flags;
  int   horz_just;
  int   vert_just;
  float align_pt[3];
} Attrib;

typedef struct _polyline {
  Common common;
  float elev[3];	/* v. 11 and later */
  int verts_follow;
#define PL_CLOSED_IN_M 		0x01
#define PL_CURVE_FIT_ADDED 	0x02
#define PL_SPLINE_FIT_ADDED 	0x04
#define PL_3D_POLYLINE 		0x08
#define PL_3D_MESH 		0x10
#define PL_CLOSED_IN_N 		0x20
#define PL_POLYFACE_MESH 	0x40
#define PL_USE_LINETYPE 	0x80
  int polyline_flags;
  float start_width;
  float end_width;
  int mesh_m;
  int mesh_n;
  int smooth_m;
  int smooth_n;
#define PL_NO_SMOOTH_SURFACE 	0
#define PL_QUADATIC_B_SPLINE 	5
#define PL_CUBIC_B_SPLINE 	6
#define PL_BEZIER_SURFACE 	8
  int smooth_type;
} Polyline;

typedef struct _vertex {
  Common common;
  float loc[3];
  float start_width;
  float end_width;
  float bulge;
#define VX_CURVE_FITTING_ADDED	0x01
#define VX_CURVE_FIT_TANGENT	0x02
#define VX_UNUSED 		0x04
#define VX_SPLINE_VERTEX_ADDED 	0x08
#define VX_SPLINE_FRAME_CONTROL 0x10
#define VX_3D_POLYLINE_VERT	0x20
#define VX_3D_POLYGON_MESH_VERT 0x40
#define VX_POLYFACE_MESH 	0x80
  int 	vert_flags;
  int   mesh_indicies[4];
  float tangent;
} VertexEntity;

typedef LineEntity Line3d;

typedef struct _face3d {
  Common common;
  float pt1[3];
  float pt2[3];
  float pt3[3];
  float pt4[3];
  int edge_vis_flag;
} Face3d;

typedef struct _dimension {
  Common common;
  char  name[25];
  float def_pt[3];
  float mid_pt[3];
  float ins_pt[3];
  int 	dim_type;
  int 	user_text_flag;
  float def_pt_linear[3];
  float def_pt_diameter[3];
  float def_pt_arc[3];
  float leader_length;
  float rotate_angle;
  int 	horizontal_direction;
} Dimension;

typedef struct _viewport {
  Common common;
  float center_pt[3];
  float width;
  float height;
  int viewport_status;
  int viewport_id;
  void *extention_data;
} Viewport;


/********* function prototypes **********/

/* common.c */
int  is_common		(int group);
void handle_common	(void *data, int group, char *value);
void common_defaults	(void *data);

/* dx.c */
Field grab_triangles	(Entity *e);
Field grab_lines	(Entity *e);
Xform build_dx_object	(Tree *tree, int members);
void  OutputToDX	(Tree *tree, char *filename);

/* entity.c */
void    process_entity_section	(FILE *fpin, List *e_list);
PFVP    get_snag_func		(char *value,int *entity);
PFV     get_output_func		(int entity);
char   *get_entity_name		(int entity);
void    add_block_to_entity	(Entity *e, Entity *block);
Entity *create_new_entity	(int type, void *data);
int     add_item_to_list	(List *list, Entity *item);
void    process_block_section	(FILE *fpin, List *b_list);
void    process_header_section	(FILE *fpin);

/* header.c */
PFVP  get_variable_snag_func		(int type);
int   add_variable_data			(char *variable_name, void *data);
int   get_type_from_variable_table	(char *variable_name);
void  print_var				(Variable *var);
void  print_header			(void);

/* ignored.c */
void     init_ignored_list		(void);
void     add_to_ignored_list		(int group);
void     print_ignored			(void);
Ignored *find_ignored			(int group);

/* print_funcs.c */
void print_blocks		(List *b_list);
void print_entities		(List *e_list);
void print_histogram		(List *e_list);
void do3dFaces			(Entity *e, Collection *c);
void doLines			(Entity *e, Collection *c);
void doPolyLines		(Entity *e, Collection *c);
void doTheRest			(Entity *e, Collection *c);
void print_group		(char *s, int group, char *value, int ignored);
void print_entity_name		(Entity *e);
void output_entity		(Entity *e, Collection *c);
void print_common		(Entity *e);
void print_block_name		(Entity *e);
int  isSamePoint		(float pt1[3], float pt2[3]);

/* snag_funcs.c */
void rewind_file(void);
int get_next_group		(FILE *fp, int required,
				 char *s, char *value, int *group, int *type);
int get_type_from_group		(int group);


/* tree.h */
Entity *find_block_entity	(List *b_list, char *name);
Block  *find_block		(List *b_list, char *name);
void    init_list		(List *list);
int     isTriangle		(Entity *e);
int     isPolyline		(Entity *e);
int     isLine			(Entity *e);
Tree   *create_new_tree		(char *name, int level, Entity *block);
Tree   *build_tree		(Tree *tree,
				 List *b_list, List *e_list, char *layer);
void    print_transform 	(float mat[4][4]);
void    print_tree		(Tree *tree);
void    print_layer_usage	(Tree *tree);

/* dxf.c */
void    process_table_section	(FILE *fpin);
int     which_file_section	(char *value);
Entity *add_top_level_block	(List *list, char *top_lev);
void    add_to_top_level	(List *b_list, List *e_list, char *top_lev);

/* colors.c */
int find_color(Entity *e, RGBColor *color);


/* table.c */
int InitTables(void);
void add_table(DXFTable *tab);
int 	  which_table_type	(char *value);
DXFTable *get_table		(FILE *fpin, char *s, char *value, int *group, int *type);
DXFTable *get_layer_table	(FILE *fpin, DXFTable *table);
int 	  get_int_from_table	(int which, char *rec_name, int field);
void 	  ignore_table		(FILE *fpin, char *s, char *value, int *group, int *type);


#endif
