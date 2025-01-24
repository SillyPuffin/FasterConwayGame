
#include <raylib.h>
#include <iostream>
#include <bitset>

//my code
#include "game.h"

int main()
{
    // window parameters
    const unsigned int WINDOW_WIDTH = 1880;
    const unsigned int WINDOW_HEIGHT = 1080;

    //grid parameters must be >= 2
    const unsigned int CELLSIZE = 40;

    //game
    Game game = { WINDOW_WIDTH, WINDOW_HEIGHT, CELLSIZE };

    game.setCell(0, 0, 1);
    game.setCell(46, 0, 1);

    u8Pair info = game.getCell(46, 0);
    std::cout << std::bitset<8>(info.first) << "\n";


    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Conway's GOL");

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


