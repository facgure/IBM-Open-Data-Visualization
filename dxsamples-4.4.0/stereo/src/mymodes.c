/***********************************************************************/
/* Open Visualization Data Explorer                                    */
/* (C) Copyright IBM Corp. 1989,1999                                   */
/* ALL RIGHTS RESERVED                                                 */
/* This code licensed under the                                        */
/*    "IBM PUBLIC LICENSE - Open Visualization Data Explorer"          */
/***********************************************************************/

#include <dxconfig.h>

#define String dxString
#define Object dxObject
#define Angle dxAngle
#define Matrix dxMatrix
#define Screen dxScreen
#define Boolean dxBoolean

#include <dx/dx.h>

#undef String
#undef Object
#undef Angle
#undef Matrix
#undef Screen
#undef Boolean

#include <X11/Xlib.h>

#include <math.h>
#include <stdlib.h>     /* for getenv prototype */

#include <dxstereo.h>

/************** Custom Stereo System Modes **********************
 * One custom mode is demonstrated.  It uses two viewports placed
 * diagonally in the original window.
 ****************************************************************/

static int  demoInitializeStereoSystemMode(Display *, Window);
static int  demoExitStereoSystem(Display *, Window, Window, Window);
static int  demoCreateStereoWindows(Display *, Window,
					Window *, WindowInfo *,
					Window *, WindowInfo *);

static StereoSystemMode _defaultStereoSystemModes[1];

int 
DXEntry(int *n, StereoSystemMode **ssms)
{
    *ssms = _defaultStereoSystemModes;

    (*ssms)[0].initializeStereoSystemMode = demoInitializeStereoSystemMode;
    (*ssms)[0].exitStereo 		  = demoExitStereoSystem;
    (*ssms)[0].createStereoWindows 	  = demoCreateStereoWindows;

    *n = 1;
    return OK;
}

static int
demoInitializeStereoSystemMode(Display *dpy, Window w)
{
    return OK;
}

static int
demoExitStereoSystem(Display *dpy, Window frame, Window left, Window right)
{
    /*
     * neither of these should *ever* succeed ... we are using two
     * viewports in this demo, not child windows
     */
    if (left != frame)
	XDestroyWindow(dpy, left);
    if (right != frame)
	XDestroyWindow(dpy, right);
    return OK;
}

Error
demoCreateStereoWindows(Display *dpy, Window frame,
		Window *left, WindowInfo *leftWI,
		Window *right, WindowInfo *rightWI)
{
    XWindowAttributes xwattr;
    XGetWindowAttributes(dpy, frame, &xwattr);

    *left = frame;
    leftWI->xOffset = 0;
    leftWI->yOffset = 0;
    leftWI->xSize = xwattr.width/2;
    leftWI->ySize = xwattr.height/2;

    leftWI->aspect = xwattr.height / xwattr.width;

    *right = frame;
    rightWI->xOffset = xwattr.width/2;
    rightWI->yOffset = xwattr.height/2;
    rightWI->xSize = xwattr.width/2;
    rightWI->ySize = xwattr.height/2;

    rightWI->aspect = xwattr.height / xwattr.width;

    return OK;
}

