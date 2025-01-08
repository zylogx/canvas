/**
* Author: Wildan R Wijanarko
*/

#include "../include/color_stack.h"

void ColorStackInit(ColorStack* stack, size_t capacity) 
{
    stack->data = (Point*)MemAlloc(capacity*sizeof(Point));
    stack->size = 0;
    stack->capacity = capacity;
}

void ColorStackPush(ColorStack* stack, Point point) 
{
    if (stack->size == stack->capacity) 
    {
        stack->capacity *= 2;
        stack->data = (Point*)MemRealloc(stack->data, stack->capacity*sizeof(Point));
    }
    stack->data[stack->size++] = point;
}

Point ColorStackPop(ColorStack* stack) 
{
    return stack->data[--stack->size];
}

void ColorStackFree(ColorStack* stack) 
{
    MemFree(stack->data);
}