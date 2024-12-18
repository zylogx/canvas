#include "stdint.h"
#include "raylib.h"
#include "raymath.h"

#include <stack>
#include <cstdlib>

// Inspired by https://holyc-lang.com/docs/language-spec/learn-types
typedef double         F64;  // 64-bit floating point type (8 bytes)
typedef float          F32;  // 32-bit floating point type (4 bytes)
typedef uint64_t       U64;  // Unsigned 64-bit integer type (8 bytes)
typedef int64_t        I64;  // Signed 64-bit integer type (8 bytes)
typedef uint32_t       U32;  // Unsigned 32-bit integer type (4 bytes)
typedef int32_t        I32;  // Signed 32-bit integer type (4 bytes)
typedef uint16_t       U16;  // Unsigned 16-bit integer type (2 bytes)
typedef int16_t        I16;  // Signed 16-bit integer type (2 bytes)
typedef uint8_t        U8;   // Unsigned 8-bit integer type (1 byte)
typedef int8_t         I8;   // Signed 8-bit integer type (1 byte)
typedef bool           Bool; // Boolean (1 byte, 0 or 1 only)
typedef void           U0;   // Void type, has no size

typedef struct 
{
    RenderTexture2D canvas;
    Rectangle rec;
    Bool isDrawRec;
} App;

typedef struct 
{
    I32 x;
    I32 y;
} Point;

const I16 screenWidth = 800;
const I16 screenHeight = 450;