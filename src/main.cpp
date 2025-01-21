
#include <raylib.h>
#include <iostream>

int main()
{
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 1280;
    

    InitWindow(WINDOW_HEIGHT, WINDOW_HEIGHT, "Conway's GOL");

    while (WindowShouldClose() == false)
    {

        //drawing
        BeginDrawing();
        EndDrawing();
    }
}


