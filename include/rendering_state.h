#pragma once

#include "raylib.h"
#include "render_stack.h"

// Push a new rendering state onto the stack
void Push();

// Undo the last rendering operation
void Undo();

// Redo the last undone rendering operation
void Redo();

// Initialize the rendering state with a given canvas
void InitRenderingState(RenderTexture2D* canvas);

// Update the current rendering state with changes to the canvas
void UpdateRenderingState(RenderTexture2D* canvas);

// Clear all rendering states from the stack
void ClearRenderingState();