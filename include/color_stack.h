#pragma once

#include "raylib.h"
#include "types.h"

typedef struct 
{
    Point* data;
    size_t size;
    size_t capacity;
} ColorStack;

// Initialize the ColorStack
void ColorStackInit(ColorStack* stack, size_t capacity);

// Push an element onto the ColorStack
void ColorStackPush(ColorStack* stack, Point point);

// Pop an element from the ColorStack
Point ColorStackPop(ColorStack* stack);

// Check if the ColorStack is empty
bool IsColorStackEmpty(const ColorStack* stack);

// Free the ColorStack memory
void ColorStackFree(ColorStack* stack);