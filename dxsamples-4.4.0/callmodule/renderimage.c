#include "dx/dx.h"
#include <stdio.h>
#include <unistd.h>
#define MAX_PARAMS 21


main()
{
    int          rcode;
    ModuleInput  minput[MAX_PARAMS];
    ModuleOutput moutput[MAX_PARAMS];
    float        iso_value = 0.1;
    Object       isosurface_out, autocamera_out, import_cloud_out;
    Object	 render_out;

    DXInitModules();

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
    /* it again as an input to Render */
    /* If it were not referenced here, it would be deleted after the */
    /* call to DXCallModule */
    DXReference(isosurface_out);
    DXModSetObjectInput(&minput[0], "object", isosurface_out);
    DXModSetObjectOutput(&moutput[0], "camera", &autocamera_out);
    rcode = DXCallModule("AutoCamera", 1, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Render - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "object", isosurface_out);
    DXModSetObjectInput(&minput[1], "camera", autocamera_out);
    DXModSetObjectOutput(&moutput[0], "image", &render_out);
    rcode = DXCallModule("Render", 2, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* WriteImage - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "image", render_out);
    DXModSetStringInput(&minput[1], "name", "imagefile");
    DXModSetStringInput(&minput[2], "format", "tiff");
    rcode = DXCallModule("WriteImage", 3, minput, 0, moutput);

    /* now delete isosurface_out */
    DXDelete((Object)isosurface_out);
    if (rcode == ERROR) goto error;

error: exit (-1);
}
