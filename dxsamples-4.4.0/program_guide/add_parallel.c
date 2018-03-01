#include <dx/dx.h>

static Error DoAdd(Object o, float x);

m_AddParallel(Object *in, Object *out)
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
  DXCreateTaskGroup();
  if (!DoAdd(o, x))
    goto error;
  if (!DXExecuteTaskGroup())
    goto error;
  
  /* successful return */
  out[0] = o;
  return OK;
  
 error:
  DXDelete(o);
  return ERROR;
}


struct arg {
  Field field;
  float x;
};


static Error
  Add_Task(Pointer p)
{
  struct arg *arg = (struct arg *)p;
  Field field;
  float x, *from, *to;
  int i, n, rank, shape[8];
  Array a;
  Type type;
  Category category;
  
  
  /* extract our arguments */
  field = arg->field;
  x = arg->x;
  
  /* extract, typecheck, and get the data from the ``data'' component */
  a = (Array) DXGetComponentValue(field, "data");
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
  DXSetComponentValue(field, "data", (Object)a);
  
  /* the loop that actually adds x to obtain the result */
  for (i=0; i<n; i++)
    to[i] = from[i] + x;
  
  /* clean up the field */
  DXChangedComponentValues(field, "data");
  if (!DXEndField(field))
    return ERROR;
  
  return OK;
}


static
  Error
  DoAdd(Object o, float x)
{
  struct arg arg;
  int i, n;
  Object oo;
  
  /* determine the class of the object */
  switch (DXGetObjectClass(o)) {
    
  case CLASS_FIELD:
    
    /* add the task for this field */
    arg.field = (Field)o;
    arg.x = x;
    if (!DXAddTask(Add_Task, &arg, sizeof(arg), 0.0))
      return ERROR;
    break;
    
  case CLASS_GROUP:
    
    /* recursively traverse groups */
    for (i=0; oo=DXGetEnumeratedMember((Group)o, i, NULL); i++)
      if (!DoAdd(oo, x))
	return ERROR;
    break;
  }	
  
  return OK;
}
