/**
* Author: Wildan R Wijanarko
*/

#include "../include/color_picker.h"

static void DrawRgbRec(Vector2 pos, Vector2 mousePos, bool* isShowColorPicker)
{
    Vector2 drawPos {pos.x + 235, pos.y + 2};
    Color color = DARKGREEN;

    GuiColorPanel((Rectangle){drawPos.x, drawPos.y, 30, 30}, " ", &color);

    if (CheckCollisionPointRec(mousePos, {drawPos.x, drawPos.y, 30, 30}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        *isShowColorPicker = !(*isShowColorPicker);
    }
}

void ColorPicker::Draw()
{
    const Color colorsA[] {WHITE, YELLOW, ORANGE, RED, PINK, PURPLE, GREEN, BLUE, BLACK, DARKGRAY};
    const Color colorsB[] {GOLD, BROWN, DARKBROWN, DARKPURPLE, DARKGREEN, DARKBLUE, VIOLET, MAGENTA, MAROON, LIGHTGRAY, GRAY};
    const Vector2 mousePos = GetMousePosition();

    auto* data = (this);

    for (unsigned i = 0; i < 10; i++)
    {
        Rectangle recA {40 + data->pos.x + 19*i, data->pos.y + 19*0, 16, 16};
        Rectangle recB {40 + data->pos.x + 19*i, data->pos.y + 19*1, 16, 16};

        Color colorA = colorsA[i];
        Color colorB = colorsB[i];

        DrawRectangleRec(recA, colorA);
        DrawRectangleRec(recB, colorB);

        DrawRectangleLinesEx(recA, 0.6f, DARKGRAY);
        DrawRectangleLinesEx(recB, 0.6f, DARKGRAY);

        DrawRectangleLinesEx({40 + data->pos.x + 19*i, data->pos.y + 19*2, 16, 16}, 0.6f, DARKGRAY);

        if (!data->isShowColorPicker)
        {
            if (CheckCollisionPointRec(mousePos, recA) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                data->currentColor = colorA;
            }

            if (CheckCollisionPointRec(mousePos, recB) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                data->currentColor = colorB;
            }
        }
    }

    if (data->isShowColorPicker)
    {
        Vector2 guiColorPickerPos {(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 150};

        DrawRectangleRec({guiColorPickerPos.x, guiColorPickerPos.y, 280, 300}, LIGHTGRAY);

        GuiColorPicker({guiColorPickerPos.x + 40, guiColorPickerPos.y + 40, 190, 180}, "", &data->currentColor);

        if (GuiButton({(float)GetScreenWidth() / 2 + 120, (float)GetScreenHeight() / 2 + 100, 50, 30}, "OK"))
        {
            data->isShowColorPicker = false;
        }

        GuiGroupBox({guiColorPickerPos.x, guiColorPickerPos.y, 280, 300}, "Color Picker");
    }

    DrawRgbRec(data->pos, mousePos, &data->isShowColorPicker);
    DrawText("Edit", data->pos.x + 242, data->pos.y + 35, 10, BLACK);
    DrawText("Colors", data->pos.x + 234, data->pos.y + 45, 10, BLACK);
    DrawRectangleRec({data->pos.x + 4, data->pos.y + 4, 30, 30}, data->currentColor);
    DrawText("Color", data->pos.x + 5, data->pos.y + 40, 12, BLACK);
    DrawRectangleLinesEx({data->pos.x - 4, data->pos.y - 4, 280, 62}, 0.6f, DARKGRAY);
}

void FloodFill(Image* image, int x, int y, Color targetColor, Color fillColor) 
{
    if (!image || !image->data) 
    {
        return;
    }

    ColorStack stack;
    ColorStackInit(&stack, 64);

    ColorStackPush(&stack, { x, y });

    Color* pixels = (Color*)image->data;

    while (!stack.IsEmpty()) 
    {
        Point p = ColorStackPop(&stack);

        if (p.x < 0 || p.y < 0 || p.x >= image->width || p.y >= image->height) 
        {
            continue;
        }

        Color currentColor = pixels[p.y*image->width + p.x];

        if (ColorIsEqual(currentColor, targetColor)) 
        {
            pixels[p.y*image->width + p.x] = fillColor;

            ColorStackPush(&stack, { p.x + 1, p.y });
            ColorStackPush(&stack, { p.x - 1, p.y });
            ColorStackPush(&stack, { p.x, p.y + 1 });
            ColorStackPush(&stack, { p.x, p.y - 1 });
        }
    }

    ColorStackFree(&stack);
}

void PaintBucket(const RenderTexture2D& canvas, int mouseX, int mouseY, Color color)
{
    Image imageA = LoadImageFromTexture(canvas.texture);

    if (mouseX < 0 || mouseY < 0 || mouseX >= imageA.width || mouseY >= imageA.height) 
    {
        UnloadImage(imageA);
        return; // Ensure mouse position is valid
    }

    Color* pixels = (Color*)imageA.data;
    Color targetColor = pixels[mouseY*imageA.width + mouseX];
    Color fillColor = color;

    // Exit early if the target color is the same as the fill color
    if (ColorIsEqual(targetColor, fillColor)) 
    {
        UnloadImage(imageA);
        return;
    }
    else
    {
        UnloadImage(imageA);

        GetRenderingState()->Push();

        Image imageB = LoadImageFromTexture(canvas.texture);

        Color* pixels = (Color*)imageB.data;
        Color targetColor = pixels[mouseY*imageB.width + mouseX];
        Color fillColor = color;

        FloodFill(&imageB, mouseX, mouseY, targetColor, fillColor);

        // Update the texture with modified pixel data
        UpdateTexture(canvas.texture, imageB.data);

        // Unload image after updating the texture
        UnloadImage(imageB);
    }
}