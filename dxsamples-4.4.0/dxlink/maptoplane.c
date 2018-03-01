#include <stdio.h>
#include "dxl.h"

#ifndef BASE
#define BASE "/usr/lpp/dx"
#endif


void SyncAfterExecute(DXLConnection *conn)
{
  int status=1;

   while (status) {
      sleep(1);
      if (DXLIsMessagePending(conn))
         DXLHandlePendingMessages(conn);
      DXLGetExecutionStatus(conn, &status);
   }
}


/* 
 * this routine simply prints the maximum value as received from the
 * DXLOutput tool 
 */
void max_handler(DXLConnection *conn, const char *name, const char *value, 
                 void *data)
{
   printf("max value = %s\n", value);
}
   

 
main(int argc, char *argv[])
{
    DXLConnection *conn = NULL;
    char result[100];
    int status;

    /* 
     * Start Data explorer user interface in -edit mode, with certain
     * in -edit mode, with certain menus turned off.
     */
    conn = DXLStartDX("dx -edit -noExitOptions -noExecuteMenus -noConnectionMenus",NULL);


    if (conn == NULL)
    {
	fprintf(stderr,"Could not connect\n");
	perror("DXLStartDX");
	exit(1);
    }

    /* 
     *  Set the handler for the DXLOutput tool which is labelled 
     *  "maptoplane_max"
     */
    DXLSetValueHandler(conn, "maptoplane_max", max_handler, NULL);


    /* 
     * Load the visual program to run. Set the value of the DXLInput
     * tool which is labelled "file_to_import".
     * Also set the value of the DXLInput tool which is labelled
     * "maptoplane_point". 
     */
    DXLLoadVisualProgram(conn, BASE"/samples/dxlink/maptoplane.net");
    DXLSetString(conn, "file_to_import","temperature");
    DXLSetValue(conn, "maptoplane_point", "[0 5000 5000]");

    /*
     * Execute the visual program and check for input from maptoplane_max.
     */
    DXLExecuteOnce(conn);
    SyncAfterExecute(conn);

    /* Change the value for the DXLInput tool labelled "maptoplane_point
     * and execute again.
     */
    DXLSetValue(conn, "maptoplane_point", "[10000 5000 5000]");
    DXLExecuteOnce(conn);
    SyncAfterExecute(conn);

    /* Change the value for the DXLInput tool labelled "maptoplane_point
     * and execute again.
     */
    DXLSetValue(conn, "maptoplane_point", "[20000 5000 5000]");
    DXLExecuteOnce(conn);
    SyncAfterExecute(conn);

    /* Change the value for the DXLInput tool labelled "maptoplane_point
     * and execute again.
     */
    DXLSetValue(conn, "maptoplane_point", "[30000 5000 5000]");
    DXLExecuteOnce(conn);
    SyncAfterExecute(conn);

    /* Change the value for the DXLInput tool labelled "maptoplane_point
     * and execute again.
     */
    DXLSetValue(conn, "maptoplane_point", "[50000 5000 5000]");
    DXLExecuteOnce(conn);
    SyncAfterExecute(conn);

    printf("Hit return to quit:");
    gets(result);
    DXLExitDX(conn);
}
