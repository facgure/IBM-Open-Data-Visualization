#include <dx/dx.h>


  Error m_Hello(Object *in, Object *out)
{
  char message[30], *greeting;
  
  if (!in[0])
    sprintf(message,"hello world");
  else {
    DXExtractString(in[0],&greeting);
    sprintf(message,"%s %s", "hello", greeting);
  }
  
  out[0] = (Object)DXNewString(message);
  return OK;
}

