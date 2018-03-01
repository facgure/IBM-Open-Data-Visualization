#include <stdio.h>
#include "dxl.h"

#ifndef BASE
#define BASE "/usr/lpp/dx"
#endif


/* 
 * define an error handler
 */
void ErrorHandler(DXLConnection *conn, const char *msg, void *data)
{
    printf("DXL Error: %s\n", msg);
}
  

  
main(int argc, char *argv[])
{
    DXLConnection *conn = NULL;
    char result[100];


    /* 
     * Start Data Explorer in -image mode with certain menus disabled.
     */
    conn = DXLStartDX("dx -image -noExitOptions -noExecuteMenus -noConnectionMenus",NULL); 

    if (conn == NULL)
    {
	fprintf(stderr,"Could not connect\n");
	perror("DXLStartDX");
	exit(1);
    }


    /* 
     * Set the error handler 
     */
    DXLSetErrorHandler(conn, ErrorHandler, NULL);


    /* 
     * Load the visual program to run
     */
    DXLLoadVisualProgram(conn, BASE"/samples/dxlink/sealevel.net");

    /*
     * Set the value of the DXLInput tool which is labelled 
     * "contour_line_value" and execute.
     */
    DXLSetValue(conn, "contour_line_value", "0");
    DXLExecuteOnce(conn);


    /*
     * Set the value of the DXLInput tool which is labelled 
     * "contour_line_value" and execute.
     */
    DXLSetValue(conn, "contour_line_value", "2");
    DXLExecuteOnce(conn);

    /*
     * Set the value of the DXLInput tool which is labelled 
     * "contour_line_value" and execute.
     */
    DXLSetValue(conn, "contour_line_value", "5");
    DXLExecuteOnce(conn);

    /*
     * Set the value of the DXLInput tool which is labelled 
     * "contour_line_value" and execute.
     */
    DXLSetValue(conn, "contour_line_value", "20");
    DXLExecuteOnce(conn);

    /*
     * Set the value of the DXLInput tool which is labelled 
     * "contour_line_value" and execute.
     */
    DXLSetValue(conn, "contour_line_value", "50");
    DXLExecuteOnce(conn);

    printf("An image window will appear\n");
    printf("and a sequence of images will be created.\n");
    printf("When you are finished, hit return to quit:");
    gets(result);
    DXLExitDX(conn);
}
