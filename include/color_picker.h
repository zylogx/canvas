/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "raylib.h"
#include "color_stack.h"
#include "raygui/src/raygui.h"

#include <stdint.h>

struct ColorPicker
{
    Vector2 pos;
    Color currentColor;
    bool isShowColorPicker;
};

// Initialize the color picker with default or specific settings
void InitColorPicker(ColorPicker* picker);

// Retrieve the currently selected color from the color picker
Color GetColorPickerCurrentColor(ColorPicker picker);

// Check if the color picker is currently visible
bool IsColorPickerShown(ColorPicker picker);

// Draw the color picker and handle user interactions
void DrawColorPicker(ColorPicker* picker, Vector2 mousePos);

// Replace connected targetColor in image starting at (x, y) with fillColor
void FloodFill(Image* image, int x, int y, Color targetColor, Color fillColor);
