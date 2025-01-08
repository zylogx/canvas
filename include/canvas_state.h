/**
* Author: Wildan R Wijanarko
*/

#pragma once

struct CanvasState 
{ 
    bool isCanvasEnabled {true};
};

void DisableCanvas();

void EnableCanvas();

bool IsCanvasEnabled();