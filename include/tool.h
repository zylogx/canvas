/**
* Author: Wildan R Wijanarko
*/

#include "rendering_state.h"
#include "raygui/src/raygui.h"

struct ToolData
{
    float size;
    bool isUpdate;
    uint8_t selectedTool;
};

struct BrushData
{
    float size;
    Color color;
};

// Erase part of the canvas at the given position with the specified size
void Rubber(RenderTexture renderer, Vector2 mousePos, float size);

// Draw a selector or indicator for the current tool at the mouse position
void DrawSelector(ToolData* data, Vector2 mousePos);

// Draw a brush stroke on the canvas at the specified position
void DrawBrushToCanvas(BrushData brush, RenderTexture2D renderer, Vector2 mousePos);

// Update the size of the tool dynamically based on the mouse position
bool DrawToolSizeUpdater(ToolData* tool, Vector2 mousePos, Vector2 pos);