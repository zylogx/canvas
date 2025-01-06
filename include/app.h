#pragma once

#include "color_picker.h"
#include "canvas.h"
#include "rendering_state.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

typedef struct
{
    float size;
    bool isUpdate;
} ToolData;

typedef struct BrushData
{
    float size;
    Color color;
} BrushData;

typedef struct 
{
    Canvas canvas;
    Rectangle rec;
    bool isDrawRec;
    ColorPicker colorPicker;
    Rectangle toolbarRec;
    int32_t selectedTool;
    BrushData brushData;
    ToolData toolData;
} App;

const int16_t screenWidth = 800;
const int16_t screenHeight = 450;