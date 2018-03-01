/*
 * sample asynchronous outboard module.
 * This program starts a simulation running with given parameters.
 * It then listens for output from the simulation, outputing a field
 * containing the information. If the user changes any parameter,
 * this program restarts the simulation with the new parameters.
 * 
 *
 * see watchsocket.mdf, which must be loaded before this can be run.
 * also see Makefile_architecture_name for how to compile this.
 * In this directory is also a sample "simulation" program called 
 * my_simulation.c. Socket-specific code may be found in socket.c.
 * A sample visual program is watchsocket.net.
 */

#include <dx/dx.h>


static Pointer  id             = NULL;
static int      data_is_ready  = 0; 

Error  worker(int, Pointer);
extern Error ConnectSocket(int *, int);
Error  GetInputs(Object *, char **, float *, float *, float *, float *, 
                 float *);

Error  m_WatchSocket(Object *in, Object *out)
{
  static  float     a,b,c,d;
  static  float     stepsize;
  static  int       startsim = 1;
  static  int       count, fd;
  static  Field     output=NULL; 
  static  Array     positions=NULL, data=NULL;
  int               numread;
  int               numbytes_data;
  int               numbytes_limits;
  float             buff[4], new_a, new_b, new_c, new_d;
  float             new_stepsize;
  ushort            port = 2000;
  char              command[1024];
  char              *sim_name;


  /* the number of bytes that will be read each time (x and y) */
  numbytes_data = 2*sizeof(float);
  
  /* the number of bytes that will be read for the plot limits */
  numbytes_limits = 4*sizeof(float);
  
  /* get the input parameters from the in vector */
  if (!GetInputs(in, &sim_name, &new_a, &new_b, &new_c, &new_d, &new_stepsize))
    goto error;
  
  /*
   * if any factor has changed, then we must restart the simulation
   */
  if ((new_a != a) || (new_b != b) || (new_c != c) || (new_d != d) ||
      (new_stepsize != stepsize))
    {
      startsim = 1;
      a = new_a;
      b = new_b;
      c = new_c;
      d = new_d;
      stepsize = new_stepsize;
    }
  
  if (startsim) 
    {
      /*
       * initialize count
       */
      count = 0;
      
      /* 
       * get the id of this module for DXReadyToRun 
       */
      id = DXGetModuleId();
      if (!id)
	{
	  out[0] = NULL;
          goto error;
	}
      
      /* 
       * start the simulation 
       */
      sprintf(command,"%s %f %f %f %f %f &", sim_name, a, b, c, d, stepsize); 
      system(command);  
      startsim = 0;
      
      /* 
       * wait a bit for the simulation to get the socket going 
       */
      sleep(5);
      
      /* 
       * connect to the socket (which should have been created by 
       * sim_name) 
       */

      if (!ConnectSocket(&fd, port)) 
	{
	  goto error;
	}
      
      /* 
       * set up an input handler to listen on the file descriptor fd 
       * for new input from the simulation. Whenever new data is
       * ready on the file descriptor, worker will be run.
       */
      DXRegisterInputHandler(worker, fd, NULL);
      
      /* 
       * create the output field and arrays 
       */
      output = DXNewField();
      if (!output) goto error;
            
      /* 
       * create the positions and data arrays 
       */ 
      positions = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 1);
      if (!positions) goto error;
      data = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 0, NULL);
      if (!data) goto error;
      
      /* 
       * place the positions and data into the field
       */ 
      if (!DXSetComponentValue(output, "positions", (Object)positions))
	goto error;
      if (!DXSetComponentValue(output, "data", (Object)data))
	goto error;
      
      /* set the data as dependent on positions */
      if (!DXSetComponentAttribute(output, "data", "dep", 
                                   (Object)DXNewString("positions")));
      
      /* We will place the output object in the cache, so that
       * we always have it available. Each time that new data 
       * appears on the file descriptor, we're just going to add
       * new values to the positions and data components, so we 
       * always need to have a copy of the last created version of
       * the output field. Also, if m_WatchSocket gets called without
       * new data being present on the file descriptor, which may even
       * be after the simulation has quit running, (for example if the
       * output of this module held by the executive is deleted from
       * the cache), we need to be able to reoutput the field. 
       */
      if (!DXSetCacheEntry((Object)output, CACHE_PERMANENT, "output", 0, 0))
	goto error;
    }  
  
  /* 
   * if new data is on the file-descriptor... 
   */
  if (data_is_ready)
    {
      /* 
       * get the object (as it's been created so far) from the cache
       */
      output = (Field)DXGetCacheEntry("output", 0, 0);
      
      /* 
       * get the data off of the file descriptor 
       */
      numread = read(fd, buff, numbytes_data);
      
      /* 
       * if numread < 0, there was an error reading from the socket
       */
      if (numread < 0)
	{
	  DXSetError(ERROR_INTERNAL,"failure to read from socket");
	  goto error;
	}
      
      /* 
       * if numread == 0, then the simulation must have completed
       */
      else if (numread==0) 
	{
	  /* 
	   * unregister the input handler 
	   */
	  DXRegisterInputHandler(NULL, fd, NULL);
	  
	  /*
	   * set the output to the last version
	   */
	  out[0] = (Object)output;
	}
      
      /* 
       * else we successfully read from the socket 
       */
      else
	{
	  
	  /* 
	   * re-register the input handler 
	   */
	  DXRegisterInputHandler(worker, fd, NULL);
	  
	  
	  /* 
	   * buff[0] is the x value. Place it in the positions component
	   */
	  if (!DXAddArrayData(positions, count, 1, &buff[0]))
	    goto error;     
	  
	  /* 
	   * buff[1] is the y value. Place it in the data component
	   */
	  if (!DXAddArrayData(data, count, 1, &buff[1]))
	    goto error;     
	  
	  /* 
	   * increment count
	   */
	  count++;
	  
	  /*
	   * clean up the field 
	   */
	  DXChangedComponentValues(output,"positions"); 
	  DXEndField(output);
	  
	  /* 
	   * reset the data_is_ready flag
	   */
	  data_is_ready = 0;
	  
	}
      /* 
       * output the field
       */ 
      out[0] = (Object)output; 
    }
  
  /* 
   * else we were called for some reason other than there being new
   * data on the file descriptor. For example, the copy of the object
   * in the executive's cache may have been deleted. In this case, 
   * just output the last version of output.
   */ 
  else 
    {
      out[0] = DXGetCacheEntry("output",0,0); 
    }
  
  return OK; 
  
  
 error:
  DXDelete((Object)output);
  return ERROR;
}


/* 
 * This routine is run whenever new data appears on the file-descriptor.
 * This was set up in the call to DXRegisterInputHandler.
 */
Error worker(int fd, Pointer arg)
{

  /*
   * set the data_is_ready flag.
   */
  data_is_ready = 1;
 
  /* 
   * Indicate that m_WatchSocket is ready to be run again. If Data
   * Explorer is in execute-on-change mode, m_WatchSocket will be
   * run immediately. Otherwise it will be run the next time an
   * execution is requested by the user.
   */ 
  DXReadyToRun(id);
  
  /* 
   * unregister the input handler. It will be re-registered once the
   * data is read off of the socket.
   */
  DXRegisterInputHandler(NULL, fd, NULL); 
  
}


Error GetInputs(Object *in, char **sim_name, float *a, float *b, 
                float *c, float *d, float *stepsize)
{
  /* 
   * the first input is the name of the simulation program 
   */
  if (!in[0]) 
    {
      DXSetError(ERROR_MISSING_DATA,"program_name must be provided");
      goto error;
    } 
  else
    {
      if (!DXExtractString(in[0], sim_name)) 
	{
	  DXSetError(ERROR_BAD_PARAMETER,"program_name must be a string");
	  goto error;
	}
    }

  /*
   * The second input is the factor a to be applied to the simulation
   */
  if (in[1])
   {
     if (!DXExtractFloat(in[1], a)) 
       {
         DXSetError(ERROR_BAD_PARAMETER, "a must be a scalar value");
         goto error;
       }
   }
   else
     *a = .15;

  /*
   * The third input is the factor b to be applied to the simulation
   */
  if (in[2])
   {
     if (!DXExtractFloat(in[2], b)) 
       {
         DXSetError(ERROR_BAD_PARAMETER, "b must be a scalar value");
         goto error;
       }
   }
   else
     *b = .20;

  /*
   * The fourth input is the factor c to be applied to the simulation
   */
  if (in[3])
   {
     if (!DXExtractFloat(in[3], c)) 
       {
         DXSetError(ERROR_BAD_PARAMETER, "c must be a scalar value");
         goto error;
       }
   }
   else
     *c = 5;

  /*
   * The fifth input is the factor d to be applied to the simulation
   */
  if (in[4])
   {
     if (!DXExtractFloat(in[4], d)) 
       {
         DXSetError(ERROR_BAD_PARAMETER, "d must be a scalar value");
         goto error;
       }
   }
   else
     *d = 10;

  /*
   * The sixth input is the number of steps to be applied to the simulation
   */
  if (in[5])
   {
     if (!DXExtractFloat(in[5], stepsize)) 
       {
         DXSetError(ERROR_BAD_PARAMETER, "stepsize must be a scalar value");
         goto error;
       }
   }
   else
     *stepsize = 1;


   return OK;
error:
   return ERROR;


}
