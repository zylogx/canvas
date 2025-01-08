/**
* Author: Wildan R Wijanarko
*/

#include "../include/canvas_state.h"

CanvasState canvasState {};

void DisableCanvas()
{
    canvasState.isCanvasEnabled = false;
}

void EnableCanvas()
{
    canvasState.isCanvasEnabled = true;
}

bool IsCanvasEnabled()
{
    return canvasState.isCanvasEnabled;
}