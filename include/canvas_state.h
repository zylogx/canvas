/**
* Author: Wildan R Wijanarko
*/

#pragma once

struct CanvasState 
{ 
    bool isCanvasEnabled {true};
};

// Disable the canvas
void DisableCanvas();

// Enable the canvas
void EnableCanvas();

// Check if the canvas is enabled
bool IsCanvasEnabled();
