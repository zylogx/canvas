#pragma once

#include "color_picker.h"
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
    RenderTexture2D canvas;
    Rectangle rec;
    bool isDrawRec;
    ColorPicker colorPicker;
    Rectangle toolbarRec;
    int32_t selectedTool;
    BrushData brushData;
    ToolData toolData;
} App;

typedef struct 
{
    int32_t x;
    int32_t y;
} Point;

typedef struct 
{
    Point* data;
    size_t size;
    size_t capacity;
} ColorStack;

const int16_t screenWidth = 800;
const int16_t screenHeight = 450;