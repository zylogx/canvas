/**
* Author: Wildan R Wijanarko
*/

#include "../include/tool.h"

void Rubber(RenderTexture renderer, Vector2 mousePos, float size)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        GetRenderingState()->Push();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {   
        BeginTextureMode(renderer);
        
        // Y-flip adjustment for OpenGL
        Rectangle flippedRec = {mousePos.x, renderer.texture.height - (mousePos.y + 10), size*10, size*10};
        
        // Draw the rectangle with flipped Y-coordinate
        DrawRectangleLinesEx(flippedRec, 1.4f, RAYWHITE);
        
        EndTextureMode();
    }
}

void DrawSelector(ToolData* data, Vector2 mousePos)
{
    // Define selector position and size
    Vector2 pos {380.0f, 15.0f};
    float selectorWidth = 100.0f;
    float selectorHeight = 62.0f;

    float iconSizeW = selectorWidth/3.0f;
    float iconSizeH = selectorHeight/2.0f;

    pos.x = pos.x - 4.0f;
    pos.y = pos.y - 4.0f;

    Rectangle recA {pos.x + iconSizeW*0, pos.y, iconSizeW, iconSizeH};
    Rectangle recB {pos.x + iconSizeW*1, pos.y, iconSizeW, iconSizeH};
    Rectangle recC {pos.x + iconSizeW*2, pos.y, iconSizeW, iconSizeH};
    
    Rectangle recD {pos.x + iconSizeW*0, pos.y + iconSizeH*1, iconSizeW, iconSizeH};
    Rectangle recE {pos.x + iconSizeW*1, pos.y + iconSizeH*1, iconSizeW, iconSizeH};
    Rectangle recF {pos.x + iconSizeW*2, pos.y + iconSizeH*1, iconSizeW, iconSizeH};

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

bool DrawToolSizeUpdater(ToolData* tool, Vector2 mousePos, Vector2 pos)
{
    Rectangle rec {pos.x, pos.y, 45, 62};
    DrawRectangleLinesEx(rec, 0.8f, BLACK);

    DrawRectangleRec({pos.x + 2, pos.y + 6, 41, 2}, BLACK);
    DrawRectangleRec({pos.x + 2, pos.y + 11, 41, 5}, BLACK);
    DrawRectangleRec({pos.x + 2, pos.y + 19, 41, 7}, BLACK);
    DrawRectangleRec({pos.x + 2, pos.y + 29, 41, 9}, BLACK);

    DrawText("Size", pos.x + 10, pos.y + 45, 12, BLACK);

    if (CheckCollisionPointRec(mousePos, rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        tool->isUpdate = true;
    }

    int onHover = 0;
    if (tool->isUpdate)
    {
        float y = pos.y + 64.0f;
        Rectangle recA {pos.x, y + 18*0, 45, 18};
        Rectangle recB {pos.x, y + 18*1, 45, 18};
        Rectangle recC {pos.x, y + 18*2, 45, 18};
        Rectangle recD {pos.x, y + 18*3, 45, 18};

        // Background
        DrawRectangleRec({pos.x, y, 45, 18*4}, (Color){245, 246, 247, 255});

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

        DrawRectangleRec({recA.x + 2, recA.y + 2, recA.width - 4, 4},  (onHover == 1) ? GRAY : BLACK);
        DrawRectangleRec({recB.x + 2, recB.y + 2, recB.width - 4, 7},  (onHover == 2) ? GRAY : BLACK);
        DrawRectangleRec({recC.x + 2, recC.y + 2, recC.width - 4, 9},  (onHover == 3) ? GRAY : BLACK);
        DrawRectangleRec({recD.x + 2, recD.y + 2, recD.width - 4, 11}, (onHover == 4) ? GRAY : BLACK);
    }

    if (tool->isUpdate && onHover != 0 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        tool->isUpdate = !tool->isUpdate;
    }

    return tool->isUpdate;
}