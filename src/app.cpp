/**
* Author: Wildan R Wijanarko
*/

#include "../include/app.h"

static bool isUpdateToolSize = false;

void DrawDottedRec(Rectangle rec, Color color) 
{
    const float dotLength = 10.0f;
    const float gapLength = 5.0f;

    // Top side
    for (float currentX = rec.x; currentX < rec.x + rec.width; currentX += dotLength + gapLength) 
    {
        float nextX = fminf(currentX + dotLength, rec.x + rec.width);
        DrawLine((int16_t)currentX, (int16_t)rec.y, (int16_t)nextX, (int16_t)rec.y, color);
    }

    // Right side
    for (float currentY = rec.y; currentY < rec.y + rec.height; currentY += dotLength + gapLength) 
    {
        float nextY = fminf(currentY + dotLength, rec.y + rec.height);
        DrawLine((int16_t)(rec.x + rec.width), (int16_t)currentY, (int16_t)(rec.x + rec.width), (int16_t)nextY, color);
    }

    // Bottom side
    for (float currentX = rec.x + rec.width; currentX > rec.x; currentX -= dotLength + gapLength) 
    {
        float nextX = fmaxf(currentX - dotLength, rec.x);
        DrawLine((int16_t)currentX, (int16_t)(rec.y + rec.height), (int16_t)nextX, (int16_t)(rec.y + rec.height), color);
    }

    // Left side
    for (float currentY = rec.y + rec.height; currentY > rec.y; currentY -= dotLength + gapLength) 
    {
        float nextY = fmaxf(currentY - dotLength, rec.y);
        DrawLine((int16_t)rec.x, (int16_t)currentY, (int16_t)rec.x, (int16_t)nextY, color);
    }
}

void DrawRecToCanvas(const RenderTexture2D canvas, Rectangle* rec, bool* isDrawRec)
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

        *rec = (Rectangle){left, top, right - left, bottom - top};

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
        GetRenderingState()->Push();

        BeginTextureMode(canvas);
        
        // Y-flip adjustment for OpenGL
        Rectangle flippedRec = (Rectangle){rec->x, canvas.texture.height - (rec->y + rec->height), rec->width, rec->height};
        
        // Draw the rectangle with flipped Y-coordinate
        DrawRectangleLinesEx(flippedRec, 1.4f, BLACK);
        
        EndTextureMode();
    }
}

static void PaintBucket(const RenderTexture2D canvas, int32_t mouseX, int32_t mouseY, Color color)
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
    if (ColorIsEqual(targetColor, fillColor)) 
    {
        UnloadImage(imageA);
        return;
    }
    else
    {
        UnloadImage(imageA);

        GetRenderingState()->Push();

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

static void CheckUndoRedoKeys()
{
    const bool isControlDown = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);

    if (isControlDown)
    {
        if (IsKeyPressed(KEY_Z))
        {
            GetRenderingState()->Undo();
        }
        else if (IsKeyPressed(KEY_Y))
        {
            GetRenderingState()->Redo();
        }
    }
}

static void Rubber(RenderTexture renderer, Vector2 mousePos, float size)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        GetRenderingState()->Push();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {   
        BeginTextureMode(renderer);
        
        // Y-flip adjustment for OpenGL
        Rectangle flippedRec = (Rectangle){mousePos.x, renderer.texture.height - (mousePos.y + 10), size*10, size*10};
        
        // Draw the rectangle with flipped Y-coordinate
        DrawRectangleLinesEx(flippedRec, 1.4f, RAYWHITE);
        
        EndTextureMode();
    }
}

static void DrawSelector(void* appData, Vector2 mousePos)
{
    App* data = (App*)appData;

    // Define selector position and size
    Vector2 pos = {380.0f, 15.0f};
    float selectorWidth = 100.0f;
    float selectorHeight = 62.0f;

    float iconSizeW = selectorWidth/3.0f;
    float iconSizeH = selectorHeight/2.0f;

    pos.x = pos.x - 4.0f;
    pos.y = pos.y - 4.0f;

    Rectangle recA = {pos.x + iconSizeW*0, pos.y, iconSizeW, iconSizeH};
    Rectangle recB = {pos.x + iconSizeW*1, pos.y, iconSizeW, iconSizeH};
    Rectangle recC = {pos.x + iconSizeW*2, pos.y, iconSizeW, iconSizeH};
    
    Rectangle recD = {pos.x + iconSizeW*0, pos.y + iconSizeH*1, iconSizeW, iconSizeH};
    Rectangle recE = {pos.x + iconSizeW*1, pos.y + iconSizeH*1, iconSizeW, iconSizeH};
    Rectangle recF = {pos.x + iconSizeW*2, pos.y + iconSizeH*1, iconSizeW, iconSizeH};

    if (CheckCollisionPointRec(mousePos, recA))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // Brush
            data->selectedTool = 0;
        }

        DrawRectangleRec(recA, GRAY);
    }
    else if (CheckCollisionPointRec(mousePos, recB))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // Paint bucket
            data->selectedTool = 1;
        }
        
        DrawRectangleRec(recB, GRAY);
    }
    else if (CheckCollisionPointRec(mousePos, recC))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // Rectangle
            data->selectedTool = 2;
        }

        DrawRectangleRec(recC, GRAY);
    }
    else if (CheckCollisionPointRec(mousePos, recD))
    {
        DrawRectangleRec(recD, GRAY);
    }
    else if (CheckCollisionPointRec(mousePos, recE))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // Rubber
            data->selectedTool = 4;
        }

        DrawRectangleRec(recE, GRAY);
    }
    else if (CheckCollisionPointRec(mousePos, recF))
    {
        DrawRectangleRec(recF, GRAY);
    }

    DrawRectangleLinesEx(recA, 0.6, DARKGRAY);
    DrawRectangleLinesEx(recB, 0.6, DARKGRAY);
    DrawRectangleLinesEx(recC, 0.6, DARKGRAY);

    DrawRectangleLinesEx(recD, 0.6, DARKGRAY);
    DrawRectangleLinesEx(recE, 0.6, DARKGRAY);
    DrawRectangleLinesEx(recF, 0.6, DARKGRAY);

    GuiDrawIcon(ICON_PENCIL, recA.x, recA.y, 2, BLACK);
    GuiDrawIcon(ICON_COLOR_BUCKET, recB.x, recB.y, 2, BLACK);
    GuiDrawIcon(ICON_BOX, recC.x, recC.y, 2, BLACK);

    GuiDrawIcon(ICON_TEXT_A, recD.x, recD.y, 2, BLACK);
    GuiDrawIcon(ICON_RUBBER, recE.x, recE.y, 2, BLACK);
    GuiDrawIcon(ICON_ZOOM_ALL, recF.x, recF.y, 2, BLACK);
}

static void DrawBrushToCanvas(BrushData brush, RenderTexture2D renderer, Vector2 mousePos)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        GetRenderingState()->Push();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {   
        BeginTextureMode(renderer);
        
        // Flip Y-axis for texture drawing
        float flippedY = renderer.texture.height - mousePos.y;

        // Draw the brush circle at the adjusted position
        DrawCircle((int32_t)mousePos.x, (int32_t)flippedY, brush.size, brush.color);
        
        EndTextureMode();
    }
}

bool DrawToolSizeUpdater(ToolData* tool, Vector2 mousePos, Vector2 pos)
{
    Rectangle rec = {pos.x, pos.y, 45, 62};
    DrawRectangleLinesEx(rec, 0.8f, BLACK);

    DrawRectangleRec((Rectangle){pos.x + 2, pos.y + 6, 41, 2}, BLACK);
    DrawRectangleRec((Rectangle){pos.x + 2, pos.y + 11, 41, 5}, BLACK);
    DrawRectangleRec((Rectangle){pos.x + 2, pos.y + 19, 41, 7}, BLACK);
    DrawRectangleRec((Rectangle){pos.x + 2, pos.y + 29, 41, 9}, BLACK);

    DrawText("Size", pos.x + 10, pos.y + 45, 12, BLACK);

    if (CheckCollisionPointRec(mousePos, rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        tool->isUpdate = true;
    }

    int8_t onHover = 0;
    if (tool->isUpdate)
    {
        const float y = pos.y + 64.0f;
        Rectangle recA = {pos.x, y + 18*0, 45, 18};
        Rectangle recB = {pos.x, y + 18*1, 45, 18};
        Rectangle recC = {pos.x, y + 18*2, 45, 18};
        Rectangle recD = {pos.x, y + 18*3, 45, 18};

        // Background
        DrawRectangleRec((Rectangle){pos.x, y, 45, 18*4}, (Color){245, 246, 247, 255});

        if (CheckCollisionPointRec(mousePos, recA))
        {
            onHover = 1;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                tool->size = 1.0f;
            }
        }
        else if (CheckCollisionPointRec(mousePos, recB))
        {
            onHover = 2;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                tool->size = 3.0f;
            }
        }
        else if (CheckCollisionPointRec(mousePos, recC))
        {
            onHover = 3;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                tool->size = 5.0f;
            }
        }
        else if (CheckCollisionPointRec(mousePos, recD))
        {
            onHover = 4;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                tool->size = 7.0f;
            }
        }

        DrawRectangleLinesEx(recA, 0.8f, BLACK);
        DrawRectangleLinesEx(recB, 0.8f, BLACK);
        DrawRectangleLinesEx(recC, 0.8f, BLACK);
        DrawRectangleLinesEx(recD, 0.8f, BLACK);

        DrawRectangleRec((Rectangle){recA.x + 2, recA.y + 2, recA.width - 4, 4}, (onHover == 1) ? GRAY : BLACK);
        DrawRectangleRec((Rectangle){recB.x + 2, recB.y + 2, recB.width - 4, 7}, (onHover == 2) ? GRAY : BLACK);
        DrawRectangleRec((Rectangle){recC.x + 2, recC.y + 2, recC.width - 4, 9}, (onHover == 3) ? GRAY : BLACK);
        DrawRectangleRec((Rectangle){recD.x + 2, recD.y + 2, recD.width - 4, 11}, (onHover == 4) ? GRAY : BLACK);
    }

    if (tool->isUpdate && onHover != 0 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        tool->isUpdate = !tool->isUpdate;
    }

    return tool->isUpdate;
}

void UpdateApp(void* appData)
{
    App* data = (App*)appData;

    Vector2 mousePos = GetMousePosition();

    Color currentColor = GetColorPickerCurrentColor(data->colorPicker);

    CheckUndoRedoKeys();

    data->brushData.color = currentColor;

    UpdateCanvas(&data->canvas, mousePos);

    BeginDrawing();
    DrawCanvas(&data->canvas);

    if (!CheckCollisionPointRec(mousePos, data->toolbarRec) && !isUpdateToolSize)
    {
        if (data->selectedTool == 0)
        {
            data->brushData.size = data->toolData.size;

            DrawBrushToCanvas(data->brushData, data->canvas.renderer, mousePos);
        }
        else if (data->selectedTool == 1)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
            {
                int32_t mouseX = (int32_t)mousePos.x;
                int32_t mouseY = (int32_t)mousePos.y;

                PaintBucket(data->canvas.renderer, mouseX, mouseY, currentColor);
            }
        }
        else if (data->selectedTool == 2)
        {
            DrawRecToCanvas(data->canvas.renderer, &data->rec, &data->isDrawRec);
        }
        // else if (data->selectedTool == 3)
        // {
        //     
        // } 
        else if (data->selectedTool == 4)
        {
            Rubber(data->canvas.renderer, mousePos, data->toolData.size);
        }  
    }

    DrawRectangleRec(data->toolbarRec, LIGHTGRAY);

    // Undo and redo
    Rectangle undoRec = {20, 5, 30, 24};
    Rectangle redoRec = {55, 5, 30, 24};

    if (CheckCollisionPointRec(mousePos, undoRec))
    {
        DrawRectangleRec(undoRec, GRAY);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            GetRenderingState()->Undo();
        }
    }

    if (CheckCollisionPointRec(mousePos, redoRec))
    {
        DrawRectangleRec(redoRec, GRAY);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            GetRenderingState()->Redo();
        }
    }

    DrawSelector(data, mousePos);

    DrawRectangleLinesEx(undoRec, 0.6f, BLACK);
    DrawRectangleLinesEx(redoRec, 0.6f, BLACK);

    GuiDrawIcon(ICON_UNDO, undoRec.x + 4, undoRec.y + 4, 1, BLACK);
    GuiDrawIcon(ICON_REDO, redoRec.x + 4, redoRec.y + 4, 1, BLACK);

    isUpdateToolSize = DrawToolSizeUpdater(&data->toolData, mousePos, (Vector2){310.0f, 11.0f});

    DrawColorPicker(&data->colorPicker, mousePos);
    DrawFPS(5, 100);
    EndDrawing();
}

App InitApp()
{
    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(120);
    SetExitKey(0);

    App appData = {0};
    appData.canvas = InitCanvas(GetScreenWidth(), GetScreenHeight());
    
    appData.rec = (Rectangle){0};
    appData.isDrawRec = false;
    
    InitColorPicker(&appData.colorPicker);
    InitRenderingState(appData.canvas);

    appData.toolbarRec = (Rectangle){0.0f, 0.0f, GetScreenWidth(), 90.0f};

    appData.brushData.color = BLACK;
    appData.brushData.size = 6.0f;

    appData.toolData.size = 1.0f;

    return appData;
}

void CloseApp(void* appData)
{
    App* data = (App*)appData;

    UnloadRenderTexture(data->canvas.renderer);
    CloseWindow();
}

int main()
{
    App app = InitApp();

    while (!WindowShouldClose())
    {
        UpdateApp(&app);
    }

    CloseApp(&app);

    return 0;
}