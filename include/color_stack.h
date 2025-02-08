/**
* Author: Wildan R Wijanarko
*/

#pragma once

#include "raylib.h"
#include "type.h"

#include <stddef.h>

struct ColorStack
{
    Point* data;
    size_t size;
    size_t capacity;

    // Check if the ColorStack is empty
    inline bool IsEmpty() const { return size == 0; }
};

// Initialize the ColorStack
void ColorStackInit(ColorStack* stack, size_t capacity);

// Push an element onto the ColorStack
void ColorStackPush(ColorStack* stack, Point point);

// Pop an element from the ColorStack
Point ColorStackPop(ColorStack* stack);

// Free the ColorStack memory
void ColorStackFree(ColorStack* stack);