#pragma once

#include "raylib.h"
#include "constant.h"

struct Canvas
{
    RenderTexture2D renderer;
    float width;
    float height;
};

// Initialize the canvas
Canvas InitCanvas(float width, float height);

// Update the canvas
void UpdateCanvas(Canvas* canvas);

// Draw the canvas
void DrawCanvas(Canvas* canvas);