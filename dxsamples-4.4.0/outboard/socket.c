#include <dx/dx.h> 

#if DXD_HAS_UNIX_SYS_INCLUDES
#include <sys/file.h>
#endif

#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <sys/un.h>

#if DXD_NEEDS_SYS_SELECT_H
#include <sys/select.h>
#endif

#if DXD_HAS_UNIX_SYS_INCLUDES
#include <sys/utsname.h>
#endif


#define SOCK_QUEUE_LENGTH       1
#define SOCK_ACCEPT_TIMEOUT     60      /* Seconds */

extern Error CreateSocket(int *connect_fd, short port)
{
  int     i, fd, write_fd, sts, length; 
  fd_set  fds;
  struct  linger sl;
  struct  sockaddr_un userver;
  struct  timeval to; 
  

#if DXD_HAS_GETDTABLESIZE
    int width = getdtablesize();
#else
    int width = MAXFUPLIM;
#endif

  /* set up the socket */
  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0) {
    DXSetError(ERROR_INTERNAL, "failure to create socket");
    return ERROR;
  }
  
 
  /* set the socket options */ 
  sl.l_onoff = 1;
  sl.l_linger = 0;
  setsockopt(fd, SOL_SOCKET, SO_LINGER, (char *)&sl, sizeof(sl));
  
  memset((char *)&userver, 0, sizeof(userver));
  userver.sun_family = AF_UNIX;
  sprintf(userver.sun_path,"/tmp/.DX-unix/DX%d", port);
  length = sizeof (userver) - sizeof(userver.sun_path) +
    strlen (userver.sun_path);


  unlink(userver.sun_path); 
  
  sts = bind(fd, (struct sockaddr *)&userver, length);
  if (sts < 0) {
    DXSetError(ERROR_INTERNAL,"failure of bind");
    return ERROR;  
  }

  /* listen on the file-descriptor for a connection */
  if (listen(fd, SOCK_QUEUE_LENGTH) < 0) {
    DXSetError(ERROR_INTERNAL, "failure of listen");
    return ERROR;
  }
 
  /* wait until a connection is made on file-descriptor fd */ 
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  to.tv_sec = SOCK_ACCEPT_TIMEOUT;
  to.tv_usec = 0;

  sts = select(width, (SelectPtr) &fds, NULL, NULL, &to);

  if (sts < 0) {
    DXSetError(ERROR_INTERNAL,"failure of select");
    return ERROR;
  }
  else if (sts == 0) {
    DXSetError(ERROR_INTERNAL,"connection timed out");
    return ERROR;
  }

  /* accept the connection */  
  *connect_fd = accept(fd, (struct sockaddr *)&userver, &length);
 
  close(fd); 
  return OK;
 
}


extern Error ConnectSocket(int *fd, int port)
{
  struct sockaddr_un userver;
  int                 length;

      memset((char *)&userver, 0, sizeof(userver));
      userver.sun_family = AF_UNIX;
      sprintf(userver.sun_path, "/tmp/.DX-unix/DX%d", port);
      length = sizeof (userver) - sizeof(userver.sun_path) +
        strlen (userver.sun_path);
      *fd = socket(AF_UNIX, SOCK_STREAM, 0);

      if (connect(*fd, (struct sockaddr *) &userver, length) < 0)
        {
          DXSetError(ERROR_INTERNAL,"failure to connect to socket");
          return ERROR;
        }

      return OK;


}
