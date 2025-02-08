/**
* Author: Wildan R Wijanarko
*/

#include "../include/app.h"

#include <functional>

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

void App::Run()
{
    auto* data = (this);

    Vector2 mousePos = GetMousePosition();

    Color currentColor = data->colorPicker.GetCurrentColor();

    data->toolData.selectedTool = 0;

    std::function<void()> Update = [&]()
    {
        CheckUndoRedoKeys();
        data->brushData.color = currentColor;
        mousePos = GetMousePosition();
        currentColor = data->colorPicker.GetCurrentColor();
        data->toolData.isUpdate = isUpdateToolSize;
        UpdateCanvas(&data->canvas, mousePos);
    };

    std::function<void()> Render = [&]()
    {
        BeginDrawing();
        ClearBackground((Color){204, 213, 229, 255});
        DrawCanvas(&data->canvas);

        if (!CheckCollisionPointRec(mousePos, data->toolbarRec) && !isUpdateToolSize && IsCanvasEnabled() && !data->colorPicker.IsShown())
        {
            if (data->toolData.selectedTool == 0)
            {
                data->brushData.size = data->toolData.size;

                CanvasObject::DrawBrush(data->brushData, data->canvas.renderer, mousePos);
            }
            else if (data->toolData.selectedTool == 1)
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
                {
                    int mouseX = (int)mousePos.x - canvasPosX;
                    int mouseY = (int)mousePos.y - canvasPosY;

                    PaintBucket(data->canvas.renderer, mouseX, mouseY, currentColor);
                }
            }
            else if (data->toolData.selectedTool == 2)
            {
                CanvasObject::DrawRectangleLines(data->canvas.renderer, &data->rec, &data->isDrawRec);
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
        Rectangle undoRec {20, 5, 30, 24};
        Rectangle redoRec {55, 5, 30, 24};

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

        isUpdateToolSize = DrawToolSizeUpdater(&data->toolData, mousePos, {310.0f, 11.0f});

        data->colorPicker.Draw();
        DrawFPS(9, 100);
        EndDrawing();
    };

    while (!WindowShouldClose())
    {
        Update();
        Render();
    }
}

int main()
{
    App app {};
    app.Run();

    return 0;
}
