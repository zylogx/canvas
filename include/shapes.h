/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "raylib.h"
#include "constant.h"
#include "rendering_state.h"

namespace CanvasObject
{
    struct BrushData
    {
        float size;
        Color color;
    };

    // Draw a dotted rectangle lines
    void DrawDottedRectangleLines(Rectangle rec, Color color);
    
    // Draw a rectangle lines on the canvas
    void DrawRectangleLines(const RenderTexture2D& canvas, Rectangle* rec, bool* isDrawRec);

    // Draw a brush stroke on the canvas at the specified position
    void DrawBrush(const BrushData& brush, const RenderTexture2D& renderer, Vector2 mousePos);
}
