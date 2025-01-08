/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "tool.h"
#include "canvas.h"
#include "rectangle.h"
#include "color_picker.h"
#include "rendering_state.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

struct App
{
    Canvas canvas;
    Rectangle rec;
    bool isDrawRec;
    ColorPicker colorPicker;
    Rectangle toolbarRec;
    BrushData brushData;
    ToolData toolData;
};

const int16_t screenWidth = 1080;
const int16_t screenHeight = 720;