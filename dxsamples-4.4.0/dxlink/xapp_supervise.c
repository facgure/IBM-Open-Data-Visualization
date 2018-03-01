/* this example uses dxlink to display an interactive image 
 * of an isosurface of a data field.
 * The "user interface" is completely independent of the Data Explorer 
 * user interface. Note that this could also have been implemented using 
 * the DXCallModule interface for stand-alone applications. The disadvantage
 * would have been that executive features such as caching of outputs 
 * would not have been available in that case 
 */


#include <Xm/Xm.h> 
#include <Xm/Form.h> 
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/DrawingA.h>
#include "dxl.h"


void radio_cloudCB(Widget, XtPointer, XtPointer);
void drawing_resizeCB(Widget, XtPointer, XtPointer);
void radio_rainCB(Widget, XtPointer, XtPointer);
void radio_tempCB(Widget, XtPointer, XtPointer);
void renderingmodeCB(Widget, XtPointer, XtPointer);

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



main(argc, argv)
int argc;
char *argv[];
{
   Widget          toplevel, main_w, label, drawingarea, radio_box;
   Widget          radio_cloud, radio_rain, radio_temp, rendering_mode;
   XtAppContext    app;
   XmString        xms;
   DXLConnection   *conn;
   int             n;
   Arg             wargs[50];
   Window          window;
   char            msg[1024];


   /* 
    *  Start the Data Explorer executive.
    */
   conn = DXLStartDX("dx -exonly",NULL);
   if (!conn)
    {
      printf("could not start dx");
      exit(0);
    }


   XtSetLanguageProc (NULL, NULL, NULL);

   toplevel = XtVaAppInitialize (&app, "Demos", 
                                 NULL, 0, &argc, argv, 
                                 DefaultResources, NULL);

   DXLInitializeXMainLoop(app, conn);


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
   XtSetArg(wargs[n], XmNleftOffset, 30); n++;
   radio_box = (Widget)XmCreateRadioBox(main_w, "choice", wargs, n);
   XtManageChild(radio_box);

   xms = XmStringCreateSimple("cloudwater");
   radio_cloud = XtVaCreateManagedWidget("radio_cloud",
                      xmToggleButtonWidgetClass, radio_box,
                      XmNlabelString, xms,
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
                 (XtPointer)conn);
   XtAddCallback(radio_rain, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_rainCB,
                 (XtPointer)conn);
   XtAddCallback(radio_temp, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_tempCB,
                 (XtPointer)conn);

 



   /* define the macro which executes the function of importing some
    * data, creating an isosurface, defining a camera, and using
    * SuperviseWindow and SuperviseState to create and manage an
    * interactive image window. Note that we pass the windowid of
    * the DrawingArea widget to SuperviseWindow.
    * The variables "filename", "resetcamera", and "resetobject" will
    * be set within the callbacks. The variable "size" will be
    * set using DXLInputNamed. This allows a default other than 
    * the default of 640x480. DXLInputNamed could also have been
    * used for other changable parameters, but instead we chose
    * to set them directly using the variable names renderingmode
    * and filename.
    *
    * The "mode" parameter to SuperviseState is set to 0, for the
    * built-in "rotate" interaction mode.
    */
   exDXLBeginMacroDefinition(conn, "macro MyMacro()");
   DXLSend(conn, "  data = Import(filename);");
   DXLSend(conn, "  iso  = Isosurface(data); ");
   DXLSend(conn, "  defCam = AutoCamera(iso); ");
   sprintf(msg,"sz = DXLInputNamed(\"windowsize\", [400, 400]);");
   DXLSend(conn,msg); 
   sprintf(msg,"where,size,events=SuperviseWindow(size=sz,parent=%d,sizeFlag=1);",
           window);
   DXLSend(conn, msg);
   DXLSend(conn, "mode = 0;");
   sprintf(msg,"object, camera, where2, events2 = SuperviseState(where, defCam, resetcamera, iso, resetobject, size, events, mode);");
   DXLSend(conn, msg);
   DXLSend(conn,"o = Options(object,\"rendering mode\", renderingmode);");
   DXLSend(conn, "Display(object, camera, where);");
   exDXLEndMacroDefinition(conn);


   /* resize callback */
   XtAddCallback(drawingarea, XmNresizeCallback,
                 (XtCallbackProc)drawing_resizeCB,
                 (XtPointer)conn);

   /* add a callback for the rendering mode */
   XtAddCallback(rendering_mode, XmNvalueChangedCallback,
                 (XtCallbackProc)renderingmodeCB,
                 (XtPointer)NULL);




   XtAppMainLoop (app);

}

/*
 * the following is the resize callback for the drawing area. If the
 * drawing area is resized, we need to send the new size to SuperviseWindow
 */
void drawing_resizeCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   char            msg[1024];
   DXLConnection   *conn = (DXLConnection *)xp1;
   Dimension       width, height;
   int             wid, hei;

   XtVaGetValues(w, 
                 XtNwidth, &width, 
                 XtNheight, &height,
                 NULL);
   wid = (int)width;
   hei = (int)height;
   sprintf(msg,"[%d, %d]", wid, hei);
   DXLSetValue(conn, "windowsize",msg);
   exDXLExecuteOnChangeNamed(conn,"MyMacro"); 


}

/* 
 * The following are the callbacks for the buttons in the
 * user interface created above. In each case, the file
 * name of the appropriate data file is sent to the
 * Data Explorer executive. 
 */
void radio_cloudCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   DXLConnection *conn = (DXLConnection *)xp1;

   /* if the toggle button is pressed in.... */
   if (XmToggleButtonGetState(w)) {

     /* turn off Execute On Change mode so that some variables can be set 
      * without causing an execution 
      */
     DXLEndExecuteOnChange(conn);

     /* send down the new filename. Also reset the camera, and reset
      * the object to SuperviseState
      */
     DXLSend(conn,"filename = \"cloudwater.dx\";");
     DXLSend(conn,"resetcamera = 1;");
     DXLSend(conn,"resetobject = 1;");

     /* execute once with the reset camera and object*/
     exDXLExecuteOnceNamed(conn,"MyMacro"); 

     /* now set resetcamera to 0 so that rotations can be performed */
     /* set resetObject to 1 so that the object from the cache is used */
     DXLSend(conn,"resetcamera = 0;");
     DXLSend(conn,"resetobject = 0;");

     /* go into execute on change mode */
     exDXLExecuteOnChangeNamed(conn,"MyMacro"); 
   }
}
void radio_rainCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   /* see comments in radio_cloudCB */
   DXLConnection *conn = (DXLConnection *)xp1;
   if (XmToggleButtonGetState(w)) {
     DXLEndExecuteOnChange(conn);
     DXLSend(conn,"filename = \"rainwater.dx\";");
     DXLSend(conn,"resetcamera = 1;");
     DXLSend(conn,"resetobject = 1;");
     exDXLExecuteOnceNamed(conn,"MyMacro"); 
     DXLSend(conn,"resetcamera = 0;");
     DXLSend(conn,"resetobject = 0;");
     exDXLExecuteOnChangeNamed(conn,"MyMacro"); 
   }
}
void radio_tempCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   /* see comments in radio_cloudCB */
   DXLConnection *conn = (DXLConnection *)xp1;
   if (XmToggleButtonGetState(w)) {
     DXLEndExecuteOnChange(conn);
     DXLSend(conn,"filename = \"temperature.dx\";");
     DXLSend(conn,"resetcamera = 1;");
     DXLSend(conn,"resetobject = 1;");
     exDXLExecuteOnceNamed(conn,"MyMacro"); 
     DXLSend(conn,"resetcamera = 0;");
     DXLSend(conn,"resetobject = 0;");
     exDXLExecuteOnChangeNamed(conn,"MyMacro"); 
   }
}

void renderingmodeCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   DXLConnection *conn = (DXLConnection *)xp1;
   /* if the toggle button is pressed in.... */
   if (XmToggleButtonGetState(w)) {
      DXLSend(conn, "renderingmode = hardware;");
      exDXLExecuteOnChangeNamed(conn,"MyMacro"); 
   }
   else {
      DXLSend(conn, "renderingmode = software;");
      exDXLExecuteOnChangeNamed(conn,"MyMacro"); 
   }
}


