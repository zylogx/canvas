#include "../include/rendering_state.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static RenderStack undoStack = {0};
static RenderStack redoStack = {0};

static Canvas* renderCanvas = NULL;

void UpdateRenderingState(Canvas* canvas)
{
    renderCanvas = canvas;
}

void InitRenderingState(Canvas* canvas)
{
    renderCanvas = canvas;

    RenderStackInit(&undoStack, 4);
    RenderStackInit(&redoStack, 4);

    assert(renderCanvas != NULL);
}

void Push()
{
    RenderTexture2D savedState = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    if (savedState.id == 0)
    {
        printf("Error: Failed to load render texture!\n");
        return;
    }

    BeginTextureMode(savedState);
    DrawTexture(renderCanvas->renderer.texture, 0, 0, WHITE);
    EndTextureMode();

    RenderStackPush(&undoStack, savedState);
}

void Undo()
{
    if (!IsRenderStackEmpty(&undoStack))
    {
        RenderTexture2D current = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        if (current.id == 0)
        {
            printf("Error: Failed to load render texture!\n");
            return;
        }

        BeginTextureMode(current);
        DrawTexture(renderCanvas->renderer.texture, 0, 0, WHITE);
        EndTextureMode();
        RenderStackPush(&redoStack, current);

        RenderTexture2D lastState = RenderStackPop(&undoStack);
        BeginTextureMode(renderCanvas->renderer);
        DrawTexture(lastState.texture, 0, 0, WHITE);
        EndTextureMode();
        UnloadRenderTexture(lastState);
    }
    else
    {
        printf("Info: No state to undo.\n");
    }
}

void Redo()
{
    if (!IsRenderStackEmpty(&redoStack))
    {
        // Save the current state to undo stack
        RenderTexture2D savedState = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        if (savedState.id == 0)
        {
            printf("Error: Failed to load render texture for redo!\n");
            return;
        }

        BeginTextureMode(savedState);
        DrawTexture(renderCanvas->renderer.texture, 0, 0, WHITE);
        EndTextureMode();
        RenderStackPush(&undoStack, savedState);

        // Pop the last state from redo stack and apply it to the canvas
        RenderTexture2D redoState = RenderStackPop(&redoStack);
        BeginTextureMode(renderCanvas->renderer);
        DrawTexture(redoState.texture, 0, 0, WHITE);
        EndTextureMode();

        UnloadRenderTexture(redoState);
    }
    else
    {
        printf("Info: No state to redo.\n");
    }
}

void ClearRenderingState()
{
    RenderStackFree(&undoStack);
    RenderStackFree(&redoStack);
}
