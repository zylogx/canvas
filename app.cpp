#include "app.h"

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

U0 DrawCanvas(RenderTexture2D target)
{
    ClearBackground(RAYWHITE);
    DrawTextureRec(
        target.texture, 
        (Rectangle) { 0, 0, (float)target.texture.width, (float)target.texture.height }, 
        (Vector2) { 0, 0 }, 
        WHITE
    );
}

U0 DrawRecToCanvas(RenderTexture2D canvas, Rectangle* rec, Bool* isDrawRec)
{
    static F32 dx = 0.0f, dy = 0.0f;
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
        F32 x1 = dx, x2 = mousePos.x;
        F32 y1 = dy, y2 = mousePos.y;
        
        // Calculate left, top, right, and bottom for rectangle based on direction
        F32 left   = (x1 < x2) ? x1 : x2;
        F32 right  = (x1 > x2) ? x1 : x2;
        F32 top    = (y1 < y2) ? y1 : y2;
        F32 bottom = (y1 > y2) ? y1 : y2;

        *rec = (Rectangle){ left, top, right - left, bottom - top };

        // Ensure the rectangle is being drawn while dragging
        DrawDottedRec(*rec, BLACK);
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        // Flag when the rectangle is drawn
        *isDrawRec = true;
    }

    if (*isDrawRec)
    {
        BeginTextureMode(canvas);
        
        // Y-flip adjustment for OpenGL
        Rectangle flippedRec = (Rectangle){ rec->x, canvas.texture.height - (rec->y + rec->height), rec->width, rec->height };
        
        // Draw the rectangle with flipped Y-coordinate
        DrawRectangleLinesEx(flippedRec, 1.4f, BLACK);
        
        EndTextureMode();
    }
}

U0 FloodFill(Image* image, I32 x, I32 y, Color targetColor, Color fillColor)
{
    std::stack<Point> stack;
    stack.push({ x, y });

    Color* pixels = (Color*)image->data;

    while (!stack.empty())
    {
        Point p = stack.top();
        stack.pop();

        if (p.x < 0 || p.y < 0 || p.x >= image->width || p.y >= image->height)
        {
            continue;
        }

        Color currentColor = pixels[p.y * image->width + p.x];

        if ((currentColor.r == targetColor.r) && 
            (currentColor.g == targetColor.g) && 
            (currentColor.b == targetColor.b) && 
            (currentColor.a == targetColor.a)) 
        {
            pixels[p.y * image->width + p.x] = fillColor;

            stack.push({ p.x + 1, p.y });
            stack.push({ p.x - 1, p.y });
            stack.push({ p.x, p.y + 1 });
            stack.push({ p.x, p.y - 1 });
        }
    }
}

U0 UpdateApp(U0* appData)
{
    App* data = (App*)appData;

    BeginDrawing();
    DrawCanvas(data->canvas);
    DrawRecToCanvas(data->canvas, &data->rec, &data->isDrawRec);  

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) 
    {
        Vector2 mousePos = GetMousePosition();
        Image image = LoadImageFromTexture(data->canvas.texture);

        I32 mouseX = (I32)mousePos.x;
        I32 mouseY = (I32)mousePos.y;

        Color* pixels = (Color*)image.data;
        Color targetColor = pixels[mouseY * image.width + mouseX];
        Color fillColor = (Color){ rand() % 256, rand() % 256, rand() % 256, 255 };

        FloodFill(&image, mouseX, mouseY, targetColor, fillColor);

        // Update the texture with modified pixel data
        UpdateTexture(data->canvas.texture, image.data);

        // Unload image after updating the texture
        UnloadImage(image);
    }

    DrawFPS(5, 5);
    EndDrawing();
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
    appData.isDrawRec = false;

    return appData;
}

U0 CloseApp(U0* appData)
{
    App* data = (App*)appData;

    UnloadRenderTexture(data->canvas);
    CloseWindow();
}

I32 main()
{
    App app = InitApp();

    while (!WindowShouldClose())
    {
        UpdateApp(&app);
    }

    CloseApp(&app);

    return 0;
}
