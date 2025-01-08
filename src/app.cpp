/**
* Author: Wildan R Wijanarko
*/

#include "../include/app.h"

static bool isUpdateToolSize = false;

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

void UpdateApp(void* appData)
{
    App* data = (App*)appData;

    Vector2 mousePos = GetMousePosition();

    Color currentColor = GetColorPickerCurrentColor(data->colorPicker);

    auto update = [&]()
    {
        CheckUndoRedoKeys();
        data->brushData.color = currentColor;
        mousePos = GetMousePosition();
        currentColor = GetColorPickerCurrentColor(data->colorPicker);
        UpdateCanvas(&data->canvas, mousePos);
    };

    auto render = [&]()
    {
        BeginDrawing();
        ClearBackground((Color){204, 213, 229, 255});
        DrawCanvas(&data->canvas);

        if (!CheckCollisionPointRec(mousePos, data->toolbarRec) && !isUpdateToolSize && IsCanvasEnabled())
        {
            if (data->toolData.selectedTool == 0)
            {
                data->brushData.size = data->toolData.size;

                DrawBrushToCanvas(data->brushData, data->canvas.renderer, mousePos);
            }
            else if (data->toolData.selectedTool == 1)
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
                {
                    int32_t mouseX = (int32_t)mousePos.x;
                    int32_t mouseY = (int32_t)mousePos.y;

                    PaintBucket(data->canvas.renderer, mouseX, mouseY, currentColor);
                }
            }
            else if (data->toolData.selectedTool == 2)
            {
                DrawRecToCanvas(data->canvas.renderer, &data->rec, &data->isDrawRec);
            }
            // else if (data->selectedTool == 3)
            // {
            //     
            // } 
            else if (data->toolData.selectedTool == 4)
            {
                Rubber(data->canvas.renderer, mousePos, data->toolData.size);
            }  
        }
        else
        {
            EnableCanvas();
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

        DrawSelector(&data->toolData, mousePos);

        DrawRectangleLinesEx(undoRec, 0.6f, BLACK);
        DrawRectangleLinesEx(redoRec, 0.6f, BLACK);

        GuiDrawIcon(ICON_UNDO, undoRec.x + 4, undoRec.y + 4, 1, BLACK);
        GuiDrawIcon(ICON_REDO, redoRec.x + 4, redoRec.y + 4, 1, BLACK);

        isUpdateToolSize = DrawToolSizeUpdater(&data->toolData, mousePos, (Vector2){310.0f, 11.0f});

        DrawColorPicker(&data->colorPicker, mousePos);
        DrawFPS(5, 100);
        EndDrawing();
    };

    while (!WindowShouldClose())
    {
        update();
        render();
    }
}

App InitApp()
{
    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(120);
    SetExitKey(0);

    App appData = {0};
    appData.canvas = InitCanvas();
    
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
    UpdateApp(&app);
    CloseApp(&app);

    return 0;
}
