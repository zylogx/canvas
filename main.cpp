#include "app.h"

U0 App::Run()
{
    Rectangle rec = { 0 };
    Bool isDrawRec = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawCanvas(canvas);
        DrawRecToCanvas(canvas, rec, isDrawRec);  
        DrawFPS(5, 5);
        EndDrawing();
    }
}

I32 main() 
{ 
    AppPtr()->Init();
    AppPtr()->Run();
    AppPtr()->Close();
}