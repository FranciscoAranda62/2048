#include "../include/raylib.h"
#include "../include/board.h"
#include <time.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <math.h>

#define size 4

struct rep_board {
    int score;
    int cell[size][size];
    int dificult;
};

enum direccion {izquierda, derecha, arriba, abajo};

TBoard createNewTBoard(unsigned int *seed) {
    TBoard nuevo = new rep_board;
    nuevo->score = 0;
    nuevo->dificult = 3;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++){
            nuevo->cell[i][j] = 0; 
        }
    }
    nuevo->cell[0][0] = getRandomNum(nuevo, seed);
    return nuevo;
}

void getRandomCellFree(TBoard board, int *fila, int *columna, unsigned int *seed) {
    int posicionesVacias[16][2]; // Matriz para almacenar las posiciones vacías
    int contador = 0;
    // Buscar posiciones vacías y almacenarlas en la matriz
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board->cell[i][j] == 0) {
                posicionesVacias[contador][0] = i;
                posicionesVacias[contador][1] = j;
                contador++;
            }
        }
    }
    if (contador > 0) {
        // Generar un índice aleatorio y obtener la posición correspondiente
        srand(*seed); // Usar la semilla proporcionada
        int indiceAleatorio = rand() % contador;
        *fila = posicionesVacias[indiceAleatorio][0];
        *columna = posicionesVacias[indiceAleatorio][1];
    } else {
        // Si no se encontraron posiciones vacías, se establecen valores fuera del rango del arreglo
        *fila = -1;
        *columna = -1;
    }
}

int getCellContent(TBoard board, int f, int c) {
    return board->cell[f][c];
}

void drawBoard(TBoard board, int cellSize, int separation) {
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            //Calula la pocicion de la nueva celda en cada iteracion
            int x = separation * j + cellSize * (j - 1);
            int y = separation * i + cellSize * (i - 1) + 150;
            //Dibuja la celda y imprime el contenido del tablero
            DrawRectangle(x, y, cellSize, cellSize, ORANGE);
            std::string texto = std::to_string(getCellContent(board, j - 1, i - 1));
            DrawText(texto.c_str(), x, y, cellSize, BLACK);
        }
    }
}

int getRandomNum(TBoard board, unsigned int *seed) {
    // Inicializar el generador de números aleatorios con la semilla proporcionada
    srand(*seed);

    // Obtener un número aleatorio en el rango 0-maxNum
    int randomIndex = (rand() % board->dificult - 1) + 2;

    // Devolver el número aleatorio
    return pow(2, randomIndex);
}
