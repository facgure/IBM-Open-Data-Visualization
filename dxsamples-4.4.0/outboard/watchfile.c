/*
 * sample asynchronous outboard module.
 * 
 * uses a signal to ask to be woken after a certain delay.
 * if a given file has been changed, reimport the data.
 *
 * see watchfile.mdf, which must be loaded before this can be run.
 * also see Makefile_architecture.name for how to compile this.
 */

#include <dx/dx.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>


static Pointer  id = NULL;
static time_t   lastchanged; 
static int      seconds;
static char     filename[1024];


/* 
 * this routine is called each time the alarm signal is
 * issued.
 */
void signalcatch()
{
    struct stat Buffer;
    time_t changed; 


    /* stat the file to find out its last modification time */
    if (stat(filename, &Buffer) == 0)
    {  
      /* the last time the file was changed */  
      changed = Buffer.st_mtime;  

      /* compare to the last time the file was checked */
      if (lastchanged != changed) 
      {
         /* the file has changed. Rerun the main program. */
         DXReadyToRun(id);
      }
      /* else the file hasn't changed since last time we checked */
      else 
      {
         /* go back to sleep for some seconds, but first reset the
          * alarm */
         signal(SIGALRM, signalcatch);
         alarm(seconds);
      }   
   } 
}	


Error
m_WatchFile(Object *in, Object *out)
{
    struct stat Buffer;
    char *file;
   
    /* the first input is the filename to check */
    if (!in[0]) 
    {
       DXSetError(ERROR_MISSING_DATA,"missing filename");
       return ERROR;
    }

    if (!DXExtractString(in[0], &file)) 
    {
       DXSetError(ERROR_BAD_PARAMETER,"filename must be a string");
       return ERROR;
    }
    
 
    /* put the filename into a static global variable */
    strcpy(filename,file);

    /* the second input is the number of seconds to wait between checks */
    /* the default is 10 seconds */
    if (!in[1])
       seconds = 10;
    else 
    {
       if (!DXExtractInteger(in[1], &seconds)) 
       {
         DXSetError(ERROR_BAD_PARAMETER,"seconds must be an integer");
         return ERROR;
       }
    }

    
    /* the first time through, get the module id for the DXReadyToRun call */
    if (!id) {
	id = DXGetModuleId();
	if (!id) {
	    out[0] = NULL;
	    return ERROR;
	}
    }


    /* get the last modification time of the file */
    if (stat(filename, &Buffer) != 0) {
       DXSetError(ERROR_BAD_PARAMETER,"file %s not found", filename);
       return ERROR;
    }

    lastchanged = Buffer.st_mtime;  


    /* import the data from the file */
    out[0] = DXImportDX(filename, NULL, NULL, NULL, NULL);


    /* set the alarm for the next wakeup */
    signal(SIGALRM, signalcatch);
    alarm(seconds);


    return OK;
}
