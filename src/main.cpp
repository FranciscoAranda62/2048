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

    // Definir variables de estado para cada tecla
    bool rightKeyPressed = false;
    bool leftKeyPressed = false;
    bool upKeyPressed = false;
    bool downKeyPressed = false;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) {
            if (!rightKeyPressed && getStatusAnimation(board) == false) {
                moveRight(board, &seed);
                rightKeyPressed = true;
            }
        } else {
            rightKeyPressed = false;
        }
        
        if (IsKeyDown(KEY_LEFT)) {
            if (!leftKeyPressed && getStatusAnimation(board) == false) {
                moveLeft(board, &seed);
                leftKeyPressed = true;
            }
        } else {
            leftKeyPressed = false;
        }

        if (IsKeyDown(KEY_UP)) {
            if (!upKeyPressed && getStatusAnimation(board) == false) {
                moveUp(board, &seed);
                upKeyPressed = true;
            }
        } else {
            upKeyPressed = false;
        }

        if (IsKeyDown(KEY_DOWN)) {
            if (!downKeyPressed && getStatusAnimation(board) == false) {
                moveDown(board, &seed);
                downKeyPressed = true;
            }
        } else {
            downKeyPressed = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);
            drawBackground(cellSize, separation);
            if (getStatusAnimation(board) == true) {
                drawAnimation(board, cellSize, separation);
            } else {
                drawBoard(board, cellSize, separation);
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}