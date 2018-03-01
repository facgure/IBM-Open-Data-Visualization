/* this example uses dxlink to find the maximum value of a data field */
/* the "user interface" is completely independent of the Data Explorer */
/* user interface. Note that this could also have been implemented using */
/* the DXCallModule interface for stand-alone applications. The disadvantage*/
/* would have been that executive features such as caching of outputs */
/* would not have been available in that case */


#include <Xm/Xm.h> 
#include <Xm/Form.h> 
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/Text.h>
#include "dxl.h"


void radio_cloudCB(Widget, XtPointer, XtPointer);
void radio_rainCB(Widget, XtPointer, XtPointer);
void radio_windCB(Widget, XtPointer, XtPointer);
void radio_tempCB(Widget, XtPointer, XtPointer);
void cloudhandler(DXLConnection *, const char *, const char *, void *);
void rainhandler(DXLConnection *, const char *, const char *, void *);
void windhandler(DXLConnection *, const char *, const char *, void *);
void temphandler(DXLConnection *, const char *, const char *, void *);

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
   Widget          toplevel, main_w, label, textfield, radio_box;
   Widget          radio_cloud, radio_rain, radio_temp, radio_wind;
   XtAppContext    app;
   XmString        xms;
   DXLConnection   *conn;
   int             n;
   Arg             wargs[50];


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
                 XmNwidth,    400,
                 XmNheight,   180,
                 XmNfractionBase, 5,
                 NULL);

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
   XtAddCallback(radio_cloud, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_cloudCB,
                 (XtPointer)conn);

   xms = XmStringCreateSimple("rainwater");
   radio_rain = XtVaCreateManagedWidget("radio_rain",
                      xmToggleButtonWidgetClass, radio_box,
                      XmNlabelString, xms,
                      NULL);
   XmStringFree(xms);
   XtAddCallback(radio_rain, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_rainCB,
                 (XtPointer)conn);
 
   xms = XmStringCreateSimple("temperature");
   radio_temp = XtVaCreateManagedWidget("radio_temp",
                      xmToggleButtonWidgetClass, radio_box,
                      XmNlabelString, xms,
                      NULL);
   XmStringFree(xms);
   XtAddCallback(radio_temp, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_tempCB,
                 (XtPointer)conn);

   xms = XmStringCreateSimple("wind");
   radio_wind = XtVaCreateManagedWidget("radio_wind",
                      xmToggleButtonWidgetClass, radio_box,
                      XmNlabelString, xms,
                      NULL);
   XmStringFree(xms);
   XtAddCallback(radio_wind, XmNvalueChangedCallback,
                 (XtCallbackProc)radio_windCB,
                 (XtPointer)conn);

   


   xms = XmStringCreateSimple("returned value:");
   label = XtVaCreateManagedWidget("label",
                 xmLabelWidgetClass,
                 main_w,
                 XmNtopAttachment,   XmATTACH_WIDGET,
                 XmNtopWidget,       radio_box,
                 XmNbottomAttachment,   XmATTACH_FORM,
                 XmNleftAttachment,  XmATTACH_FORM,
                 XmNlabelString, xms,
                 NULL);
   XmStringFree(xms);  

   textfield = XtVaCreateManagedWidget("text",
                 xmTextWidgetClass,
                 main_w,
                 XmNtopAttachment,   XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget,       label,
                 XmNleftAttachment,  XmATTACH_WIDGET,
                 XmNleftWidget,      label,
                 XmNrightAttachment,  XmATTACH_FORM,
                 XmNbottomAttachment,  XmATTACH_FORM,
                 NULL); 


   /* 
    * Set the handlers for the various parameters
    */ 
   DXLSetValueHandler(conn,"cloudmax", cloudhandler, textfield);
   DXLSetValueHandler(conn,"rainmax", rainhandler, textfield);
   DXLSetValueHandler(conn,"windmax", windhandler, textfield);
   DXLSetValueHandler(conn,"tempmax", temphandler, textfield);


   XtRealizeWidget (toplevel);
   XtAppMainLoop (app);
}


/*
 * The following are the handlers for data coming from DXLOutput. If
 * data is received by the handler, it is presented in a text widget.
 */
void rainhandler(DXLConnection *conn, const char *name, const char *value,
             void *data)
{
   char string[100];
   Widget text_widget = (Widget)data;

   sprintf(string,"rainwater max value = %s", value);
   XmTextSetString(text_widget, string);
}
void cloudhandler(DXLConnection *conn, const char *name, const char *value,
             void *data)
{
   char string[100];
   Widget text_widget = (Widget)data;

   sprintf(string,"cloudwater max value = %s", value);
   XmTextSetString(text_widget, string);
}
void temphandler(DXLConnection *conn, const char *name, const char *value,
             void *data)
{
   char string[100];
   Widget text_widget = (Widget)data;

   sprintf(string,"temperature max value = %s", value);
   XmTextSetString(text_widget, string);
}
void windhandler(DXLConnection *conn, const char *name, const char *value,
             void *data)
{
   char string[100];
   Widget text_widget = (Widget)data;

   sprintf(string,"wind max value = %s", value);
   XmTextSetString(text_widget, string);
}


/* 
 * The following are the callbacks for the buttons in the
 * user interface created above. In each case, some simple
 * script language commands are sent to the Data Explorer
 * executive. The maximum as computed by the Statistics
 * module is then input to the DXLOutput tool. The handlers
 * defined above wait for values to be received from 
 * DXLOutput, and then present the result in the text widget.
 */
void radio_cloudCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   DXLConnection *conn = (DXLConnection *)xp1;
   DXLSend(conn, "g = Import(\"cloudwater\");");
   DXLSend(conn, "mean,sd,var,min,max = Statistics(g);");
   DXLSend(conn, "DXLOutput(\"cloudmax\", max);");
}
void radio_rainCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   DXLConnection *conn = (DXLConnection *)xp1;
   DXLSend(conn, "g = Import(\"rainwater\");");
   DXLSend(conn, "mean,sd,var,min,max = Statistics(g);");
   DXLSend(conn, "DXLOutput(\"rainmax\", max);");
}
void radio_tempCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   DXLConnection *conn = (DXLConnection *)xp1;
   DXLSend(conn, "g = Import(\"temperature\");");
   DXLSend(conn, "mean,sd,var,min,max = Statistics(g);");
   DXLSend(conn, "DXLOutput(\"tempmax\", max);");
}
void radio_windCB(Widget w, XtPointer xp1, XtPointer xp2)
{
   DXLConnection *conn = (DXLConnection *)xp1;
   DXLSend(conn, "g = Import(\"wind\");");
   DXLSend(conn, "mean,sd,var,min,max = Statistics(g);");
   DXLSend(conn, "DXLOutput(\"windmax\", max);");
}
