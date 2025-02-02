/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "tool.h"
#include "canvas.h"
#include "shapes.h"
#include "color_picker.h"
#include "rendering_state.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

const int screenWidth = 1080;
const int screenHeight = 720;

class App
{
private:
    Canvas canvas;
    Rectangle rec;
    bool isDrawRec;
    ColorPicker colorPicker;
    Rectangle toolbarRec;
    ToolData toolData;
    CanvasObject::BrushData brushData;

public:
    App()
    {
        InitWindow(screenWidth, screenHeight, "");
        SetTargetFPS(120);
        SetExitKey(0);
        canvas = InitCanvas();
        
        rec = (Rectangle){0};
        isDrawRec = false;
        
        InitRenderingState(canvas);

        toolbarRec = (Rectangle){0.0f, 0.0f, GetScreenWidth(), 90.0f};

        brushData.color = BLACK;
        brushData.size = 6.0f;

        toolData.size = 1.0f;
    }

    ~App()
    {
        UnloadRenderTexture(canvas.renderer);
        CloseWindow();
    }

    // Run the app
    void Run();
};