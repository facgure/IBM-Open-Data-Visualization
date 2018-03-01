/******************************************************************************
 * PURPOSE: PIC2dx.c - Reads a BioRad(tm) PIC formatted file and ships
 * it out to stdout.  This will allow a user to employ the '!' syntax in
 * the import module(s) for DX nets wishing to read the PIC file.
 *
 *
 * NOTES:   Compile with:
 *             gcc -I. -fullwarn -g -o PIC2dx PIC2dx.c -lmalloc -lm
 * HISTORY: 07/99, George McGregor, EPA/MMTSI, Created.
 *          07/99, Randall Hopper, EPA/LMOS, speed up and NT port.
 *          08/99, Randall Hopper, add:
 *                 - scale DX file to real-world units (microns) based on 
 *                   NOTE AXIS specs
 *                 - reject unsupported files (rotational slices about an axis,
 *                   time course set, 16-bit samples, etc.); file must be 
 *                   a rectilinear X,Y,Z volume registered in microns
 *                 - more error checking
 *                 - NOTE VARIABLEs and some other relevent NOTE info read 
 *                   and attached to DX field as attributes, as well as
 *                   formatted and attached as a "Caption" attribute (see below)
 *
 * DX FIELD NOTES:
 * 
 *    - Field image volume is registered in microns
 *    - ATTRIBUTES
 *      - VARIABLEs   - Attached to field using an attribute with the same name
 *                      (e.g. attribute "LENS_MAGNIFICATION" number 60.000000)
 *      - "Filename"      - Name of PIC file on disk
 *      - "Original Name" - Name of PIC file from header
 *      - "Mixer"         - For a multi-section image, the mixer ('A'|'B'|'C')
 *                   
 *****************************************************************************/


/*================================ INCLUDES =================================*/


#include <stdio.h>
#include <stdlib.h>     /* For atoi(), rand().           */
#include <string.h>     /* For strncpy(), memcmp().      */
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _WIN32
#  include <io.h>       /*  _setmode()  */
#endif
#include <assert.h>

/*================================ DEFINES ==================================*/

#define INT16 unsigned short

#define HEADER_LEN 76
#define NOTE_LEN   96

#define AXT_D       1               /* distance in microns  		*/
#define AXT_T		2				/* time in sec					*/
#define AXT_A		3				/* angle in degrees				*/
#define AXT_I		4				/* intensity in grey levels		*/
#define AXT_M4		5				/* 4-bit merged image			*/
#define AXT_R		6				/* Ratio						*/
#define AXT_LR		7				/* Log Ratio					*/
#define AXT_P		8				/* Product						*/
#define AXT_C		9				/* Calibrated					*/
#define AXT_PHOTON	10				/* intensity in photons/sec		*/
#define AXT_RGB     11              /* RGB type                     */
#define AXT_SEQ		12				/* SEQ type (eg 'experiments')	*/
#define AXT_6D		13				/* 6th level of axis			*/
#define AXT_TC		14				/* Time Course axis				*/
#define AXT_S		15				/* Intensity signoid cal		*/
#define AXT_LS		16				/* Intensity log signoid cal	*/
#define AXT_BASE	0xFF			/* mask for axis TYPE			*/
#define AXT_XY		0x100			/* axis is XY, needs updating by LENS */
#define AXT_WORD	0x200			/* axis is word. only corresponds to axis[0] */

#define MIN(a,b) ((a)<(b)?(a):(b))
#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))
#define SWAP_HWORDS(A) (((A) >> 16) & 0x0000ffff) | (((A) << 16 ) & 0xffff0000)
#define SWAP_BYTES(A) (((A) >> 8 ) & 0x00ff00ff) | (((A) << 8 ) & 0xff00ff00)

/*================================= ENUMS ===================================*/

enum
{
	MERGE_OFF = 0, /* Image is not merged 							*/
	MERGE_16 = 1, /* All Pixels merged. 16 color (4-bit) 			*/
	MERGE_ALTERNATE = 2, /* Alternate pixels merged, 128 color (7-bit) 	*/
	MERGE_COLUMN = 3, /* Alternate columns merged						*/
	MERGE_ROW = 4, /* Alternate rows merged						*/
	MERGE_MAXIMUM = 5, /* Maximum pixels merged						*/
	MERGE_OPT12 = 6, /* 64-color (12-bit) optimized 2-image merge	*/
	MERGE_OPT12_V2 = 7, /* As above except convert look up table		*/
	/* saved after the notes in file, as opposed	*/
	/* to at the end of each image data.			*/
};

enum
{
	NOTE_TYPE_LIVE = 1, /* Information about live collection		*/
	NOTE_TYPE_FILE1 = 2, /* Note from image #1						*/
	NOTE_TYPE_NUMBER = 3, /* Number in multiple image file			*/
	NOTE_TYPE_USER = 4, /* User notes generated notes				*/
	NOTE_TYPE_LINE = 5, /* Line mode info							*/
	NOTE_TYPE_COLLECT = 6, /* Collect mode info						*/
	NOTE_TYPE_FILE2 = 7, /* Note from image #2						*/
	NOTE_TYPE_SCALEBAR = 8, /* Scale bar info							*/
	NOTE_TYPE_MERGE = 9, /* Merge Info								*/
	NOTE_TYPE_THRUVIEW = 10, /* Thruview Info							*/
	NOTE_TYPE_ARROW = 11, /* Arrow info								*/
	NOTE_TYPE_VARIABLE = 20, /* Again internal variable ,except held as  */
	NOTE_TYPE_STRUCTURE = 21 /* a structure. */
};

enum
{
	NOTES_STATUS_ALL = 0x0100, /* Whether not is displayed on all images */
	NOTES_STATUS_DISPLAY = 0x0200, /* Note is currently displayed			*/
	NOTES_STATUS_POSITION = 0X0400 /* Note has been positioned by the user	*/
};

/*================================= TYPES ===================================*/

typedef struct {
	INT16 nx, ny; /*  0 2*2  image width and height in pixels*/
	INT16 npic; /*  4 2    number of images in file*/
	INT16 ramp1_min, 
	ramp1_max; /*  6 2*2  LUT1 ramp min. and max.*/
	unsigned notes; /* 10 4    no notes=0; has notes=non zero*/
	INT16 byte_format; /* 14 2    bytes=TRUE(1); words=FALSE(0)*/
	INT16 n; /* 16 2    image number within file*/
	char name[32]; /* 18 32   file name*/
	INT16 merged; /* 50 2    merged format*/
	INT16 color1; /* 52 2    LUT1 color status*/
	INT16 file_id; /* 54 2    valid .PIC file=12345*/
	INT16 ramp2_min, ramp2_max; /* 56 2*2  LUT2 ramp min. and max.*/
	INT16 color2; /* 60 2    LUT2 color status*/
	INT16 edited; /* 62 2    image has been edited=TRUE(1)*/
	INT16 lens; /* 64 2    Integer part of lens magnification*/
	float mag_factor; /* 66 4    4 byte real mag. factor (old ver.)*/
	INT16 dummy[3]; /* 70 6    6 byte filler now,
		                               (older versions stored a 4 byte real lens 
		                                mag here.) */
}
PIC_header;

typedef struct { /*  Other PIC file information we want to track */
	int image_size_in_bytes;
	long notes_offset;
	long file_size;

	float origin[3]; /*  3D origin         X,Y,Z, in microns  */
	float delta [3]; /*  3D voxel spacings X,Y,Z, in microns  */

	int mixer_num; /*  A=0,B=1,C=2  (-1 = N/A)              */
	int num_channel_files; /*  One channel per file                 */
}
PIC_info;

typedef struct {
	INT16 level;
	unsigned int next;
	INT16 num;
	INT16 status;
	INT16 type;
	INT16 x;
	INT16 y;
	char text[81];
}
PIC_note;


/* Union for endian check */
union {
	long l;
	char c[sizeof (long)];
}
u;

static int bigEndian=0; /* global endian check */


/*============================ PUBLIC FUNCTIONS =============================*/


/******************************************************************************
 * PURPOSE: main - Tests the data import routines by reading the named
 *          file and writing a full copy and then reading a series of random
 *          subsets from the file.
 * INPUTS:  int argc     The number of arguments. Should be 2.
 *          char* argv[] The argument strings: argv[1] = name of file to read.
 * OUTPUTS: None
 * RETURNS: 0 if successful, else 1.
 * NOTES:   Usage:   PIC2dx -i <input-file-name> [-o <output-file-name>] 
 *          Example: PIC2dx -i cells01.pic
 *****************************************************************************/

void bytes_to_short(unsigned short *, FILE *);
void bytes_to_int(unsigned int *, FILE *);
void PIC_read_header( FILE *PICfs, PIC_header *header );
void PIC_getAxisInfo( FILE *PICfs, const PIC_header *header, PIC_info *info );
void PIC_parse_variable_note( const PIC_note *note, 
char name[81], char val[81] );
int is_scalar_variable( char name[] );
void PIC_addNotesAsAttr( FILE *PICfs,
const PIC_header *header,
PIC_info *info, FILE *outfs);
void PIC_addImageNotes( FILE *PICfs,
const PIC_header *header, const int picNum,
PIC_info *info, FILE *outfs);

int
main(int argc, char **argv)
{
	FILE *PICfs, *outfs;

	int i;
	int objCounter=1, posObj=1, conObj=2;

	char *PIC_Filename = NULL, *outputFileName= NULL, *p;
	unsigned char data, *id;
	float out_data, *od;
	int arg;
	int seriesSet = 0;
	PIC_header header;
	PIC_info info;

	/* Determine system endianess */
	u.l = 1;
	bigEndian = u.c[sizeof (long) -1]; /* LSB = 0, MSB = 1 */

	id = &data;
	od = &out_data;

	/*  PARSE ARGUMENTS  */
	for ( arg = 1; arg < argc; arg++ )
		if ( strcmp( argv[arg], "-i" ) == 0 && arg < argc-1 )
		{
			PIC_Filename = argv[arg+1];
			arg++;
		}
		else if ( strcmp( argv[arg], "-o" ) == 0 && arg < argc-1 )
		{
			outputFileName = argv[arg+1];
			arg++;
		}
		else if ( strcmp( argv[arg], "-series" ) == 0 )
		{
			seriesSet = 1;
		}
	else
		break;

	if ( arg < argc-1 )
	{
		fprintf( stderr, "Unknown option: %s\n", argv[arg] );
		fprintf( stderr, "Usage: pic2dx -i inputfile -o outputfile -series\n" );
		exit(1);
	}

	if ( !PIC_Filename )
	{
		fprintf( stderr, "No input filename was specified\n" );
		fprintf( stderr, "Usage: pic2dx -i inputfile -o outputfile -series\n" );
		exit(1);
	}

	PICfs = fopen(PIC_Filename, "rb");
	if ( !PICfs )
	{
		fprintf( stderr, "Couldn't open input file, check filename and permissions.\n" );
		exit(1);
	}

	if(outputFileName)
		outfs = fopen(outputFileName, "wb");
	else
		{
#ifdef _WIN32
		_setmode(fileno(stdout),O_BINARY);
#endif
		outfs = stdout;
	};
	if ( !outfs )
	{
		fprintf( stderr, "Couldn't open output file, check filename and permissions.\n" );
		exit(1);
	}

	/*  READ FILE HEADER AND SPECS FROM NOTES  */
	PIC_read_header( PICfs, &header );
	if( header.notes != 0)
		PIC_getAxisInfo( PICfs, &header, &info );

	/* My code starts here. --------------------------------------*/

	if(seriesSet) { /* Out as series data */
		fprintf(outfs, "object %d class gridpositions counts %d %d\n", 
		posObj, header.ny, header.nx);
		fprintf(outfs, "  origin %g %g\n", info.origin[1], info.origin[0] );
		fprintf(outfs, "  delta %g 0\n", info.delta[1]);
		fprintf(outfs, "  delta 0 %g\n", info.delta[0]);
		fprintf(outfs, "attribute \"dep\" string \"positions\"\n");
		fprintf(outfs, "\n#\n");

		fprintf(outfs, "object %d class gridconnections counts %d %d\n",
		conObj, header.ny, header.nx);
		fprintf(outfs, "attribute \"element type\" string \"quads\"\n");
		fprintf(outfs, "attribute \"dep\" string \"connections\"\n");
		fprintf(outfs, "attribute \"ref\" string \"positions\"\n");
		fprintf(outfs, "\n#\n");

		/* If series, do loop over number of images */

		objCounter=3;
		for(i=0; i< header.npic; i++) {
			fprintf(outfs, "object %d class array type %s rank 0 items %d lsb ieee \n",
			objCounter++, header.byte_format?"unsigned char":"unsigned 2-byte integer",
			header.nx*header.ny);
			fprintf(outfs, "data file %s,%d\n", PIC_Filename, 
			HEADER_LEN+i*header.nx*header.ny*(header.byte_format?1:2));
			fprintf(outfs, "attribute \"dep\" string \"positions\"\n");
			PIC_addImageNotes( PICfs, &header, i+1, &info, outfs);
			fprintf(outfs, "\n#\n");
		}
		for(i=0; i< header.npic; i++) {
			fprintf(outfs, "object %d class field\n", objCounter++);
			fprintf(outfs, "  component \"positions\" value %d\n", posObj);
			fprintf(outfs, "  component \"connections\" value %d\n", conObj);
			fprintf(outfs, "  component \"data\" value %d\n", objCounter-header.npic-1);
			fprintf(outfs, "#\n");
		}
		fprintf(outfs, "object \"default\" class series\n");
		for(i=0; i< header.npic; i++)
		{
			fprintf(outfs, "  member %d position %g value %d\n", i, i*info.delta[2], objCounter-header.npic+i);
		}
		fprintf(outfs, "#\n\nend\n");
	}
	else /* Out as 3-D data. */
	{
		fprintf(outfs, "object %d class gridpositions counts %d %d %d\n", posObj,
		header.npic, header.ny, header.nx );
		fprintf(outfs, "  origin  %g   %g   %g\n", 
		info.origin[2], info.origin[1], info.origin[0] );
		fprintf(outfs, "  delta   0   0   %g\n", info.delta[2] );
		fprintf(outfs, "  delta   0   %g   0\n", info.delta[1] );
		fprintf(outfs, "  delta   %g   0   0\n", info.delta[0] );
		fprintf(outfs, "attribute \"dep\" string \"positions\"\n");
		fprintf(outfs, "#\n");

		fprintf(outfs, "object %d class gridconnections counts %d %d %d\n", conObj,
		header.npic, header.ny, header.nx);
		fprintf(outfs, "attribute \"element type\" string \"cubes\"\n");
		fprintf(outfs, "attribute \"dep\" string \"connections\"\n");
		fprintf(outfs, "attribute \"ref\" string \"positions\"\n");
		fprintf(outfs, "#\n");

		fprintf(outfs, "object %d class array type %s rank 0 items %d lsb ieee\n", 
		conObj+1, header.byte_format?"unsigned char":"unsigned 2-byte integer", 
		header.npic*header.nx*header.ny);
		fprintf(outfs, "data file %s,%d\n", PIC_Filename, 
		HEADER_LEN);
		fprintf(outfs, "attribute \"dep\" string \"positions\"\n");
		fprintf(outfs, "#\n");
		fprintf(outfs, "object \"PIC\" class field\n");
		fprintf(outfs, "  component \"positions\" value %d\n", posObj);
		fprintf(outfs, "  component \"connections\" value %d\n", conObj);
		fprintf(outfs, "  component \"data\" value %d\n", conObj+1);

	}

	/* Begin adding field attributes. */

	if ( (p = strrchr( PIC_Filename, '/' )) != NULL )
		p++;
	else
		p = PIC_Filename;

	fprintf(outfs, "attribute \"filename\" string \"%s\"\n", p);
	fprintf( outfs, "attribute \"original_name\" string \"%s\"\n", header.name );
	fprintf(outfs, "attribute \"lens_magnification\" value %d\n", header.lens);
	if(header.mag_factor > 0)
		fprintf(outfs, "attribute \"mag_factor\" value %f\n", header.mag_factor);
	if(header.edited == 1)
		fprintf(outfs, "attribute \"edited\" value 1\n");

	PIC_addNotesAsAttr( PICfs, &header, &info, outfs);

	if ( info.mixer_num >= 0 )
		fprintf( outfs, "attribute \"Mixer\" string \"%c\"\n", 
		info.mixer_num + 'A' );

	fprintf(outfs, "\n#\nend\n");

	if(PICfs)
		fclose(PICfs);
	if(outfs)
		fclose( outfs );

	return(0);
}

/* bytes_to_short */

void
bytes_to_short(unsigned short *value, FILE *PICfs)
{

	unsigned char in[2];
	unsigned short val;

	fread (in, 2, sizeof (unsigned char), PICfs);
	memcpy(&val, in, 2);

	if(bigEndian) {
		*value = SWAP_BYTES(val);
	}
	else
		*value = val;
}

/* bytes_to_int */

void
bytes_to_int(unsigned int *value, FILE *PICfs)
{
	unsigned char in[4];
	unsigned int val;

	fread (in, 4, sizeof (unsigned char), PICfs);
	memcpy(&val, in, 4);

	if(bigEndian)
	{
		*value = SWAP_BYTES(SWAP_HWORDS(val));
	}
	else
		*value = val;
}

void
bytes_to_float(float *value, FILE *PICfs)
{
	unsigned char in[4];
	unsigned int val;

	fread (in, 4, sizeof (unsigned char), PICfs);
	memcpy(&val, in, 4);

	if(bigEndian)
	{
		val = SWAP_BYTES(SWAP_HWORDS(val));
		memcpy(value, &val, 4);
	}
	else
		memcpy(value, &val, 4);
}

void PIC_read_header( FILE *PICfs, PIC_header *header )
{
	assert( header && PICfs );

	fseek( PICfs, 0, SEEK_SET );

	bytes_to_short(&header->nx, PICfs); 
	bytes_to_short(&header->ny, PICfs);
	bytes_to_short(&header->npic, PICfs);

	/* Fill in the rest of the data */

	bytes_to_short(&header->ramp1_min, PICfs);

	bytes_to_short(&header->ramp1_max, PICfs);
	bytes_to_int(&header->notes, PICfs);

	bytes_to_short(&header->byte_format, PICfs);
	bytes_to_short(&header->n, PICfs);

	fread (&header->name[0], 32, sizeof (unsigned char), PICfs);

	bytes_to_short(&header->merged, PICfs);
	if(header->merged != 0) {
		printf("Merged Bio-Rad(TM) files currently not supported.\n");
		exit(0);
	}
	bytes_to_short(&header->color1, PICfs);
	bytes_to_short(&header->file_id, PICfs);
	if(header->file_id != 12345) {
		printf("This does not seem to be a Bio-Rad(TM) .PIC Image File.\n");
		exit(0);
	}
	bytes_to_short(&header->ramp2_min, PICfs);
	bytes_to_short(&header->ramp2_max, PICfs);
	bytes_to_short(&header->color2, PICfs);
	bytes_to_short(&header->edited, PICfs);
	bytes_to_short(&header->lens, PICfs);
	bytes_to_float(&header->mag_factor, PICfs);

	bytes_to_short(&header->dummy[0], PICfs);
	bytes_to_short(&header->dummy[1], PICfs);
	bytes_to_short(&header->dummy[2], PICfs);
}


int PIC_read_nth_note( FILE *PICfs,
const PIC_header *header, 
const PIC_info *info, int index, 
PIC_note *note )
{
	long note_offset = info->notes_offset + index * NOTE_LEN;

	if ( note_offset+NOTE_LEN > info->file_size )
		return 0;

	fseek( PICfs, note_offset, SEEK_SET );

	bytes_to_short(&note->level , PICfs);
	bytes_to_int (&note->next , PICfs);
	bytes_to_short(&note->num , PICfs);
	bytes_to_short(&note->status, PICfs);
	bytes_to_short(&note->type , PICfs);
	bytes_to_short(&note->x , PICfs);
	bytes_to_short(&note->y , PICfs);
	fread (&note->text[0], 80, 1, PICfs);
	note->text[ sizeof(note->text)-1 ] = '\0';

#ifdef DEBUG	
	fprintf(stdout, "NOTE_TYPE: %d, TEXT: %s\n", note->type, note->text);
	fprintf(stdout, "\tNEXT: %d\tNUM: %d\tSTATUS: %d\n", note->next, note->num, note->status);
	fprintf(stdout, "\tX: %d\tY: %d\n", note->x, note->y);
#endif

	return 1;
}

void PIC_addImageNotes( FILE *PICfs,
const PIC_header *header, const int picNum,
PIC_info *info, FILE *outfs)
{
	int i = 0;
	int noteLive = 0;
	PIC_note note;

	while ((PIC_read_nth_note( PICfs, header, info, i++, &note )))
	{
		if (note.type == NOTE_TYPE_LIVE && picNum == note.status)
			fprintf( outfs, "attribute \"note_live_%d\" string \"%s\"\n", noteLive++, note.text);
	}

}

int is_scalar_variable( char name[] )
{
	static const char *Scalar_vars[] =
		{
		"SCALE_FACTOR" , "LENS_MAGNIFICATION", 
		"PIXEL_BIT_DEPTH", "Z_CORRECT_FACTOR"
	};
	int i;

	for ( i = 0; i < ARRAY_LEN( Scalar_vars ); i++ )
		if ( strcmp( Scalar_vars[i], name ) == 0 )
			return 1;
	return 0;
}

void PIC_addNotesAsAttr( FILE *PICfs,
const PIC_header *header,
PIC_info *info, FILE *outfs)
{
	int i = 0;
	int noteLive = 0;
	PIC_note note;
#define NOTELENGTH 80
#define MAXNOTES 30
	char buf[NOTELENGTH+1 * MAXNOTES] = "\0";
	int captionCount = MAXNOTES;
	char varname[81];
	char vartext[81];

	while ((PIC_read_nth_note( PICfs, header, info, i++, &note )))
	{
		if (note.type == NOTE_TYPE_LIVE && note.status == 1)
			fprintf( outfs, "attribute \"NOTE_LIVE_%d\" string \"%s\"\n", noteLive++, note.text);

		if (note.type == NOTE_TYPE_FILE1 || note.type == NOTE_TYPE_FILE2)
			fprintf( stderr, "NOTE_TYPE_FILE1: attribute \"note_file\" string \"%s\"\n", note.text);

#ifdef REDUNDANT
		if (note.type == NOTE_TYPE_NUMBER)
			fprintf( stderr, "NOTE_TYPE_NUMBER: %s\n", note.text);
#endif


		if (( note.type == NOTE_TYPE_VARIABLE ) &&
			( strncmp( note.text, "AXIS_4", 6 ) == 0 )) /* Z Axis */
		{
			int axis_type; 
			float deg1, deg2;
			sscanf( &note.text[8], "%d", &axis_type);
			if(axis_type == AXT_A) {
				sscanf(&note.text[8], "%d %g %g ", &axis_type, 
				&deg1, &deg2);
				fprintf(outfs, "attribute \"AXIS_ANGLE1\" value %g\n", deg1);
				fprintf(outfs, "attribute \"AXIS_ANGLE2\" value %g\n", deg2);
			}
		}
		if (( note.type == NOTE_TYPE_VARIABLE ) &&
			( strncmp( note.text, "AXIS_5", 6 ) == 0 )) /* Z Axis */
		{
			int axis_type; 
			float deg1, deg2;
			sscanf( &note.text[8], "%d", &axis_type);
			if(axis_type == AXT_A) {
				sscanf(&note.text[8], "%d %g %g ", &axis_type, 
				&deg1, &deg2);
				fprintf(outfs, "attribute \"AXIS_ANGLE3\" value %g\n", deg1);
				fprintf(outfs, "attribute \"AXIS_ANGLE4\" value %g\n", deg2);
			}
		}
		/*  Extract VARIABLE notes with "AXIS_9" at the front  */
		if (( note.type == NOTE_TYPE_VARIABLE ) &&
			( strncmp( note.text, "AXIS_9", 6 ) == 0 ))
		{
			float origin, inc;
			int axis_type;

			sscanf( &note.text[8], "%d %g %g ", &axis_type, &origin, &inc );

			if ( axis_type == AXT_RGB )
			{
				fprintf( outfs, "attribute \"CHANNEL\" value %g\n", origin );
				fprintf( outfs, "attribute \"TOTAL_CHANNELS\" value %g\n", inc );
			}
		}


		if ( note.type == NOTE_TYPE_VARIABLE )
		{
			PIC_parse_variable_note( &note, varname, vartext);
			if ( is_scalar_variable( varname ) )
			{
				fprintf(outfs, "attribute \"%s\" number %s\n", varname, vartext );
				if (captionCount-- > 0)
					sprintf(buf, "%s%-20s: %s\\n", buf, varname, vartext);
			}
			else
				fprintf(outfs, "attribute \"%s\" string \"%s\"\n", varname, vartext );
			if (captionCount-- > 0 )
				sprintf(buf, "%s%-20s: %s\\n", buf, varname, vartext);


		}

		if(note.next == 0)
			break;
	}

	sprintf( buf, "%s%-20s: %.3f,%.3f,%.3f", buf, "X,Y,Z spacing", 
	info->delta[0], info->delta[1], info->delta[2] );
	fprintf( outfs, "attribute \"Caption\" string \"%s\"\n", buf );

}

void PIC_getAxisInfo( FILE *PICfs,
const PIC_header *header, 
PIC_info *info )
{
	int i, axis, axis_type;
	PIC_note note;

	/*  Compute file size and offsets  */
	info->image_size_in_bytes = header->nx * header->ny * 
		( header->byte_format ? 1 : 2 );
	info->notes_offset = HEADER_LEN + header->npic * info->image_size_in_bytes;

	fseek( PICfs, 0, SEEK_END );
	info->file_size = ftell( PICfs );

	if(info->file_size < info->notes_offset + 96) {
		fprintf(stderr, "File seems to be to small, check for corruption.\n");
		exit (1);
	}

	/*  Now read some details from the NOTES  */
	info->origin[0] = info->origin[1] = info->origin[2] = 0.0;
	info->delta [0] = info->delta [1] = info->delta [2] = 1.0;
	info->mixer_num = -1;
	info->num_channel_files = 1;

	i = 0;
	while ((PIC_read_nth_note( PICfs, header, info, i++, &note )))
	{
		/*  Extract VARIABLE notes with "AXIS_[234]" at the front  */
		if (( note.type == NOTE_TYPE_VARIABLE ) &&
			( strncmp( note.text, "AXIS_2", 6 ) == 0 )) /* Horizontal Axis */
		{
			axis = 0;
			sscanf( &note.text[7], " %d %g %g ", &axis_type, 
			&info->origin[ axis ], &info->delta[ axis ] );
			if ( axis_type != AXT_D )
			{
				fprintf( stderr, "Unsupported file.  Axis %d (AXIS_%c) is not a "
					"distance in microns.\n",
				axis, note.text[5] );
				exit(1);
			}
		}
		if (( note.type == NOTE_TYPE_VARIABLE ) &&
			( strncmp( note.text, "AXIS_3", 6 ) == 0 )) /* Vertical Axis */
		{
			axis = 1;
			sscanf( &note.text[7], " %d %g %g ", &axis_type, 
			&info->origin[ axis ], &info->delta[ axis ] );
			if ( axis_type != AXT_D )
			{
				fprintf( stderr, "Unsupported file.  Axis %d (AXIS_%c) is not a "
					"distance in microns.\n",
				axis, note.text[5] );
				exit(1);
			}
		}
		if (( note.type == NOTE_TYPE_VARIABLE ) &&
			( strncmp( note.text, "AXIS_4", 6 ) == 0 )) /* Z Axis */
		{
			axis = 2;
			sscanf( &note.text[7], " %d ", &axis_type);
			if(axis_type == AXT_D) 
				sscanf(&note.text[7], " %d %g %g ", &axis_type, 
				&info->origin[ axis ], &info->delta[ axis ] );
			else {
#ifdef DEBUG
				fprintf(stderr, "Unimplemented AXIS_4\n");
#endif
			}
		}
		/*  Extract VARIABLE notes with "AXIS_9" at the front  */
		if (( note.type == NOTE_TYPE_VARIABLE ) &&
			( strncmp( note.text, "AXIS_9", 6 ) == 0 ))
		{
			float origin, inc;
			sscanf( &note.text[6], " %d %g %g ", &axis_type, &origin, &inc );

			if ( axis_type != AXT_RGB )
			{
				fprintf( stderr, "Unsupported file.  AXIS_9 type is not RGB.\n" );
				exit(1);
			}
			info->mixer_num = origin;
			info->num_channel_files = inc;
		}

		if(note.next == 0)
			break;
	}

}


void PIC_parse_variable_note( const PIC_note *note,
char name[81],
char val [81] )
{
	char text[81], *name_end, *p;

	assert( note->type == NOTE_TYPE_VARIABLE );

	text[0] = '\0';
	strncat( text, note->text, sizeof(text)-1 );

	if ( (p = strchr( text, '=' )) == NULL )
		p = strchr( text, ' ' );
	assert( p && p > text );

	if ( *p == '=' )
	{
		if ( p > text && *(p-1) == ' ' )
			name_end = p-2;
		else
			name_end = p-1;
		p++;
		if ( *p == ' ' )
			p++;
	}
	else 
		{
		name_end = p-1;
		p++;
	}

	name[0] = '\0';
	strncat( name, text, MIN( 80, name_end-text+1 ) );
	val[0] = '\0';
	strncat( val, p, 80 );
}


