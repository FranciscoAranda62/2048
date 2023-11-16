#include "../include/raylib.h"
#include "../include/board.h"
#include <time.h>
#include <stdlib.h>

// g++ src/main.cpp src/board.cpp -o game.exe -static -I include -L lib -l raylib -l opengl32 -l gdi32 -l winmm
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 500;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "2048");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    unsigned int seed = (unsigned int)time(NULL);
    srand(seed); // Inicializar la semilla

    int cellSize = screenWidth / 5;
    int separation = cellSize / 5;
    TBoard board = createNewTBoard(&seed);
    //int board[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT))
            moveRight(board, &seed);
        if (IsKeyDown(KEY_LEFT))
            moveLeft(board, &seed);
        if (IsKeyDown(KEY_UP))
            moveUp(board, &seed);
        if (IsKeyDown(KEY_DOWN))
            moveDown(board, &seed);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawBoard(board, cellSize, separation);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}