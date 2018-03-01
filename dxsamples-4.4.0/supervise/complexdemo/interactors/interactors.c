/*
 * This file contains two user-defined interactors used
 * in the 'peru' sample.  NOTE: These are only appropriate
 * for orthographic views.
 */

#include <dx/dx.h>
#include <stdio.h>
#include <math.h>

#define N_USER_INTERACTORS  2

/*
 * Here are declarations for the routines implementing the
 * two interaction modes.
 */
static void *Slide2DInitMode(Object, int, int, int *);
static void Slide2DEndMode(void *);
static void Slide2DSetCamera(void *, float *, float *,
		float *, int, float, float);
static int Slide2DGetCamera(void *, float *, float *,
		float *, int *, float *, float *);
static void Slide2DSetRenderable(void *, Object);
static int  Slide2DGetRenderable(void *, Object *);
static void  Slide2DEventHandler(void *, DXEvent *);

static void *CaptionInitMode(Object, int, int, int *);
static void CaptionEndMode(void *);
static void CaptionSetRenderable(void *, Object);
static int  CaptionGetRenderable(void *, Object *);
static void  CaptionEventHandler(void *, DXEvent *);


/*
 * The table containing the two interactors
 */
static UserInteractor interactorTable[N_USER_INTERACTORS];

/*
 * This is the procedure that DX calls to load the interactors.
 * The procedure returns the number of interactors and a pointer
 * to the table.
 */
int
DXEntry(int *n, void *t)
{
    interactorTable[0].InitMode =      Slide2DInitMode;
    interactorTable[0].EndMode =       Slide2DEndMode;
    interactorTable[0].SetCamera =     Slide2DSetCamera;
    interactorTable[0].GetCamera =     Slide2DGetCamera; 
    interactorTable[0].SetRenderable = Slide2DSetRenderable;
    interactorTable[0].GetRenderable = Slide2DGetRenderable;
    interactorTable[0].EventHandler =  Slide2DEventHandler;

    interactorTable[1].InitMode =      CaptionInitMode;
    interactorTable[1].EndMode =       CaptionEndMode;
    interactorTable[1].SetCamera =     NULL;
    interactorTable[1].GetCamera =     NULL;
    interactorTable[1].SetRenderable = CaptionSetRenderable;
    interactorTable[1].GetRenderable = CaptionGetRenderable;
    interactorTable[1].EventHandler =  CaptionEventHandler;

    *n = N_USER_INTERACTORS;
    *(long **)t = (long *)interactorTable;

    return 1;
}
     
/***** SLIDE2D *****/

/* 
 * This is the Slide2D interactor.  It operates on objects
 * of type 'transformed object', which in fact is any object
 * collected into the scene that has a transform node at its root
 * and which has an 'object tag' attribute containing the string
 * 'transformed object'.  The argument is either the index of the
 * member of the scene group on which to operate, or the name
 * of the member.
 *
 * The left button slides the object in 2-D.
 * If a middle-button stroke is more up and down than
 * left to right, it spins the object; otherwise, it
 * zooms the camera.
 */

/*
 * Here is the local data structure for the interactor.
 * We need to retain the camera info, the object info and the
 * argument.  We also set up some scaling factors for use 
 * while performing the transformations/
 */

typedef struct slide2DData
{
    int w, h;
    float gt;
    float gr;
    Object args;
    char *label;
    int index;
    Object obj;
    float to[3], from[3], up[3];
    int p;
    float fov, width, o_width;
    int strokeStart;
    struct {
	int x, y;
    } buttonPosition[3];
} *Slide2DData;

static void *
Slide2DInitMode(Object args, int w, int h, int *mask)
{
    Slide2DData sdata = (Slide2DData)DXAllocateZero(sizeof(struct slide2DData));
    if (! sdata)
	return NULL;

    /* 
     * Parse the argument.  It better be a string or an integer.
     */
    if (! args)
    {
        DXWarning("Slide2D: argument required (index 0 used)");
	sdata->label = NULL;
	sdata->index = 0;
    }
    else if (DXGetObjectClass(args) == CLASS_STRING)
    {
	sdata->args = DXReference(args);
	sdata->label = DXGetString((String)args);
    }
    else if (DXGetObjectClass(args) == CLASS_ARRAY)
    {
	Type t;

	DXGetArrayInfo((Array)args, NULL, &t, NULL, NULL, NULL);
	if (t == TYPE_INT)
	{
	    sdata->args = DXReference(args);
	    sdata->index = *(int *)DXGetArrayData((Array)args);
	    sdata->label = NULL;
	}
	else if (t == TYPE_UBYTE ||
		 t == TYPE_BYTE  ||
		 t == TYPE_STRING)
	{
	    sdata->args = DXReference(args);
	    sdata->label = (char *)DXGetArrayData((Array)args);
	    sdata->index = -1;
	}
	else
	{
	    DXWarning("Slide2D: argument must be string or int (index 0 used)");
	    sdata->label = NULL;
	    sdata->index = 0;
	}

    }

    /*
     * Save the window width and height for scaling purposes
     */
    sdata->w = w;
    sdata->h = h;

    /*
     * Initially, the button is up
     */
    sdata->strokeStart = 1;

    /* 
     * Pay attention to only the left and middle buttons
     */
    *mask = DXEVENT_LEFT | DXEVENT_MIDDLE;
	
    return (void *)sdata;
}

static void 
Slide2DSetCamera(void *data, float to[3], float from[3], float up[3],
		    int proj, float fov, float width)
{
    Slide2DData sdata = (Slide2DData)data;

    /*
     * Set camera: scarf up the camera info.  Compute a
     * scaling factor
     */
    if (proj)
    {
	float dx = to[0] - from[0];
	float dy = to[1] - from[1];
	float dz = to[2] - from[2];
	float vd = sqrt(dx*dx + dy*dy + dz*dz);
	sdata->gt = (fov * vd) / sdata->w;
    }
    else
	sdata->gt = width / sdata->w;

    sdata->to[0] = to[0];
    sdata->to[1] = to[1];
    sdata->to[2] = to[2];

    sdata->from[0] = from[0];
    sdata->from[1] = from[1];
    sdata->from[2] = from[2];

    sdata->up[0] = up[0];
    sdata->up[1] = up[1];
    sdata->to[2] = to[2];

    sdata->p = proj;
    sdata->fov = fov;
    sdata->width = width;

    sdata->gr = 1.0 / sdata->w;
}

static int 
Slide2DGetCamera(void *data, float to[3], float from[3], float up[3],
		    int *proj, float *fov, float *width)
{
    Slide2DData sdata = (Slide2DData)data;

    /* 
     * return the (possibly updated) camera info
     */

    to[0] = sdata->to[0];
    to[1] = sdata->to[1];
    to[2] = sdata->to[2];

    from[0] = sdata->from[0];
    from[1] = sdata->from[1];
    from[2] = sdata->from[2];

    up[0] = sdata->up[0];
    up[1] = sdata->up[1];
    up[2] = sdata->up[2];

    *proj = sdata->p;
    *fov = sdata->fov;
    *width = sdata->width;

    return 1;
}

static void
Slide2DSetRenderable(void *data, Object obj)
{
    Slide2DData sdata = (Slide2DData)data;
    if (sdata->obj)
	DXDelete(sdata->obj);

    /*
     * Scarf up the scene object group
     */
    if (DXGetObjectClass(obj) != CLASS_GROUP)
    {
	DXWarning("Slide2D: Object must be a group of transfomrmed objects");
	sdata->obj = NULL;
    }
    else
	sdata->obj = DXReference(obj);
}
    

static int
Slide2DGetRenderable(void *data, Object *obj)
{
    Slide2DData sdata = (Slide2DData)data;

    /*
     * return the screen object group
     */
    if (sdata->obj)
    {
	*obj = sdata->obj;
	return 1;
    }
    else
    {
	*obj = NULL;
	return 0;
    }
}

static void Slide2DStartStroke(void *, DXMouseEvent *);
static void Slide2DEndStroke(void *, DXMouseEvent *);

static void
Slide2DEventHandler(void *data, DXEvent *event)
{
    Slide2DData sdata = (Slide2DData)data;

    switch (event->any.event)
    {
	case DXEVENT_LEFT:
	case DXEVENT_MIDDLE:
	    switch (event->mouse.state)
	    {
		case BUTTON_DOWN:
		    Slide2DStartStroke(data, (DXMouseEvent *)event);
		    break;

		case BUTTON_MOTION:
		    if (sdata->strokeStart)
			Slide2DStartStroke(data, (DXMouseEvent *)event);
		    else
			Slide2DEndStroke(data, (DXMouseEvent *)event);
		    break;

		case BUTTON_UP:
		    Slide2DStartStroke(data, (DXMouseEvent *)event);
		    break;
		
		default:
		    break;
	    }
	    break;
	
	default:
	    break;
    }
}

static void 
Slide2DStartStroke(void *data, DXMouseEvent *event)
{
    Slide2DData sdata = (Slide2DData)data;
    int b = WHICH_BUTTON(event);

    /*
     * Just keep track of which button is pressed.
     */
    sdata->buttonPosition[b].x = event->x;
    sdata->buttonPosition[b].y = event->y;
    sdata->strokeStart = 0;
    return;
    
}

static void
Slide2DEndStroke(void *data, DXMouseEvent *event)
{
    Slide2DData sdata = (Slide2DData)data;
    int b = WHICH_BUTTON(event);
    int dx, dy;
    Object member;
    char *name;
    Point corners[8];
    int i;

    if (!sdata)
	return;

    sdata = (Slide2DData)data;
    if (! sdata)
	return;
    
    dx = sdata->buttonPosition[b].x - event->x;
    dy = -(sdata->buttonPosition[b].y - event->y);

    sdata->buttonPosition[b].x = event->x;
    sdata->buttonPosition[b].y = event->y;

    sdata->strokeStart = 1;
    
    /*
     * End stroke.  If its the left button or up'n'downish right button,
     * we operate on a member of the scene group.  If its a left'n'right
     * middle button, we operate on the camera
     */
    if (b == 0 || ((b == 1) && (abs(dy) > abs(dx))))
    {
	Group og = (Group)sdata->obj, ng;

	/*
	 * Can't do anything if we don't have a scene object group or an 
	 * arg telling us what member to operate on
	 */
	if ((sdata->label == NULL && sdata->index == -1) || !sdata->obj)
	    return;

	/*
	 * Create a new group header.
	 */
	ng = (Group)DXCopy(sdata->obj, COPY_ATTRIBUTES);
	if (! ng)
	    return;

	/*
	 * Loop through the scene object group.  For each member NOT matching
	 * the argument, just copy it into the new group.  Otherwise, twiddle
	 * its transform matrix and put it there.
	 */
	i = 0;
	while (NULL != (member = DXGetEnumeratedMember((Group)og, i++, &name)))
	{
	    if ((sdata->label && !strcmp(name, sdata->label)) 
		    || ((i-1) == sdata->index))
	    {
		Object attr;

		/* 
		 * This is the one to manipulate.  It better be a 
		 * 'transformed object' type.
		 */

		attr = DXGetAttribute(member, "object type");
		if (! attr)
		    continue;
		
		if (strcmp(DXGetString((String)attr), "transformed object"))
		    continue;

		if (b == 0)
		{
		    Vector x;
		    Xform oldx = (Xform)member;
		    Matrix oldm, newm;
		    Object xchild;

		    /*
		     * Its the left button.  Determine the world space
		     * vector corresponding to the screen-space stroke, and
		     * concatenate it onto the matrix already associated with
		     * the object, and create a new DX Transform object with
		     * the old transforms child and the concatenated matrices.
		     */

		    x.x = -(dx * sdata->gt);
		    x.y = -(dy * sdata->gt);
		    x.z = 0.0;

		    DXGetXformInfo(oldx, &xchild, &oldm);

		    newm = DXTranslate(x);
		    newm = DXConcatenate(oldm, newm);
		    member = (Object)DXNewXform(xchild, newm);
		    DXSetAttribute(member, "object type", attr);
		}
		else if (DXBoundingBox(member, corners))
		{
		    Angle r;
		    Xform oldx = (Xform)member;
		    Matrix oldm, newm;
		    Object xchild;
		    Vector center;

		    /*
		     * Then its an up'n'down middle button stroke. Create
		     * a matrix that transforms the object back to the origin,
		     * rotates it, and then back to where it was.
		     */


		    DXGetXformInfo(oldx, &xchild, &oldm);

		    /*
		     * Move the object so its center is at the origin
		     */
		    center.x = -(corners[0].x + corners[7].x) / 2.0;
		    center.y = -(corners[0].y + corners[7].y) / 2.0;
		    center.z = -(corners[0].z + corners[7].z) / 2.0;
		    newm = DXTranslate(center);
		    oldm = DXConcatenate(oldm, newm);


		    /*
		     * Rotate
		     */
		    r = dy * sdata->gr * (2*3.14159);
		    newm = DXRotateZ(r);
		    oldm = DXConcatenate(oldm, newm);

		    /*
		     * Move the object back to its original location
		     */
		    center.x = -center.x;
		    center.y = -center.y;
		    center.z = -center.z;
		    newm = DXTranslate(center);
		    oldm = DXConcatenate(oldm, newm);

		    member = (Object)DXNewXform(xchild, oldm);
		    DXSetAttribute(member, "object type", attr);
		}
	    }
	    DXSetMember(ng, name, member);
	}

	DXDelete(sdata->obj);
	sdata->obj = DXReference((Object)ng);


    }
    else if ((b == 1) && (abs(dx) > abs(dy)))
    {
	/*
	 * Its a left'n'right middle button.  Alter the camera's width
	 * parameter based on a scaled stroke dx.
	 */
	sdata->width += (sdata->width * dx)/sdata->w;
    }

    return;

}

static void
Slide2DEndMode(void *data)
{
    Slide2DData sdata = (Slide2DData)data;

    /*
     * Clean up
     */
    if (sdata)
    {
	if (sdata->args)
	    DXDelete(sdata->args);
	
	if (sdata->obj)
	    DXDelete(sdata->obj);
    }

    DXFree(data);
}

/***** CAPTION *****/

/*
 * The caption interactor enables you to interactively add captions to the
 * scene, move them, and update the caption string.   It operates on objects 
 * of type 'caption'.  The argument is the name of the member of the scene
 * object group to be created or modified.  If there is no member of the 
 * scene object group with that name, it creates a new caption.
 *
 * The left button allows you to move the current caption.  Typing into the
 * window allows you to modify its string, including backspace (I think there
 * may be a problem here... You seem to be able to backspace past the end of 
 * the string).
 *
 * Generally, objects of type 'caption' are standard DX captions that have 
 * a 'object type' attribute of 'caption', and two additional attributes:
 * 'string', containing the current string, and 'position', containing the
 * current position.  The caption is modified by extracting these attributes
 * from the member specified by the label, updating the information therein
 * based on the user's input events, creating a new caption with the resulting
 * information, and replacing it into the scene object group under the  same 
 * name.  This causes its predecessor to be deleted.
 */

typedef struct captionData
{
    int w, h;
    int x, y;
    char *label;
    Object obj;
} *CaptionData;

static void *
CaptionInitMode(Object args, int w, int h, int *mask)
{
    Object caption;
    CaptionData sdata = (CaptionData)DXAllocateZero(sizeof(struct captionData));
    if (! sdata)
	return NULL;
    
    if (!args || !DXExtractString(args, &sdata->label))
    {
	DXWarning("Caption interactor: label required");
	sdata->label = NULL;
	return sdata;
    }

    sdata->w = w;
    sdata->h = h;


    /*
     * Only care about left buttons and key press events
     */
    *mask = DXEVENT_LEFT | DXEVENT_KEYPRESS;
    
    return (void *)sdata;
}

static void
CaptionSetRenderable(void *data, Object obj)
{
    CaptionData sdata = (CaptionData)data;
    if (sdata->obj)
	DXDelete(sdata->obj);

    /*
     * Receive the current state of the object
     */
    if (DXGetObjectClass(obj) != CLASS_GROUP)
    {
	DXWarning("Caption: Object must be a group");
	sdata->obj = NULL;
    }
    else
	sdata->obj = DXReference(obj);
}
    

static int
CaptionGetRenderable(void *data, Object *obj)
{
    CaptionData sdata = (CaptionData)data;

    /*
     * Return the current state of the object
     */
    if (sdata->obj)
    {
	*obj = sdata->obj;
	return 1;
    }
    else
    {
	*obj = NULL;
	return 0;
    }
}

static void
CaptionEndMode(void *data)
{
    CaptionData sdata = (CaptionData)data;

    /*
     * Clean up
     */
    if (sdata)
    {
	if (sdata->obj)
	    DXDelete(sdata->obj);
    }

    DXFree(data);
}

static void CaptionMouseButton(void *, DXMouseEvent *);
static void CaptionKeyStruck(void *, DXKeyPressEvent *);

static void
CaptionEventHandler(void *data, DXEvent *event)
{
    switch (event->any.event)
    {
	case DXEVENT_LEFT:
	    CaptionMouseButton(data, (DXMouseEvent *)event);
	    break;
	
	case DXEVENT_KEYPRESS:
	    CaptionKeyStruck(data, (DXKeyPressEvent *)event);
	    break;
	
	default:
	    break;
    }
}

static void 
CaptionMouseButton(void *data, DXMouseEvent *event)
{
    Object caption;
    Object obType;
    CaptionData sdata = (CaptionData)data;
    int b = WHICH_BUTTON(event);
    int x, y;

    x = event->x;
    y = event->y;

    if (sdata->label == NULL)
	return;

    /*
     * Get the member by name.  If no caption by this name exists,
     * there's nothing to move, so we return.
     */
    caption = DXGetMember((Group)sdata->obj, sdata->label);
    if (! caption)
	return;
    
    /*
     * Make sure its an appropriate object 
     */
    obType = DXGetAttribute(caption, "object type");
    if (!obType || strcmp(DXGetString((String)obType), "caption"))
	return;

    /*
     * Only should see left buttons, but check anyway.
     */
    if (b == 0)
    {
	ModuleInput min[2];
	ModuleOutput mout[1];
	Object strattr, posattr;
	float *xyz;

	/*
	 * If its a caption object, it better have these attributes.
	 * A little error checking might be called for.
	 */
	strattr = DXGetAttribute(caption, "string");
	posattr = (Object)DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);
	DXAddArrayData((Array)posattr, 0, 1, NULL);
	xyz = (float *)DXGetArrayData((Array)posattr);

	/*
	 * Move to the current position (0-1 space requires the division 
	 * by the screen width in pixels.
	 */
	xyz[0] = ((float)x)/sdata->w;
	xyz[1] = ((float)(sdata->h - y))/sdata->w;
	xyz[2] = 0.0;

	DXReference(strattr);
	DXReference(posattr);

	/*
	 * Use CallModule to call the Caption module.  This creates the
	 * new caption object.
	 */
	DXModSetObjectInput(min+0, "string", strattr);
	DXModSetObjectInput(min+1, "position", posattr);
	DXModSetObjectOutput(mout+0, "caption", &caption);
	DXCallModule("Caption", 2, min, 1, mout);

	/*
	 * Replace attributes onto caption
	 */
	DXSetAttribute(caption, "string", strattr);
	DXSetAttribute(caption, "position", posattr);
	DXSetAttribute(caption, "object type", obType);

	/* 
	 * Replace it into scene object group
	 */
	DXSetMember((Group)sdata->obj, sdata->label, caption);

	DXDelete(strattr);
	DXDelete(posattr);
    }

    return;
}

static void
CaptionKeyStruck(void *data, DXKeyPressEvent *event)
{
    CaptionData sdata = (CaptionData)data;
    ModuleInput min[2];
    ModuleOutput mout[1];
    Object caption, strattr, posattr;
    Object postion;
    float *xyz;
    int i, oldl, n, *ptr;
    char *oldstr;
    char *newstr;
    Object obType = NULL;
    int x = event->x;
    int y = event->y;
    char c = event->key;

    if (sdata->label == NULL)
	return;

    caption = DXGetMember((Group)sdata->obj, sdata->label);
    if (caption)
    {
	/*
	 * If there was already a member by that name, make sure its
	 * an appropriate object and get its state attributes.
	 */
	obType = DXGetAttribute(caption, "object type");
	if (!obType || strcmp(DXGetString((String)obType), "caption"))
	    return;

	strattr = DXGetAttribute(caption, "string");
	posattr = DXGetAttribute(caption, "position");
    }
    else
    {
	/*
	 * Otherwise, we'll initialize the state.  We'll need an object
	 * type attribute to put on the result (We re-use the one on the
	 * input if there already was a caption)
	 */
	obType = (Object)DXNewString("caption");

	/*
	 * Initial state is an empty string at the current mouse
	 * position.
	 */
	strattr = (Object)DXNewString("");
	posattr = (Object)DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);
	DXAddArrayData((Array)posattr, 0, 1, NULL);
	xyz = (float *)DXGetArrayData((Array)posattr);
	xyz[0] = ((float)x)/sdata->w;
	xyz[1] = ((float)(sdata->h - y))/sdata->w;
	xyz[2] = 0.0;
    }

    DXExtractString(strattr, &oldstr);

    /*
     * Update the string.
     */
    oldl = strlen(oldstr);

    if (c == 0x8 /* BACKSPACE */)
    {
	if (oldl > 0)
	{
	    newstr = (char *)DXAllocate(oldl + 1);
	    for (i = 0; i < oldl-1; i++)
		newstr[i] = oldstr[i];
	    newstr[oldl-1] = '\0';
	}
    }
    else
    {
	newstr = (char *)DXAllocate(oldl + 2);
	for (i = 0; i < oldl; i++)
	    newstr[i] = oldstr[i];
	newstr[oldl] = c;
	newstr[oldl+1] = '\0';
    }

    strattr = (Object)DXNewString(newstr);
	
    DXReference(strattr);
    DXReference(posattr);

    /*
     * Use CallModule to call the Caption module.  This creates the
     * new caption object.
     */
    DXModSetObjectInput(min+0, "string", strattr);
    DXModSetObjectInput(min+1, "position", posattr);
    DXModSetObjectOutput(mout+0, "caption", &caption);
    DXCallModule("Caption", 2, min, 1, mout);

    /*
     * Replace attributes onto caption
     */
    DXSetAttribute(caption, "string", strattr);
    DXSetAttribute(caption, "position", posattr);
    DXSetAttribute(caption, "object type", obType);

    /* 
     * Replace it into scene object group
     */
    DXSetMember((Group)sdata->obj, sdata->label, caption);

    DXDelete(strattr);
    DXDelete(posattr);

    return;
}
