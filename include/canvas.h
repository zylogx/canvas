/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "raylib.h"
#include "constant.h"
#include "shapes.h"
#include "canvas_state.h"

struct Canvas
{
    RenderTexture2D renderer {};
    float width {400.0f};
    float height {400.0f};
};

// Initialize the canvas
Canvas InitCanvas();

// Update the canvas
void UpdateCanvas(Canvas* canvas, Vector2 mousePos);

// Draw the canvas
void DrawCanvas(Canvas* canvas);