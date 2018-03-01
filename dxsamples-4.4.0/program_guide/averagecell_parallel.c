#include <dx/dx.h>
  
static Error DoAverageCell(Object);



Error m_AverageCellParallel(Object *in, Object *out)
{
  Object o=NULL;
  
  if (!in[0]) {
    DXSetError(ERROR_BAD_PARAMETER,"missing input");
    goto error;
  }
  
  o = DXCopy(in[0], COPY_STRUCTURE);
  
  /* "Grow" the fields so that the averaging can take place across 
   * partition boundaries"
   * We do not need to grow beyond the original boundaries of the
   * data, and we only need to grow the "data" component. 
   */
  if (!DXGrow(o, 1, GROW_NONE, "data", NULL))
    goto error;
  
  /* create the task group */
  if (!DXCreateTaskGroup())
    goto error;
  
  /* the add tasks will be added in DoAverageCell */
  if (!DoAverageCell(o))
    goto error;
  
  if (!DXExecuteTaskGroup())
    goto error;
  
  /* We can now call DXShrink to shrink the grown field */
  /* first recursively remove any "original data" components */
  if (DXExists(o, "original data"))
    DXRemove(o,"original data");
  if (!DXShrink(o))
    goto error;

  out[0] = o; 
  return OK;
 error:
  DXDelete((Object)o);
  return ERROR;
}



struct arg {
  Field field;
};

static Error AddCellTask(Pointer p)
{
  struct arg *arg = (struct arg *)p;
  int i, j, numitems, shape, *neighbors_ptr, sum, neighbor;
  int dim, counts[3];
  char *attribute;
  float *data_ptr, *newdata_ptr, dataaverage;
  Array connections, data, newdata=NULL, neighbors;
  Field field;
  
  field = arg->field;
  
  
  /* Get the connections component, find
   * out how many connections there are. Determine the element type
   * of the connections. 
   */    
  
  connections = (Array)DXGetComponentValue(field,"connections");
  if (!connections) {
    DXSetError(ERROR_MISSING_DATA,"input has no connections");
    goto error;
  }
  if (!DXGetArrayInfo(connections, &numitems, NULL, NULL, NULL, NULL)) {
    goto error;
  }
  if (!(attribute= 
	(char *)DXGetString((String)DXGetComponentAttribute(field,
							    "connections", 
							    "element type")))) {
    DXSetError(ERROR_MISSING_DATA,
	       "missing connection element type attribute");
    goto error;
  }
    
    
    
  /* Get the data component, and get the data dependancy attribute. */
  
  data = (Array)DXGetComponentValue(field,"data");
  if (!data) {
    DXSetError(ERROR_MISSING_DATA,"input has no data");
    goto error;
  }
  if (!(attribute= 
	(char *)DXGetString((String)DXGetComponentAttribute(field,
							    "data", 
							    "dep")))) {
    DXSetError(ERROR_MISSING_DATA,
	       "missing data dependancy attribute");
    goto error;
  }
  
  
  /* In this example, the data must be dependent on the connections */
  if (strcmp(attribute,"connections")) {
    DXSetError(ERROR_DATA_INVALID,
	       "data must be dependent on connections");
    goto error;
  }
  
  
  /* for this example, the data is required to be floating point scalar */
  if (!DXTypeCheck(data, TYPE_FLOAT, CATEGORY_REAL, 0, NULL)) {
    DXSetError(ERROR_DATA_INVALID, "data must be floating point scalar");
    goto error;
  }
  
  /* Get a pointer to the data */
  
  data_ptr = (float *)DXGetArrayData(data);
  
  
  /*  make a new data component, allocate space in it, and get a 
   *  pointer to it 
   */
  newdata = DXNewArray(TYPE_FLOAT,CATEGORY_REAL, 0);
  if (!DXAddArrayData(newdata, 0, numitems, NULL))
    goto error;
  newdata_ptr = (float *)DXGetArrayData(newdata);
  
      
  /* if the data is ungridded, we can use the neighbors array */
  /* if it is gridded, we will use a different method */
  
  if (!DXQueryGridConnections(connections, &dim,  counts)) {
    
    
    /* we need the neighbors of the connections */
    /* note that neighbors may be obtained only for non-gridded data */
    /* This is because there are more efficient ways to determine one's
     * neighbors for gridded data */
    
    neighbors = DXNeighbors(field);
    if (!neighbors) 
      goto error;
    neighbors_ptr = (int *)DXGetArrayData(neighbors);
    if (!DXGetArrayInfo(neighbors, NULL, NULL, NULL, NULL, &shape))
      goto error;
    
    
    for (i=0; i<numitems; i++) {
      dataaverage = data_ptr[i];
      sum = 1;
      /* shape is the number of neighbors which a connection element has */
      for (j=0; j<shape; j++) {
	neighbor = neighbors_ptr[shape*i + j];
	if (neighbor != -1) {
	  dataaverage = dataaverage + data_ptr[neighbor]; 
	  sum++;
	}
      }
      dataaverage = dataaverage/sum;
      newdata_ptr[i] = dataaverage;
    }
  }
  
  else {
    /* The connections are gridded. 
     * this example only handles 2D connections (quads) 
     */
    
    if (dim != 2) {
      DXSetError(ERROR_DATA_INVALID,"connections must be 2-dimensional");
      goto error;
    }
    
    for (i=0; i< numitems; i++) {
      dataaverage = data_ptr[i];
      sum = 1;
      
      /* there are up to 4 neighbors for every quad */
      if ((i % (counts[1]-1)) > 0) {
	neighbor = i-1;
	dataaverage = dataaverage + data_ptr[neighbor];
	sum++;
      }
      if ((i % (counts[1]-1)) < (counts[1] - 2)) {
	neighbor = i+1;
	dataaverage = dataaverage + data_ptr[neighbor];
	sum++;
      }
      neighbor = i-(counts[1]-1);
      if (neighbor>=0 && neighbor<numitems) {
	dataaverage = dataaverage + data_ptr[neighbor];
	sum++;
      }
      neighbor = i+(counts[1]-1);
      if (neighbor>=0 && neighbor<numitems) {
	dataaverage = dataaverage + data_ptr[neighbor];
	sum++;
      }
      dataaverage = dataaverage/sum;
      newdata_ptr[i] = dataaverage;
    }
  }
  
  /* Place the new data component in the field */
  
  DXSetComponentValue(field, "data", (Object)newdata);
  newdata=NULL; 
  
  /* we have changed the data component */
  if (!DXChangedComponentValues(field,"data"))
    goto error;
  
  
  return OK;
 error:
  DXDelete((Object)newdata);
  return ERROR;
}



static Error DoAverageCell(Object object)
{
  Object subo;
  struct arg arg;
  int i;
  
  switch (DXGetObjectClass(object)) {
  case (CLASS_FIELD):
    
    arg.field = (Field)object;   
    if (!DXAddTask(AddCellTask, &arg, sizeof(arg), 0.0))
      goto error; 
    break;
    
  case (CLASS_GROUP):
    
    /* If object is a group, recursively call DoAverageCell */
    for (i=0; subo=DXGetEnumeratedMember((Group)object, i, NULL); i++) {
      if (!DoAverageCell(subo))
	return ERROR;
    }
    break;
  }
  return OK;
 error:
  return ERROR;
}
