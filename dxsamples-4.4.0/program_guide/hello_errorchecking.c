#include <dx/dx.h>


Error m_HelloErrorChecking(Object *in, Object *out)
{
  char message[30], *greeting, *longmessage=NULL;
  
  if (!in[0]) {
    sprintf(message,"hello world");
    out[0] = (Object)DXNewString(message);
  }
  else {
    if (!DXExtractString(in[0],&greeting)) {
      DXSetError(ERROR_BAD_PARAMETER,"value must be a string");
      goto error;
    }
    if (strlen(greeting)<=23) {
      sprintf(message,"%s %s", "hello", greeting);
      out[0] = (Object)DXNewString(message);
    }
    else {
      longmessage = (char *)DXAllocate(strlen(greeting)*sizeof(char) + 7);
      if (!longmessage)
	goto error;
      sprintf(longmessage,"%s %s", "hello", greeting);
      out[0] = (Object)DXNewString(longmessage);
      DXFree((Pointer)longmessage);
    }
  }
  return OK;
  
 error:
  DXFree((Pointer)longmessage);
  return ERROR;
}

