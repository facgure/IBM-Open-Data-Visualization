/*
 * (C) COPYRIGHT International Business Machines Corp. 1994
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in 
 * supporting documentation, and that the name of IBM not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific prior written permission. 
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,       
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY        
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.       
 *
 * IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isfound(int *buffer, int *indices, int nindices, int *values, int nvalues)
{
    int i, j;
    for (i = 0; i < nindices; ++i) {
	for (j = 0; j < nvalues; ++j) {
	    if (buffer[indices[i]] == values[j])
		return 1;
	}
    }
    return 0;
}

main(int argc, char *argv[])
{
    char  *basefile;
    char  *unvfile;
    char  *allfiles;
    char  text[500];
    char  *found, **speciesnames = NULL;
    char  s[2], speciesname[500];
    int	  axisym, index, numi, numj, numk, i, j, k;
    int   neighbor[8];
    int	  celli, cellj, cellk;
    int	  celltype, celltypenumber;
    int	  numdim=0, xyz, uvw, vdim;
    int   line;
    int	  nspecies=0;
    int	  speciescount=0;
    int   eflag=0, tflag=0, pflag=0, sflag=0, evflag=0, vflag=0, dflag=0;
    int  tcflag=0, shflag=0, tkflag=0, teflag=0;
    float datavalue;
    float *positionbuffer;
    int   *cellbuffer;
    int    invalids[1];
    int    ninvalids = 0;
    int    inlets[64];
    int    ninlets = 0;
    int    nactct = 0;

    FILE  *input;

    if(argc != 2){
	    printf("Usage: %s fluent_universal_file_name \n",argv[0]);
	    exit(1);
    }

    allfiles       = (char *)malloc(strlen(argv[argc-1])+50);
    basefile       = (char *)malloc(strlen(argv[argc-1])+50);
    unvfile        = (char *)malloc(strlen(argv[argc-1])+50);
    strcpy(unvfile,argv[argc - 1]);

    input = fopen(unvfile,"r");
    if (input == NULL){
	    perror(unvfile);
	    exit(1);
    }
     
    while (fgets(text, 500, input) != NULL){
	found = strstr(text, "GRID SIZE");
	if (found){
		fgets(text, 500, input);
		sscanf(text, "%d %d %d %d", &celli, &cellj, &cellk, &axisym);
		numi = celli - 1;
		numj = cellj - 1;
		if(cellk==1) numk=1; else numk = cellk-1;
		if(numk>1){ 
		    numdim=3; 
		    printf("Object \"connectionsarray\" class gridconnections" 
			" counts %d %d %d\n", numi, numj, numk);
		    printf("attribute \"element type\" string \"cubes\"\n");
		}
		else{ 
		    numdim=2;
		    printf("Object \"connectionsarray\" class gridconnections"
			" counts %d %d \n", numi, numj);
		    printf("attribute \"element type\" string \"quads\"\n");
		}
		printf("attribute \"ref\" string \"positions\"\n");
	}
	found = strstr(text, "CELL TYPE TABLE");
	if (found){
	    fgets(text, 500, input);
	    sscanf(text, "%d", &nactct);
	    for (i=0; i<nactct; i++){
		fgets(text, 500, input);
		sscanf(text, "%d %s", &celltypenumber, s);
		if(s[0] == 'I'){
		    inlets[ninlets++]= celltypenumber;
		}
		if(s[0] == '.'){
		    invalids[ninvalids]= celltypenumber;
		    ninvalids=1;
		}
	    }
	}
	found = strstr(text, "CURVILINEAR");
	if (found){
	    printf("Object \"positionsarray\" class array type float rank 1" 
		" shape %d items %d data follows\n", numdim, numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*numdim*sizeof(float));
	    for (xyz=0; xyz<numdim; xyz++){
		for (k=0; k<numk; k++){
		    for (j=0; j<numj; j++){
			for (i=0; i<numi; i++){
			    fscanf(input, "%f", &datavalue);
			    index=(k*(numj*numi)+j*numi+i)*numdim+xyz;
			    positionbuffer[index]=datavalue;
			}
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			for (xyz=0; xyz<numdim; xyz++){
			    index=(k*(numj*numi)+j*numi+i)*numdim+xyz;
			    printf("%f ",positionbuffer[index]);
			}
			printf("\n");
		    }
		}
	    }
	}  
	found = strstr(text, "CELL TYPES");
	if (found){
	    cellbuffer=(int*)malloc(celli*cellj*cellk*sizeof(int));
	    for (k=0; k<cellk; k++){
		for (j=0; j<cellj; j++){
		    for (i=0; i<celli; i++){
			fscanf(input, "%d", &celltype);
			index=(k*(cellj*celli)+j*celli+i);
			cellbuffer[index]=celltype;
		    }
		}
	    }
	    printf("Object \"invalidpositionsarray\" class array type byte rank 0 items %d data follows\n", numi*numj*numk);
	    if (numdim==3){
		for (i=0; i<numi; i++){
		    for (j=0; j<numj; j++){
			for (k=0; k<numk; k++){
			    neighbor[0]=((i+0)+(j+0)*celli+(k+0)*(cellj*celli));
			    neighbor[1]=((i+1)+(j+0)*celli+(k+0)*(cellj*celli));
			    neighbor[2]=((i+0)+(j+1)*celli+(k+0)*(cellj*celli));
			    neighbor[3]=((i+1)+(j+1)*celli+(k+0)*(cellj*celli));
			    neighbor[4]=((i+0)+(j+0)*celli+(k+1)*(cellj*celli));
			    neighbor[5]=((i+1)+(j+0)*celli+(k+1)*(cellj*celli));
			    neighbor[6]=((i+0)+(j+1)*celli+(k+1)*(cellj*celli));
			    neighbor[7]=((i+1)+(j+1)*celli+(k+1)*(cellj*celli));
			    if (isfound(cellbuffer, neighbor, 8,
					invalids, ninvalids))
				printf("00 ");
			    else
				printf("01 ");
			}
			printf("\n");
		    }
		}
	    }
	    else{
		for (i=0; i<numi; i++){
		    for (j=0; j<numj; j++){
			neighbor[0]=((i+0)+(j+0)*celli);
			neighbor[1]=((i+1)+(j+0)*celli);
			neighbor[2]=((i+0)+(j+1)*celli);
			neighbor[3]=((i+1)+(j+1)*celli);
			if (isfound(cellbuffer, neighbor, 4, 
				    invalids, ninvalids))
			    printf("00 ");
			else
			    printf("01 ");
		    }
		    printf("\n");
		}
	    }
	    printf("attribute \"dep\" string \"positions\"\n");
	    printf("Object \"inletsarray\" class array type byte rank 0 items %d data follows\n", numi*numj*numk);
	    if (numdim==3){
		for (i=0; i<numi; i++){
		    for (j=0; j<numj; j++){
			for (k=0; k<numk; k++){
			    neighbor[0]=((i+0)+(j+0)*celli+(k+0)*(cellj*celli));
			    neighbor[1]=((i+1)+(j+0)*celli+(k+0)*(cellj*celli));
			    neighbor[2]=((i+0)+(j+1)*celli+(k+0)*(cellj*celli));
			    neighbor[3]=((i+1)+(j+1)*celli+(k+0)*(cellj*celli));
			    neighbor[4]=((i+0)+(j+0)*celli+(k+1)*(cellj*celli));
			    neighbor[5]=((i+1)+(j+0)*celli+(k+1)*(cellj*celli));
			    neighbor[6]=((i+0)+(j+1)*celli+(k+1)*(cellj*celli));
			    neighbor[7]=((i+1)+(j+1)*celli+(k+1)*(cellj*celli));
			    {
				if (isfound(cellbuffer, neighbor, 8, 
					inlets, ninlets))
				    printf("01 ");
				else
				    printf("00 ");
			    }
			}
			printf("\n");
		    }
		}
	    }
	    else{
		for (i=0; i<numi; i++){
		    for (j=0; j<numj; j++){
			neighbor[0]=((i+0)+(j+0)*celli);
			neighbor[1]=((i+1)+(j+0)*celli);
			neighbor[2]=((i+0)+(j+1)*celli);
			neighbor[3]=((i+1)+(j+1)*celli);
			{
			    if (isfound(cellbuffer, neighbor, 4, 
				    inlets, ninlets))
			    printf("01 ");
			else
			    printf("00 ");
			}
		    }
		    printf("\n");
		}
	    }
	    printf("attribute \"dep\" string \"positions\"\n");
	    printf("Object \"inletsfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"inletsarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(cellbuffer);
	}  
	found = strstr(text, "STATIC PRESSURE");
	if (found){
	    pflag=1;
	    printf("Object \"pressurearray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"pressurefield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"pressurearray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(positionbuffer);
	}
	found = strstr(text, "U-VELOCITY");
	if (found){
	    positionbuffer=(float*)malloc(numi*numj*numk*numdim*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i)*numdim+0;
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	}
	found = strstr(text, "V-VELOCITY");
	if (found){
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i)*numdim+1;
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    if(numdim==2){
		printf("Object \"velocityarray\" class array type float rank 1 shape %d items %d data follows\n", numdim, numi*numj*numk);
		for (i=0; i<numi; i++){
		    for (j=0; j<numj; j++){
			for (k=0; k<numk; k++){
			    for (xyz=0; xyz<numdim; xyz++){
				index=(k*(numj*numi)+j*numi+i)*numdim+xyz;
				printf("%f ",positionbuffer[index]);
			    }
			    printf("\n");
			}
		    }
		}
		printf("Object \"velocityfield\" class field\n");
		printf("  component \"connections\" \"connectionsarray\"\n");
		printf("  component \"positions\" \"positionsarray\"\n");
		printf("  component \"data\" \"velocityarray\"\n");
		printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    }
	    free(positionbuffer);
	}
	found = strstr(text, "W-VELOCITY");
	if (found){
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i)*numdim+2;
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    if(numdim==3){
		printf("Object \"velocityarray\" class array type float rank 1 shape %d items %d data follows\n", numdim, numi*numj*numk);
		for (i=0; i<numi; i++){
		    for (j=0; j<numj; j++){
			for (k=0; k<numk; k++){
			    for (xyz=0; xyz<numdim; xyz++){
				index=(k*(numj*numi)+j*numi+i)*numdim+xyz;
				printf("%f ",positionbuffer[index]);
			    }
			    printf("\n");
			}
		    }
		}
		printf("Object \"velocityfield\" class field\n");
		printf("  component \"connections\" \"connectionsarray\"\n");
		printf("  component \"positions\" \"positionsarray\"\n");
		printf("  component \"data\" \"velocityarray\"\n");
		printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    }
	    free(positionbuffer);
	}
	found = strstr(text, "ENTHALPY AND TEMPERATURE");
	if (found){
	    eflag=1;
	    printf("Object \"enthalpyarray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"enthalpyfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"enthalpyarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    tflag=1;
	    printf("Object \"temperaturearray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"temperaturefield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"temperaturearray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(positionbuffer);
	}
	found = strstr(text, "271 PROPERTIES: DEN, EFF VIS, VIS, THERM COND, SPEC HEAT");
	if (found){
	    dflag=1;
	    printf("Object \"densityarray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"densityfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"densityarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    evflag=1;
	    printf("Object \"eviscosityarray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"eviscosityfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"eviscosityarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(positionbuffer);
	    vflag=1;
	    printf("Object \"viscosityarray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"viscosityfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"viscosityarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(positionbuffer);
	    tcflag=1;
	    printf("Object \"conductivityarray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"conductivityfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"conductivityarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(positionbuffer);
	    shflag=1;
	    printf("Object \"specheatarray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"specheatfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"specheatarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(positionbuffer);
	}

	found = strstr(text, "SPECIES");
	if (found){
	    fgets(text, 500, input);
	    sscanf(text, "%d", &nspecies);
	    speciesnames=(char**)malloc(nspecies*sizeof(char*));
	    for (speciescount=0; speciescount<nspecies; speciescount++){
		fgets(text, 500, input);
		strcpy(speciesname, text);
		speciesnames[speciescount] = strdup(speciesname);
		printf("Object \"%sarray\" class array type float rank 0 "
			"items %d data follows\n",
			speciesname, numi*numj*numk);
		positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
		for (k=0; k<numk; k++){
		    for (j=0; j<numj; j++){
			for (i=0; i<numi; i++){
			    fscanf(input, "%f", &datavalue);
			    index=(k*(numj*numi)+j*numi+i);
			    positionbuffer[index]=datavalue;
			}
		    }
		}
		/* Get the last newline */
		fgets(text, 500, input);

		for (i=0; i<numi; i++){
		    for (j=0; j<numj; j++){
			for (k=0; k<numk; k++){
			    index=(k*(numj*numi)+j*numi+i);
			    printf("%f ", positionbuffer[index]);
			    printf("\n");
			}
		    }
		}
		printf("Object \"%sfield\" class field\n", speciesname);
		printf("  component \"connections\" \"connectionsarray\"\n");
		printf("  component \"positions\" \"positionsarray\"\n");
		printf("  component \"data\" \"%sarray\"\n",speciesname);
		printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
		free(positionbuffer);
	    }
	}

	found = strstr(text, "TURB. KINETIC ENERGY");
	if (found){
	    tkflag=1;
	    printf("Object \"turbkinarray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"turbkinfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"turbkinarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(positionbuffer);
	}
	found = strstr(text, "TURB. ENERGY DISSIPATION");
	if (found){
	    teflag=1;
	    printf("Object \"turbenergyarray\" class array type float rank 0 items %d data follows\n", numi*numj*numk);
	    positionbuffer=(float*)malloc(numi*numj*numk*sizeof(float));
	    for (k=0; k<numk; k++){
		for (j=0; j<numj; j++){
		    for (i=0; i<numi; i++){
			fscanf(input, "%f", &datavalue);
			index=(k*(numj*numi)+j*numi+i);
			positionbuffer[index]=datavalue;
		    }
		}
	    }
	    for (i=0; i<numi; i++){
		for (j=0; j<numj; j++){
		    for (k=0; k<numk; k++){
			index=(k*(numj*numi)+j*numi+i);
			printf("%f ", positionbuffer[index]);
			printf("\n");
		    }
		}
	    }
	    printf("Object \"turbenergyfield\" class field\n");
	    printf("  component \"connections\" \"connectionsarray\"\n");
	    printf("  component \"positions\" \"positionsarray\"\n");
	    printf("  component \"data\" \"turbenergyarray\"\n");
	    printf("  component \"invalid positions\" \"invalidpositionsarray\"\n");
	    free(positionbuffer);
	}
    } 


    printf("Object \"fluentdata\" class group\n");
    printf("  member \"inlets\" value \"inletsfield\"\n");
    printf("  member \"pressure\" value \"pressurefield\"\n");
    printf("  member \"velocity\" value \"velocityfield\"\n");
    if (eflag) {printf("  member \"enthalpy\" value \"enthalpyfield\"\n");}
    if (tflag) {printf("  member \"temperature\" value \"temperaturefield\"\n");}
    if (dflag) {printf("  member \"density\" value \"densityfield\"\n");}
    if (evflag) {printf("  member \"eviscosity\" value \"eviscosityfield\"\n");}
    if (vflag) {printf("  member \"viscosity\" value \"viscosityfield\"\n");}
    if (tcflag) {printf("  member \"thermcond\" value \"conductivityfield\"\n");}
    if (shflag) {printf("  member \"specheat\" value \"specheatfield\"\n");}
    if (speciesnames) {
	for (speciescount=0; speciescount<nspecies; speciescount++){
	    printf("  member \"%s\" value \"%sfield\"\n", 
		speciesnames[speciescount], speciesnames[speciescount]);
	    free(speciesnames[speciescount]);
	}
	free(speciesnames);
    }
    if (tkflag) {printf("  member \"turbkin\" value \"turbkinfield\"\n");}
    if (teflag) {printf("  member \"turbenergy\" value \"turbenergyfield\"\n");}
    fclose(input);
}
