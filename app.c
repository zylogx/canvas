#include "app.h"

U0 StackInit(Stack* stack, Size capacity) 
{
    stack->data = MemAlloc(capacity*sizeof(Point));
    stack->size = 0;
    stack->capacity = capacity;
}

U0 StackFree(Stack* stack) 
{
    MemFree(stack->data);
}

Bool StackIsEmpty(Stack* stack) 
{
    return stack->size == 0;
}

U0 StackPush(Stack* stack, Point point) 
{
    if (stack->size == stack->capacity) 
    {
        stack->capacity *= 2;
        stack->data = MemRealloc(stack->data, stack->capacity*sizeof(Point));
    }
    stack->data[stack->size++] = point;
}

Point StackPop(Stack* stack) 
{
    return stack->data[--stack->size];
}

Bool ColorsEqual(Color c1, Color c2) 
{
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

U0 FloodFill(Image* image, int x, int y, Color targetColor, Color fillColor) 
{
    if (!image || !image->data) 
    {
        return;
    }

    Stack stack;
    StackInit(&stack, 64);

    StackPush(&stack, (Point){ x, y });

    Color* pixels = (Color*)image->data;

    while (!StackIsEmpty(&stack)) 
    {
        Point p = StackPop(&stack);

        if (p.x < 0 || p.y < 0 || p.x >= image->width || p.y >= image->height) 
        {
            continue;
        }

        Color currentColor = pixels[p.y * image->width + p.x];

        if (ColorsEqual(currentColor, targetColor)) 
        {
            pixels[p.y * image->width + p.x] = fillColor;

            StackPush(&stack, (Point){ p.x + 1, p.y });
            StackPush(&stack, (Point){ p.x - 1, p.y });
            StackPush(&stack, (Point){ p.x, p.y + 1 });
            StackPush(&stack, (Point){ p.x, p.y - 1 });
        }
    }

    StackFree(&stack);
}

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

U0 DrawCanvas(const RenderTexture2D target)
{
    ClearBackground(RAYWHITE);
    DrawTextureRec(
        target.texture, 
        (Rectangle) { 0, 0, (float)target.texture.width, (float)target.texture.height }, 
        (Vector2) { 0, 0 }, 
        WHITE
    );
}

U0 DrawRecToCanvas(const RenderTexture2D canvas, Rectangle* rec, Bool* isDrawRec)
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

static U0 PaintBucket(const RenderTexture2D canvas, I32 mouseX, I32 mouseY, Color color)
{
    Image imageA = LoadImageFromTexture(canvas.texture);

    if (mouseX < 0 || mouseY < 0 || mouseX >= imageA.width || mouseY >= imageA.height) 
    {
        UnloadImage(imageA);
        return; // Ensure mouse position is valid
    }

    Color* pixels = (Color*)imageA.data;
    Color targetColor = pixels[mouseY*imageA.width + mouseX];
    Color fillColor = color;

    // Exit early if the target color is the same as the fill color
    if (targetColor.r == fillColor.r &&
        targetColor.g == fillColor.g &&
        targetColor.b == fillColor.b &&
        targetColor.a == fillColor.a) 
    {
        UnloadImage(imageA);
        return;
    }
    else
    {
        UnloadImage(imageA);

        Image imageB = LoadImageFromTexture(canvas.texture);

        Color* pixels = (Color*)imageB.data;
        Color targetColor = pixels[mouseY*imageB.width + mouseX];
        Color fillColor = color;

        FloodFill(&imageB, mouseX, mouseY, targetColor, fillColor);

        // Update the texture with modified pixel data
        UpdateTexture(canvas.texture, imageB.data);

        // Unload image after updating the texture
        UnloadImage(imageB);
    }
}

U0 UpdateApp(U0* appData)
{
    App* data = appData;

    Vector2 mousePos = GetMousePosition();

    Color currentColor = GetColorPickerCurrentColor(data->colorPicker);

    BeginDrawing();
    DrawCanvas(data->canvas);
    DrawRecToCanvas(data->canvas, &data->rec, &data->isDrawRec);  

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) 
    {
        I32 mouseX = (I32)mousePos.x;
        I32 mouseY = (I32)mousePos.y;

        PaintBucket(data->canvas, mouseX, mouseY, currentColor);
    }

    DrawRectangleRec(data->toolbarRec, LIGHTGRAY);
    DrawColorPicker(&data->colorPicker, mousePos);
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
    
    InitColorPicker(&appData.colorPicker);

    appData.toolbarRec = (Rectangle){ 0.0f, 0.0f, GetScreenWidth(), 90.0f };

    return appData;
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
