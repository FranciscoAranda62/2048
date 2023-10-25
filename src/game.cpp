#include "../include/raylib.h"
#include "../include/game.h"

struct rep_board {
    int ** cell;
};

TBoard createNewTBoard() {
    TBoard nuevo = new rep_board;
    nuevo->cell = new int*[4]; // Primero asignamos memoria para las filas
    for (int i = 0; i < 4; i++) {
        nuevo->cell[i] = new int[4]; // Luego asignamos memoria para las columnas de cada fila
    }

    // Inicializamos todas las celdas a 0
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            nuevo->cell[i][j] = 0;
        }
    }
    return nuevo;
}