#pragma once

#include "raylib.h"
#include "raygui/src/raygui.h"

#include <stdint.h>

typedef struct ColorPicker
{
    Vector2 pos;
    Color currentColor;
    bool isShowColorPicker;
} ColorPicker;

// Initialize the color picker with default or specific settings
void InitColorPicker(ColorPicker* picker);

// Retrieve the currently selected color from the color picker
Color GetColorPickerCurrentColor(const ColorPicker picker);

// Check if the color picker is currently visible
bool IsColorPickerShown(const ColorPicker picker);

// Draw the color picker and handle user interactions
void DrawColorPicker(ColorPicker* picker, Vector2 mousePos);
