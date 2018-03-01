/* this example uses dxcallmodule to display an interactive image 
 * of an isosurface of a data field.
 * The "user interface" is completely independent of the Data Explorer 
 * user interface. Note that this could also have been implemented using 
 * the DXLink interface for stand-alone applications. 
 */

#define Screen dxScreen
#define String dxString
#define Object dxObject
#include <dx/dx.h>
#undef String
#undef Screen
#undef Object 
#include <Xm/Xm.h> 
#include <Xm/Form.h> 
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/DrawingA.h>

/* global variables */
static char *datafilename="cloudwater";
static int changedfilename=1;
static int changedsize=0;
static int changedrenderingmode=0;
static int window_w=400, window_h=400;
static Window window;
static int hardware=0;




void drawing_resizeCB(Widget, XtPointer, XtPointer);
void renderingmodeCB(Widget, XtPointer, XtPointer);
void radio_cloudCB(Widget, XtPointer, XtPointer);
void radio_rainCB(Widget, XtPointer, XtPointer);
void radio_tempCB(Widget, XtPointer, XtPointer);
void MyMacro(char *, int, int, dxObject);
static void NewSize(int, int);
static void NewFileName(char *);
static Boolean DoIt(XtPointer);

#define WINDOWNAME "Window"

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



XtAppContext    app;



/* this routine is checked by the X event loop. It
 * first calls DXCheckRIH to see if any registered
 * input handlers need to be called (for example in
 * the case of mouse events or keyboard events that
 * the Supervise modules have registered an interest
 * in, and then calls DoIt
 */
Boolean
XCheckRIH(void *v)
{
    DXCheckRIH(0);
    DoIt(NULL);
    return False;
}

main(argc, argv)
int argc;
char *argv[];
{
   Widget          toplevel, main_w, label, drawingarea, radio_box;
   Widget          radio_cloud, radio_rain, radio_temp, rendering_mode;
   XmString        xms;
   int             n;
   Arg             wargs[50];
   char            msg[1024];


   DXInitModules();


   toplevel = XtVaAppInitialize (&app, "Demos", 
                                 NULL, 0, &argc, argv, 
                                 DefaultResources, NULL);



   /* 
    * Create the user interface for this application
    */
   main_w = XtVaCreateManagedWidget("form",
                 xmFormWidgetClass, toplevel,
                 XmNwidth,    420,
                 XmNheight,   500,
                 XmNfractionBase, 5,
                 NULL);



   /* create the radio buttons */
   n = 0; 
   XtSetArg(wargs[n], XmNtopAttachment, XmATTACH_FORM); n++;
   XtSetArg(wargs[n], XmNleftAttachment, XmATTACH_FORM); n++;
   XtSetArg(wargs[n], XmNtopOffset, 3); n++;
   XtSetArg(wargs[n], XmNleftOffset, 30); n++;
   radio_box = (Widget)XmCreateRadioBox(main_w, "choice", wargs, n);
   XtManageChild(radio_box);

   xms = XmStringCreateSimple("cloudwater");
   radio_cloud = XtVaCreateManagedWidget("radio_cloud",
                      xmToggleButtonWidgetClass, radio_box,
                      XmNlabelString, xms,
                      XmNset, True,
                      NULL);
   XmStringFree(xms);

   xms = XmStringCreateSimple("rainwater");
   radio_rain = XtVaCreateManagedWidget("radio_rain",
                      xmToggleButtonWidgetClass, radio_box,
                      XmNlabelString, xms,
                      NULL);
   XmStringFree(xms);
   xms = XmStringCreateSimple("temperature");
   radio_temp = XtVaCreateManagedWidget("radio_temp",
                      xmToggleButtonWidgetClass, radio_box,
                      XmNlabelString, xms,
                      NULL);
   XmStringFree(xms);
   

   /* create the toggle button to set rendering to hw or sw */
   xms = XmStringCreateSimple("Use Hardware Rendering");
   rendering_mode = XtVaCreateManagedWidget("rendering_mode",
                      xmToggleButtonWidgetClass, main_w,
                      XmNlabelString, xms,
                      XmNset, False,
                      XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                      XmNtopWidget, radio_cloud,
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, radio_box,
                      XmNleftOffset, 10,
                      XmNrightAttachment, XmATTACH_FORM,
                      XmNalignment, XmALIGNMENT_BEGINNING,
                      NULL);
   XmStringFree(xms);  


   /* create the label and drawing area into which the image will be placed */
   xms = XmStringCreateSimple("Image");
   label = XtVaCreateManagedWidget("label",
                 xmLabelWidgetClass,
                 main_w,
                 XmNtopAttachment,   XmATTACH_WIDGET,
                 XmNtopWidget,       radio_box,
                 XmNleftAttachment,  XmATTACH_FORM,
                 XmNleftOffset,  30,
                 XmNlabelString, xms,
                 NULL);
   XmStringFree(xms);  


   drawingarea = XtVaCreateManagedWidget("drawingarea",
                 xmDrawingAreaWidgetClass,
                 main_w,
                 XmNtopAttachment,   XmATTACH_WIDGET,
                 XmNtopWidget,       label,
                 XmNleftAttachment,  XmATTACH_FORM,
                 XmNleftOffset,  30,
                 XmNrightAttachment,  XmATTACH_FORM,
                 XmNrightOffset,  30,
                 XmNbottomAttachment,  XmATTACH_FORM,
                 XmNbottomOffset,  30,
                 NULL); 


   XtRealizeWidget (toplevel);


   /* get the window id of the drawing area */
   /* we'll need this to pass to SuperviseWindow */
   window = XtWindow(drawingarea); 



   /* these callbacks are for the three buttons in the radio box */
   /* the callbacks modify the imported data file used */
   XtAddCallback(radio_cloud, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_cloudCB,
                 (XtPointer)NULL);
   XtAddCallback(radio_rain, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_rainCB,
                 (XtPointer)NULL);
   XtAddCallback(radio_temp, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_tempCB,
                 (XtPointer)NULL);

   /* add a resize callback for the drawing area */
   XtAddCallback(drawingarea, XmNresizeCallback,
                 (XtCallbackProc)drawing_resizeCB,
                 (XtPointer)NULL);

   /* add a callback for the rendering mode */
   XtAddCallback(rendering_mode, XmNvalueChangedCallback,
                 (XtCallbackProc)renderingmodeCB,
                 (XtPointer)NULL);


   /* DoIt checks for input (mouse events) in the window. This allows
    * rotation interaction
    */
   XtAppAddWorkProc(app, (XtWorkProc)XCheckRIH, NULL);
   XtAppMainLoop (app);

}

/*
 * the following is the resize callback for the drawing area. If the
 * drawing area is resized, we need to send the new size to SuperviseWindow
 */
void drawing_resizeCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   char            msg[1024];
   Dimension       width, height;
   int             wid, hei;

   XtVaGetValues(w, 
                 XtNwidth, &width, 
                 XtNheight, &height,
                 NULL);
   window_w = (int)width;
   window_h = (int)height;
   NewSize(window_w, window_h);
}

/* 
 * The following are the callbacks for the buttons in the
 * user interface created above. In each case, the file
 * name of the appropriate data file is sent to Import 
 */
void radio_cloudCB(Widget w, XtPointer xp1, XtPointer xp2)
{

   /* if the toggle button is pressed in.... */
   if (XmToggleButtonGetState(w)) {
     NewFileName("cloudwater.dx");
   }
}
void radio_rainCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   if (XmToggleButtonGetState(w)) {
     NewFileName("rainwater.dx");
   }
}
void radio_tempCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   if (XmToggleButtonGetState(w)) {
     NewFileName("temperature.dx");
   }
}
void renderingmodeCB(Widget w, XtPointer xp1, XtPointer xp2)
{

   /* if the toggle button is pressed in.... */
   if (XmToggleButtonGetState(w)) 
      hardware = 1;
   else
      hardware = 0;
   changedrenderingmode=1;
}


/* Note that because we are using DXCallModule, instead of DXLink,
 * we do not have the advantages of the Data Explorer executive in
 * its management of execution and caching of intermediate results.
 * In this example, we do some rough optimization in terms of only
 * running Import and Isosurface when necessary, but, for example,
 * when we return to a previously imported data set, Import and
 * Isosurface will be rerun. We could of course have built in 
 * more intelligence here; the point is just that you, as the
 * developer, need to build in these smarts; the executive isn't
 * there to do it for you.
 * This routine is set as a WorkProc for the X Event loop; thus
 * it will get called regularly to check for mouse events in the 
 * window. If nothing significant has happened, it will simply return.
 */
static Boolean DoIt(XtPointer clientdata)
{
   ModuleInput  min[10];
   ModuleOutput mout[10];
   dxObject imported, supwhere, supsize, supevents, optobj;
   dxObject stateobj, statecam, statewhere,camera;
   Array windowsize;
   int *windowsize_ptr;
   static dxObject isosurface = NULL;


   /* call SuperviseWindow and check for events */
   windowsize = DXNewArray(TYPE_INT,CATEGORY_REAL,1, 2);
   DXAddArrayData(windowsize,0,1,NULL);
   windowsize_ptr = DXGetArrayData(windowsize);
   windowsize_ptr[0]=window_w;
   windowsize_ptr[1]=window_h;
   DXModSetObjectInput(&min[0],"size", (dxObject)windowsize);
   DXModSetIntegerInput(&min[1],"parent", window);
   DXModSetIntegerInput(&min[2],"sizeFlag",1);
   DXModSetStringInput(&min[3],"name",WINDOWNAME);
   DXModSetObjectOutput(&mout[0],"where", &supwhere);
   DXModSetObjectOutput(&mout[1],"size", &supsize);
   DXModSetObjectOutput(&mout[2],"events", &supevents);
   DXCallModule("SuperviseWindow",4,min,3,mout);


   /* if there's nothing to do (no events, and nothing has changed),
    * return.
    */
   if ((!supevents)&&
       (!changedfilename)&&(!changedsize)&&(!changedrenderingmode)) {
      DXDelete(supwhere);
      DXDelete(supsize);
      DXDelete(supevents);
      return False;
   }

   /* if the filename has changed, we need to import, run isosurface,
    * and call the Supervise modules */
   if (changedfilename) {

     changedfilename = 0;

     DXModSetStringInput(&min[0],"name",datafilename);
     DXModSetObjectOutput(&mout[0],"data",&imported);
     DXCallModule("Import",1,min,1,mout);

     DXModSetObjectInput(&min[0],"data",imported);
     if(isosurface)
         DXDelete(isosurface);
     DXModSetObjectOutput(&mout[0],"surface",&isosurface);
     DXCallModule("Isosurface",1,min,1,mout); 

     /* we need to reference isosurface, because we'll need it again
      * later (for SuperviseState), and otherwise it would be deleted
      * once CallModule for AutoCamera has run 
      */ 
     DXReference((dxObject)isosurface);
     DXModSetObjectInput(&min[0], "object", isosurface);
     DXModSetObjectOutput(&mout[0], "camera", &camera);
     DXCallModule("AutoCamera",1,min,1,mout);


     /* call SuperviseState, with resetCamera=1, since we want to
      * reset the camera for the new object 
      */
     DXModSetObjectInput(&min[0],"where",supwhere);
     DXModSetObjectInput(&min[1],"defaultCamera", camera);
     DXModSetIntegerInput(&min[2],"resetCamera", 1);
     DXModSetObjectInput(&min[3],"object",isosurface);
     DXModSetIntegerInput(&min[4],"resetObject",1);
     DXModSetObjectInput(&min[5],"size",supsize);
     DXModSetObjectInput(&min[6],"events",supevents);
     DXModSetIntegerInput(&min[7],"mode",0);
     DXModSetObjectOutput(&mout[0],"object",&stateobj);
     DXModSetObjectOutput(&mout[1],"camera",&statecam);
     DXModSetObjectOutput(&mout[2],"where",&statewhere);
     DXCallModule("SuperviseState",8,min,3,mout);

     if (hardware) {
        DXModSetObjectInput(&min[0],"input",stateobj);
        DXModSetStringInput(&min[1],"attribute","rendering mode");
        DXModSetStringInput(&min[2],"value","hardware");
        DXModSetObjectOutput(&mout[0],"output",&optobj);
        DXCallModule("Options",3,min,1,mout);

        /* set the input object for Display */
        DXModSetObjectInput(&min[0],"object",optobj);
     }
     else {
        /* set the input object for Display */
        DXModSetObjectInput(&min[0],"object",stateobj);
     }
     /* call Display */
     DXModSetObjectInput(&min[1],"camera",statecam);
     DXModSetObjectInput(&min[2],"where",statewhere);
     DXCallModule("Display",3,min,0,mout);
   }

   /* If we've changed the window size, or an event has occurred,
    * or the rendering mode has changed between hw and sw,
    * we don't need to do the Import and Isosurface parts, since
    * the data file has not changed
    */
   else if ((changedsize)||(supevents)||(changedrenderingmode)) {
     /* if the size of the window has changed, we do need to rerun
      * SuperviseWindow with the new size
      */
     if (changedsize) {

       changedsize = 0;

       windowsize = DXNewArray(TYPE_INT,CATEGORY_REAL,1, 2);
       DXAddArrayData(windowsize,0,1,NULL);
       windowsize_ptr = DXGetArrayData(windowsize);
       windowsize_ptr[0]=window_w;
       windowsize_ptr[1]=window_h;
       DXModSetObjectInput(&min[0],"size", (dxObject)windowsize);
       DXModSetIntegerInput(&min[1],"parent", window);
       DXModSetIntegerInput(&min[2],"sizeFlag",1);
       DXModSetStringInput(&min[3],"name",WINDOWNAME);
       /* delete previous outputs of SuperviseWindow before reassigning them */
       DXDelete(supwhere);
       DXModSetObjectOutput(&mout[0],"where", &supwhere);
       DXDelete(supsize);
       DXModSetObjectOutput(&mout[1],"size", &supsize);
       DXDelete(supevents);
       DXModSetObjectOutput(&mout[2],"events", &supevents);
       DXCallModule("SuperviseWindow",4,min,3,mout);
     }
     if (changedrenderingmode = 1) changedrenderingmode=0;

     /* now call SuperviseState and Display */
     DXModSetObjectInput(&min[0],"where",supwhere);
     DXModSetObjectInput(&min[1],"object",isosurface);
     DXModSetIntegerInput(&min[2],"resetObject",0);
     DXModSetObjectInput(&min[3],"size",supsize);
     DXModSetObjectInput(&min[4],"events",supevents);
     DXModSetIntegerInput(&min[5],"mode",0);
     DXModSetObjectOutput(&mout[0],"object",&stateobj);
     DXModSetObjectOutput(&mout[1],"camera",&statecam);
     DXModSetObjectOutput(&mout[2],"where",&statewhere);
     DXCallModule("SuperviseState",6,min,3,mout);

     if (hardware) {
        DXModSetObjectInput(&min[0],"input",stateobj);
        DXModSetStringInput(&min[1],"attribute","rendering mode");
        DXModSetStringInput(&min[2],"value","hardware");
        DXModSetObjectOutput(&mout[0],"output",&optobj);
        DXCallModule("Options",3,min,1,mout);

        /* set the input object for Display */
        DXModSetObjectInput(&min[0],"object",optobj);
     }
     else {
        /* set the input object for Display */
        DXModSetObjectInput(&min[0],"object",stateobj);
     }

     /* call Display */
     DXModSetObjectInput(&min[1],"camera",statecam);
     DXModSetObjectInput(&min[2],"where",statewhere);
     DXCallModule("Display",3,min,0,mout);


   }
   return False; 
}

/* the size of the window has changed; set some global variables */
static void NewSize(int w, int h)
{
   window_w = w;
   window_h = h;
   changedsize = 1;
   DoIt(NULL);
}

/* the filename has changed; set some global variables */
static void NewFileName(char *filename)
{
   datafilename = filename;
   changedfilename = 1;
   DoIt(NULL);
}

