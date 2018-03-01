#include <dx/dx.h>
  
  
  static Error DoMakeX(Object in, float size);


m_MakeX(Object *in, Object *out)
{
  Object o = NULL;
  float size;
  
  /* copy the structure if in[0] */
  if (!in[0]) {
    DXSetError(ERROR_BAD_PARAMETER, "missing object");
    goto error;
  }
  o = (Object)DXCopy(in[0], COPY_STRUCTURE);
  if (!o)
    goto error;
  
  /* DXExtract floating point parameter from in[1] (default 1) */
  if (!in[1])
    size = 1;
  else if (!DXExtractFloat(in[1], &size)) {
    DXSetError(ERROR_BAD_PARAMETER, "size must be a scalar value");
    goto error;
  }
  
  /* Call DoMakeX() to do the recursive traversal.  */
  if (!DoMakeX(o, size))
    goto error;
  
  /* successful return */
  out[0] = o;
  return OK;
  
 error:
  DXDelete(o);
  return ERROR;
}


static
  Error
  DoMakeX(Object o, float size)
{
  Array old_positions=NULL, new_positions=NULL, new_connections=NULL;
  Object oo;
  float *old_pos, *new_pos; 
  int *new_con;
  int i, n;
  
  /* determine the class of the object */
  switch (DXGetObjectClass(o)) {
    
  case CLASS_FIELD:
    
    /* 
     * DXExtract the ``positions'' component array from the input
     * object.  Then typecheck it, determine its size, and get
     * a pointer to the array data.
     */
    old_positions = (Array) DXGetComponentValue((Field)o, "positions");
    if (!old_positions) {
      DXSetError(ERROR_MISSING_DATA, "input has no positions");
      return ERROR;
    }
    if (!DXTypeCheck(old_positions, TYPE_FLOAT, CATEGORY_REAL, 1, 3)) {
      DXSetError(ERROR_BAD_TYPE, 
		 "positions are not 3D floating point");
      return ERROR;
    }
    if (!DXGetArrayInfo(old_positions, &n, NULL, NULL, NULL, NULL))
      return ERROR;
    old_pos = (float *) DXGetArrayData(old_positions);
    if (!old_pos)
      return ERROR;
    
    /*
     * Since we plan to change the number of positions and of
     * connections, we must delete all other components that depend
     * on, refer to, or are derived from the "positions" or from
     * the "connections" component, because they are now invalid.
     */
    DXChangedComponentStructure((Field)o, "positions");
    DXChangedComponentStructure((Field)o, "connections");
    
    /*
     * Create a new array for the output positions, allocate
     * space in it, and put it in the output field.  There will
     * be four positions in the output for every position in the
     * input field.
     */
    new_positions = (Array)DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);
    if (!new_positions)
      return ERROR;
    if (!DXAddArrayData(new_positions, 0, 4*n, NULL))
      goto error;
    new_pos = (float *) DXGetArrayData(new_positions);
    if (!new_pos)
      goto error;
    if (!DXSetComponentValue((Field)o, "positions", (Object)new_positions))
      goto error;
    new_positions=NULL;
    
    /*
     * Create a new array for the output connections, allocate
     * space in it, and put it in the output field.  There will
     * be two connections in the output for every position in the
     * input field.
     */
    new_connections = (Array)DXNewArray(TYPE_INT, CATEGORY_REAL, 1, 2);
    if (!new_connections)
      return ERROR;
    if (!DXAddArrayData(new_connections, 0, 2*n, NULL))
      goto error;
    new_con = (int *) DXGetArrayData(new_connections);
    if (!new_con)
      goto error;
    if (!DXSetComponentValue((Field)o, "connections", (Object)new_connections))
      goto error;
    new_connections=NULL;
    
    /*
     * Set the "ref" and "element type" attributes of the
     * connections component
     */
    DXSetComponentAttribute((Field)o, "connections", 
			    "ref", (Object)DXNewString("positions"));
    DXSetComponentAttribute((Field)o, "connections", 
			    "element type", (Object)DXNewString("lines"));
    
    /*
     * The loop that actually makes the "x";
     * the "x" will be in the x-y plane
     */
    for (i=0; i<n; i++) {
      new_pos[12*i     ] = old_pos[3*i  ] - size;
      new_pos[12*i +  1] = old_pos[3*i+1];
      new_pos[12*i +  2] = old_pos[3*i+2];
      new_pos[12*i +  3] = old_pos[3*i  ] + size;
      new_pos[12*i +  4] = old_pos[3*i+1];
      new_pos[12*i +  5] = old_pos[3*i+2];
      new_pos[12*i +  6] = old_pos[3*i  ];
      new_pos[12*i +  7] = old_pos[3*i+1] + size;
      new_pos[12*i +  8] = old_pos[3*i+2];
      new_pos[12*i +  9] = old_pos[3*i  ];
      new_pos[12*i + 10] = old_pos[3*i+1] - size;
      new_pos[12*i + 11] = old_pos[3*i+2];
	    
      new_con[4*i  ] = 4*i; 
      new_con[4*i+1] = 4*i + 1; 
      new_con[4*i+2] = 4*i + 2; 
      new_con[4*i+3] = 4*i + 3; 
    }
    
    /* finalize field */
    if (!DXEndField((Field)o))
      return ERROR;
    break;
    
  case CLASS_GROUP:
    
    /* recursively traverse groups */
    for (i=0; oo=(Object)DXGetEnumeratedMember((Group)o, i, NULL); i++)
      if (!DoMakeX(oo, size))
	return ERROR;
    break;

  default:
    DXSetError(ERROR_BAD_PARAMETER, "object must be a group or field");
    return ERROR;
    
  }
  
  /* successful return */
  return OK;

  /* return on error */
error:
  DXDelete((Object)new_positions);
  DXDelete((Object)new_connections);
}



