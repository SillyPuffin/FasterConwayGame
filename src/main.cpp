
#include <raylib.h>
#include <iostream>

//my code
#include "game.h"

int main()
{
    // window parameters
    const unsigned int WINDOW_WIDTH = 1280;
    const unsigned int WINDOW_HEIGHT = 1280;

    //grid parameters
    const unsigned int CELLSIZE = 5;

    //game
    Game game = { WINDOW_WIDTH, WINDOW_HEIGHT, CELLSIZE };
    game.setCell(0, 100);

    InitWindow(WINDOW_HEIGHT, WINDOW_HEIGHT, "Conway's GOL");

    //fps monitor
    float timer = 0.0f;
    int fps = 0;

    while (WindowShouldClose() == false)
    {
        //delta time
        float dt = GetFrameTime();

        timer += dt;
        

        //drawing
        BeginDrawing();

        game.DrawBackground();
        game.DrawCells();

        //fps monitor
        if (timer >= 0.1) {
            timer = 0.0f;
            fps = int(1 / dt);
        }
        DrawText(TextFormat("%04i", fps), 10, 10, 30, WHITE);

        EndDrawing();
    }
}


