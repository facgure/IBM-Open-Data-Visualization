/* dx demo program - select dx visual program to run
 * by looking at thumbnail images of data.
 *
 */


#include <Xm/Xm.h> 
#include <Xm/Form.h> 
#include <Xm/Label.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/Text.h>
#include "dxl.h"

#include <stdio.h>
#include <stdlib.h>


static String DefaultResources[] =
{
    "*background:              #b4b4b4b4b4b4",
    "*foreground:              black",
#ifdef sgi
    "*fontList:                 -adobe-helvetica*bold-r*14*=bold\n\
                                -adobe-helvetica*medium-r*14*=normal\n\
                                -adobe-helvetica*medium-o*14*=oblique",
#else
    "*fontList:                 -adobe-helvetica*bold-r*14*=bold\
                                -adobe-helvetica*medium-r*14*=normal\
                                -adobe-helvetica*medium-o*14*=oblique",
#endif
   "*XmToggleButton.selectColor:            CadetBlue",
    "*XmText.shadowThickness:                1",
    NULL
};


#define MAXBUTTONS 40
#define NUMHIGH 2 
#define BUTTONSIZE 150
#define SHOWMESIZE 20
#define QUITSIZE 40 
DXLConnection   *conn;
DXLConnection   *bconn;

void button_CB(Widget, XtPointer, XtPointer);
void quitbutton_CB(Widget, XtPointer, XtPointer);

#define MAXLEN 1024
static int init_data(char *filename, int *numbuttons, char **programs, char **images);


main(argc, argv)
int argc;
char *argv[];
{
    Widget          toplevel, main_w;
    Widget          picture[MAXBUTTONS],button[MAXBUTTONS],quitbutton;
    XtAppContext    app;
    XmString        xms;
    int             numbuttons, i, row, column, numcolumns;
    char            buf[1024];
    char            *programs[MAXBUTTONS];
    char            *images[MAXBUTTONS];
    
    
    if (argc != 2) {
	fprintf(stderr, "usage: %s net_listfile\n", argv[0]);
	fprintf(stderr, "  where net_listfile contains lists of filenames, 2 per line\n");
	fprintf(stderr, "  the first is a .net file, and the second is an image file\n");
	fprintf(stderr, "  the image should be 150x150 in size.\n");
	exit (-1);
    }
    
    
    /* 
     *  Start the Data Explorer executive.
     */
    conn = DXLStartDX("dx -menubar -noMessageInfoOption -wizard -8bitcmap 1",NULL);
    if (!conn) {
	fprintf(stderr, "could not start dx");
	exit(0);
    }
    
    XtSetLanguageProc (NULL, NULL, NULL);
    
    toplevel = XtVaAppInitialize (&app, "Demos", 
				  NULL, 0, &argc, argv, 
				  DefaultResources, NULL);
    
    /* 
     * Create the user interface for this application
     */
    
    if (init_data(argv[1], &numbuttons, programs, images) < 0)
	exit (-1);
    
    
    numcolumns = (numbuttons-1)/NUMHIGH + 1;
    
    main_w = XtVaCreateManagedWidget("form",
		     xmFormWidgetClass, toplevel,
		     XmNwidth,    (BUTTONSIZE)*numcolumns,
		     XmNheight,   (BUTTONSIZE+SHOWMESIZE)*NUMHIGH +QUITSIZE,
		     XmNfractionBase, 5,
		     NULL);
    
    for (i=0; i<numbuttons; i++) {
	
	xms = XmStringCreateSimple("Show Me");
	if (i==0) {
	    picture[i] = XtVaCreateManagedWidget("draw", 
					 xmDrawingAreaWidgetClass, main_w,
					 XmNwidth, BUTTONSIZE,
					 XmNheight, BUTTONSIZE,
					 XmNlabelString, xms,
					 XmNtopAttachment, XmATTACH_FORM,
					 XmNleftAttachment, XmATTACH_FORM,
					 NULL);
	    button[i] = XtVaCreateManagedWidget("pushb", 
					xmPushButtonWidgetClass, main_w,
					XmNwidth, BUTTONSIZE,
					XmNheight, SHOWMESIZE,
					XmNlabelString, xms,
					XmNtopAttachment, XmATTACH_WIDGET,
					XmNtopWidget,picture[i],
					XmNleftAttachment, XmATTACH_FORM,
					NULL);
	}
	else {
	    column = i/NUMHIGH;
	    row = i-column*NUMHIGH;
	    if (row==0) { 
		picture[i] = XtVaCreateManagedWidget("draw", 
				     xmDrawingAreaWidgetClass, main_w,
				     XmNwidth, BUTTONSIZE,
				     XmNheight, BUTTONSIZE,
				     XmNlabelString, xms,
				     XmNtopAttachment, XmATTACH_FORM,
				     XmNleftAttachment, XmATTACH_WIDGET,
				     XmNleftWidget, picture[i-NUMHIGH],
				     NULL);
		button[i] = XtVaCreateManagedWidget("pushb", 
				    xmPushButtonWidgetClass, main_w,
				    XmNwidth, BUTTONSIZE,
				    XmNheight, SHOWMESIZE,
				    XmNlabelString, xms,
				    XmNtopAttachment, XmATTACH_WIDGET,
				    XmNtopWidget, picture[i],
				    XmNleftAttachment, XmATTACH_WIDGET,
				    XmNleftWidget, button[i-NUMHIGH],
				    NULL);
	    }
	    else {
		if (column==0) {
		    picture[i] = XtVaCreateManagedWidget("draw", 
					 xmDrawingAreaWidgetClass, main_w,
					 XmNwidth, BUTTONSIZE,
					 XmNheight, BUTTONSIZE,
					 XmNlabelString, xms,
					 XmNtopAttachment, XmATTACH_WIDGET,
					 XmNtopWidget, button[i-1],
					 XmNleftAttachment, XmATTACH_FORM,
					 NULL);
		    button[i] = XtVaCreateManagedWidget("pushb", 
					xmPushButtonWidgetClass, main_w,
					XmNwidth, BUTTONSIZE,
					XmNheight, SHOWMESIZE,
					XmNlabelString, xms,
					XmNtopAttachment, XmATTACH_WIDGET,
					XmNtopWidget, picture[i],
					XmNleftAttachment, XmATTACH_FORM,
					NULL);
		}
		else {
		    picture[i] = XtVaCreateManagedWidget("draw", 
					 xmDrawingAreaWidgetClass, main_w,
					 XmNwidth, BUTTONSIZE,
					 XmNheight, BUTTONSIZE,
					 XmNlabelString, xms,
					 XmNtopAttachment, XmATTACH_WIDGET,
					 XmNtopWidget, button[i-1],
					 XmNleftAttachment, XmATTACH_WIDGET,
					 XmNleftWidget, picture[i-NUMHIGH],
					 NULL);
		    button[i] = XtVaCreateManagedWidget("pushb", 
					xmPushButtonWidgetClass, main_w,
					XmNwidth, BUTTONSIZE,
					XmNheight, SHOWMESIZE,
					XmNlabelString, xms,
					XmNtopAttachment, XmATTACH_WIDGET,
					XmNtopWidget, picture[i],
					XmNleftAttachment, XmATTACH_WIDGET,
					XmNleftWidget, button[i-NUMHIGH],
					NULL);
		}
	    }
	}
	
	XtAddCallback(button[i], XmNactivateCallback,
		      (XtCallbackProc)button_CB,
		      programs[i]);
	
	XmStringFree(xms);
    }
    /* do the quit button */
    xms = XmStringCreateSimple("Quit");
    quitbutton = XtVaCreateManagedWidget("pushb", 
				xmPushButtonWidgetClass, main_w,
				XmNwidth, QUITSIZE,
				XmNheight, QUITSIZE,
				XmNlabelString, xms,
				XmNtopAttachment, XmATTACH_WIDGET,
				XmNtopWidget, button[NUMHIGH-1],
				XmNleftAttachment, XmATTACH_FORM,
				XmNrightAttachment, XmATTACH_FORM,
				NULL);
    XtAddCallback(quitbutton, XmNactivateCallback,
		      (XtCallbackProc)quitbutton_CB,
		      conn);
    XtRealizeWidget (toplevel);
 
    
    
    
    XSync(XtDisplay(toplevel), False);
    
    bconn = DXLStartDX("dx -execonly -8bitcmap 1",NULL);
    if (! bconn) {
	fprintf(stderr, "second dxlconn failed\n");
	exit(1);
    }
    
    exDXLLoadScript(bconn, "LoadImage.net");
    for (i=0; i<numbuttons; i++) {
	sprintf(buf, "X8,:0.0,#X%d", XtWindow(picture[i]));
	DXLSetString(bconn, "where", buf);
	DXLSetString(bconn, "img", images[i]);
	DXLExecuteOnce(bconn);
	DXLHandlePendingMessages(bconn);
    }
    
    DXLInitializeXMainLoop(app, conn);
    
    XtAppMainLoop (app);

}

/* open filename.  expects it to contain this format:
 *    .net name    image name
 *   etc
 *
 * the images should be 150x150 and should correspond to the result of running
 * the given .net file.
 */
static int init_data(char *filename, int *numbuttons, char **programs, char **images)
{
    FILE *fp;
    int i, len;
    char temp_prog[MAXLEN], temp_imag[MAXLEN];
    
    if ((fp = fopen(filename, "r")) == NULL) {
	fprintf(stderr, "cannot open '%s' for reading\n", filename);
	return -1;
    }

    *numbuttons = 0;
    i = 0;
    while (1) {
	if (fscanf(fp, "%s %s", temp_prog, temp_imag) != 2)
	    break;

	len = strlen(temp_prog);
	programs[i] = malloc(len + 1);
	if (!programs[i])
	    return -1;
	
	strcpy(programs[i], temp_prog);
	
	len = strlen(temp_imag);
	images[i] = malloc(len + 1);
	if (!images[i])
	    return -1;
	
	strcpy(images[i], temp_imag);
	i++;
    }
    
    *numbuttons = i;
    return 0;
}


/* 
 * The following are the callbacks for the buttons. 
 */
void button_CB(Widget w, XtPointer xp1, XtPointer xp2)
{
   DXLLoadVisualProgram(conn,(char *)xp1);
   DXLExecuteOnce(conn);
}

void quitbutton_CB(Widget w, XtPointer xp1, XtPointer xp2)
{
   DXLConnection *conn =  (DXLConnection *)xp1;
   DXLExitDX(conn);
   exit(1);
}
