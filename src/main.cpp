#include "raylib.h"
#include <string>

// g++ src/main.cpp -o game.exe -static -I include -L lib -l raylib -l opengl32 -l gdi32 -l winmm
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 500;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    int cellSize = screenWidth / 5;
    int separation = cellSize / 5;
    int board[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (int i = 1; i <= 4; i++) {
                for (int j = 1; j <= 4; j++) {
                    int x = separation * j + cellSize * (j - 1);
                    int y = separation * i + cellSize * (i - 1) + 150;
                    DrawRectangle(x, y, cellSize, cellSize, ORANGE);
                    std::string texto = std::to_string(board[j - 1][i - 1]);
                    DrawText(texto.c_str(), x, y, cellSize, BLACK);
                }
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