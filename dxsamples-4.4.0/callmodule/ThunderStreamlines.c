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
    int rcode;
    ModuleInput  minput[MAX_PARAMS];
    ModuleOutput moutput[MAX_PARAMS];
    float cap_position[] = {  0.5,  0.02};
    Array cap_pos_array = NULL;
    Object caption_out = NULL;
    Object import_cloud_out = NULL;
    float iso_value =  0.11;
    Object isosurface_out = NULL;
    float color_opacity =  0.3;
    Object color_out = NULL;
    Object import_wind_out = NULL;
    Array grid_pt_array = NULL;
    float grid_point[] = {  35000,  600,  25000};
    Array grid_shape_array = NULL;
    float grid_shape[] = {  3000,  0,  0,  0,  0,  3000};
    int grid_density = 5;
    Object grid_out = NULL;
    Object streamline_out = NULL;
    Object map_out = NULL;
    Object autocolor_out = NULL;
    float ribbon_width = 400;
    Object ribbon_out = NULL;
    Object options_out = NULL;
    float spec_value = 0.1;
    float diffuse_value = 0.9;
    Object collect_out = NULL;
    Object autocamera_out = NULL;

    DXInitModules();

    /* This program will exit when the enter key is hit */
    DXRegisterInputHandler(exit_handler, STDIN_FILENO, 0) ;

    /* Caption - setup in/outputs and call module */
    DXModSetStringInput(&minput[0], "string", 
        "data courtesy of NCSA, University of Illinois at Urbana-Champaign");
    cap_pos_array = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 2);
    if (cap_pos_array == NULL) goto error;
    if (DXAddArrayData(cap_pos_array, 0, 1, cap_position) == NULL)
        goto error;
    DXModSetObjectInput(&minput[1], "position", (Object)cap_pos_array);
    DXModSetObjectOutput(&moutput[0], "caption", &caption_out);
    rcode = DXCallModule("Caption", 2, minput, 1, moutput);
    if (rcode == ERROR) goto error;

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

    /* Color - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "input", isosurface_out);
    DXModSetStringInput(&minput[1], "color", "light blue");
    DXModSetFloatInput(&minput[2], "opacity", color_opacity);
    DXModSetObjectOutput(&moutput[0], "colored", &color_out);
    rcode = DXCallModule("Color", 3, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Import - setup in/outputs and call module */
    DXModSetStringInput(&minput[0], "name", 
                         "wind");
    DXModSetObjectOutput(&moutput[0], "data", &import_wind_out);
    rcode = DXCallModule("Import", 1, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Grid - setup in/outputs and call module */
    grid_pt_array = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);
    if (grid_pt_array == NULL) goto error;
    if (DXAddArrayData(grid_pt_array, 0, 1, grid_point) == NULL)
        goto error;
    DXModSetObjectInput(&minput[0], "point", (Object)grid_pt_array);
    DXModSetStringInput(&minput[1], "structure", "ellipse");
    grid_shape_array = DXNewArray(TYPE_FLOAT, CATEGORY_REAL, 1, 3);
    if (grid_shape_array == NULL) goto error;
    if (DXAddArrayData(grid_shape_array, 0, 2, grid_shape) == NULL)
        goto error;
    DXModSetObjectInput(&minput[2], "shape", (Object)grid_shape_array);
    DXModSetIntegerInput(&minput[3], "density", grid_density);
    DXModSetObjectOutput(&moutput[0], "grid" , &grid_out);
    rcode = DXCallModule("Grid", 4, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Streamline - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "data", import_wind_out);
    DXModSetObjectInput(&minput[1], "start", grid_out);
    DXModSetIntegerInput(&minput[2], "flag", 1);
    DXModSetObjectOutput(&moutput[0], "line", &streamline_out);
    rcode = DXCallModule("Streamline", 3, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* AutoColor - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "data", streamline_out);
    DXModSetObjectInput(&minput[1], "min", streamline_out);
    DXModSetObjectOutput(&moutput[0], "mapped", &autocolor_out);
    rcode = DXCallModule("AutoColor", 2, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Ribbon - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "line", autocolor_out);
    DXModSetFloatInput(&minput[1], "width", ribbon_width);
    DXModSetObjectOutput(&moutput[0], "ribbon", &ribbon_out);
    rcode = DXCallModule("Ribbon", 2, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Options - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "input", ribbon_out);
    DXModSetStringInput(&minput[1], "attribute", "specular");
    DXModSetFloatInput(&minput[2], "value", spec_value);
    DXModSetStringInput(&minput[3], "attribute1", "diffuse");
    DXModSetFloatInput(&minput[4], "value1", diffuse_value);
    DXModSetObjectOutput(&moutput[0], "output", &options_out);
    rcode = DXCallModule("Options", 5, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Collect - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "object", caption_out);
    DXModSetObjectInput(&minput[1], "object1", color_out);
    DXModSetObjectInput(&minput[2], "object2", options_out);
    DXModSetObjectOutput(&moutput[0], "group", &collect_out);
    rcode = DXCallModule("Collect", 3, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* AutoCamera - setup in/outputs and call module */
    /* We need to reference collect_out because we will use */
    /* it again in the call to Display */
    DXReference(collect_out);
    DXModSetObjectInput(&minput[0], "object", collect_out);
    DXModSetObjectOutput(&moutput[0], "camera", &autocamera_out);
    rcode = DXCallModule("AutoCamera", 1, minput, 1, moutput);
    if (rcode == ERROR) goto error;

    /* Display - setup in/outputs and call module */
    DXModSetObjectInput(&minput[0], "object", collect_out);
    DXModSetObjectInput(&minput[1], "camera", autocamera_out);
    rcode = DXCallModule("Display", 2, minput, 0, moutput);
    if (rcode == ERROR) goto error;

    fprintf(stderr, "Hit the enter key to exit: ");
    while(1) {
        DXCheckRIH(1);
    }

error: exit (-1);
}
