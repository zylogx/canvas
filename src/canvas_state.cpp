/**
* Author: Wildan R Wijanarko
*/

#include "../include/canvas_state.h"

static bool isCanvasEnabled = true;

void DisableCanvas()
{
    isCanvasEnabled = false;
}

void EnableCanvas()
{
    isCanvasEnabled = true;
}

bool IsCanvasEnabled()
{
    return isCanvasEnabled;
}