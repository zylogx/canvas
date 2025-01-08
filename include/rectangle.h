/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "raylib.h"
#include "types.h"
#include "rendering_state.h"

#include <cmath>

// Draw a dotted rectangle
void DrawDottedRec(Rectangle rec, Color color);

// Draw a rectangle on the canvas
void DrawRecToCanvas(const RenderTexture2D canvas, Rectangle* rec, bool* isDrawRec);