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

static VariableTable variables[] = {
  {"$ACADVER",	  V_STRING,	NULL,	"the AutoCAD drawing database version number."},
  {"$ANGBASE",	  V_FLOAT,	NULL,	"Angle 0 direction."},
  {"$ANGDIR",	  V_INTEGER,	NULL,	"1=clockwise angles, 0=counterclockwise."},
  {"$ATTDIA",	  V_INTEGER,	NULL,	"Attribute entry dialogues, 1 = on, 0 = off    "},
  {"$ATTMODE",	  V_INTEGER,	NULL,	"Attribute visibility: 0=none, 1=normal, 2=all."},
  {"$ATTREQ",	  V_INTEGER,	NULL,	"Attribute prompting during INSERT, 1=on, 0=off."},
  {"$AUNITS",	  V_INTEGER,	NULL,	"UNITS format for angles."},
  {"$AUPREC",	  V_INTEGER,	NULL,	"UNITS precision for angles."},
  {"$AXISMODE",	  V_INTEGER,	NULL,	"axis on if nonzero."},
  {"$AXISUNIT",	  V_2VECTOR,	NULL,	"axis X and Y tick spacing."},
  {"$BLIPMODE",	  V_INTEGER,	NULL,	"blip mode on if nonzero."},
  {"$CECOLOR",	  V_INTEGER,	NULL,	"entity color number; 0 = BYBLOCK, 256 = BYLAYER."},
  {"$CELTYPE",	  V_STRING,	NULL,	"entity linetype name, or BYBLOCK or BYLAYER."},
  {"$CHAMFERA",	  V_FLOAT,	NULL,	"first chamfer distance."},
  {"$CHAMFERB",	  V_FLOAT,	NULL,	"second chamfer distance."},
  {"$CLAYER",	  V_STRING,	NULL,	"current layer name."},
  {"$COORDS",	  V_INTEGER,	NULL,	"0=coord disp, 1=cont update, 2=d<a format."},
  {"$DIMALT",	  V_INTEGER,	NULL,	"alternate unit dimension performed if nonzero."},
  {"$DIMALTD",	  V_INTEGER,	NULL,	"alternate unit decimal places."},
  {"$DIMALTF",	  V_FLOAT,	NULL,	"alternate unit scale factor."},
  {"$DIMAPOST",	  V_STRING,	NULL,	"alternate dimensioning suffix    "},
  {"$DIMASO",	  V_INTEGER,	NULL,	"1=create ass dimensioning, 0=draw ind entities."},
  {"$DIMASZ",	  V_FLOAT,	NULL,	"dimensioning arrow size."},
  {"$DIMBLK",	  V_STRING,	NULL,	"arrow block name."},
  {"$DIMBLK1",	  V_STRING,	NULL,	"first arrow block name."},
  {"$DIMBLK2",	  V_STRING,	NULL,	"second arrow block name."},
  {"$DIMCEN",	  V_FLOAT,	NULL,	"size of center mark/lines."},
  {"$DIMDLE",	  V_FLOAT,	NULL,	"dimension line extension."},
  {"$DIMDLI",	  V_FLOAT,	NULL,	"dimension line increment."},
  {"$DIMEXE",	  V_FLOAT,	NULL,	"extension line extension."},
  {"$DIMEXO",	  V_FLOAT,	NULL,	"extension line offset."},
  {"$DIMLFAC",	  V_FLOAT,	NULL,	"linear measurements scale factor."},
  {"$DIMLIM",	  V_INTEGER,	NULL,	"dimension limits generated if nonzero."},
  {"$DIMPOST",	  V_STRING,	NULL,	"general dimensioning suffix."},
  {"$DIMRND",	  V_FLOAT,	NULL,	"rounding value for dimension distances."},
  {"$DIMSAH",	  V_INTEGER,	NULL,	"use separate arrow blocks if nonzero."},
  {"$DIMSCALE",	  V_FLOAT,	NULL,	"overall dimensioning scale factor."},
  {"$DIMSE1",	  V_INTEGER,	NULL,	"first extension line suppressed if nonzero."},
  {"$DIMSE2",	  V_INTEGER,	NULL,	"second extension line suppressed if nonzero."},
  {"$DIMSHO",	  V_INTEGER,	NULL,	"1=Recompute dims while drag, 0=drag orig image."},
  {"$DIMSOXD",	  V_INTEGER,	NULL,	"suppress outside-ext dims lines if nonzero."},
  {"$DIMTAD",	  V_INTEGER,	NULL,	"text above dimension line if nonzero."},
  {"$DIMTIH",	  V_INTEGER,	NULL,	"text inside horizontal if nonzero."},
  {"$DIMTIX",	  V_INTEGER,	NULL,	"force text inside extensions if nonzero."},
  {"$DIMTM",	  V_FLOAT,	NULL,	"minus tolerance."},
  {"$DIMTOFL",	  V_INTEGER,	NULL,	"if otuside exts, force line ext if nonzero."},
  {"$DIMTOH",	  V_INTEGER,	NULL,	"text outside horizontal if nonzero."},
  {"$DIMTOL",	  V_INTEGER,	NULL,	"dimension tolerances generated if nonzero."},
  {"$DIMTP",	  V_FLOAT,	NULL,	"plus tolerance."},
  {"$DIMTSZ",	  V_FLOAT,	NULL,	"dimensioning tick size: 0=no ticks."},
  {"$DIMTVP",	  V_FLOAT,	NULL,	"text vertical position."},
  {"$DIMTXT",	  V_FLOAT,	NULL,	"dimensioning text height."},
  {"$DIMZIN",	  V_INTEGER,	NULL,	"zero suppression for feet & inch dimensions."},
  {"$DRAGMODE",	  V_INTEGER,	NULL,	"0=off, 1=on, 2=auto."},
  {"$ELEVATION",  V_FLOAT,	NULL,	"current elevation set by ELEV command."},
  {"$EXTMAX",	  V_3VECTOR,	NULL,	"XY drawing extents upper right corner (in WCS)."},
  {"$EXTMIN",	  V_3VECTOR,	NULL,	"XY drawing extents lower left corner (in WCS)."},
  {"$FILLETRAD",  V_FLOAT,	NULL,	"fillet radius."},
  {"$FILLMODE",	  V_INTEGER,	NULL,	"FILL mode on if nonzero."},
  {"$FLATLAND",	  V_INTEGER,	NULL,	"force compatibility with older vers if nonzero."},
  {"$HANDLING",	  V_INTEGER,	NULL,	"handles enabled if nonzero."},
  {"$HANDSEED",	  V_STRING,	NULL,	"next available handle."},
  {"$INSBASE",	  V_3VECTOR,	NULL,	"insertion base set by BASE command (in WCS)."},
  {"$LIMCHECK",	  V_INTEGER,	NULL,	"nonzero if limits checking is on."},
  {"$LIMMAX",	  V_2VECTOR,	NULL,	"XY drawing limits upper right corner (in WCS)."},
  {"$LIMMIN",	  V_2VECTOR,	NULL,	"XY drawing limits lower left corner (in WCS)."},
  {"$LTSCALE",	  V_FLOAT,	NULL,	"global linetype scale."},
  {"$LUNITS",	  V_INTEGER,	NULL,	"UNITS format for coordinates and distances."},
  {"$LUPREC",	  V_INTEGER,	NULL,	"UNITS precision for coordinates and distances."},
  {"$MENU",	  V_STRING,	NULL,	"name of menu file."},
  {"$MIRRTEXT",	  V_INTEGER,	NULL,	"MIRROR text if nonzero."},
  {"$ORTHOMODE",  V_INTEGER,	NULL,	"ORTHO mode on if nonzero."},
  {"$OSMODE",	  V_INTEGER,	NULL,	"running object snap modes."},
  {"$PDMODE",	  V_INTEGER,	NULL,	"point display mode."},
  {"$PDSIZE",	  V_FLOAT,	NULL,	"point display size."},
  {"$PLINEWID",	  V_FLOAT,	NULL,	"default Polyline width."},
  {"$QTEXTMODE",  V_INTEGER,	NULL,	"quick text mode on if nonzero."},
  {"$REGENMODE",  V_INTEGER,	NULL,	"REGENAUTO mode on if nonzero."},
  {"$SKETCHINC",  V_FLOAT,	NULL,	"sketch record increment."},
  {"$SKPOLY",	  V_INTEGER,	NULL,	"0=sketch lines, 1=sketch polylines."},
  {"$SPLFRAME",	  V_INTEGER,	NULL,	"spline control polygon display, 1 = on, 0 = off."},
  {"$SPLINESEGS", V_INTEGER,	NULL,	"number of line segments per spline patch."},
  {"$SPLINETYPE", V_INTEGER,	NULL,	"spline curve type for PEDIT Spline."},
  {"$SURFTAB1",	  V_INTEGER,	NULL,	"number of mesh tabulations in first direction."},
  {"$SURFTAB2",	  V_INTEGER,	NULL,	"number of mesh tabulations in second direction."},
  {"$SURFTYPE",	  V_INTEGER,	NULL,	"surface type for PEDIT Smooth."},
  {"$SURFU",	  V_INTEGER,	NULL,	"surface density in M direction."},
  {"$SURFV",	  V_INTEGER,	NULL,	"surface density in N direction."},
  {"$TDCREATE",	  V_FLOAT,	NULL,	"date/time of drawing creation."},
  {"$TDINDWG",	  V_FLOAT,	NULL,	"cumulative editing time for this drawing."},
  {"$TDUPDATE",	  V_FLOAT,	NULL,	"date/time of last drawing update."},
  {"$TDUSRTIMER", V_FLOAT,	NULL,	"user elapsed timer."},
  {"$TEXTSIZE",	  V_FLOAT,	NULL,	"default text height."},
  {"$TEXTSTYLE",  V_STRING,	NULL,	"current text style name."},
  {"$THICKNESS",  V_FLOAT,	NULL,	"current thickness set by ELEV command."},
  {"$TRACEWID",	  V_FLOAT,	NULL,	"default Trace width."},
  {"$UCSNAME",	  V_STRING,	NULL,	"Name of current UCS."},
  {"$UCSORG",	  V_3VECTOR,	NULL,	"origin of current UCS (in WCS)."},
  {"$UCSXDIR",	  V_3VECTOR,	NULL,	"direction of current UCSs X axis."},
  {"$UCSYDIR",	  V_3VECTOR,	NULL,	"direction of current UCSs Y axis."},
  {"$USERI1",	  V_INTEGER,	NULL,	"user integer 1."},
  {"$USERI2",	  V_INTEGER,	NULL,	"user integer 2."},
  {"$USERI3",	  V_INTEGER,	NULL,	"user integer 3."},
  {"$USERI4",	  V_INTEGER,	NULL,	"user integer 4."},
  {"$USERI5",	  V_INTEGER,	NULL,	"user integer 5."},
  {"$USERR1",	  V_FLOAT,	NULL,	"user real 1."},
  {"$USERR2",	  V_FLOAT,	NULL,	"user real 2."},
  {"$USERR3",	  V_FLOAT,	NULL,	"user real 3."},
  {"$USERR4",	  V_FLOAT,	NULL,	"user real 4."},
  {"$USERR5",	  V_FLOAT,	NULL,	"user real 5."},
  {"$USRTIMER",	  V_INTEGER,	NULL,	"0=timer off, 1=timer on."},
  {"$WORLDVIEW",  V_INTEGER,	NULL,	"1=set UCS/WCS DVIEW/VPOINT, 0=dont change UCS."},
  {"$DIMCLRD",	  V_INTEGER,    NULL,   "Dimension line color"},
  {"$DIMCLRE", 	  V_INTEGER,	NULL,	"Dimension extension line color"},
  {"$DIMCLRT", 	  V_INTEGER,	NULL,	"Dimension text color"},
  {"$DIMGAP", 	  V_FLOAT, 	NULL,	"Dimension line gap"},
  {"$DIMSTYLE",   V_STRING,	NULL,	"Dimension style name"},
  {"$DIMTFAC", 	  V_FLOAT,	NULL,	"Dimension tolerance display scale"},
  {"$DWGCODEPAGE",V_STRING,     NULL, 	"Drawing code page."},
  {"$MAXACTVP",   V_INTEGER,	NULL,	"Sets maximum number of viewports"},
  {"$PELEVATION", V_FLOAT,	NULL,	"Current paper space elevation "},
  {"$PEXTMAX", 	  V_3VECTOR,  	NULL,	"Maximum X, Y, and Z paper space"},
  {"$PEXTMIN", 	  V_3VECTOR,  	NULL,	"Minimum X, Y, and Z paper space"},
  {"$PLIMCHECK",  V_INTEGER,	NULL,	"Limits checking in paper space"},
  {"$PLIMMAX", 	  V_2VECTOR, 	NULL,	"Maximum X and Y limits in paper space"},
  {"$PLIMMIN", 	  V_2VECTOR, 	NULL,	"Minimum X and Y limits in paper space"},
  {"$PLINEGEN",   V_INTEGER,	NULL,	"Governs the generation of linetype"},
  {"$PSLTSCALE",  V_INTEGER,	NULL,	"Controls paper space linetype"},
  {"$PUCSNAME",   V_STRING,	NULL,	"Current paper space UCS name"},
  {"$PUCSORG", 	  V_3VECTOR,  	NULL,	"Current paper space UCS origin"},
  {"$PUCSXDIR",   V_3VECTOR, 	NULL,	"Current paper space UCS X axis"},
  {"$PUCSYDIR",   V_3VECTOR, 	NULL,	"Current paper space UCS Y axis"},
  {"$SHADEDGE",   V_INTEGER,	NULL,	"Shade edge info"},
  {"$SHADEDIF",   V_INTEGER,	NULL,	"Percent ambient/diffuse light"},
  {"$SKETCHINC",  V_FLOAT,	NULL,	"Sketch record increment"},
  {"$SKPOLY", 	  V_INTEGER,	NULL,	"0 = sketch lines, 1 = sketch"},
  {"$TILEMODE",   V_INTEGER,	NULL,	"1 for previous release"},
  {"$UNITMODE",   V_INTEGER,	NULL,	"Low bit set = display fractions"},
  {"$VISRETAIN",  V_INTEGER,	NULL,	"0 = don't retain Xref-dependent"},
  {"$WORLDVIEW",  V_INTEGER,	NULL,	"1 = set UCS to WCS during"},
  /* Additional variables found in nceres.dxf */
  {"$PINSBASE",	  V_3VECTOR, 	NULL, 	""},
  /* Additional variables found in klingon.dxf */
  {"$TREEDEPTH",  V_INTEGER, 	NULL,	""},
  /* Additional varaibles found in entrpris.dxf */
  {"$VIEWCTR",    V_2VECTOR,	NULL,	""},
  {"$VIEWSIZE",   V_FLOAT,	NULL, 	""},
  {"$SNAPMODE",   V_INTEGER,	NULL, 	""},
  {"$SNAPUNIT",   V_2VECTOR,	NULL, 	""},
  {"$SNAPBASE",   V_2VECTOR,	NULL, 	""},
  {"$SNAPANG",    V_FLOAT,	NULL, 	""},
  {"$SNAPSTYLE",  V_INTEGER,	NULL, 	""},
  {"$SNAPISOPAIR",V_INTEGER,	NULL, 	""},
  {"$GRIDMODE",   V_INTEGER,	NULL, 	""},
  {"$GRIDUNIT",   V_2VECTOR,	NULL, 	""},
  {"$FASTZOOM",   V_INTEGER,	NULL, 	""},
  {"$VIEWDIR",    V_3VECTOR,	NULL, 	""},
};
static nVariables = sizeof(variables)/sizeof(VariableTable);

void *ignore_var	(FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_2vector	(FILE *fpin, char *s, char *value, int *group, int *type);
void *snag_3vector	(FILE *fpin, char *s, char *value, int *group, int *type);

static Table variable_type_table[] = {
  {"",	V_STRING,	snag_string,	NULL},
  {"",	V_2VECTOR,	snag_2vector,	NULL},
  {"",	V_3VECTOR,	snag_3vector,	NULL},
  {"",	V_FLOAT,	snag_float,	NULL},
  {"",	V_INTEGER,	snag_integer,	NULL},
  {"",	V_UNKNOWN,      ignore_var,     NULL}
};
static nVartypes = sizeof(variable_type_table)/sizeof(Table);

/*
 * Get a snag function to read in a varaible
 */
PFVP get_variable_snag_func(int type)
{
  int i;

  for (i=0;i<nVartypes;i++) {
    if (type == variable_type_table[i].value) {
      return variable_type_table[i].snag_func;
    }
  }
  return ignore_var;
}

/*
 * Add a variable's data to the variable table
 */
int add_variable_data(char *variable_name, void *data)
{
  int i;

  for (i=0;i<nVariables;i++) {
    if (strstr(variables[i].name,variable_name)) {
      /* Only return exact matches */
      if (strstr(variable_name, variables[i].name)) {
	variables[i].data = data;
	return TRUE;
      }
    }
  }
  return FALSE;
}

/*
 * What type of variable is this?
 */
int get_type_from_variable_table(char *variable_name)
{
  int i;

  for (i=0;i<nVariables;i++) {
    if (strstr(variables[i].name, variable_name)) {
      /* Only return exact matches */
      if (strstr(variable_name, variables[i].name)) {
	return variables[i].var_type;
      }
    }
  }
  return V_UNKNOWN;
}

/*
 * Ignore an entire variable definition in the file
 */
void *ignore_var(FILE *fpin, char *s, char *value, int *group, int *type)
{

  /* Spin until the next varaible definition or end section */
  print_group("Header: Ignored varaible defintion: ", *group, value, IGNORE);
  get_next_group(fpin, REQUIRED, s, value, group, type);

  while ((*group != G_VARIABLE_NAME) && (*group != G_FILE_SEP)) {
    print_group("Header: Ignored group:              ", *group, value, IGNORE);
    get_next_group(fpin, REQUIRED, s, value, group, type);
  }

  rewind_file();
  return (void *)NULL;
}

/*
 * Snag a string type variable
 */
void *snag_string(FILE *fpin, char *s, char *value, int *group, int *type)
{
  char *item;

  get_next_group(fpin, REQUIRED, s, value, group, type);

  if (*group > 9 || *group < 1) {
    /* Did not find a string */
    rewind_file();
    return NULL;
  }

  /* This is a malloc!!!!! */
  item = strdup(value);
  if (!item) {
    fprintf(stderr,"malloc failed\n");
    exit(0);
  }
  
  return (void *)item;
}

/*
 * Snag a single float type variable
 */
void *snag_float(FILE *fpin, char *s, char *value, int *group, int *type)
{
  float *item;

  get_next_group(fpin, REQUIRED, s, value, group, type);

  if (*group > 59 || *group < 10) {
    rewind_file();
    return NULL;
  }
  
  item = (float *)malloc(sizeof(float));
  if (!item) {
    fprintf(stderr,"malloc failed\n");
    exit(0);
  }

  *item = atof(value);
  return (void *)item;
}

/*
 * Snag a single int type variable
 */
void *snag_integer(FILE *fpin, char *s, char *value, int *group, int *type)
{
  int *item;

  get_next_group(fpin, REQUIRED, s, value, group, type);

  if (*group > 79 || *group < 60) {
    rewind_file();
    return NULL;
  }

  item = (int *)malloc(sizeof(int));
  if (!item) {
    fprintf(stderr,"malloc failed\n");
    exit(0);
  }
  
  *item = strtol(value,NULL,0);
  return (void *)item;
}

/*
 * Snag a two floats type variable
 */
void *snag_2vector(FILE *fpin, char *s, char *value, int *group, int *type)
{
  float *f;
  float *item;

  item = (float *)malloc(sizeof(float)*2);
  if (!item) {
    fprintf(stderr,"malloc failed\n");
    exit(0);
  }
  
  f = (float *)snag_float(fpin, s, value, group, type);
  if (f) item[0] = *f; else item[0] = 0.0;
  f = (float *)snag_float(fpin, s, value, group, type);
  if (f) item[1] = *f; else item[1] = 0.0;

  return (void *)item;
}

/*
 * Snag a three floats type variable
 */
void *snag_3vector(FILE *fpin, char *s, char *value, int *group, int *type)
{
  float *f;
  float *item;

  item = (float *)malloc(sizeof(float)*3);
  if (!item) {
    fprintf(stderr,"malloc failed\n");
    exit(0);
  }

  f = (float *)snag_float(fpin, s, value, group, type);
  if (f) item[0] = *f; else item[0] = 0.0;
  f = (float *)snag_float(fpin, s, value, group, type);
  if (f) item[1] = *f; else item[1] = 0.0;
  f = (float *)snag_float(fpin, s, value, group, type);
  if (f) item[2] = *f; else item[2] = 0.0;

  return (void *)item;
}

/*
 * For debugging: print the value and name of a variable.
 */
void print_var(Variable *var)
{
  int i;
  float f[3];

  printf("  %-10.10s (%-30.30s): ", var->name, var->comment);
  switch (var->var_type) {
  case V_FLOAT:
    f[0] = *(float *)var->data;
    printf("\t%5.5f\n", f[0]);
    break;
  case V_INTEGER:
    i = *(int *)var->data;
    printf("\t%d\n", i);
    break;
  case V_STRING:
    printf("\t%-20.20s\n", (char *)var->data);
    break;
  case V_2VECTOR:
    f[0] = ((float *)var->data)[0];
    f[1] = ((float *)var->data)[1];
    printf("\t[%5.5f %5.5f]\n",
	   f[0], f[1]);
    break;
  case V_3VECTOR:
    f[0] = ((float *)var->data)[0];
    f[1] = ((float *)var->data)[1];
    f[2] = ((float *)var->data)[2];
    printf("\t[%5.5f %5.5f %5.5f]\n",
	   f[0], f[1], f[2]);
    break;
  }
}

/*
 * For debugging: print all variables found in the header section of the file.
 */
void print_header(void)
{
  int i;

  printf("\n--Varaibles defined in header section----------------------------\n");
  printf("\n   Name     Comment                       Value\n");
  printf("--------------------------------------------------------------------\n");

  for (i=0;i<nVariables;i++) {
    if (variables[i].data != NULL) {
      print_var(&variables[i]);
    }
  }
  printf("\n\n");
}

