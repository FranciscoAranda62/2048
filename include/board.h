#ifndef GAME_H
#define GAME_H

typedef struct rep_board * TBoard;

// Funcion para crear un nuevo tablero 4x4
TBoard createNewTBoard(unsigned int *seed);

// Get an random free poscition on the board
void getRandomCellFree(TBoard board, int *fila, int *columna, unsigned int *seed);

// Return cell content
int getCellContent(TBoard board, int f, int c);

void drawBoard(TBoard board, int cellSize, int separation);

// Retrona un numero aletaorio dentro de los validos
// Si maxNum es 2, el numero aleatorio sera 2 o 4
int getRandomNum(TBoard board, unsigned int *seed);

void moveLeft(TBoard board, unsigned int *seed);

void moveRight(TBoard board, unsigned int *seed);

void moveUp(TBoard board, unsigned int *seed);

void moveDown(TBoard board, unsigned int *seed);

#endif