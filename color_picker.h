#include "raylib.h"
#include "types.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

typedef struct ColorPicker
{
    Vector2 pos;
    Color currentColor;
    Bool isShowColorPicker;
} ColorPicker;

U0 InitColorPicker(ColorPicker* picker)
{
    picker->pos = (Vector2){500.0f, 15.0f};
    picker->currentColor = BLACK;
    picker->isShowColorPicker = false;
}

Color GetColorPickerCurrentColor(const ColorPicker picker)
{
    return picker.currentColor;
}

Bool IsColorPickerShown(const ColorPicker* picker)
{
    return picker->isShowColorPicker;
}

static U0 DrawRgbRec(Vector2 pos, Vector2 mousePos, Bool* isShowColorPicker)
{
    Vector2 drawPos = {pos.x + 235, pos.y + 2};
    Color color = DARKGREEN;

    GuiColorPanel((Rectangle){drawPos.x, drawPos.y, 30, 30}, " ", &color);

    if (CheckCollisionPointRec(mousePos, (Rectangle){drawPos.x, drawPos.y, 30, 30}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        *isShowColorPicker = !(*isShowColorPicker);
    }
}

U0 DrawColorPicker(ColorPicker* picker, Vector2 mousePos)
{
    Color colorsA[] = {WHITE, YELLOW, ORANGE, RED, PINK, PURPLE, GREEN, BLUE, BLACK, DARKGRAY};
    Color colorsB[] = {GOLD, BROWN, DARKBROWN, DARKPURPLE, DARKGREEN, DARKBLUE, VIOLET, MAGENTA, MAROON, LIGHTGRAY, GRAY};

    for (U32 i = 0; i < 10; i++)
    {
        Rectangle recA = {40 + picker->pos.x + 19 * i, picker->pos.y + 19 * 0, 16, 16};
        Rectangle recB = {40 + picker->pos.x + 19 * i, picker->pos.y + 19 * 1, 16, 16};

        Color colorA = colorsA[i];
        Color colorB = colorsB[i];

        DrawRectangleRec(recA, colorA);
        DrawRectangleRec(recB, colorB);

        DrawRectangleLinesEx(recA, 0.6f, DARKGRAY);
        DrawRectangleLinesEx(recB, 0.6f, DARKGRAY);

        DrawRectangleLinesEx((Rectangle){40 + picker->pos.x + 19 * i, picker->pos.y + 19 * 2, 16, 16}, 0.6f, DARKGRAY);

        if (!picker->isShowColorPicker)
        {
            if (CheckCollisionPointRec(mousePos, recA) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                picker->currentColor = colorA;
            }

            if (CheckCollisionPointRec(mousePos, recB) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                picker->currentColor = colorB;
            }
        }
    }

    if (picker->isShowColorPicker)
    {
        Vector2 guiColorPickerPos = {(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 150};

        DrawRectangleRec((Rectangle){guiColorPickerPos.x, guiColorPickerPos.y, 280, 300}, LIGHTGRAY);

        GuiColorPicker((Rectangle){guiColorPickerPos.x + 40, guiColorPickerPos.y + 40, 190, 180}, "", &picker->currentColor);

        if (GuiButton((Rectangle){(float)GetScreenWidth() / 2 + 120, (float)GetScreenHeight() / 2 + 100, 50, 30}, "OK"))
        {
            picker->isShowColorPicker = false;
        }

        GuiGroupBox((Rectangle){guiColorPickerPos.x, guiColorPickerPos.y, 280, 300}, "Color Picker");
    }

    DrawRgbRec(picker->pos, mousePos, &picker->isShowColorPicker);
    DrawText("Edit", picker->pos.x + 242, picker->pos.y + 35, 10, BLACK);
    DrawText("Colors", picker->pos.x + 234, picker->pos.y + 45, 10, BLACK);
    DrawRectangleRec((Rectangle){picker->pos.x + 4, picker->pos.y + 4, 30, 30}, picker->currentColor);
    DrawText("Color", picker->pos.x + 5, picker->pos.y + 40, 12, BLACK);
    DrawRectangleLinesEx((Rectangle){picker->pos.x - 4, picker->pos.y - 4, 280, 62}, 0.6f, DARKGRAY);
}
