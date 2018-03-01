#include <dx/dx.h>
#include <stdio.h>
#include <math.h>

/* This is a very simple user interactor which pays attention only
 * to left mouse button events. If the user presses and drags the
 * left mouse button in a horizontal direction, the camera zooms in
 * on the object being viewed. If the user presses and drags the
 * left mouse button in a vertical direction, the camera zooms out.
 * Since this interactor modifies only the camera (zooming in or out)
 * and does not modify the object, the routines GetRenderable and
 * SetRenderable are set to NULL. See the demonstation in ../complexdemo
 * for a demonstation that also modifies the object being viewed.
 */

/* This demonstration only works in orthographic projection */

#define N_USER_INTERACTORS 1

/* 
 * Here are declarations for the routines implementing the
 * interaction mode
 */
static void *ZoomInitMode(Object, int, int, int *);
static void ZoomEndMode(void *);
static void ZoomSetCamera(void *, float *, float *,
		float *, int, float, float);
static int ZoomGetCamera(void *, float *, float *, float *, 
              int *, float *, float *);
static void ZoomEventHandler(void *data, DXEvent *event);

/*
 * The table containing the interactor
 */
static UserInteractor interactorTable[N_USER_INTERACTORS];

/*
 * This is the procedure that DX calls to load the interactor.
 * The procedure returns the number of interactors and a pointer
 * to the table.
 */

int
DXEntry(int *n, void *t)
{
    interactorTable[0].InitMode =      ZoomInitMode;
    interactorTable[0].EndMode =       ZoomEndMode; 
    interactorTable[0].SetCamera =     ZoomSetCamera; 
    interactorTable[0].GetCamera =     ZoomGetCamera; 
    interactorTable[0].SetRenderable = NULL; 
    interactorTable[0].GetRenderable = NULL; 
    interactorTable[0].EventHandler =  ZoomEventHandler; 

    *n = N_USER_INTERACTORS;
    *(long **)t = (long *)interactorTable;
 
    return 1;
}

typedef struct zoomData
{
    int w, h;
    float to[3];
    float from[3];
    float up[3];
    int projection;
    float fov;
    float width;
    int buttonUp;
    int x, y;
} *ZoomData;

static void *
ZoomInitMode(Object args, int w, int h, int *mask)
{
    ZoomData zdata = (ZoomData)DXAllocateZero(sizeof(struct zoomData));
    if (! zdata)
	return NULL;

    zdata->w = w;
    zdata->h = h;

    zdata->buttonUp = 1;

    /* left mouse button will zoom the camera */ 
    *mask = DXEVENT_LEFT;
	
    return (void *)zdata;
}


/* store away all the information about the camera, so that
 * ZoomEndStroke can modify it.
 */
static void 
ZoomSetCamera(void *data, float to[3], float from[3], float up[3],
		    int proj, float fov, float width)
{
    ZoomData zdata = (ZoomData)data;
    /* store away the camera info */
    zdata->to[0] = to[0];
    zdata->to[1] = to[1];
    zdata->to[2] = to[2];
    zdata->from[0] = from[0];
    zdata->from[1] = from[1];
    zdata->from[2] = from[2];
    zdata->up[0] = up[0];
    zdata->up[1] = up[1];
    zdata->up[2] = up[2];
    zdata->projection = proj;
    zdata->fov = fov;
    zdata->width = width;
}


/* 
 * return the modified camera.
 */
static int
ZoomGetCamera(void *data, float *to, float *from, float *up,
              int *projection, float *fov, float *width)
{
    ZoomData zdata = (ZoomData)data;
    to[0] = zdata->to[0];
    to[1] = zdata->to[1];
    to[2] = zdata->to[2];
    from[0] = zdata->from[0];
    from[1] = zdata->from[1];
    from[2] = zdata->from[2];
    up[0] = zdata->up[0];
    up[1] = zdata->up[1];
    up[2] = zdata->up[2];
    *projection = zdata->projection;
    *fov = zdata->fov;
    *width = zdata->width;
    return 1;
}

static void
ZoomEndMode(void *data)
{
    DXFree(data);
}

static void ZoomStartStroke(void *, DXMouseEvent *event);
static void ZoomEndStroke(void *, DXMouseEvent *event);

static void
ZoomEventHandler(void *data, DXEvent *event)
{
    ZoomData zdata = (ZoomData)data;

    switch (event->any.event)
    {
	case DXEVENT_LEFT:
	    switch(event->mouse.state)
	    {
		case BUTTON_DOWN:
		    ZoomStartStroke(data, (DXMouseEvent *)event);
		    break;
		
		case BUTTON_MOTION:
		    if (zdata->buttonUp)
			ZoomStartStroke(data, (DXMouseEvent *)event);
		    else
			ZoomEndStroke(data, (DXMouseEvent *)event);
		    break;
		
		case BUTTON_UP:
		    ZoomEndStroke(data, (DXMouseEvent *)event);
		    break;
	    }
	    break;
	
	default:
	    break;
    }
}
    

static void 
ZoomStartStroke(void *data, DXMouseEvent *event)
{
    ZoomData zdata = (ZoomData)data;
    zdata->buttonUp = 0;
    zdata->x = event->x;
    zdata->y = event->y;
    return;
}

/*
 * modify the camera based on how far the mouse has moved, and
 * in which direction.
 */
static void
ZoomEndStroke(void *data, DXMouseEvent *event)
{
    int dx, dy;
    float t;
    ZoomData zdata = (ZoomData)data;

    zdata->buttonUp = 1;

    dx = zdata->x - event->x;
    dy = zdata->y - event->y;

    zdata->x = event->x;
    zdata->y = event->y;

    /* only the absolute value of the distance moved matters */
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    /* 
     * to zoom in or zoom out, modify the "width" of the camera
     */
    if (dx > dy) {    /* zoom in */
          t = 1 + 10*((float)dx/zdata->w);
          zdata->width = zdata->width/t;
    }
    else {    /* zoom out */
          zdata->width = zdata->width*(1+10*((float)dy/zdata->h));
    }
    return;
}

