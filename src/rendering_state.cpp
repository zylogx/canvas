/**
* Author: Wildan R Wijanarko
*/

#include "../include/rendering_state.h"
#include "../include/canvas.h"

template class RenderingState<Canvas>;

std::shared_ptr<RenderingState<Canvas>> renderingState {nullptr};

void InitRenderingState(Canvas& canvas)
{
    renderingState = std::make_shared<RenderingState<Canvas>>();
    renderingState->Init(&canvas);
}

std::shared_ptr<RenderingState<Canvas>> GetRenderingState()
{
    return renderingState;
}

template <typename T>
void RenderingState<T>::Init(T* appCanvas)
{
    canvas = appCanvas;
}

template <typename T>
void RenderingState<T>::Push()
{
    // Save the current state to the undo stack
    RenderTexture2D savedState = LoadRenderTexture(canvas->width, canvas->height);
    BeginTextureMode(savedState);
    DrawTexture(canvas->renderer.texture, 0, 0, WHITE); // Save the current canvas state
    EndTextureMode();
    undoStack.push(savedState);

    // Clear the redo stack (redo is invalid after a new action)
    while (!redoStack.empty()) 
    {
        UnloadRenderTexture(redoStack.top());
        redoStack.pop();
    }
}

template <typename T>
void RenderingState<T>::Undo()
{
    if (!undoStack.empty()) 
    {
        // Save current state to the redo stack
        RenderTexture2D savedState = LoadRenderTexture(canvas->width, canvas->height);
        BeginTextureMode(savedState);
        DrawTexture(canvas->renderer.texture, 0, 0, WHITE); // Save the current canvas state
        EndTextureMode();
        redoStack.push(savedState);

        // Restore the canvas from the undo stack
        RenderTexture2D lastCanvasState = undoStack.top();
        undoStack.pop();
        BeginTextureMode(canvas->renderer);
        DrawTexture(lastCanvasState.texture, 0, 0, WHITE);
        EndTextureMode();

        UnloadRenderTexture(lastCanvasState); // Free memory
    }
}

template <typename T>
void RenderingState<T>::Redo()
{
    if (!redoStack.empty()) 
    {
        // Save current state to the undo stack
        RenderTexture2D savedState = LoadRenderTexture(canvas->width, canvas->height);
        BeginTextureMode(savedState);
        DrawTexture(canvas->renderer.texture, 0, 0, WHITE); // Save the current canvas state
        EndTextureMode();
        undoStack.push(savedState);

        // Restore the canvas from the redo stack
        RenderTexture2D lastRedoState = redoStack.top();
        redoStack.pop();
        BeginTextureMode(canvas->renderer);
        DrawTexture(lastRedoState.texture, 0, 0, WHITE);
        EndTextureMode();

        UnloadRenderTexture(lastRedoState); // Free memory
    } 
}

template <typename T>
void RenderingState<T>::Clear()
{
    while (!undoStack.empty()) 
    {
        UnloadRenderTexture(undoStack.top());
        undoStack.pop();
    }
    while (!redoStack.empty()) 
    {
        UnloadRenderTexture(redoStack.top());
        redoStack.pop();
    }
}

Vector2 GetMouseDrawPosition()
{
    // Flip Y-axis for texture drawing
    float flippedY = renderingState->GetCanvas()->renderer.texture.height - GetMouseY();

    return {GetMouseX() - canvasPosX, flippedY + canvasPosY};
}

int GetMouseDrawX()
{
    return GetMouseX() - canvasPosX;
}

int GetMouseDrawY()
{
    // Flip Y-axis for texture drawing
    float flippedY = renderingState->GetCanvas()->renderer.texture.height - GetMouseY();

    return flippedY + canvasPosY;
}