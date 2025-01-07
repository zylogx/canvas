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
    void Init(T* appCanvas);
    void Push();
    void Undo();
    void Redo();
    void Clear();
};

class Canvas;

void InitRenderingState(Canvas& canvas);

std::shared_ptr<RenderingState<Canvas>> GetRenderingState();

// Declare explicit instantiation
extern template class RenderingState<Canvas>;