#include <dx/dx.h>
#include <stdio.h>
#include <math.h>

/* This is a simple user interactor which pays attention only
 * to left mouse button and keypress events. If the user clicks on a location
 * on the object being viewed, a text glyph of the data value
 * at the picked point is shown. If the user types keys, the characters
 * are placed in a (single) caption
 */

#define N_USER_INTERACTORS 1

/* 
 * Here are declarations for the routines implementing the
 * interaction mode
 */
static void *PickCaptionInitMode(Object, int, int, int *);
static void PickCaptionEndMode(void *);
static void PickCaptionSetCamera(void *, float *, float *,
		float *, int, float, float);
static int PickCaptionGetCamera(void *, float *, float *, float *, 
              int *, float *, float *);
static void PickCaptionEventHandler(void *, DXEvent *);
static void PickCaptionSetRenderable(void *, Object);
static int PickCaptionGetRenderable(void *, Object *);

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
    interactorTable[0].InitMode =    PickCaptionInitMode;
    interactorTable[0].EndMode =  	PickCaptionEndMode; 
    interactorTable[0].SetCamera =  PickCaptionSetCamera; 
    interactorTable[0].GetCamera =  PickCaptionGetCamera; 
    interactorTable[0].SetRenderable = PickCaptionSetRenderable; 
    interactorTable[0].GetRenderable = PickCaptionGetRenderable; 
    interactorTable[0].EventHandler =  PickCaptionEventHandler; 

    *n = N_USER_INTERACTORS;
    *(long **)t = (long *)interactorTable;
 
    return 1;
}

typedef struct pickCaptionData
{
    int w, h;
    float to[3];
    float from[3];
    float up[3];
    int projection;
    float fov;
    float width;
    int x, y;
    Object obj;
} *PickCaptionData;

static void *
PickCaptionInitMode(Object args, int w, int h, int *mask)
{
    PickCaptionData pdata = (PickCaptionData)DXAllocateZero(sizeof(struct 
               pickCaptionData));
    if (! pdata)
	return NULL;

    pdata->w = w;
    pdata->h = h;


    /* we are only interested in left mouse button and keypress events */ 
    *mask = DXEVENT_LEFT | DXEVENT_KEYPRESS;
	
    return (void *)pdata;
}

/* store away the object */
static void
PickCaptionSetRenderable(void *data, Object obj)
{
  PickCaptionData pdata = (PickCaptionData)data;
  if (pdata->obj)
    DXDelete(pdata->obj);

  pdata->obj = DXReference(obj);
}


/* store away all the information about the camera, so that
 * Pick and Caption can use it.
 */
static void 
PickCaptionSetCamera(void *data, float to[3], float from[3], float up[3],
		    int proj, float fov, float width)
{
    PickCaptionData pdata = (PickCaptionData)data;
    /* store away the camera info */
    pdata->to[0] = to[0];
    pdata->to[1] = to[1];
    pdata->to[2] = to[2];
    pdata->from[0] = from[0];
    pdata->from[1] = from[1];
    pdata->from[2] = from[2];
    pdata->up[0] = up[0];
    pdata->up[1] = up[1];
    pdata->up[2] = up[2];
    pdata->projection = proj;
    pdata->fov = fov;
    pdata->width = width;
}


/* 
 * return the modified camera.
 * since this UserInteractor does not modify
 * the camera, we can simply return 0
 */
static int
PickCaptionGetCamera(void *data, float *to, float *from, float *up,
              int *projection, float *fov, float *width)
{
    return 0;
}

/* return the modified object to Data Explorer. The object may be
 * modified both when picking is done (to add in text glyphs) or
 * when a captions are created (adding in a caption object)
 */ 
static int
PickCaptionGetRenderable(void *data, Object *obj)
{
  PickCaptionData pdata = (PickCaptionData)data;

  /* return the current state */
  if (pdata->obj)
  {
     *obj = pdata->obj;
     return 1;
  }
  else
  {
     *obj = NULL;
     return 0;
  }
}



static void
PickCaptionEndMode(void *data)
{
    DXFree(data);
}

static void PickButtonDown(void *, DXMouseEvent *event);
static void CaptionKeyStruck(void *, DXKeyPressEvent *event);

static void
PickCaptionEventHandler(void *data, DXEvent *event)
{
    PickCaptionData pdata = (PickCaptionData)data;

    switch (event->any.event)
    {
	case DXEVENT_LEFT:
	    switch(event->mouse.state)
	    {
                /* we only care about button down events */
		case BUTTON_DOWN:
		    PickButtonDown(data, (DXMouseEvent *)event);
		    break;

                default:
                    break;
	    }
	    break;
	case DXEVENT_KEYPRESS:
            CaptionKeyStruck(data, (DXKeyPressEvent *)event);
            break;
	
	default:
	    break;
    }
}

static void CaptionKeyStruck(void *data, DXKeyPressEvent *event)
{
    ModuleInput min[4];
    ModuleOutput mout[1];
    Array strattr=NULL, posattr=NULL;
    float *xy_ptr=NULL;
    int oldl, i;
    PickCaptionData pdata = (PickCaptionData)data;
    Object caption, capstring;
    Group newgroup;
    char *oldstr, *newstr;

    char c = event->key;

    pdata->x = event->x;
    pdata->y = event->y;


    /* see if we've already started a caption yet */
    capstring = DXGetMember((Group)pdata->obj,"caption");
    if (capstring) {
      /* extract the current string */
      strattr = (Array)DXGetAttribute((Object)capstring, "string");
      posattr = (Array)DXGetAttribute((Object)capstring,"position");
    }
    else {
      strattr = (Array)DXNewString("");
      posattr = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 2);
      DXAddArrayData(posattr, 0, 1, NULL);
      xy_ptr = DXGetArrayData(posattr);
      xy_ptr[0]=(float)(pdata->x)/(float)pdata->w;
      xy_ptr[1]=(float)(pdata->h-pdata->y)/(float)pdata->h;
    }

    DXExtractString((Object)strattr, &oldstr);
    /* update the string */


    oldl = strlen(oldstr);
    if (c == 0x8 /*backspace*/) {
      if (oldl > 0) {
        newstr = (char *)DXAllocate(oldl);
        for (i=0; i < oldl-1; i++) 
           newstr[i]= oldstr[i];
        newstr[oldl-1] = '\0';
      }
    }
    else {
      newstr = (char *)DXAllocate(oldl + 2);
      for (i = 0; i < oldl; i++) 
        newstr[i] = oldstr[i];
      newstr[oldl] = c;
      newstr[oldl+1] = '\0';
    }


    /* reference this as it will be deleted by DXCallModule */
    DXReference((Object)posattr);

    DXModSetStringInput(&min[0], "string", newstr);
    DXModSetObjectInput(&min[1],"position",(Object)posattr);
    DXModSetObjectOutput(&mout[0],"caption", &caption);
    DXCallModule("Caption",2,min,1,mout);
    
    /* replace attributes onto caption */
    DXSetStringAttribute(caption,"string",newstr);
    DXSetAttribute(caption,"position",(Object)posattr);
   

    DXSetMember((Group)pdata->obj,"caption",caption);
 
    return; 



}
    

static void 
PickButtonDown(void *data, DXMouseEvent *event)
{
    ModuleInput min[4];
    ModuleOutput mout[1];
    Camera camera;
    Array xy=NULL;
    int *xy_ptr;
    int one=1;
    Object pickfield, glyphs=NULL, object, oldglyphs=NULL, newglyphs;
    PickCaptionData pdata = (PickCaptionData)data;
    Point f, t, u;
   


    /* we need to know the x,y location of the pick event */

    pdata->x = event->x;
    pdata->y = event->y;


    oldglyphs = DXGetMember((Group)pdata->obj,"glyphs");

    /* we need to create the proper inputs for the Pick and AutoGlyph modules */

    /* first make the "locations" input, which is simply the x,y location
     * of the pick */
    xy = DXNewArray(TYPE_INT, CATEGORY_REAL, 1, 2);
    DXAddArrayData(xy, 0, 1, NULL);
    xy_ptr = DXGetArrayData(xy);
    xy_ptr[0]=pdata->x;
    xy_ptr[1]=pdata->y;
    /* now create the camera input for Pick */
    camera = DXNewCamera();
    DXSetOrthographic(camera, pdata->width, (float)pdata->h/(float)pdata->w);
    DXSetResolution(camera, pdata->w, 1.0);
    f = DXPt(pdata->from[0], pdata->from[1], pdata->from[2]); 
    t = DXPt(pdata->to[0], pdata->to[1], pdata->to[2]); 
    u = DXPt(pdata->up[0], pdata->up[1], pdata->up[2]); 
    DXSetView(camera, f, t, u);


    /* use DXCallModule to call Pick */     
    DXModSetObjectInput(&min[0],"locations", (Object)xy);
    DXModSetObjectInput(&min[1],"object", (Object)pdata->obj);
    DXModSetObjectInput(&min[2],"camera", (Object)camera);
    DXModSetIntegerInput(&min[3],"interpolate", 1);
    DXModSetObjectOutput(&mout[0],"picked", &pickfield);
    DXCallModule("Pick", 4, min, 1, mout);

    /* check whether the pick field is empty */
    if (!DXEmptyField((Field)pickfield)) {
      /* the pickfield is not empty. Call AutoGlyph on the result */
      DXModSetObjectInput(&min[0],"data", pickfield);
      DXModSetStringInput(&min[1],"type", "text");
      DXModSetObjectOutput(&mout[0],"glyphs", &glyphs);
      DXCallModule("AutoGlyph", 2, min, 1, mout);


      /* create the output object, which includes both the original object
         and the new glyph field */
      if (oldglyphs) {
        newglyphs = (Object)DXNewGroup();
        DXSetEnumeratedMember((Group)newglyphs,0,oldglyphs);
        DXSetEnumeratedMember((Group)newglyphs,1,glyphs);
        DXSetMember((Group)pdata->obj,"glyphs",newglyphs);
      }
      else {
        DXSetMember((Group)pdata->obj,"glyphs",glyphs);
      }

    }

    return;
}

