/*
 * Automatically generated on "/tmp/averagecell.mb" by DX Module Builder
 */

#include "dx/dx.h"

static Error traverse(Object *, Object *);
static Error doLeaf(Object *, Object *);

/*
 * Declare the interface routine.
 */
int
AverageCell_worker(
   int, int, int *,
    int, float *,
    int, float *);

Error
m_AverageCell(Object *in, Object *out)
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
          Object new_in[1], new_out[1];

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
      Object new_in[1], new_out[1];


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
      Object new_in[1], new_out[1];


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
       Object new_in[1], new_out[1];


       /* input "input" is Field/Group */
       if (in[0])
         DXGetClippedInfo((Clipped)in[0], &new_in[0], NULL);
       else
         new_in[0] = NULL;


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
  Pointer *in_data[1], *out_data[1];
  int in_knt[1], out_knt[1];
  Type type;
  Category category;
  int rank, shape;
  Object attr, src_dependency_attr = NULL;
  char *src_dependency = NULL;
  Object element_type_attr;
  char *element_type;
  int p_knt = -1;
  /*
   * Regular connections info
   */
  int c_knt, c_nv, c_dim, *c_counts = NULL;

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

    DXGetArrayInfo(array, &p_knt, NULL, NULL, NULL, NULL);
    array = (Array)DXGetComponentValue(field, "connections");
    if (! array)
    {
      DXSetError(ERROR_BAD_CLASS, "\"input\" contains no connections component");
      goto error;
    }

    /*
     * Check that the field's element type matches that requested
     */
    element_type_attr = DXGetAttribute((Object)array, "element type");
    if (! element_type_attr)
    {
        DXSetError(ERROR_DATA_INVALID,
            "input \"input\" has no element type attribute");
        goto error;
    }

    if (DXGetObjectClass(element_type_attr) != CLASS_STRING)
    {
        DXSetError(ERROR_DATA_INVALID,
            "input \"input\" element type attribute is not a string");
        goto error;
    }

    if (strcmp(DXGetString((String)element_type_attr), "quads"))
    {
        DXSetError(ERROR_DATA_INVALID,
            "input \"input\" invalid element type");
        goto error;
    }

    /* 
     * Input[0] should have regular connections.  First check
     * that they are, and while you're at it, get the
     * dimensionality so we can size an array later.
     */
    if (! DXQueryGridConnections(array, &c_dim, NULL))
    {
      DXSetError(ERROR_BAD_CLASS, "\"input\" connections component is not regular");
      goto error;
    }

    /* 
     * Allocate arrays for connections counts.
     * Check that the allocation worked, then get the info.
     */
    c_counts = (int   *)DXAllocate(c_dim*sizeof(int));
    if (! c_counts)
      goto error;

    DXQueryGridConnections(array, NULL, c_counts);
    DXGetArrayInfo(array, &c_knt, NULL, NULL, NULL, &c_nv);

 }
  /*
   * If the input argument is not NULL then we get the 
   * data array: either the object itself, if its an 
   * array, or the data component if the argument is a field
   */
  if (! in[0])
  {
    array = NULL;
    in_data[0] = NULL;
    in_knt[0] = 0;
  }
  else
  {
    if (DXGetObjectClass(in[0]) == CLASS_ARRAY)
    {
      array = (Array)in[0];
    }
    else
    {
      if (DXGetObjectClass(in[0]) != CLASS_FIELD)
      {
        DXSetError(ERROR_BAD_CLASS, "\"input\" should be a field");
        goto error;
      }

      array = (Array)DXGetComponentValue((Field)in[0], "data");
      if (! array)
      {
        DXSetError(ERROR_MISSING_DATA, "\"input\" has no data component");
        goto error;
      }

      if (DXGetObjectClass((Object)array) != CLASS_ARRAY)
      {
        DXSetError(ERROR_BAD_CLASS, "data component of \"input\" should be an array");
        goto error;
      }
    }

    /* 
     * get the dependency of the data component
     */
    attr = DXGetAttribute((Object)array, "dep");
    if (! attr)
    {
      DXSetError(ERROR_MISSING_DATA, "data component of \"input\" has no dependency");
      goto error;
    }

    if (DXGetObjectClass(attr) != CLASS_STRING)
    {
      DXSetError(ERROR_BAD_CLASS, "dependency attribute of data component of \"input\"");
      goto error;
    }

  /*
   * The dependency of this arg should be connections
   */
    if (strcmp("connections", DXGetString((String)attr)))
    {
      DXSetError(ERROR_DATA_INVALID, "data dependency of \"input\" must be connections");
      goto error;
    }

    DXGetArrayInfo(array, &in_knt[0], &type, &category, &rank, &shape);
    if (type != TYPE_FLOAT || category != CATEGORY_REAL ||
        rank != 0)
    {
      DXSetError(ERROR_DATA_INVALID, "input \"input\"");
      goto error;
    }

    in_data[0] = DXGetArrayData(array);
    if (! in_data[0])
      goto error;

  }
  /*
   * Create an output data array typed according to the
   * specification given
   */
  array = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, 0);
  if (! array)
    goto error;

  /*
   * This output data array will be dep connections - and sized
   * appropriately - if the appropriate size is known
   */
  if (c_knt == -1)
  {
    DXSetError(ERROR_DATA_INVALID,
      "cannot make output \"output\" dep on connections because no connections were found in input \"input\"");
    goto error;
  }

  out_knt[0] = c_knt;

  if (! DXSetAttribute((Object)array, "dep", (Object)DXNewString("connections")))
    goto error;
  /*
   * Actually allocate the array data space
   */
  if (! DXAddArrayData(array, 0, out_knt[0], NULL))
    goto error;

  /*
   * If the output vector slot is not NULL, then it better be a field, and
   * we'll add the new array to it as its data component (delete any prior
   * data component so that its attributes won't overwrite the new component's)
   * Otherwise, place the new array into the out vector.
   */
  if (out[0])
  {
    if (DXGetObjectClass(out[0]) != CLASS_FIELD)
    {
      DXSetError(ERROR_INTERNAL, "non-field object found in output vector");
      goto error;
    }

    if (DXGetComponentValue((Field)out[0], "data"))
      DXDeleteComponent((Field)out[0], "data");

    if (! DXSetComponentValue((Field)out[0], "data", (Object)array))
      goto error;

  }
  else
    out[0] = (Object)array;
  /*
   * Now get the pointer to the contents of the array
   */
  out_data[0] = DXGetArrayData(array);
  if (! out_data[0])
    goto error;

  /*
   * Call the user's routine.  Check the return code.
   */
  result = AverageCell_worker(
    c_knt, c_nv, c_counts,
    in_knt[0], (float *)in_data[0],
    out_knt[0], (float *)out_data[0]);

  if (! result)
     if (DXGetError()==ERROR_NONE)
        DXSetError(ERROR_INTERNAL, "error return from user routine");

  /*
   * In either event, clean up allocated memory
   */

error:
  /*
   * Free the arrays allocated for the regular connections
   * counts
   */
  DXFree((Pointer)c_counts);
  return result;
}

int
AverageCell_worker(
    int c_knt, int c_nv, int *c_counts,
    int input_knt, float *input_data,
    int output_knt, float *output_data)
{
  /*
   * The arguments to this routine are:
   *
   *  c_knt:           total count of input connections elements
   *  c_nv:            number of vertices per element
   *  c_counts:        vertex count along each axis of regular positions grid
   *
   * The following are inputs and therefore are read-only.  The default
   * values are given and should be used if the knt is 0.
   *
   * input_knt, input_data:  count and pointer for input "input"
   *                   no default value given.
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

   int *itemcounts = NULL, i, neighbor;

   /* make scratch space to hold the number of items added for each element */
   itemcounts = DXAllocate(input_knt*sizeof(int));
   if (!itemcounts)
      goto error;

   /* 
    * first initialize the output data component to zero, and itemcounts to
    * zero. 
    */
   for (i=0; i<input_knt; i++) {
      output_data[i] = 0;
      itemcounts[i]=0;
   }

   /* for each data value, add that value to the appropriate items in the
    * output data array. Also increment itemcounts for those cells.
    */
   for (i=0; i<input_knt; i++) {
      /* first do itself */
      output_data[i]+=input_data[i];
      itemcounts[i]++;

      /* now do neighbors in fastest varying dimension */
      neighbor = i-1;
      if (neighbor >= 0 && ((i % (c_counts[1]-1)) != 0)) {
         output_data[neighbor]+=input_data[i];
         itemcounts[neighbor]++;
      }

      neighbor = i+1;
      if (neighbor < input_knt &&(((i+1)%(c_counts[1]-1)) != 0)) {
         output_data[neighbor]+=input_data[i];
         itemcounts[neighbor]++;
      }

      /* now do neighbors in slowest varying dimension */
      neighbor = i - (c_counts[1]-1);
      if (neighbor >= 0) {
         output_data[neighbor]+=input_data[i];
         itemcounts[neighbor]++;
      }
      neighbor = i + (c_counts[1]-1);
      if (neighbor < input_knt) {
         output_data[neighbor]+=input_data[i];
         itemcounts[neighbor]++;
      }
   }

   /* now divide by the number of items added for that cell */
   for (i=0; i< input_knt; i++)
      output_data[i] = output_data[i]/itemcounts[i];

   DXFree((Pointer)itemcounts);
   return OK;
error:
   DXFree((Pointer)itemcounts);
   return ERROR;
}

  
