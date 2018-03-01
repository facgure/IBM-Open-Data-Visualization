/*
 * sample asynchronous module.
 * 
 * uses a signal to ask to be woken and rerun after a certain delay.
 * could also use a Registered Input Handler to watch a file descriptor
 * and ask to be rerun if input appears on that descriptor.
 *
 * see async.mdf, which must be loaded before this can be run.
 * also see Makefile_async for how to compile this.
 */

#include <dx/dx.h>
#include <unistd.h>
#include <signal.h>


extern Pointer DXGetModuleId();
extern Error DXReadyToRun(Pointer id);
extern Error DXFreeModuleId(Pointer id);


static Pointer id = NULL;


void sigcatch()
{
    DXReadyToRun(id);
}	


Error
m_Async(Object *in, Object *out)
{
    int i;

    /* if the input is a valid number, return it as the module return
     * value (in addition to using it as the number of seconds to sleep).
     * if the input is null, return the default sleep time.
     */
    out[0] = in[0];

    if (!in[0]) {
	i = 20;
	out[0] = (Object)DXNewArray(TYPE_INT, CATEGORY_REAL, 0);
	DXAddArrayData((Array)out[0], 0, 1, (Pointer)&i);
    } 
    else if (!DXExtractInteger(in[0], &i)) {
	DXSetError(ERROR_INVALID_DATA, "timeout must be an integer");
	out[0] = NULL;
	return ERROR;
    }

    /* negative sleep means quit without setting the handler */
    if (i <= 0) {
	DXFree(id);
	return OK;
    }

    /* arrange a handler to catch the signal */
    signal(SIGALRM, sigcatch);
    
    /* the first time, get the module id for the DXReadyToRun call */
    if (!id) {
	id = DXGetModuleId();
	if (!id) {
	    out[0] = NULL;
	    return ERROR;
	}
    }

    /* ask the operating system for a signal after this number of seconds */ 
    DXMessage("in Async, setting delay to %d seconds", i);
    alarm(i);

    return OK;
}
