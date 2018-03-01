
#include <stdio.h>
/* necessary for hdf library routines */
#include <df.h>

/* IMPORTANT NOTE:  Data Explorer does not provide HDF libraries. This
   sample module is for illustrative purposes only. Because we do not
   provide the HDF libraries, it will not be possible for you to compile
   and link this sample module unless you yourself have the HDF 
   libraries installed */

#define MAXRANK 3

main(argc, argv)
     char *argv[];
{
  FILE *in;
  char filename[80];
  int dims, counts[MAXRANK], numelements, i, j;
  float deltas[MAXRANK*MAXRANK], origins[MAXRANK], *databuf=NULL;
  
  if (argc < 2) {
    fprintf(stderr,"Usage simpleimportfilter <filename> \n");
    return 0;
  }
  
  strcpy(filename, argv[1]);
  in = fopen(filename,"r");
  if (!in) {
    fprintf(stderr, "cannot open filename %s\n", filename);
    return 0;
  }
  
  
  /* check to see that the file is accessible, and is an hdf file */
  /* DFishdf is an hdf library routine */
  if (DFishdf(filename) != 0) {
    fprintf(stderr, 
            "file \"%s\" is either not accessible, or is not an hdf file\n", 
	    filename);
    return 0;
  }
  
  /* initialize */
  /* DFSDrestart is an HDF library routine */
  DFSDrestart();
  
  /* this returns the dimensionality of the grid (1D, 2D, etc) in dims. The
     number of positions in each dimension is returned in the array counts */
  /* DFSDgetdims is an HDF library routine */
  DFSDgetdims(filename, &dims, counts, MAXRANK);
  
  /* figure out how many elements there are in the data array */
  numelements=1;
  for (i=0; i<dims; i++) {
    numelements = numelements*counts[i];
  }
  
  /* create a buffer for the data */
  databuf = (float *)malloc(numelements*sizeof(float));
  if (!databuf) {
    fprintf(stderr,"out of memory\n");
    return 0;
  }
  
  /* read the data from the HDF file to the data array */
  /* DFSDgetdata is an HDF library routine */
  DFSDgetdata(filename, dims, counts, databuf);
  
  /* write the data array on standard output */
  printf("object 1 class array type float rank 0 items %d data follows\n",
         numelements);
  for (i=0; i<numelements; i++) 
    printf(" %f\n ", databuf[i]);
  
  
  /* set the dependency of the data to be on positions */
  printf("attribute \"dep\" string \"positions\"\n ");
  
  /* now create the position origin and deltas; 
     origin 0 and deltas 1 in each dimension */
  for (i=0; i<dims; i++) {
    origins[i] = 0.0;
    for (j=0; j<dims; j++) {
      if (i==j)
	deltas[i*dims + j] = 1.0;
      else
	deltas[i*dims + j] = 0.0;
    }
  }
  
  /* write out the connections and positions*/
  switch (dims) {
  case (1):
    printf("object 2 class gridconnections counts %d\n", counts[0]);
    printf("object 3 class gridpositions counts %d\n", counts[0]);
    printf(" origin %f\n", origins[0]);
    printf(" delta  %f\n", deltas[0]);
    break;
  case (2):
    printf("object 2 class gridconnections counts %d %d\n", 
	   counts[0], counts[1]);
    printf("object 3 class gridpositions counts %d %d\n", 
	   counts[0], counts[1]);
    printf(" origin %f %f\n", origins[0], origins[1]);
    printf(" delta  %f %f\n", deltas[0], deltas[1]);
    printf(" delta  %f %f\n", deltas[2], deltas[3]);
    break;
  case (3):
    printf("object 2 class gridconnections counts %d %d %d\n", 
	   counts[0], counts[1], counts[2]);
    printf("object 3 class gridpositions counts %d %d %d\n", 
	   counts[0], counts[1], counts[2]);
    printf(" origin %f %f %f\n", origins[0], origins[1], origins[2]);
    printf(" delta  %f %f %f\n", deltas[0], deltas[1], deltas[2]);
    printf(" delta  %f %f %f\n", deltas[3], deltas[4], deltas[5]);
    printf(" delta  %f %f %f\n", deltas[6], deltas[7], deltas[8]);
    break;
  default:
    printf(stderr,"dimensionality must be 1D, 2D, or 3D");
    return 0;
  }
  
  
  /* write out the description of the field */
  printf("object 4 class field\n");
  printf("  component \"data\" value 1\n");
  printf("  component \"connections\" value 2\n");
  printf("  component \"positions\" value 3\n");
  
  return 1;
  
  
}


