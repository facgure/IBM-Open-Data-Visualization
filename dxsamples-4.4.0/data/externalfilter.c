/* This simple program will read the file specified on the command line, 
   and output a dx format file.  The program assumes a simple file format 
   for the specified file, such that the first three integers in the file 
   represent the number of counts in x, y, and z, the next three floating 
   point numbers represent the origin of the coordinate system, and the 
   next three floating point numbers represent the deltas along x, y, and z. 
   An example data file in this particular format can be found in 
   external.data. A visual program which imports 
   this file using the external command syntax may be found in 
   ImportExternalFilter.net 
*/ 

#include <stdio.h>
#include <stdlib.h>

main(argc, argv)
   char *argv[];
{
   FILE *in1;
   char datafile[30];
   int numx, numy, numz, i, j, k;
   float originx, originy, originz;
   float deltax, deltay, deltaz;
   float datavalue;

   if (argc < 2)  {
        printf("Usage externalfilter <filename>\n");
        return 0;
   }
   strcpy(datafile,argv[1]);
   in1=fopen(datafile,"r");

   /* read the number of positions in x, y, and z */
   fscanf(in1, "%d %d %d", &numx, &numy, &numz);

   /* read the origin */
   fscanf(in1, "%f %f %f", &originx, &originy, &originz);

   /* read the deltas */
   fscanf(in1, "%f %f %f", &deltax, &deltay, &deltaz);

   /* write the positions component object to standard output */
   printf("Object 1 class gridpositions counts %d %d %d\n", numx,numy,numz);
   printf("  origin %f %f %f\n", originx, originy, originz);
   printf("  delta %f 0.0 0.0\n", deltax);
   printf("  delta 0.0 %f 0.0\n", deltay);
   printf("  delta 0.0 0.0 %f\n", deltaz);

   /* write the connections component object to standard output */
   printf("Object 2 class gridconnections counts %d %d %d\n", numx, numy, numz);
   printf("   attribute \"ref\" string \"positions\" \n");
   printf("   attribute \"element type\" string \"cubes\" \n");

   /* write the data component object to standard output */
   printf("Object 3 class array type float rank 0 items %d data follows \n",
                   numx*numy*numz);
   for (i=0; i<numx; i++) {
     for (j=0; j<numy; j++) {
       for (k=0; k<numz; k++) {
         fscanf(in1, "%f", &datavalue);
         printf("%f   ", datavalue);
       }
       printf("\n");
     }
   }
   printf("\n");
   printf("   attribute \"dep\" string \"positions\" \n");

   /* write the description of the field to standard output */
   printf("Object 4 class field\n");
   printf("  component \"positions\" 1\n");     
   printf("  component \"connections\" 2\n");     
   printf("  component \"data\" 3\n");     
   fclose(in1);
}        

