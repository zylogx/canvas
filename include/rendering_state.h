/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "raylib.h"

#include <memory>
#include <stack>

template <typename T>
class RenderingState
{
private:
    std::stack<RenderTexture2D> undoStack {};
    std::stack<RenderTexture2D> redoStack {};
    T* canvas {nullptr};

public:
    // Initialize the canvas
    void Init(T* appCanvas);

    // Push a new rendering state onto the stack
    void Push();

    // Undo the last rendering operation
    void Undo();

    // Redo the last undone rendering operation
    void Redo();

    // Clear all rendering states from the stack
    void Clear();

    // Retrieve the canvas
    inline T* GetCanvas() const { return canvas; }
};

// Forward declaration of the Canvas class
class Canvas;

// Initialize the rendering state with a given canvas
void InitRenderingState(Canvas& canvas);

// Retrieve the rendering state as a shared pointer
std::shared_ptr<RenderingState<Canvas>> GetRenderingState();

// Declare explicit instantiation
extern template class RenderingState<Canvas>;

// Get the current mouse position for drawing on the canvas
Vector2 GetMouseDrawPosition();

// Get the current X-coordinate of the mouse for drawing on the canvas
int GetMouseDrawX();

// Get the current Y-coordinate of the mouse for drawing on the canvas
int GetMouseDrawY();