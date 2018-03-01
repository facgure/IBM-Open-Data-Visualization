/*
 * Automatically generated on "/tmp/makexeff.mb" by DX Module Builder
 */

#include "dx/dx.h"

static Error traverse(Object *, Object *);
static Error doLeaf(Object *, Object *);

/*
 * Declare the interface routine.
 */
int
MakeXEfficient_worker(
    int, int, float *,
    int, float *,
    int, float *,
    int, float *);

Error
m_MakeXEfficient(Object *in, Object *out)
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

  /* User-added declarations */
  float *scratch, *in_ptr, size;
  Point inpoint, *out_pos_ptr;
  ArrayHandle handle;
  Array connections;
  Line *conn_ptr;

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

    /* change to doLeaf so that regular positions are not expanded */

    if (!(handle = DXCreateArrayHandle(array)))
       goto error;

    scratch = DXAllocate(3*sizeof(float));
    if (!scratch)
       goto error;

    DXGetArrayInfo(array, &p_knt, NULL, NULL, NULL, &p_dim);


 }

/* New User code starts here */

/* Make the new positions array for the output */
   array = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);
   if (! array)
      goto error;


/* Check that the positions are three dimensional: */
   if (p_dim != 3) {
      DXSetError(ERROR_DATA_INVALID,"input positions must be 3-dimensional");
      goto error;
   }
/* Allocate space in the new positions array */
   if (! DXAddArrayData(array, 0, 4*p_knt, NULL))
      goto error;

/* Get a pointer to the output positions */
   out_pos_ptr  = (Point *)DXGetArrayData(array);


/* Make a connections component for the output */
   connections = DXNewArray(TYPE_INT, CATEGORY_REAL, 1, 2);

/* Allocate space in the new connections array */
   if (! DXAddArrayData(connections, 0, 2*p_knt, NULL))
      goto error;
   DXSetAttribute((Object)connections, "element type",
                  (Object)DXNewString("lines"));
/* Get a pointer to the new connections */
   conn_ptr = (Line *)DXGetArrayData(connections);

/* Now "draw" the x's */
   for (i=0; i< p_knt; i++) {
       /* the following line accesses the position via the
        * array handling routines 
        */
       in_ptr = (float *)DXIterateArray(handle, i, in_ptr, scratch);
       inpoint = DXPt(in_ptr[0], in_ptr[1], in_ptr[2]);
       DXExtractFloat(in[1], &size);
       out_pos_ptr[4*i]   = DXPt(inpoint.x - size, inpoint.y, inpoint.z);
       out_pos_ptr[4*i+1] = DXPt(inpoint.x + size, inpoint.y, inpoint.z);
       out_pos_ptr[4*i+2] = DXPt(inpoint.x, inpoint.y - size, inpoint.z);
       out_pos_ptr[4*i+3] = DXPt(inpoint.x, inpoint.y + size, inpoint.z);

       conn_ptr[2*i] = DXLn(4*i, 4*i+1);
       conn_ptr[2*i+1] = DXLn(4*i+2, 4*i+3);
   }

/* Clean up; we're about to significantly modify the positions and connections
 */
   DXChangedComponentStructure((Field)out[0],"positions");
   DXChangedComponentStructure((Field)out[0],"connections");

/* Now place the new positions and connections in the output field */
   DXSetComponentValue((Field)out[0], "positions", (Object)array);
   DXSetComponentValue((Field)out[0], "connections", (Object)connections);

/* Finalize the field */
   DXEndField((Field)out[0]);

/* Delete scratch and handle */
   DXFree((Pointer)scratch);
   DXFreeArrayHandle(handle);

/* return */
   return OK;
error:

/* Delete scratch and handle */
   DXFree((Pointer)scratch);
   DXFreeArrayHandle(handle);
   return ERROR;


}

int
MakeXEfficient_worker(
    int p_knt, int p_dim, float *p_positions,
    int input_knt, float *input_data,
    int size_knt, float *size_data,
    int output_knt, float *output_data)
{
  /*
   * The arguments to this routine are:
   *
   *  p_knt:           total count of input positions
   *  p_dim:           dimensionality of input positions
   *  p_positions:     pointer to positions list
   *
   * The following are inputs and therefore are read-only.  The default
   * values are given and should be used if the knt is 0.
   *
   * input_knt, input_data:  count and pointer for input "input"
   *                   no default value given.
   * size_knt, size_data:  count and pointer for input "size"
   *                   non-descriptive default value is "1"
   *
   *  The output data buffer(s) are writable.
   *  The output buffer(s) are preallocated based on
   *     the dependency (positions or connections),
   *     the size of the corresponding positions or
   *     connections component in the first input, and
   *     the data type.
   *
   * output_knt, output_data:  count and pointer for output "output"
   */

  /*
   * User's code goes here
   */
  
}
