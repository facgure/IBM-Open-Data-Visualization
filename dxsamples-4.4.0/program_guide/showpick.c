#include <dx/dx.h>

static Error DoPick(Object, Object, RGBColor, int, int, int, int);
static Error SetColor(Object, RGBColor);

Error m_ShowPick(Object *in, Object *out)
{
  Object o = NULL, pickfield;
  char *colorstring;
  int colorwhich, poke, pick, depth;
  RGBColor color;
  
  /* 
   * copy the structure of in[0], which is the object in which
   * picking took place 
   */
  
  if (!in[0]) {
    DXSetError(ERROR_BAD_PARAMETER, "missing input");
    goto error;
  }
  o = (Object)DXCopy(in[0], COPY_STRUCTURE);
  if (!o)
    goto error;
  

  /* 
   * first set all the colors to white, to initialize 
   */
  
  if (!SetColor(o, DXRGB(1.0, 1.0, 1.0)))
    goto error;
  
  
  /* 
   * in[1] is the pick field. 
   * If the pick field is null or an empty field, just return 
   * the copy of the object 
   */

  if (!in[1] || DXEmptyField((Field)in[1])) {
    out[0] = o;
    return OK;
  }
  pickfield = in[1];
  
  
  
  /* 
   * get the color which will be used 
   * to set picked objects, which is in[2] 
   */

  if (in[2]) {
    if (!DXExtractString((Object)in[2], &colorstring)) {
      DXSetError(ERROR_BAD_PARAMETER,"color must be a string");
      goto error;
    }
    
    /*
     * convert the colorname to an RGB vector 
     */

    if (!DXColorNameToRGB(colorstring, &color))
      goto error;
  } 
  else {
    
    /* 
     * the default color is red 
     */
    
    color = DXRGB(1.0, 0.0, 0.0);
  }
  
  
  /* 
   * are we to color just the element, just the vertex, or the entire field? 
   * The default is to color just the picked element
   */
  
  if (!in[3]) {
    colorwhich = 0;
  }
  else {
    if (!DXExtractInteger(in[3], &colorwhich)) {
      DXSetError(ERROR_BAD_PARAMETER,"colorwhich flag must be 0, 1, or 2");
      goto error;
    }
    if ((colorwhich < 0)&&(colorwhich > 2)) {
      DXSetError(ERROR_BAD_PARAMETER,"colorwhich flag must be 0, 1, or 2");
      goto error;
    }
  }
  
  /* 
   * are we to selecting a particular poke, or all of them?
   */

  if (!in[4]) {
    poke = -1;
  }
  else {
    if (!DXExtractInteger(in[4], &poke)) {
      DXSetError(ERROR_BAD_PARAMETER,"poke must be a non-negative integer");
      goto error;
    }
    if (poke < 0) {
      DXSetError(ERROR_BAD_PARAMETER,"poke must be a non-negative integer");
      goto error;
    }
  }
  
  
  /* 
   * are we to selecting a particular pick, or all of them?
   */
  
  if (!in[5]) {
    pick = -1;
  }
  else {
    if (!DXExtractInteger(in[5], &pick)) {
      DXSetError(ERROR_BAD_PARAMETER,"pick must be a non-negative integer");
      goto error;
    }
    if (pick < 0) {
      DXSetError(ERROR_BAD_PARAMETER,"pick must be a non-negative integer");
      goto error;
    }
  }
  
  /* 
   * are we to selecting a depth?
   */
  
  if (!in[6]) {
    depth = -1;
  }
  else {
    if (!DXExtractInteger(in[6], &depth)) {
      DXSetError(ERROR_BAD_PARAMETER,"depth must be a non-negative integer");
      goto error;
    }
    if (depth < 0) {
      DXSetError(ERROR_BAD_PARAMETER,"depth must be a non-negative integer");
      goto error;
    }
  }
  
 
  /* 
   * traverse the pick object, using the pick structure, 
   * passing the given parameters 
   */ 

  if (!DoPick(o, pickfield, color, colorwhich, poke, pick, depth))
    goto error;
  
  /* 
   * delete the opacities component 
   */

  if (DXExists(o, "opacities"))
    DXRemove(o,"opacities");
  
  
  
  /* 
   * successful return 
   */

  out[0] = o;
  return OK;
  
  /* 
   * return on error 
   */  

 error:
  DXDelete(o);
  return ERROR;
}

static Matrix Identity = {
    {
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 }
    }
};

/* 
 * The DoPick routine traverses  the picked object 
 */

static
  Error
  DoPick(Object o, Object pickfield, RGBColor color, int colorwhich, 
	 int pokes, int picks, int depth)
{
  int pokecount, pickcount, poke, pick, i, pathlength;
  int vertexid, elementid, *path, numitems, index;
  Object current;
  Matrix matrix;
  Array a, newcolors=NULL, oldcolors;
  char *depatt;
  RGBColor *newcolors_ptr, oldcolor;
  int pokemin, pokemax;
  int pickmin, pickmax;
  int thisdepth;



  /* 
   * pickfield is expected to be a field
   */

  if (!(DXGetObjectClass(pickfield)==CLASS_FIELD)) {
    DXSetError(ERROR_DATA_INVALID,"pickfield must be a field");
    goto error;
  }
  
  
  /* 
   * find out how many pokes there are 
   */
  
  DXQueryPokeCount((Field)pickfield, &pokecount);
  
  /* 
   * The user has specified to mark all pokes 
   */

  if (pokes < 0) {
    pokemin = 0, pokemax = pokecount-1;
  }
  
  /* 
   * The user has specified a poke larger than the number present 
   */
  
  else if (pokes > pokecount-1) {
    DXSetError(ERROR_BAD_PARAMETER,
	       "only %d pokes are present", pokecount);
    return ERROR;
  }
  
  /*
   * Consider only the given poke
   */

  else
    pokemin = pokemax = pokes;
  


  
  /* 
   * for each poke... 
   */
  
  for (poke=pokemin; poke<=pokemax; poke++) {
    
    /* 
     * find out how many picks there are in this poke
     */
    
    if (!DXQueryPickCount((Field)pickfield, poke, &pickcount))
      goto error;
    
    /*
     * The user has specified that we are to consider all of the picks
     */
    

    /* 
     * Warning if this particular poke does not contain that many picks
     */
    if (picks > pickcount-1) {
      DXWarning("poke %d contains only %d picks", poke, pickcount);
    }
    
    else {
      if (picks < 0) {
        pickmin = 0, pickmax = pickcount-1;
      }
      else { 
        pickmin = pickmax = picks;
      }
      
      /* 
       * for each pick ... 
       */
      
      for (pick=pickmin; pick<=pickmax; pick++) {
	
	/* 
	 * for the given pickfield, the current poke number, and the 
	 * current pick number,  get the traversal path "path", the length
	 * of the traversal path "pathlength", and the id's of the 
	 * picked element and the picked vertex
	 */ 
	
	DXQueryPickPath((Field)pickfield, poke, pick, &pathlength, &path,
			&elementid, &vertexid);
	
	/* 
	 * initialize current to the picked object, and matrix to the
	 * identity matrix
	 */ 
	
	current = o;
	matrix = Identity;
	if (depth != -1 && pathlength > depth)
	  thisdepth = depth;
	else
	  thisdepth = pathlength;
	
	/* 
	 * iterate through the pick path 
	 */
	
	for (i=0; i<thisdepth; i++) {
	  current = DXTraversePickPath(current, path[i], &matrix);
	  if (!current)
	    goto error;
	}
	
	/* 
	 * current is now the field level of the picked object, and we have
	 * the element and vertex id's of the picked object 
	 */
	
	if (colorwhich == 2 || DXGetObjectClass(current) != CLASS_FIELD) {
	  
	  /* 
	   * we are simply to color the entire field
	   */ 
	  
	  if (!SetColor(current, color))
	    goto error;
	}
	else {
	  
	  /* 
	   * Otherwise, we want to set the indicated element or vertex to
	   * the given color. We start by making a new colors component
	   * (not compact), but only if the input colors component
	   * is still compact. If it's already expanded, then just
	   * modify it 
	   */
	  
	  /* 
	   * first determine the dependency of the colors
	   */
	  
	  if (colorwhich == 0) {
	    if (a = (Array)DXGetComponentValue((Field)current, "connections")) {
	      index = elementid;
	      depatt = "connections";
	    }
	    else if (a = (Array)DXGetComponentValue((Field)current, "faces")) {
	      index = elementid;
	      depatt = "faces";
	    }
	    else {
	      a = (Array)DXGetComponentValue((Field)current, "positions");
	      index = vertexid;
	      depatt = "positions";
	    }
	  }
	  else {
	    a = (Array)DXGetComponentValue((Field)current, "positions");
	    index = vertexid;
	    depatt = "positions";
	  }	
	  
	  /* 
	   * Determine how many items are there. 
	   */
	  
	  if (!DXGetArrayInfo(a, &numitems,NULL,NULL,NULL,NULL))
	    goto error;
	  
	  /*  
	   * if the traversal index is greater than the number of items,
	   * something is wrong 
	   */
	  
	  if (index >= numitems) {
	    DXSetError(ERROR_DATA_INVALID,
		       "pick structure does not correspond to picked object");
	    goto error;
	  }
	  
	  /*
	   * Get the original colors component
	   */
	  
	  oldcolors = (Array)DXGetComponentValue((Field)current, "colors");
	  
	  /* 
	   * If it is a constant array, we need to expand it so that we can
	   * set just one element or vertex to the given color
	   */
	  
	  if (DXQueryConstantArray((Array)oldcolors, NULL, &oldcolor)) {
	    
	    /*
	     * create a new colors array, and allocate space in it 
	     */
	    
	    newcolors = DXNewArray(TYPE_FLOAT,CATEGORY_REAL, 1, 3);
	    if (!DXAddArrayData(newcolors, 0, numitems, NULL))
	      goto error;
	    
	    /* 
	     * start by setting all colors to the original constant color 
	     */
	    
	    newcolors_ptr = (RGBColor *)DXGetArrayData(newcolors);
	    for (i=0; i<numitems; i++) {
	      newcolors_ptr[i] = oldcolor;
	    }
	    
	      
	    /* 
	     * replace the colors in the field with the new colors
	     * component 
	     */
	    
	    if (!DXSetComponentValue((Field)current, "colors",
				     (Object)newcolors))
	      goto error;
	    newcolors=NULL;
	    
	    DXSetComponentAttribute((Field)current, "colors", "dep",
				    (Object)DXNewString(depatt));
	  }
	    
	    
	  else {
	    
	    /* 
	     * The colors are already expanded, presumably from an
	     * earlier pick in this field 
	     */

	    newcolors_ptr = (RGBColor *)DXGetArrayData(oldcolors);
	  }
	  
	  
	  /* 
	   * set the correct triangle or position to the given color. 
	   */
	  newcolors_ptr[index] = color;
	}
      }
    }
  }
  
  return OK;
  
 error:
  DXDelete((Object)newcolors);
  return ERROR;
}





/* 
 * This routine simply sets all colors in object o to the given color 
 */

static Error SetColor(Object o, RGBColor color)
{
  Object subo;
  Array a, newcolors=NULL;
  int numitems, i;


  switch (DXGetObjectClass(o)) {
    
    
  case (CLASS_GROUP):

    /* 
     * if o is a group, call SetColor recursively on its children 
     */

    for (i=0; subo = DXGetEnumeratedMember((Group)o, i, NULL); i++) 
      SetColor(subo, color);
    break;
    
    
  case (CLASS_XFORM):

    /* 
     * if o is an xform, call SetColor on its child
     */

    DXGetXformInfo((Xform)o, &subo, NULL);
    SetColor(subo, color);
    break;
    
    
  case (CLASS_CLIPPED):

    /* 
     * if o is an clipped object, call SetColor on its child 
     */

    DXGetClippedInfo((Clipped)o, &subo, NULL);
    SetColor(subo, color);
    break;
    

  case (CLASS_FIELD):

    /* 
     * if o is a field, set the colors to the given color 
     */
    
    if (DXEmptyField((Field)o))
      return OK;
    

    /* 
     * The number of colors and the dependency of the colors will
     * depend on whether connections are present. If not, then
     * it is checked whether faces are present. Otherwise, the 
     * colors will be dependent on positions. 
     */
    
    if (a = (Array)DXGetComponentValue((Field)o, "connections")) {
      DXGetArrayInfo(a, &numitems, NULL, NULL, NULL, NULL);
      newcolors = (Array)DXNewConstantArray(numitems, &color,
					    TYPE_FLOAT,
					    CATEGORY_REAL, 1, 3);
      DXSetComponentValue((Field)o, "colors", (Object)newcolors);
      newcolors = NULL;
      DXSetComponentAttribute((Field)o,"colors", "dep", 
			      (Object)DXNewString("connections"));
    }
    else if (a = (Array)DXGetComponentValue((Field)o, "faces")) { 
      DXGetArrayInfo(a, &numitems, NULL, NULL, NULL, NULL);
      newcolors = (Array)DXNewConstantArray(numitems, &color,
					    TYPE_FLOAT,
					    CATEGORY_REAL, 1, 3);
      DXSetComponentValue((Field)o, "colors", (Object)newcolors);
      newcolors = NULL;
      DXSetComponentAttribute((Field)o,"colors", "dep", 
			      (Object)DXNewString("faces"));
    }
    else {
      a = (Array)DXGetComponentValue((Field)o, "positions"); 
      DXGetArrayInfo(a, &numitems, NULL, NULL, NULL, NULL);
      newcolors = (Array)DXNewConstantArray(numitems, &color,
					    TYPE_FLOAT,
					    CATEGORY_REAL, 1, 3);
      DXSetComponentValue((Field)o, "colors", (Object)newcolors);
      newcolors = NULL;
      DXSetComponentAttribute((Field)o,"colors", "dep", 
			      (Object)DXNewString("positions"));
    }
    
    break;
  }
  
  
  /* 
   * successful return, or return on error 
   */

  return OK;
 error:
  DXDelete((Object)newcolors);
  return ERROR;
}
