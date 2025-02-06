/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "light.h"
#include <stdint.h>

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Apply Intensity ***/
uint32_t light_applyIntensity(const uint32_t Color, const double Factor)
{
    uint32_t a, b, color, g, r;
    double factor;

    /*** Apply Intensity ***/
    /* Clamp Factor */
    if(Factor > 1.0)
        factor = 1.0;
    else if(Factor < 0.0)
        factor = 0.0;
    else
        factor = Factor;

    /* Apply Intensity */
    a = Color & 0xFF000000;
    r = (uint32_t)((Color & 0x00FF0000) * factor);
    g = (uint32_t)((Color & 0x0000FF00) * factor);
    b = (uint32_t)((Color & 0x000000FF) * factor);
    color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

    return color;
}
