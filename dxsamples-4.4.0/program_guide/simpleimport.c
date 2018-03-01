
#include <dx/dx.h>

/* necessary for hdf library routines */

/* IMPORTANT NOTE:  Data Explorer does not provide HDF libraries. This
   example program is for illustrative purposes only. Because we do
   not provide hdf libraries, it will not be possible for you to compile
   and link this module unless you have the HDF libraries yourself 
*/


#include <df.h>

#define MAXRANK 3

Error m_SimpleImport(Object *in, Object *out)
{
  Array a=NULL;
  Field f=NULL;
  char *filename;
  int dims, counts[MAXRANK], numelements, i, j;
  float deltas[MAXRANK*MAXRANK], origins[MAXRANK], *data;
  
  /* extract the file name from in[0] */
  if (!in[0]) {
    DXSetError(ERROR_BAD_PARAMETER,"missing filename");
    goto error;
  }
  else if (!DXExtractString(in[0], &filename)) {
    DXSetError(ERROR_BAD_PARAMETER, "filename must be a string");
    goto error;
  }

  /* check to see that the file is accessible, and is an hdf file */
  /* DFishdf is an hdf library routine */
  if (DFishdf(filename) != 0) {
     DXSetError(ERROR_BAD_PARAMETER,
                "file \"%s\" is either not accessible, or is not an hdf file", 
                 filename);
     goto error;
  }

  /* initialize */
  /* DFSDrestart is an HDF library routine */
  DFSDrestart();

  /* this returns the dimensionality of the grid (1D, 2D, etc) in dims. The
     number of positions in each dimension is returned in the array counts */
  /* DFSDgetdims is an HDF library routine */
  DFSDgetdims(filename, &dims, counts, MAXRANK);

  /* make a new data array (scalar) */
  a = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0);
  if (!a)
    goto error;

  /* figure out how many elements there are in the data array */
  for (i=0, numelements=1; i<dims; numelements *= counts[i], i++)
    ; 
  
  /* allocate space in the data array */
  if (!DXAddArrayData(a, 0, numelements, NULL))
     goto error;

  /* get a pointer to the data array */
  data = (float *)DXGetArrayData(a);
  if (!data)
     goto error; 

  /* read the data from the HDF file to the data array */
  /* DFSDgetdata is an HDF library routine */
  DFSDgetdata(filename, dims, counts, data);

  /* create a new field */
  f = DXNewField();
  if (!f)
    goto error;

  /* set the dependency of the data to be on positions */
  if (!DXSetStringAttribute((Object)a, "dep", "positions"))
    goto error;

  /* set the data array as the data component of f */
  if (!DXSetComponentValue(f, "data", (Object)a))
    goto error;
  a=NULL;

  /* create the connections array */
  /* DXMakeGridConnections will set the element type */
  a = DXMakeGridConnectionsV(dims, counts);
  if (!a)
     goto error;
  if (!DXSetComponentValue(f, "connections", (Object)a))
     goto error;
  a=NULL;

  /* now create the positions array; origin 0 and deltas 1 in each dimension */
  for (i=0; i<dims; i++) {
     origins[i] = 0.0;
     for (j=0; j<dims; j++) {
       if (i==j)
         deltas[i*dims + j] = 1.0;
       else
         deltas[i*dims + j] = 0.0;
     }
  }

  a = DXMakeGridPositionsV(dims, counts, origins, deltas);
  if (!a)
    goto error;
  if (!DXSetComponentValue(f, "positions", (Object)a))
    goto error;
  a=NULL; 


  /* EndField sets default attributes (such as setting the connections
     attribute to be "ref" positions), and creates the bounding box.  */
  if (!DXEndField(f))
    goto error;

  out[0]=f;
  return OK;

 error:
  DXDelete((Object)f);
  DXDelete((Object)a);
  return ERROR;
}


