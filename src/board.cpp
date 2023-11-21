#include "../include/raylib.h"
#include "../include/board.h"
#include <time.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <math.h>

#define size 4

#define MAX_COLORS_COUNT 21          // Number of colors available

struct rep_board {
    int score;
    int cell[size][size];
    int dificult;
};

enum direccion {izquierda, derecha, arriba, abajo};

Color colors[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE };

TBoard createNewTBoard(unsigned int *seed) {
    TBoard nuevo = new rep_board;
    nuevo->score = 0;
    nuevo->dificult = 3;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++){
            nuevo->cell[i][j] = 0; 
        }
    }
    int fila, columna;
    getRandomCellFree(nuevo, &fila, &columna, seed);
    nuevo->cell[fila][columna] = getRandomNum(nuevo, seed);
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
        // Actualizar la semilla después de generar el número aleatorio
        *seed = rand();
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
            if (getCellContent(board, j - 1, i - 1) != 0) {
                //Calula la pocicion de la nueva celda en cada iteracion
                int x = separation * j + cellSize * (j - 1);
                int y = separation * i + cellSize * (i - 1) + 150;
                //Dibuja la celda y imprime el contenido del tablero
                DrawRectangle(x, y, cellSize, cellSize, colors[(int)log2(getCellContent(board, j - 1, i - 1))]);
                std::string texto = std::to_string(getCellContent(board, j - 1, i - 1));
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
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - 1; j++) {
            int aux = j + 1;
            // Si el valor que esta en esa celda es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar.
            // Si el valor que esta en esa celda es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo. 
            if (board->cell[j][i] == 0) {
                while (aux < size && board->cell[aux][i] == board->cell[j][i]) {
                    aux++;
                }
                if (aux < size) {
                    board->cell[j][i] = board->cell[aux][i];
                    board->cell[aux][i] = 0;
                    aux++;
                }
                while (aux < size && board->cell[aux][i] == 0) {
                    aux++;
                }
                if (aux < size && board->cell[aux][i] == board->cell[j][i]){
                    board->cell[j][i] = board->cell[j][i] * 2;
                    board->cell[aux][i] = 0;
                }
            } else {
                while (aux < size && board->cell[aux][i] == 0) {
                    aux++;
                }
                if (aux < size && board->cell[aux][i] == board->cell[j][i]){
                    board->cell[j][i] = board->cell[j][i] * 2;
                    board->cell[aux][i] = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila] = getRandomNum(board, seed);
}

void moveRight(TBoard board, unsigned int *seed) {
    for (int i = 0; i < size; i++) {
        for (int j = size - 1; j > 0; j--) {
            int aux = j - 1;
            // Si el valor que esta en esa celda es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar.
            // Si el valor que esta en esa celda es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo. 
            if (board->cell[j][i] == 0) {
                while (aux >= 0 && board->cell[aux][i] == board->cell[j][i]) {
                    aux--;
                }
                if (aux >= 0) {
                    board->cell[j][i] = board->cell[aux][i];
                    board->cell[aux][i] = 0;
                    aux--;
                }
                while (aux >= 0 && board->cell[aux][i] == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[aux][i] == board->cell[j][i]){
                    board->cell[j][i] = board->cell[j][i] * 2;
                    board->cell[aux][i] = 0;
                }
            } else {
                while (aux >= 0 && board->cell[aux][i] == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[aux][i] == board->cell[j][i]){
                    board->cell[j][i] = board->cell[j][i] * 2;
                    board->cell[aux][i] = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[fila][columna] = getRandomNum(board, seed);
}

void moveUp(TBoard board, unsigned int *seed) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int aux = j + 1;
            // Si el valor que esta en esa celda es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar.
            // Si el valor que esta en esa celda es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo. 
            if (board->cell[i][j] == 0) {
                while (aux < size && board->cell[i][aux] == board->cell[i][j]) {
                    aux++;
                }
                if (aux < size) {
                    board->cell[i][j] = board->cell[i][aux];
                    board->cell[i][aux] = 0;
                    aux++;
                }
                while (aux < size && board->cell[i][aux] == 0) {
                    aux++;
                }
                if (aux < size && board->cell[i][aux] == board->cell[i][j]){
                    board->cell[i][j] = board->cell[i][j] * 2;
                    board->cell[i][aux] = 0;
                }
            } else {
                while (aux < size && board->cell[i][aux] == 0) {
                    aux++;
                }
                if (aux < size && board->cell[i][aux] == board->cell[i][j]){
                    board->cell[i][j] = board->cell[i][j] * 2;
                    board->cell[i][aux] = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[fila][columna] = getRandomNum(board, seed);
}

void moveDown(TBoard board, unsigned int *seed) {
    for (int i = 0; i < size; i++) {
        for (int j = size - 1; j > 0; j--) {
            int aux = j - 1;
            // Si el valor que esta en esa celda es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar.
            // Si el valor que esta en esa celda es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo. 
            if (board->cell[i][j] == 0) {
                while (aux >= 0 && board->cell[i][aux] == board->cell[i][j]) {
                    aux--;
                }
                if (aux >= 0) {
                    board->cell[i][j] = board->cell[i][aux];
                    board->cell[i][aux] = 0;
                    aux--;
                }
                while (aux >= 0 && board->cell[i][aux] == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[i][aux] == board->cell[i][j]){
                    board->cell[i][j] = board->cell[i][j] * 2;
                    board->cell[i][aux] = 0;
                }
            } else {
                while (aux >= 0 && board->cell[i][aux] == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[i][aux] == board->cell[i][j]){
                    board->cell[i][j] = board->cell[i][j] * 2;
                    board->cell[i][aux] = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[fila][columna] = getRandomNum(board, seed);
}