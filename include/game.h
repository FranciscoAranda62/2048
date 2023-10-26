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



#endif