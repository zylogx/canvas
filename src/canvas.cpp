#include "../include/canvas.h"

Canvas InitCanvas(float width, float height)
{
    Canvas canvas = {0};

    canvas.width = width;
    canvas.height = height;

    canvas.renderer = LoadRenderTexture(width, height);

    BeginTextureMode(canvas.renderer);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    return canvas;
}

void UpdateCanvas(Canvas* canvas)
{
    
}

void DrawCanvas(Canvas* canvas)
{
    ClearBackground(RAYWHITE);
    DrawTextureRec(
        canvas->renderer.texture, 
        (Rectangle) {0, 0, (float)canvas->renderer.texture.width, (float)canvas->renderer.texture.height}, 
        (Vector2) {0.0f, 0.0f}, 
        WHITE
    );
}