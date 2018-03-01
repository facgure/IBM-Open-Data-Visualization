/*
 * Automatically generated on "/tmp/makex.mb" by DX Module Builder
 */

#include "dx/dx.h"

static Error traverse(Object *, Object *);
static Error doLeaf(Object *, Object *);

/*
 * Declare the interface routine.
 */
int
MakeX_worker(
    int, int, float *,
    int, float *,
    int, float *,
    int, float *);

Error
m_MakeX(Object *in, Object *out)
{
  int i;

  /*
   * Initialize all outputs to NULL
   */
  out[0] = NULL;

  /*
   * Error checks: required inputs are verified.
   */

  /* Parameter "input" is required. */
  if (in[0] == NULL)
  {
    DXSetError(ERROR_MISSING_DATA, "\"input\" must be specified");
    return ERROR;
  }


  /*
   * Since output "output" is structure Field/Group, it initially
   * is a copy of input "input".
   */
  out[0] = DXCopy(in[0], COPY_STRUCTURE);
  if (! out[0])
    goto error;

  /*
   * If in[0] was an array, then no copy is actually made - Copy 
   * returns a pointer to the input object.  Since this can't be written to
   * we postpone explicitly copying it until the leaf level, when we'll need
   * to be creating writable arrays anyway.
   */
  if (out[0] == in[0])
    out[0] = NULL;

  /*
   * Call the hierarchical object traversal routine
   */
  if (!traverse(in, out))
    goto error;

  return OK;

error:
  /*
   * On error, any successfully-created outputs are deleted.
   */
  for (i = 0; i < 1; i++)
  {
    if (in[i] != out[i])
      DXDelete(out[i]);
    out[i] = NULL;
  }
  return ERROR;
}


static Error
traverse(Object *in, Object *out)
{
  switch(DXGetObjectClass(in[0]))
  {
    case CLASS_FIELD:
    case CLASS_ARRAY:
      /*
       * If we have made it to the leaf level, call the leaf handler.
       */
      if (! doLeaf(in, out))
  	     return ERROR;

      return OK;

    case CLASS_GROUP:
    {
      int   i, j;
      int   memknt;
      Class groupClass  = DXGetGroupClass((Group)in[0]);

      DXGetMemberCount((Group)in[0], &memknt);


       /*
        * Create new in and out lists for each child
        * of the first input. 
        */
        for (i = 0; i < memknt; i++)
        {
          Object new_in[2], new_out[1];

         /*
          * For all inputs that are Values, pass them to 
          * child object list.  For all that are Field/Group, get 
          * the appropriate decendent and place it into the
          * child input object list.
          */

          /* input "input" is Field/Group */
          if (in[0])
            new_in[0] = DXGetEnumeratedMember((Group)in[0], i, NULL);
          else
            new_in[0] = NULL;

          /* input "size" is Value */
          new_in[1] = in[1];

         /*
          * For all outputs that are Values, pass them to 
          * child object list.  For all that are Field/Group,  get
          * the appropriate decendent and place it into the
          * child output object list.  Note that none should
          * be NULL (unlike inputs, which can default).
          */

          /* output "output" is Field/Group */
          new_out[0] = DXGetEnumeratedMember((Group)out[0], i, NULL);

          if (! traverse(new_in, new_out))
            return ERROR;

         /*
          * Now for each output that is not a Value, replace
          * the updated child into the object in the parent.
          */

          /* output "output" is Field/Group */
          DXSetEnumeratedMember((Group)out[0], i, new_out[0]);

        }
      return OK;
    }

    case CLASS_XFORM:
    {
      int    i, j;
      Object new_in[2], new_out[1];


      /*
       * Create new in and out lists for the decendent of the
       * first input.  For inputs and outputs that are Values
       * copy them into the new in and out lists.  Otherwise
       * get the corresponding decendents.
       */

      /* input "input" is Field/Group */
      if (in[0])
        DXGetXformInfo((Xform)in[0], &new_in[0], NULL);
      else
        new_in[0] = NULL;

      /* input "size" is Value */
      new_in[1] = in[1];

      /*
       * For all outputs that are Values, copy them to 
       * child object list.  For all that are Field/Group,  get
       * the appropriate decendent and place it into the
       * child output object list.  Note that none should
       * be NULL (unlike inputs, which can default).
       */

      /* output "output" is Field/Group */
      DXGetXformInfo((Xform)out[0], &new_out[0], NULL);

      if (! traverse(new_in, new_out))
        return ERROR;

      /*
       * Now for each output that is not a Value replace
       * the updated child into the object in the parent.
       */

      /* output "output" is Field/Group */
      DXSetXformObject((Xform)out[0], new_out[0]);

      return OK;
    }

    case CLASS_SCREEN:
    {
      int    i, j;
      Object new_in[2], new_out[1];


      /*
       * Create new in and out lists for the decendent of the
       * first input.  For inputs and outputs that are Values
       * copy them into the new in and out lists.  Otherwise
       * get the corresponding decendents.
       */

      /* input "input" is Field/Group */
      if (in[0])
        DXGetScreenInfo((Screen)in[0], &new_in[0], NULL, NULL);
      else
        new_in[0] = NULL;

      /* input "size" is Value */
      new_in[1] = in[1];


      /*
       * For all outputs that are Values, copy them to 
       * child object list.  For all that are Field/Group,  get
       * the appropriate decendent and place it into the
       * child output object list.  Note that none should
       * be NULL (unlike inputs, which can default).
       */

       /* output "output" is Field/Group */
       DXGetScreenInfo((Screen)out[0], &new_out[0], NULL, NULL);

       if (! traverse(new_in, new_out))
         return ERROR;

      /*
       * Now for each output that is not a Value, replace
       * the updated child into the object in the parent.
       */

      /* output "output" is Field/Group */
       DXSetScreenObject((Screen)out[0], new_out[0]);

       return OK;
     }

     case CLASS_CLIPPED:
     {
       int    i, j;
       Object new_in[2], new_out[1];


       /* input "input" is Field/Group */
       if (in[0])
         DXGetClippedInfo((Clipped)in[0], &new_in[0], NULL);
       else
         new_in[0] = NULL;

       /* input "size" is Value */
       new_in[1] = in[1];


      /*
       * For all outputs that are Values, copy them to 
       * child object list.  For all that are Field/Group,  get
       * the appropriate decendent and place it into the
       * child output object list.  Note that none should
       * be NULL (unlike inputs, which can default).
       */

       /* output "output" is Field/Group */
       DXGetClippedInfo((Clipped)out[0], &new_out[0], NULL);

       if (! traverse(new_in, new_out))
         return ERROR;

      /*
       * Now for each output that is not a Value, replace
       * the updated child into the object in the parent.
       */

       /* output "output" is Field/Group */
       DXSetClippedObjects((Clipped)out[0], new_out[0], NULL);

       return OK;
     }

     default:
     {
       DXSetError(ERROR_BAD_CLASS, "encountered in object traversal");
       return ERROR;
     }
  }
}

static int
doLeaf(Object *in, Object *out)
{
  int i, result=0;
  Array array;
  Field field;
  Pointer *in_data[2], *out_data[1];
  int in_knt[2], out_knt[1];
  Type type;
  Category category;
  int rank, shape;
  Object attr, src_dependency_attr = NULL;
  char *src_dependency = NULL;
  /*
   * Irregular positions info
   */
  int p_knt, p_dim;
  float *p_positions;
  int c_knt = -1;
 
  /* User added declarations */
  Point *out_pos_ptr, inpoint;
  Array connections=NULL, positions=NULL;
  Line *conn_ptr;
  float size;


  /*
   * positions and/or connections are required, so the first must
   * be a field.
   */
  if (DXGetObjectClass(in[0]) != CLASS_FIELD)
  {
      DXSetError(ERROR_DATA_INVALID,
           "positions and/or connections unavailable in array object");
      goto error;
  }
  else
  {

    field = (Field)in[0];

    if (DXEmptyField(field))
      return OK;

    /* 
     * Determine the dependency of the source object's data
     * component.
     */
    src_dependency_attr = DXGetComponentAttribute(field, "data", "dep");
    if (! src_dependency_attr)
    {
      DXSetError(ERROR_MISSING_DATA, "\"input\" data component is missing a dependency attribute");
      goto error;
    }

    if (DXGetObjectClass(src_dependency_attr) != CLASS_STRING)
    {
      DXSetError(ERROR_BAD_CLASS, "\"input\" dependency attribute");
      goto error;
    }

    src_dependency = DXGetString((String)src_dependency_attr);

    array = (Array)DXGetComponentValue(field, "positions");
    if (! array)
    {
      DXSetError(ERROR_BAD_CLASS, "\"input\" contains no positions component");
      goto error;
    }

    /* 
     * The user requested irregular positions.  So we
     * get the count, the dimensionality and a pointer to the
     * explicitly enumerated positions.  If the positions
     * are in fact regular, this will expand them.
     */
    DXGetArrayInfo(array, &p_knt, NULL, NULL, NULL, &p_dim);

    p_positions = (float *)DXGetArrayData(array);
    if (! p_positions)
      goto error;

 }

/* New User code starts here */

/* 
 * Make the new positions array for the output. The positions are
 * three-dimensional. 
 */
   positions = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);
   if (! positions)
      goto error;


/* 
 * Check that the input positions are three dimensional: 
 */
   if (p_dim != 3) {
      DXSetError(ERROR_DATA_INVALID,"input positions must be 3-dimensional");
      goto error;
   }

/* 
 * Allocate space in the new positions array. We need four positions for
 * every input position (the four points making up the "x") 
 */
   if (! DXAddArrayData(positions, 0, 4*p_knt, NULL))
      goto error;

/* 
 * Get a pointer to the output positions 
 */
   out_pos_ptr  = (Point *)DXGetArrayData(positions);

/* Make a connections component for the output. The connections are
 * two-dimensional (lines) 
 */
   connections = DXNewArray(TYPE_INT, CATEGORY_REAL, 1, 2);

/* Allocate space in the new connections array. There are two lines for
 * each input position. */
   if (! DXAddArrayData(connections, 0, 2*p_knt, NULL))
      goto error;
   DXSetAttribute((Object)connections, "element type",
                  (Object)DXNewString("lines"));

/* Get a pointer to the new connections */
   conn_ptr = (Line *)DXGetArrayData(connections);

/* Get the size of the "x" */
   DXExtractFloat(in[1], &size);

/* Now "draw" the x's */
   for (i=0; i< p_knt; i++) {
       inpoint = DXPt(p_positions[3*i], p_positions[3*i+1], p_positions[3*i+2]);
       out_pos_ptr[4*i]   = DXPt(inpoint.x - size, inpoint.y, inpoint.z);
       out_pos_ptr[4*i+1] = DXPt(inpoint.x + size, inpoint.y, inpoint.z);
       out_pos_ptr[4*i+2] = DXPt(inpoint.x, inpoint.y - size, inpoint.z);
       out_pos_ptr[4*i+3] = DXPt(inpoint.x, inpoint.y + size, inpoint.z);

       conn_ptr[2*i] = DXLn(4*i, 4*i+1);
       conn_ptr[2*i+1] = DXLn(4*i+2, 4*i+3);
   }

/* 
 * Clean up; we're about to significantly modify the positions and connections 
 */
   DXChangedComponentStructure((Field)out[0],"positions");
   DXChangedComponentStructure((Field)out[0],"connections");

/* Now place the new positions and connections in the output field */
   DXSetComponentValue((Field)out[0], "positions", (Object)positions);
   positions = NULL;
   DXSetComponentValue((Field)out[0], "connections", (Object)connections);
   connections = NULL;

/* Finalize the field */
   DXEndField((Field)out[0]);


/* return */
   return OK;
error:
   DXDelete((Object)positions);
   DXDelete((Object)connections);
   return ERROR;


}

