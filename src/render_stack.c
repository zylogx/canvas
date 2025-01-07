#include "../include/render_stack.h"

void RenderStackInit(RenderStack* renderStack, size_t initialCapacity)
{
    renderStack->data = MemAlloc(initialCapacity*sizeof(RenderTexture2D));
    if (renderStack->data == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    renderStack->size = 0;
    renderStack->capacity = initialCapacity;
}

void RenderStackPush(RenderStack* renderStack, RenderTexture2D texture)
{
    if (renderStack->size == renderStack->capacity)
    {
        size_t newCapacity = renderStack->capacity*2;
        RenderTexture2D* newData = MemRealloc(renderStack->data, newCapacity*sizeof(RenderTexture2D));
        if (newData == NULL)
        {
            perror("Failed to reallocate memory");
            exit(EXIT_FAILURE);
        }
        renderStack->data = newData;
        renderStack->capacity = newCapacity;
    }
    renderStack->data[renderStack->size++] = texture;
}

RenderTexture2D RenderStackPop(RenderStack* renderStack)
{
    if (renderStack->size == 0)
    {
        fprintf(stderr, "RenderStack underflow\n");
        exit(EXIT_FAILURE);
    }
    return renderStack->data[--renderStack->size];
}

bool IsRenderStackEmpty(const RenderStack* renderStack)
{
    return renderStack->size == 0;
}

void RenderStackFree(RenderStack* renderStack)
{
    MemFree(renderStack->data);
    renderStack->data = NULL;
    renderStack->size = 0;
    renderStack->capacity = 0;
}