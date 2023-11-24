#include "../include/raylib.h"
#include "../include/board.h"
#include <time.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <math.h>

#define SIZE 4

#define MAX_COLORS_COUNT 21 // Number of colors available

struct AnimatedCell {
    int fromX, fromY;  // Initial cell position
    int toX, toY;      // New cell position
    int value;         // Cell value
    float progress;    // Animation progress (0.0 a 1.0)
};


struct rep_board {
    int score;
    int dificult;
    AnimatedCell cell[SIZE][SIZE];
};

Color colors[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE };

TBoard createNewTBoard(unsigned int *seed) {
    TBoard nuevo = new rep_board;
    nuevo->score = 0;
    nuevo->dificult = 3;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++){
            nuevo->cell[i][j].value = 0; 
        }
    }
    int fila, columna;
    getRandomCellFree(nuevo, &fila, &columna, seed);
    nuevo->cell[columna][fila].value = getRandomNum(nuevo, seed);
    nuevo->cell[columna][fila].fromX = fila;
    nuevo->cell[columna][fila].fromY = columna;
    nuevo->cell[columna][fila].toX = fila;
    nuevo->cell[columna][fila].toY = columna;
    nuevo->cell[columna][fila].progress = 1;
    return nuevo;
}

void getRandomCellFree(TBoard board, int *fila, int *columna, unsigned int *seed) {
    int posicionesVacias[16][2]; // Matriz para almacenar las posiciones vacías
    int contador = 0;
    // Buscar posiciones vacías y almacenarlas en la matriz
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board->cell[i][j].value == 0) {
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
        *columna = posicionesVacias[indiceAleatorio][0];
        *fila = posicionesVacias[indiceAleatorio][1];
        // Actualizar la semilla después de generar el número aleatorio
        *seed = rand();
    } else {
        // Si no se encontraron posiciones vacías, se establecen valores fuera del rango del arreglo
        *fila = -1;
        *columna = -1;
    }
}

int getCellContent(TBoard board, int f, int c) {
    return board->cell[f][c].value;
}

int lerp(int a, int b, float t) {
    return a + static_cast<int>(t * (b - a));
}

void drawBoard(TBoard board, int cellSize, int separation) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (getCellContent(board, j, i) != 0) {
                //Calula la pocicion de la nueva celda en cada iteracion
                int x = separation * (j + 1) + cellSize * (j);
                int y = separation * (i + 1) + cellSize * (i) + 150;
                //Dibuja la celda y imprime el contenido del tablero
                DrawRectangle(x, y, cellSize, cellSize, colors[(int)log2(getCellContent(board, j, i))]);
                std::string texto = std::to_string(getCellContent(board, j, i));
                DrawText(texto.c_str(), x, y, cellSize, BLACK);
            }
        }
    }
}

int getRandomNum(TBoard board, unsigned int *seed) {
    // Inicializar el generador de números aleatorios con la semilla proporcionada
    srand(*seed);
    // Obtener un número aleatorio en el rango 0-maxNum
    int randomIndex = (rand() % board->dificult - 1) + 2;
    // Actualizar la semilla después de generar el número aleatorio
    *seed = rand();
    // Devolver el número aleatorio
    return pow(2, randomIndex);
}

void moveLeft(TBoard board, unsigned int *seed) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int aux = j + 1;
            // Si el valor que esta en esa celda es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar.
            // Si el valor que esta en esa celda es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo. 
            if (board->cell[j][i].value == 0) {
                while (aux < SIZE && board->cell[aux][i].value == board->cell[j][i].value) {
                    aux++;
                }
                if (aux < SIZE) {
                    board->cell[j][i].value = board->cell[aux][i].value;
                    board->cell[aux][i].value = 0;
                    aux++;
                }
                while (aux < SIZE && board->cell[aux][i].value == 0) {
                    aux++;
                }
                if (aux < SIZE && board->cell[aux][i].value == board->cell[j][i].value){
                    board->cell[j][i].value = board->cell[j][i].value * 2;
                    board->cell[aux][i].value = 0;
                }
            } else {
                while (aux < SIZE && board->cell[aux][i].value == 0) {
                    aux++;
                }
                if (aux < SIZE && board->cell[aux][i].value == board->cell[j][i].value){
                    board->cell[j][i].value = board->cell[j][i].value * 2;
                    board->cell[aux][i].value = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila].value = getRandomNum(board, seed);
    board->cell[columna][fila].fromX = fila;
    board->cell[columna][fila].fromY = columna;
    board->cell[columna][fila].toX = fila;
    board->cell[columna][fila].toY = columna;
    board->cell[columna][fila].progress = 1;
}

void moveRight(TBoard board, unsigned int *seed) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = SIZE - 1; j > 0; j--) {
            int aux = j - 1;
            // Si el valor que esta en esa celda es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar.
            // Si el valor que esta en esa celda es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo. 
            if (board->cell[j][i].value == 0) {
                while (aux >= 0 && board->cell[aux][i].value == board->cell[j][i].value) {
                    aux--;
                }
                if (aux >= 0) {
                    board->cell[j][i].value = board->cell[aux][i].value;
                    board->cell[aux][i].value = 0;
                    aux--;
                }
                while (aux >= 0 && board->cell[aux][i].value == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[aux][i].value == board->cell[j][i].value){
                    board->cell[j][i].value = board->cell[j][i].value * 2;
                    board->cell[aux][i].value = 0;
                }
            } else {
                while (aux >= 0 && board->cell[aux][i].value == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[aux][i].value == board->cell[j][i].value){
                    board->cell[j][i].value = board->cell[j][i].value * 2;
                    board->cell[aux][i].value = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila].value = getRandomNum(board, seed);
    board->cell[columna][fila].fromX = fila;
    board->cell[columna][fila].fromY = columna;
    board->cell[columna][fila].toX = fila;
    board->cell[columna][fila].toY = columna;
    board->cell[columna][fila].progress = 1;
}

void moveUp(TBoard board, unsigned int *seed) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int aux = j + 1;
            // Si el valor que esta en esa celda es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar.
            // Si el valor que esta en esa celda es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo. 
            if (board->cell[i][j].value == 0) {
                while (aux < SIZE && board->cell[i][aux].value == board->cell[i][j].value) {
                    aux++;
                }
                if (aux < SIZE) {
                    board->cell[i][j].value = board->cell[i][aux].value;
                    board->cell[i][aux].value = 0;
                    aux++;
                }
                while (aux < SIZE && board->cell[i][aux].value == 0) {
                    aux++;
                }
                if (aux < SIZE && board->cell[i][aux].value == board->cell[i][j].value){
                    board->cell[i][j].value = board->cell[i][j].value * 2;
                    board->cell[i][aux].value = 0;
                }
            } else {
                while (aux < SIZE && board->cell[i][aux].value == 0) {
                    aux++;
                }
                if (aux < SIZE && board->cell[i][aux].value == board->cell[i][j].value){
                    board->cell[i][j].value = board->cell[i][j].value * 2;
                    board->cell[i][aux].value = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila].value = getRandomNum(board, seed);
    board->cell[columna][fila].fromX = fila;
    board->cell[columna][fila].fromY = columna;
    board->cell[columna][fila].toX = fila;
    board->cell[columna][fila].toY = columna;
    board->cell[columna][fila].progress = 1;
}

void moveDown(TBoard board, unsigned int *seed) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = SIZE - 1; j > 0; j--) {
            int aux = j - 1;
            // Si el valor que esta en esa celda es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar.
            // Si el valor que esta en esa celda es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo. 
            if (board->cell[i][j].value == 0) {
                while (aux >= 0 && board->cell[i][aux].value == board->cell[i][j].value) {
                    aux--;
                }
                if (aux >= 0) {
                    board->cell[i][j].value = board->cell[i][aux].value;
                    board->cell[i][aux].value = 0;
                    aux--;
                }
                while (aux >= 0 && board->cell[i][aux].value == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[i][aux].value == board->cell[i][j].value){
                    board->cell[i][j].value = board->cell[i][j].value * 2;
                    board->cell[i][aux].value = 0;
                }
            } else {
                while (aux >= 0 && board->cell[i][aux].value == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[i][aux].value == board->cell[i][j].value){
                    board->cell[i][j].value = board->cell[i][j].value * 2;
                    board->cell[i][aux].value = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila].value = getRandomNum(board, seed);
    board->cell[columna][fila].fromX = fila;
    board->cell[columna][fila].fromY = columna;
    board->cell[columna][fila].toX = fila;
    board->cell[columna][fila].toY = columna;
    board->cell[columna][fila].progress = 1;
}