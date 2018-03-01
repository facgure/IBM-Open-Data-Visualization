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
 
main(int argc, char *argv[])
{
    char  *xyzfile, *qfile, *ffile;
    char  text[500];
    int	  ni, nj, nk, ndim, ngrid, ngridvars, nf;
    int	  composite, extraneous;
    int	  field, index, invaliddata, i, j, k, xyz, uvw, grid;
    int   *gbuffer, *ibuffer;
    int   mgrid, iblank, ascii, lt;
    float datavalue;
    float *sbuffer, *vbuffer;
    float fsmach, alpha, re, time;
 
    FILE  *xyzinput;
    FILE  *qinput;
    FILE  *finput;
 
    if (argc == 1) {
	printf("Usage: %s -xyz xyzfile -q qfile\n", argv[0]);
	printf("          [-f ffile] [-dim ndim] [-mgrid]\n");
	printf("	  [-iblank] [-ascii] [-leadtrail]\n",argv[0]);
	printf("\n");
	printf("       xyzfile    is the name of the grid positions file\n");
	printf("       qfile      is the name of the solution file\n");
	printf("       ffile      is the name of the function file\n");
	printf("       ndim       is number of dimensions in the data (2 or 3) \n");
	printf("       -mgrid     if multi-grid; default: single grid\n");
	printf("       -iblank    if iblank'd; default: no iblanking\n");
	printf("       -ascii     for ascii output; default: binary output\n");
	printf("       -leadtrail if data was written with four byte Fortran\n"
	       "                  leading and trailing byte counts)\n");
	exit(1);
    }
 
    ngridvars = 4;
    ffile="none";
    ndim=3;
    mgrid=0;
    iblank=0;
    ascii=0;
    composite=0;

    while (argc > 1) {
	if (!strcmp("-xyz", argv[1])) {
	    if (argc < 2) {
		fprintf(stderr, "no filename given for -xyz flag\n");
		exit(-1);
	    }
	    xyzfile = argv[2];
	    argc--;
	    argv++;
	}
	else if (!strcmp("-q", argv[1])) {
	    if (argc < 2) {
		fprintf(stderr, "no filename given for -q flag\n");
		exit(-1);
	    }
	    qfile = argv[2];
	    argc--;
	    argv++;
	}
	else if (!strcmp("-f", argv[1])) {
	    if (argc < 2) {
		fprintf(stderr, "no filename given for -f flag\n");
		exit(-1);
	    }
	    ffile = argv[2];
	    argc--;
	    argv++;
	}
	else if (!strcmp("-dim", argv[1])) {
	    if (argc < 2) {
		fprintf(stderr, "no filename given for -dim flag\n");
		exit(-1);
	    }
	    ndim = atoi(argv[2]);
	    argc--;
	    argv++;
	}
	else if (!strcmp("-mgrid", argv[1])) {
	    mgrid=1;
	}
	else if (!strcmp("-iblank", argv[1])) {
	    iblank=1;
	}
	else if (!strcmp("-ascii", argv[1])) {
	    ascii=1;
	}
	else if (!strcmp("-leadtrail", argv[1])) {
	    lt=1;
	}
	else if (!strcmp("-composite", argv[1])) {
	    composite=1;
	}
	else {
	    printf("unrecognized parameter: %s\n", argv[1]);
	}
	argc--;
	argv++;
    }


    xyzinput = fopen(xyzfile,"r");
    if (xyzinput == NULL){
	    perror(xyzfile);
	    exit(1);
    }
 
    qinput = fopen(qfile,"r");
    if (qinput == NULL){
	    perror(qfile);
	    exit(1);
    }
     if(strcmp("none",ffile)){
	finput = fopen(ffile,"r");
	if (finput == NULL){
		perror(ffile);
		exit(1);
	}
    }
/* start reading data from xyz-file (grid positions) */

/* if multi-grid, find out how many grids */
    if(mgrid==1){
	/* remove leading byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,xyzinput);
	fread(&ngrid,4,1,xyzinput);
	/* remove trailing byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,xyzinput);
    }
    else 
	ngrid=1;
    gbuffer=(int*)malloc(ngrid*ngridvars*sizeof(int));

/* for each grid, find number of i, j and k (if any) indicies */
    /* remove leading byte count indicator  */
    if(lt==1) fread(&extraneous,4,1,xyzinput);
    for (grid=0; grid<ngrid; grid++){
	fread(&gbuffer[grid*ngridvars+0],4,1,xyzinput);
	fread(&gbuffer[grid*ngridvars+1],4,1,xyzinput);
	if(ndim ==3){
	    fread(&gbuffer[grid*ngridvars+2],4,1,xyzinput);
	}
	else gbuffer[grid*ngridvars+2] = 1;
    }
    /* remove trailing byte count indicator  */
    if(lt==1) fread(&extraneous,4,1,xyzinput);
 
    for (grid=0; grid<ngrid; grid++){
	/* remove leading byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,xyzinput);
	ni=gbuffer[grid*ngridvars+0];
	nj=gbuffer[grid*ngridvars+1];
	if (ndim == 3)
	   nk=gbuffer[grid*ngridvars+2];
	else
	   nk = 1;

	if(ndim==3){
	    printf("object \"connectionsarray%d\" class gridconnections" 
		    " counts %d %d %d\n", grid, ni, nj, nk);
	    printf("attribute \"element type\" string \"cubes\"\n");
	}
	else{
	    printf("object \"connectionsarray%d\" class gridconnections" 
		    " counts %d %d\n", grid, ni, nj);
	    printf("attribute \"element type\" string \"quads\"\n");
	}
	printf("attribute \"ref\" string \"positions\"\n");
 
	vbuffer=(float*)malloc(ni*nj*nk*ndim*sizeof(float));
	for (xyz=0; xyz<ndim; xyz++){
	    for (k=0; k<nk; k++){
		for (j=0; j<nj; j++){
		    for (i=0; i<ni; i++){
			fread(&datavalue,4,1,xyzinput);
			index=(i*(nj*nk)+j*nk+k)*ndim+xyz;
			vbuffer[index]=datavalue;

		    }
		}
	    }
	}

	if(ndim==3){
	    printf("object \"positionsarray%d\" class array type float rank 1 " 
	    "shape %d items %d %s data follows\n", grid, ndim, ni*nj*nk,
	    ascii ? "ascii":"binary msb");
	}
	else{
	    printf("object \"positionsarray%d\" class array type float rank 1 " 
	    "shape %d items %d %s data follows\n", grid, ndim, ni*nj,
	    ascii ? "ascii":"binary msb");
	}
	if (ascii==1){
	    for (i=0; i<ni; i++){
		for (j=0; j<nj; j++){
		    for (k=0; k<nk; k++){
			for (xyz=0; xyz<ndim; xyz++){
				index=(i*(nj*nk)+j*nk+k)*ndim+xyz;
				printf("%f ", vbuffer[index]);
			    }
			printf("\n"); 
		    }
		}
	    }
	}
	else{
	    fwrite(vbuffer,sizeof(float), ndim*ni*nj*nk, stdout);
	    printf("\n"); 
	}
	/* now read iblank data, if any */
	if(iblank==1){
	    ibuffer=(int*)calloc(ni*nj*nk, sizeof(int));
	    for (k=0; k<nk; k++){
		for (j=0; j<nj; j++){
		    for (i=0; i<ni; i++){
			fread(&invaliddata,4,1,xyzinput);
			index=(i*(nj*nk)+j*nk+k);
			ibuffer[index]=invaliddata;
		    }
		}
	    }
	    /* remove trailing byte count indicator  */
	    if(ndim==3){
		printf("object \"invalidarray%d\" class array type int" 
		" items %d %s data follows\n", grid, ni*nj*nk,
		ascii ? "ascii":"binary msb");
	    }
	    else{
		printf("object \"invalidarray%d\" class array type int" 
		" items %d %s data follows\n", grid, ndim, ni*nj,
		ascii ? "ascii":"binary msb");
	    }
	    if (ascii==1){
		for (i=0; i<ni; i++){
		    for (j=0; j<nj; j++){
			for (k=0; k<nk; k++){
			    index=(i*(nj*nk)+j*nk+k);
			   /* if(ibuffer[index]==0)
				printf("%d ",1); 
			    else  
				printf("%d ",0);
			    printf("\n");*/ 
			    printf("%d \n",ibuffer[index]); 
			}
		    }
		}
	    }
	    else{
		fwrite(ibuffer,sizeof(int),ni*nj*nk,stdout);
		printf("\n"); 
	    }
	    printf("attribute \"dep\" string \"positions\"\n");
	}
	free(ibuffer);
	free(vbuffer);
	if(lt==1) fread(&extraneous,4,1,xyzinput);
    }
 
/* start reading data from q-file (data) */

/* if multi-grid, find out how many grids */
/* note: this better agree with xyz file or else ... */
    if(mgrid==1){
	/* remove leading byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,qinput);
	fread(&ngrid,4,1,qinput);
	/* remove trailing byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,qinput);
    }
    else ngrid=1;

/* for each grid, find number of i, j and k (if any) indicies */
/* note: this better agree with xyz file or else ... */
    /* remove leading byte count indicator  */
    if(lt==1) fread(&extraneous,4,1,qinput);
    for (grid=0; grid<ngrid; grid++){
	    fread(&gbuffer[grid*ngridvars+0],4,1,qinput);
	    fread(&gbuffer[grid*ngridvars+1],4,1,qinput);
	    if(ndim ==3){
		fread(&gbuffer[grid*ngridvars+2],4,1,qinput);
	    }
	else gbuffer[grid*ngridvars+2] = 1;
    }
    /* remove trailing byte count indicator  */
    if(lt==1) fread(&extraneous,4,1,qinput);
 
/* now start reading grid data */
    for (grid=0; grid<ngrid; grid++){

	/* read global values */
	/* remove leading byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,qinput);
	fread(&fsmach,4,1,qinput);
	fread(&alpha, 4,1,qinput);
	fread(&re,    4,1,qinput);
	fread(&time,  4,1,qinput);
	/* remove trailing byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,qinput);

	ni=gbuffer[grid*ngridvars+0];
	nj=gbuffer[grid*ngridvars+1];
	nk=gbuffer[grid*ngridvars+2];
	sbuffer=(float*)malloc(ni*nj*nk*sizeof(float));
	vbuffer=(float*)malloc(ni*nj*nk*ndim*sizeof(float));
 
	/* remove leading byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,qinput);
	/* density (scalar) first ... */
	for (k=0; k<nk; k++){
	    for (j=0; j<nj; j++){
		for (i=0; i<ni; i++){
		    fread(&datavalue,4,1,qinput);
		    index=(i*(nj*nk)+j*nk+k);
		    sbuffer[index]=datavalue;
		}
	    }
	}
	if(ndim==3){
	    printf("object \"densityarray%d\" class array type float rank 0" 
	    " items %d %s data follows\n", grid, ni*nj*nk,
	    ascii ? "ascii":"binary msb");
	}
	else{
	    printf("object \"densityarray%d\" class array type float rank 0" 
	    " items %d %s data follows\n", grid, ni*nj,
	    ascii ? "ascii":"binary msb");
	}
	if (ascii==1){
	    for (i=0; i<ni; i++){
		for (j=0; j<nj; j++){
		    for (k=0; k<nk; k++){
			index=(i*(nj*nk)+j*nk+k);
			printf("%f ",sbuffer[index]); 
			printf("\n"); 
		    }
		}
	    }
	}
	else{
	    fwrite(sbuffer,sizeof(float),ni*nj*nk,stdout);
	    printf("\n"); 
	}
	printf("object \"densityfield%d\" class field\n", grid);
	printf("  component \"positions\" \"positionsarray%d\"\n", grid);
	if(iblank==1){
	printf("  component \"iblank positions\" \"invalidarray%d\"\n", grid);
	}
	printf("  component \"connections\" \"connectionsarray%d\"\n", grid);
	printf("  component \"data\" \"densityarray%d\"\n", grid);
 
/* now momentum - a vector of dimension equal to domain dimension */  
	for (xyz=0; xyz<ndim; xyz++){
	    for (k=0; k<nk; k++){
		for (j=0; j<nj; j++){
		    for (i=0; i<ni; i++){
			fread(&datavalue,4,1,qinput);
			index=(i*(nj*nk)+j*nk+k)*ndim+xyz;
			vbuffer[index]=datavalue;
		    }
		}
	    }
	}
	if(ndim==3){
	    printf("object \"momentumarray%d\" class array type float rank 1 " 
	    "shape %d items %d %s data follows\n", grid, ndim, ni*nj*nk,
	    ascii ? "ascii":"binary msb");
	}
	else{
	    printf("object \"momentumarray%d\" class array type float rank 1 " 
	    "shape %d items %d %s data follows\n", grid, ndim, ni*nj,
	    ascii ? "ascii":"binary msb");
	}
	if (ascii==1){
	    for (i=0; i<ni; i++){
		for (j=0; j<nj; j++){
		    for (k=0; k<nk; k++){
			for (xyz=0; xyz<ndim; xyz++){
			    index=(i*(nj*nk)+j*nk+k)*ndim+xyz;
			    printf("%f ",vbuffer[index]); 
			}
			printf("\n"); 
		    }
		}
	    }
	}
	else{
	    fwrite(vbuffer,ndim*sizeof(float),ni*nj*nk,stdout);
	    printf("\n"); 
	}
	printf("object \"momentumfield%d\" class field\n", grid);
	printf("  component \"positions\" \"positionsarray%d\"\n", grid);
	if(iblank==1){
	printf("  component \"iblank positions\" \"invalidarray%d\"\n", grid);
	}
	printf("  component \"connections\" \"connectionsarray%d\"\n", grid);
	printf("  component \"data\" \"momentumarray%d\"\n", grid);
 
/* now stagnation energy per unit volume (scalar) */
	for (k=0; k<nk; k++){
	    for (j=0; j<nj; j++){
		for (i=0; i<ni; i++){
		    fread(&datavalue,4,1,qinput);
		    index=(i*(nj*nk)+j*nk+k);
		    sbuffer[index]=datavalue;
		}
	    }
	}
	if(ndim==3){
	    printf("object \"stagnationarray%d\" class array type float rank 0" 
	    " items %d %s data follows\n", grid, ni*nj*nk,
	    ascii ? "ascii":"binary msb");
	}
	else{
	    printf("object \"stagnationarray%d\" class array type float rank 0" 
	    " items %d %s data follows\n", grid, ni*nj,
	    ascii ? "ascii":"binary msb");
	}
	if (ascii==1){
	    for (i=0; i<ni; i++){
		for (j=0; j<nj; j++){
		    for (k=0; k<nk; k++){
			index=(i*(nj*nk)+j*nk+k);
			printf("%f ",sbuffer[index]); 
			printf("\n"); 
		    }
		}
	    }
	}
	else{
	    fwrite(sbuffer,sizeof(float),ni*nj*nk,stdout);
	    printf("\n"); 
	}
	printf("object \"stagnationfield%d\" class field\n", grid);
	printf("  component \"positions\" \"positionsarray%d\"\n", grid);
	if(iblank==1){
	printf("  component \"iblank positions\" \"invalidarray%d\"\n", grid);
	}
	printf("  component \"connections\" \"connectionsarray%d\"\n", grid);
	printf("  component \"data\" \"stagnationarray%d\"\n", grid);
	free(sbuffer);
	free(vbuffer);
	/* remove trailing byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,qinput);
    }
 
    if(strcmp("none",ffile)){
	/* start reading data from f-file (data) */
	/* if multi-grid, find out how many grids */
	/* note: this better agree with xyz file or else ... */
	if(mgrid==1){
	    /* remove leading byte count indicator  */
	    if(lt==1) fread(&extraneous,4,1,finput);
	    fread(&ngrid,4,1,finput);
	    /* remove trailing byte count indicator  */
	    if(lt==1) fread(&extraneous,4,1,finput);
	}
	else ngrid=1;

    /* for each grid, find number of i, j and k (if any) indicies */
    /* note: this better agree with xyz file or else ... */
	/* remove leading byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,finput);
	for (grid=0; grid<ngrid; grid++){
	    fread(&gbuffer[grid*ngridvars+0],4,1,finput);
	    fread(&gbuffer[grid*ngridvars+1],4,1,finput);
	    if(ndim ==3){
		fread(&gbuffer[grid*ngridvars+2],4,1,finput);
	    }
	    else gbuffer[grid*ngridvars+2] = 1;
	    fread(&gbuffer[grid*ngridvars+3],4,1,finput);
	}
	/* remove trailing byte count indicator  */
	if(lt==1) fread(&extraneous,4,1,finput);
     
	/* now start reading grid data */
	for (grid=0; grid<ngrid; grid++){
	    ni=gbuffer[grid*ngridvars+0];
	    nj=gbuffer[grid*ngridvars+1];
	    nk=gbuffer[grid*ngridvars+2];
	    nf=gbuffer[grid*ngridvars+3];
	    sbuffer=(float*)malloc(ni*nj*nk*sizeof(float));
	    vbuffer=(float*)malloc(ni*nj*nk*ndim*sizeof(float));
     
	    /* remove leading byte count indicator  */
	    if(lt==1) fread(&extraneous,4,1,finput);
	    for (field=0; field<nf; field++){
		for (k=0; k<nk; k++){
		    for (j=0; j<nj; j++){
			for (i=0; i<ni; i++){
			    fread(&datavalue,4,1,finput);
			    index=(i*(nj*nk)+j*nk+k);
			    sbuffer[index]=datavalue;
			}
		    }
		}
		if(ndim==3){
		    printf("object \"f%darray%d\"class array type float rank 0"
		    " items %d %s data follows\n", field+1, grid, ni*nj*nk,
		    ascii ? "ascii":"binary msb");
		}
		else{
		    printf("object \"f%darray%d\" class array type float rank 0" 
		    " items %d %s data follows\n", field+1, grid, ni*nj,
		    ascii ? "ascii":"binary msb");
		}
		if (ascii==1){
		    for (i=0; i<ni; i++){
			for (j=0; j<nj; j++){
			    for (k=0; k<nk; k++){
				index=(i*(nj*nk)+j*nk+k);
				printf("%f ",sbuffer[index]); 
				printf("\n"); 
			    }
			}
		    }
		}
		else{
		    fwrite(sbuffer,sizeof(float),ni*nj*nk,stdout);
		    printf("\n"); 
		}
		printf("object \"f%dfield%d\" class field\n", field+1, grid);
		printf("  component \"positions\" \"positionsarray%d\"\n", grid);
		if(iblank==1){
		    printf("  component \"iblank positions\" \"invalidarray%d\"\n", grid);
		}
		printf("  component \"connections\" \"connectionsarray%d\"\n", grid);
		printf("  component \"data\" \"f%darray%d\"\n", field+1, grid);
	    }
	    /* remove trailing byte count indicator  */
	    if(lt==1) fread(&extraneous,4,1,finput);
	}
    }
     
/* and collect the multiple grids into a data explorer multi-grid */
    if (composite==1) printf("object \"densityfield\" class compositefield\n");
    else printf("object \"densityfield\" class multigrid\n");
    for (grid=0; grid<ngrid; grid++){
	printf("  member %d value \"densityfield%d\"\n", grid, grid);
    }
    if (composite==1) printf("object \"momentumfield\" class compositefield\n");
    else printf("object \"momentumfield\" class multigrid\n");
    for (grid=0; grid<ngrid; grid++){
	printf("  member %d value \"momentumfield%d\"\n", grid, grid);
    }
    if (composite==1) printf("object \"stagnationfield\" class compositefield\n");
    else printf("object \"stagnationfield\" class multigrid\n");
    for (grid=0; grid<ngrid; grid++){
	printf("  member %d value \"stagnationfield%d\"\n", grid, grid);
    }
    if(strcmp("none",ffile)){
	for (field=0; field<nf; field++){
	    if (composite==1) printf("object \"f%dfield\" class compositefield\n", field+1);
	    else printf("object \"f%dfield\" class multigrid\n", field+1);
	    for (grid=0; grid<ngrid; grid++){
		printf("  member %d value \"f%dfield%d\"\n", grid, field+1, grid);
	    }
	}
    }
 
/* and collect all the multigrid fields into a group */
    printf("object \"Plot3Ddata\" class group\n");
    printf("  attribute \"fsmach\" number %f\n", fsmach);
    printf("  attribute \"alpha\" number %f\n", alpha);
    printf("  attribute \"re\" number %f\n", re);
    printf("  attribute \"time\" number %f\n", time);
    printf("  member \"density\" value \"densityfield\"\n");
    printf("  member \"momentum\" value \"momentumfield\"\n");
    printf("  member \"stagnation\" value \"stagnationfield\"\n");
    if(strcmp("none",ffile)){
	for (field=0; field<nf; field++){
	    printf("  member \"function%d\" value \"f%dfield\"\n", field+1, field+1);
	}
    }
 skip:
 
    fclose(xyzinput);
    fclose(qinput);
    if(strcmp("none",ffile)){
	fclose(finput);
    }
     
}
