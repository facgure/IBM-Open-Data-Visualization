/*
 * sample "simulation" program which outputs results (x y values) on
 * a socket, which will be read by the program m_WatchSocket. See also
 * watchsocket.c
 */

#include <dx/dx.h> 
#include <stdio.h> 
#include <math.h>

extern Error CreateSocket(int *, short);


main(argc, argv)
   int argc;
   char *argv[];
{
  int     fd; 
  float   a, b, c, d, stepsize, i;
  int     numbytes_data;
  int     written;
  short   port = 2000;
  float   x, y, buffer[2], start, end;
  char    *message;

  start = 0;
  end = 100;

  /* 
   * grab the parameters off of the command line and convert to 
   * floating point numbers 
   */
  a = atof(argv[1]);
  b = atof(argv[2]);
  c = atof(argv[3]);
  d = atof(argv[4]);
  stepsize = atof(argv[5]);
  
  /* set up the socket */
  if (!CreateSocket(&fd, port)) {
     message = DXGetErrorMessage();     
     fprintf(stderr,message);
     exit(0);
  }

  /* 
   * number of bytes written each time for the data 
   * (two floating point numbers) 
   */ 
  numbytes_data = 2*(sizeof(float));


  /* run the simulation */
  for (i=start; i< end; i+=stepsize) {
    x = sin(a*i + c);
    y = sin(b*i + d);
    buffer[0] = x;
    buffer[1] = y;
    written = write(fd, buffer, numbytes_data);
    if (written < 0) {
      fprintf(stderr,"failure to write to socket\n");
      exit(0);
    }
  
    /* sleep for one second */ 
    sleep(1);
  }
  /* close the socket */
  close(fd);
}
