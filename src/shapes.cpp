/**
* Author: Wildan R Wijanarko
*/

#include "../include/shapes.h"

#include <cmath>

void CanvasObject::DrawDottedRectangleLines(Rectangle rec, Color color)
{
    const float dotLength = 10.0f;
    const float gapLength = 5.0f;

    // Top side
    for (float currentX = rec.x; currentX < rec.x + rec.width; currentX += dotLength + gapLength) 
    {
        float nextX = fminf(currentX + dotLength, rec.x + rec.width);
        DrawLine(currentX, rec.y, nextX, rec.y, color);
    }

    // Right side
    for (float currentY = rec.y; currentY < rec.y + rec.height; currentY += dotLength + gapLength) 
    {
        float nextY = fminf(currentY + dotLength, rec.y + rec.height);
        DrawLine((rec.x + rec.width), currentY, (rec.x + rec.width), nextY, color);
    }

    // Bottom side
    for (float currentX = rec.x + rec.width; currentX > rec.x; currentX -= dotLength + gapLength) 
    {
        float nextX = fmaxf(currentX - dotLength, rec.x);
        DrawLine(currentX, (rec.y + rec.height), nextX, (rec.y + rec.height), color);
    }

    // Left side
    for (float currentY = rec.y + rec.height; currentY > rec.y; currentY -= dotLength + gapLength) 
    {
        float nextY = fmaxf(currentY - dotLength, rec.y);
        DrawLine(rec.x, currentY, rec.x, nextY, color);
    }
}

void CanvasObject::DrawRectangleLines(const RenderTexture2D& canvas, Rectangle* rec, bool* isDrawRec)
{
    static float dx = 0.0f, dy = 0.0f;
    Vector2 mousePos = GetMousePosition();

    // Capture the starting point of the rectangle on mouse press
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        dx = mousePos.x;
        dy = mousePos.y;
    }

    *isDrawRec = false;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        float x1 = dx, x2 = mousePos.x;
        float y1 = dy, y2 = mousePos.y;
        
        // Calculate left, top, right, and bottom for rectangle based on direction
        float left   = (x1 < x2) ? x1 : x2;
        float right  = (x1 > x2) ? x1 : x2;
        float top    = (y1 < y2) ? y1 : y2;
        float bottom = (y1 > y2) ? y1 : y2;

        *rec = {left, top, right - left, bottom - top};

        // Ensure the rectangle is being drawn while dragging
        CanvasObject::DrawDottedRectangleLines(*rec, BLACK);
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        // Flag when the rectangle is drawn
        *isDrawRec = true;
    }

    if (*isDrawRec)
    {
        GetRenderingState()->Push();

        BeginTextureMode(canvas);
        
        // Y-flip adjustment for OpenGL
        Rectangle flippedRec {rec->x - canvasPosX, GetMouseDrawY(), rec->width, rec->height};
        
        // Draw the rectangle with flipped Y-coordinate
        DrawRectangleLinesEx(flippedRec, 1.4f, BLACK);
        
        EndTextureMode();
    }
}

void CanvasObject::DrawBrush(const BrushData& brush, const RenderTexture2D& renderer, Vector2 mousePos)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        GetRenderingState()->Push();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {   
        BeginTextureMode(renderer);
        
        // Draw the brush circle at the adjusted position
        DrawCircle(GetMouseDrawX(), GetMouseDrawY(), brush.size, brush.color);
        
        EndTextureMode();
    }
}