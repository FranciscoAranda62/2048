#ifndef GAME_H
#define GAME_H

typedef struct rep_board * TBoard;

// Funcion para crear un nuevo tablero 4x4
TBoard createNewTBoard();

// Check if it cell is free
// Retorna true si esta ocupada, false si no
bool isCellOccupied(TBoard board, int f, int c);

// Get an random free poscition on the board
void getCellFree(TBoard board, int * f, int *c);

// Return cell content
int getCellContent(TBoard board, int f, int c);

void drawBoard(TBoard board, int cellSize, int separation);

// Retrona un numero aletaorio dentro de los validos
// Si maxNum es 2, el numero aleatorio sera 2 o 4
int getRandomNum(int maxNum);

#endif