
#include <dx/dx.h>

static Error DoAdd(Object o, float x);

Error m_Add(Object *in, Object *out)
{
  Object o = NULL;
  float x;
  
  /* copy the structure of in[0] */
  if (!in[0]) {
    DXSetError(ERROR_BAD_PARAMETER, "missing data parameter");
    goto error;
  }
  o = (Object)DXCopy(in[0], COPY_STRUCTURE);
  if (!o)
    goto error;
  
  /* extract floating point parameter from in[1] (default 0) */
  if (!in[1])
    x = 0;
  else if (!DXExtractFloat(in[1], &x)) {
    DXSetError(ERROR_BAD_PARAMETER, "addend must be a scalar value");
    goto error;
  }
  
  /* call DoAdd() to do the recursive traversal */
  if (!DoAdd(o, x))
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
  DoAdd(Object o, float x)
{
  int i, n, rank, shape[8];
  Type type;
  Category category;
  float *from, *to;
  Array a;
  Object oo;
  
  /* determine the class of the object */
  switch (DXGetObjectClass(o)) {

  case CLASS_FIELD:
    
    /* extract, typecheck, and get the data from the ``data'' component */
    a = (Array) DXGetComponentValue((Field)o, "data");
    if (!a) {
      DXSetError(ERROR_MISSING_DATA, "field has no data");
      return ERROR;
    }
    DXGetArrayInfo(a,&n,&type,&category,&rank, shape);
    if ((type != TYPE_FLOAT)||(category != CATEGORY_REAL)||(rank != 0)) {
      DXSetError(ERROR_BAD_TYPE, "data is not scalar floating point");
      return ERROR;
    }
    from = (float *) DXGetArrayData(a);
    
    /* create a new array, allocate space in it, put it in the field */
    a = (Array)DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0);
    if (!DXAddArrayData(a, 0, n, NULL))
      return ERROR;
    to = (float *) DXGetArrayData(a);
    DXSetComponentValue((Field)o, "data", (Object)a);
    
    /* the loop that actually adds x to obtain the result */
    for (i=0; i<n; i++)
      to[i] = from[i] + x;
    
    /* clean up the field */
    DXChangedComponentValues((Field)o, "data");
    if (!DXEndField((Field)o))
      return ERROR;
    break;
    
  case CLASS_GROUP:
    
    /* recursively traverse groups */
    for (i=0; oo=(Object)DXGetEnumeratedMember((Group)o, i, NULL); i++)
      if (!DoAdd(oo, x))
	return ERROR;
    break;
  }	
  
  return OK;
}






