#pragma once

#include "raylib.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// RenderStack structure for RenderTexture2D
typedef struct RenderStack
{
    RenderTexture2D* data;
    size_t size;
    size_t capacity;
} RenderStack;

// Initialize the RenderStack
void RenderStackInit(RenderStack* renderStack, size_t initialCapacity);

// Push an element onto the RenderStack
void RenderStackPush(RenderStack* renderStack, RenderTexture2D texture);

// Pop an element from the RenderStack
RenderTexture2D RenderStackPop(RenderStack* renderStack);

// Check if the RenderStack is empty
bool IsRenderStackEmpty(const RenderStack* renderStack);

// Free the RenderStack memory
void RenderStackFree(RenderStack* renderStack);