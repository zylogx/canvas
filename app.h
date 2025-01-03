#include "color_picker.h"

typedef struct 
{
    RenderTexture2D canvas;
    Rectangle rec;
    Bool isDrawRec;
    ColorPicker colorPicker;
    Rectangle toolbarRec;
} App;

typedef struct 
{
    I32 x;
    I32 y;
} Point;

typedef struct 
{
    Point* data;
    Size size;
    Size capacity;
} Stack;

const I16 screenWidth = 800;
const I16 screenHeight = 450;