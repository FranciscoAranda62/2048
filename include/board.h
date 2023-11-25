#ifndef GAME_H
#define GAME_H

typedef struct rep_board * TBoard;

// CONSTRUCTORS

// Create a new board 4x4
TBoard createNewTBoard(unsigned int *seed);

// GETTERS

// Return cell content
int getCellContent(TBoard board, int f, int c);

// Return status animation
bool getStatusAnimation(TBoard board);

// Get a random free poscition on the board
void getRandomCellFree(TBoard board, int *fila, int *columna, unsigned int *seed);

// Retruns a random number within the range established by the difficulty
// For example if the difficult is 3, the values could be 2, 4 or 8.
int getRandomNum(TBoard board, unsigned int *seed);

// RENDERERS

// Draw the static board 
void drawBoard(TBoard board, int cellSize, int separation);

// Draw the board when it is moving
void drawAnimation(TBoard board, int cellSize, int separation);

// UPDATERS

// Move all valid cell to the left and join if valid
void moveLeft(TBoard board, unsigned int *seed);

// Move all valid cell to the right and join if valid
void moveRight(TBoard board, unsigned int *seed);

// Move all vaild cell up and join if valid
void moveUp(TBoard board, unsigned int *seed);

// Move all valid cell down and join if valid
void moveDown(TBoard board, unsigned int *seed);

#endif