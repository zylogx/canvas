#pragma once

#include "raylib.h"
#include "constant.h"

typedef struct
{
    RenderTexture2D renderer;
    float width;
    float height;
} Canvas;

// Initialize the canvas
Canvas InitCanvas(float width, float height);

// Draw the canvas
void DrawCanvas(Canvas* canvas);