#include "dx/dx.h"
#include <stdio.h>
#include <unistd.h>
#define MAX_PARAMS 21

exit_handler(int fd, Pointer arg)
{
    getchar();
    exit(0);
}

main()
{
    int          rcode;
    ModuleInput  minput[MAX_PARAMS];
    ModuleOutput moutput[MAX_PARAMS];
    float        iso_value = 0.1;
    Object       isosurface_out, autocamera_out, import_cloud_out;

    DXInitModules();

    /* This program will exit when the enter key is hit */
    DXRegisterInputHandler(exit_handler, STDIN_FILENO, 0) ;

    /* Import - setup in/outputs and call module */
    DXModSetStringInput(&minput[0], "name", 
                         "cloudwater");
    DXModSetObjectOutput(&moutput[0], "data", &import_cloud_out);
    rcode = DXCallModule("Import", 1, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Isosurface - setup in/outputs and call module */
    /* no need to reference import_cloud_out before using it here */
    /* because we will not use it again. After this module is     */
    /* called import_cloud_out will be deleted.                   */
    DXModSetObjectInput(&minput[0], "data", import_cloud_out);
    DXModSetFloatInput(&minput[1], "value", iso_value);
    DXModSetObjectOutput(&moutput[0], "surface", &isosurface_out);
    rcode = DXCallModule("Isosurface", 2, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* AutoCamera - setup in/outputs and call module */
    /* We need to reference isosurface_out, because we will need */
    /* it again as an input to Display */
    /* If it were not referenced here, it would be deleted after the */
    /* call to DXCallModule */
    DXReference(isosurface_out);
    DXModSetObjectInput(&minput[0], "object", isosurface_out);
    DXModSetObjectOutput(&moutput[0], "camera", &autocamera_out);
    rcode = DXCallModule("AutoCamera", 1, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Display - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "object", isosurface_out);
    DXModSetObjectInput(&minput[1], "camera", autocamera_out);
    rcode = DXCallModule("Display", 2, minput, 0, moutput);
    if (rcode == ERROR) goto error;

    /* Delete the extra reference we put on isosurface_out that kept */
    /* it from being deleted. */
    DXDelete(isosurface_out);

    fprintf(stderr, "Hit the enter key to exit: ");

    /* enables the refresh of the display window, and the   */
    /* handling of the exit on carriage return              */
    while(1) {
        DXCheckRIH(1); 
    } 

error: exit (-1);
}
