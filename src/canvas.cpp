/**
* Author: Wildan R Wijanarko
*/

#include "../include/canvas.h"

static Rectangle rec {0};
static Rectangle updateRecA {0};
static Rectangle updateRecB {0};
static bool isUpdateCanvasSize {false};
static int canvasUpdateT {0};

Canvas InitCanvas()
{
    Canvas canvas = {0};

    canvas.renderer = LoadRenderTexture(canvas.width, canvas.height);

    BeginTextureMode(canvas.renderer);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    return canvas;
}

void UpdateCanvas(Canvas* canvas, Vector2 mousePos)
{
    // Update the canvas rec
    rec = {canvasPosX, canvasPosY, canvas->width, canvas->height};

    // Update recs used to update the canvas size
    updateRecA = {canvas->width + 20, canvas->height + canvasPosY, 10, 8};
    updateRecB = {canvas->width + 20, (canvas->height + canvasPosY) / 2 + 50, 10, 8};

    if (CheckCollisionPointRec(mousePos, updateRecA) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
    {
        isUpdateCanvasSize = true;
        canvasUpdateT = 0;
    }

    if (CheckCollisionPointRec(mousePos, updateRecB) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
    {
        isUpdateCanvasSize = true;
        canvasUpdateT = 1;
    }

    if (isUpdateCanvasSize && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
    {
        // Save the current canvas as a temporary render texture
        RenderTexture2D oldCanvas = canvas->renderer;

        // Create a new render texture with the updated size
        RenderTexture2D newCanvas = LoadRenderTexture(canvas->width, canvas->height);

        // Copy the old canvas content onto the new canvas
        BeginTextureMode(newCanvas);
        ClearBackground(RAYWHITE); // Clear the new canvas background

        // Draw the old canvas content, aligning it to the top-left corner
        DrawTextureRec(
            oldCanvas.texture,
            {
                0, 0, 
                (float)oldCanvas.texture.width, 
                -(float)oldCanvas.texture.height // Flip vertically
            },
            {0, canvas->height - oldCanvas.texture.height}, // Adjust height alignment
            WHITE
        );
        EndTextureMode();

        // Unload the old canvas and update the renderer
        UnloadRenderTexture(oldCanvas);
        canvas->renderer = newCanvas;

        isUpdateCanvasSize = false;
    }

    if (isUpdateCanvasSize) 
    {
        DisableCanvas();

        if (canvasUpdateT == 0) 
        {
            if (mousePos.x > canvasPosX && mousePos.y > canvasPosY) 
            {
                canvas->width = mousePos.x - canvasPosX;
                canvas->height = mousePos.y - canvasPosY;
            }
        } 
        else 
        {
            if (mousePos.x > canvasPosX) 
            {
                canvas->width = mousePos.x - canvasPosX;
            }
        }
    }
}

void DrawCanvas(Canvas* canvas)
{
    DrawTexturePro(
        canvas->renderer.texture,
        {0, 0, (float)canvas->renderer.texture.width, (float)canvas->renderer.texture.height}, 
        {canvasPosX, canvasPosY, (float)canvas->renderer.texture.width, (float)canvas->renderer.texture.height}, 
        {0.0f, 0.0f},
        0.0f, 
        WHITE
    );

    if (isUpdateCanvasSize)
    {
        CanvasObject::DrawDottedRectangleLines(rec, BLACK);
    }

    DrawRectangleRec(updateRecA, DARKGRAY);
    DrawRectangleRec(updateRecB, DARKGRAY);
}