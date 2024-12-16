#include "main.h"

const I16 screenWidth = 800;
const I16 screenHeight = 450;

U0 DrawDottedRec(Rectangle rec, Color color) 
{
    const F32 dotLength = 10.0f;
    const F32 gapLength = 5.0f;

    // Top side
    for (F32 currentX = rec.x; currentX < rec.x + rec.width; currentX += dotLength + gapLength) 
    {
        F32 nextX = fminf(currentX + dotLength, rec.x + rec.width);
        DrawLine((I16)currentX, (I16)rec.y, (I16)nextX, (I16)rec.y, color);
    }

    // Right side
    for (F32 currentY = rec.y; currentY < rec.y + rec.height; currentY += dotLength + gapLength) 
    {
        F32 nextY = fminf(currentY + dotLength, rec.y + rec.height);
        DrawLine((I16)(rec.x + rec.width), (I16)currentY, (I16)(rec.x + rec.width), (I16)nextY, color);
    }

    // Bottom side
    for (F32 currentX = rec.x + rec.width; currentX > rec.x; currentX -= dotLength + gapLength) 
    {
        F32 nextX = fmaxf(currentX - dotLength, rec.x);
        DrawLine((I16)currentX, (I16)(rec.y + rec.height), (I16)nextX, (I16)(rec.y + rec.height), color);
    }

    // Left side
    for (F32 currentY = rec.y + rec.height; currentY > rec.y; currentY -= dotLength + gapLength) 
    {
        F32 nextY = fmaxf(currentY - dotLength, rec.y);
        DrawLine((I16)rec.x, (I16)currentY, (I16)rec.x, (I16)nextY, color);
    }
}

U0 DrawCanvas(RenderTexture2D canvas)
{
    ClearBackground(RAYWHITE);
    DrawTexturePro(
        canvas.texture,
        (Rectangle){ 0, 0, canvas.texture.width, -canvas.texture.height },
        (Rectangle){ 0, 0, canvas.texture.width, canvas.texture.height },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
}

U0 DrawRecToCanvas(RenderTexture2D canvas, Rectangle* rec, Bool* isDrawRec)
{
    static F32 dx = 0.0f, dy = 0.0f;
    Vector2 mousePos = GetMousePosition();

    // Capture the starting point of the rectangle
    if (IsMouseButtonPressed(0))
    {
        dx = mousePos.x;
        dy = mousePos.y;
    }

    *isDrawRec = false;

    if (IsMouseButtonDown(0))
    {
        F32 x1 = dx, x2 = mousePos.x;
        F32 y1 = dy, y2 = mousePos.y;
        
        F32 left   = (x1 < x2) ? x1 : x2;
        F32 right  = (x1 > x2) ? x1 : x2;
        F32 top    = (y1 < y2) ? y1 : y2;
        F32 bottom = (y1 > y2) ? y1 : y2;

        *rec = (Rectangle){ left, top, right - left, bottom - top };

        DrawDottedRec(*rec, BLACK);
    }
    else if (IsMouseButtonReleased(0))
    {
        *isDrawRec = true;
    }

    if (*isDrawRec)
    {
        BeginTextureMode(canvas);
        DrawRectangleLinesEx(*rec, 1.4f, BLACK);
        EndTextureMode();
    }
}

App InitApp()
{
    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(120);
    SetExitKey(0);

    App appData = { 0 };
    appData.canvas = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    
    BeginTextureMode(appData.canvas);
    ClearBackground(RAYWHITE);
    EndTextureMode();
    
    appData.rec = (Rectangle){ 0 };
    appData.isDrawRec = (Bool){ 0 };

    return appData;
}

U0 UpdateApp(U0* appData)
{
    App* data = appData;

    BeginDrawing();
    DrawCanvas(data->canvas);
    DrawRecToCanvas(data->canvas, &data->rec, &data->isDrawRec);  
    DrawFPS(5, 5);
    EndDrawing();
}

U0 CloseApp(U0* appData)
{
    App* data = appData;

    UnloadRenderTexture(data->canvas);
    CloseWindow();
}

U0 main()
{
    App app = InitApp();

    while (!WindowShouldClose())
    {
        UpdateApp(&app);
    }

    CloseApp(&app);
}
