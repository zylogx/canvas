/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "canvas.h"
#include "rectangle.h"
#include "color_picker.h"
#include "rendering_state.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

struct ToolData
{
    float size;
    bool isUpdate;
};

struct BrushData
{
    float size;
    Color color;
};

struct App
{
    Canvas canvas;
    Rectangle rec;
    bool isDrawRec;
    ColorPicker colorPicker;
    Rectangle toolbarRec;
    int32_t selectedTool;
    BrushData brushData;
    ToolData toolData;
};

const int16_t screenWidth = 1080;
const int16_t screenHeight = 720;