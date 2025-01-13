/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "raylib.h"
#include "color_stack.h"
#include "rendering_state.h"
#include "raygui/src/raygui.h"

#include <stdint.h>

class ColorPicker
{
private:
    Vector2 pos {500.0f, 15.0f};
    Color currentColor {BLACK};
    bool isShowColorPicker {false};

public:
    // Retrieve the currently selected color from the color picker
    inline Color GetCurrentColor() const { return currentColor; }

    // Check if the color picker is currently visible
    inline bool IsShown() const { return isShowColorPicker; }

    // Draw the color picker and handle user interactions
    void Draw();
};

// Replace connected targetColor in image starting at (x, y) with fillColor
void FloodFill(Image* image, int x, int y, Color targetColor, Color fillColor);

// Apply the flood fill
void PaintBucket(const RenderTexture2D& canvas, int mouseX, int mouseY, Color color);
