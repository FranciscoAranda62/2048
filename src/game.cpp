#include "../include/raylib.h"
#include "../include/game.h"
#include <time.h>
#include <stdlib.h>

#define size 4

struct rep_board {
    int score;
    int ** cell;
};

TBoard createNewTBoard() {
    TBoard nuevo = new rep_board;
    nuevo->score = 0;
    nuevo->cell = new int*[size]; // Primero asignamos memoria para las filas
    for (int i = 0; i < size; i++) {
        nuevo->cell[i] = new int[size]; // Luego asignamos memoria para las columnas de cada fila
    }

    // Inicializamos todas las celdas a 0
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            nuevo->cell[i][j] = 0;
        }
    }
    return nuevo;
}

bool isCellOccupied(TBoard board, int f, int c) {
    if (board->cell[f][c] == 0)
        return false;
    return true;
}

void getCellFree(TBoard board, int * f, int *c) {
    srand(time(NULL));
    do {
        *f = rand() % size;
        *c = rand() % size;
    } while(isCellOccupied(board, *f, *c));
}

int getCellContent(TBoard board, int f, int c) {
    return board->cell[f][c];
}
