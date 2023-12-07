#include "../include/raylib.h"
#include "../include/board.h"
#include <time.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <math.h>

#define SIZE 4

#define MAX_COLORS_COUNT 21 // Number of colors available

const float animationSpeed = 0.01;
const int heightBoard = 230;

struct AnimatedCell {
    int toF, toC;      // New cell position
    int value, beforeValue; // Cell value
    float progress;    // Animation progress (0.0 a 1.0)
};


struct rep_board {
    int score;
    int dificult;
    bool statusAnimation;
    AnimatedCell cell[SIZE][SIZE];
};

Color colors[MAX_COLORS_COUNT] = {
    DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
    GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
    GREEN, SKYBLUE, PURPLE, BEIGE 
};

TBoard createNewTBoard(unsigned int *seed) {
    TBoard nuevo = new rep_board;
    nuevo->score = 0;
    nuevo->dificult = 3;
    nuevo->statusAnimation = false;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++){
            nuevo->cell[i][j].value = 0;
            nuevo->cell[i][j].beforeValue = 0;
            nuevo->cell[i][j].progress = 1;
            nuevo->cell[i][j].toF = j;
            nuevo->cell[i][j].toC = i;
        }
    }
    int fila, columna;
    getRandomCellFree(nuevo, &fila, &columna, seed);
    nuevo->cell[columna][fila].value = getRandomNum(nuevo, seed);
    nuevo->cell[columna][fila].toF = fila;
    nuevo->cell[columna][fila].toC = columna;
    nuevo->cell[columna][fila].progress = 1;
    nuevo->score = nuevo->score + nuevo->cell[columna][fila].value;
    return nuevo;
}

int getCellContent(TBoard board, int f, int c) {
    return board->cell[f][c].value;
}

bool getStatusAnimation(TBoard board) {
    return board->statusAnimation;
}

void getRandomCellFree(TBoard board, int *toFila, int *tocolumna, unsigned int *seed) {
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
        *tocolumna = posicionesVacias[indiceAleatorio][0];
        *toFila = posicionesVacias[indiceAleatorio][1];
        // Actualizar la semilla después de generar el número aleatorio
        *seed = rand();
    } else {
        // Si no se encontraron posiciones vacías, se establecen valores fuera del rango del arreglo
        *toFila = -1;
        *tocolumna = -1;
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

void drawBackground(int cellSize, int separation) {
    Rectangle rec;
    rec.height = cellSize * 4 + separation * 5;
    rec.width = cellSize * 4 + separation * 5;
    rec.x = 0;
    rec.y = heightBoard;
    DrawRectangleRounded(rec, 0.1, 4, GRAY);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            //Calula la pocicion de la nueva celda en cada iteracion
            int x = separation * (j + 1) + cellSize * (j);
            int y = separation * (i + 1) + cellSize * (i) + heightBoard;
            //Dibuja la celda y imprime el contenido del tablero
            rec.height = cellSize;
            rec.width = cellSize;
            rec.x = x;
            rec.y = y;
            DrawRectangleRounded(rec, 0.4, 1, BLACK);
        }
    }
}

void drawBoard(TBoard board, int cellSize, int separation) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (getCellContent(board, j, i) != 0) {
                //Calula la pocicion de la nueva celda en cada iteracion
                Rectangle rec;
                rec.height = cellSize;
                rec.width = cellSize;
                rec.x = separation * (j + 1) + cellSize * (j);
                rec.y = separation * (i + 1) + cellSize * (i) + heightBoard;
                // Text size and position
                int fontSize = cellSize;
                std::string texto = std::to_string(getCellContent(board, j, i));
                while (MeasureText(texto.c_str(), fontSize) > cellSize - 10) {
                    fontSize = fontSize - 5;
                }
                int textX = rec.x + (rec.height * 0.5) - MeasureText(texto.c_str(), fontSize) * 0.5;
                int textY = rec.y + (rec.width * 0.5) - (fontSize * 0.5);
                //Dibuja la celda y imprime el contenido del tablero
                DrawRectangleRounded(rec, 0.4, 1, colors[(int)log2(getCellContent(board, j, i))]);
                DrawText(texto.c_str(), textX, textY, fontSize, BLACK);
            }
        }
    }
}

int lerp(int a, int b, float t) {
    return a + int(t * (b - a));
}

void drawAnimation(TBoard board, int cellSize, int separation) {
    board->statusAnimation = false;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            int currentValue;
            // Actualizar la animación
            if (board->cell[i][j].progress < 1.0) {
                currentValue = board->cell[i][j].beforeValue;
                board->cell[i][j].progress += animationSpeed;
                board->statusAnimation = true;
            } else {
                currentValue = board->cell[i][j].value;
            }

            if (currentValue != 0) {
                //Calula la pocicion de la celda
                int fY = separation * (j + 1) + cellSize * (j) + heightBoard;
                int fX = separation * (i + 1) + cellSize * (i);
                int tY = separation * (board->cell[i][j].toF + 1) + cellSize * (board->cell[i][j].toF) + heightBoard;
                int tX = separation * (board->cell[i][j].toC + 1) + cellSize * (board->cell[i][j].toC);

                // Calcular la posición actual en función del progreso
                int currentX = lerp(fX, tX, board->cell[i][j].progress);
                int currentY = lerp(fY, tY, board->cell[i][j].progress);
                Rectangle rec;
                rec.height = cellSize;
                rec.width = cellSize;
                rec.x = currentX;
                rec.y = currentY;

                // Text size and position
                int fontSize = cellSize;
                std::string texto = std::to_string(currentValue);
                while (MeasureText(texto.c_str(), fontSize) > cellSize - 10) {
                    fontSize = fontSize - 5;
                }
                int textX = rec.x + (rec.height * 0.5) - MeasureText(texto.c_str(), fontSize) * 0.5;
                int textY = rec.y + (rec.width * 0.5) - (fontSize * 0.5);

                // Dibujar la celda animada en su posición actual
                DrawRectangleRounded(rec, 0.4, 1, colors[(int)log2(currentValue)]);
                DrawText(texto.c_str(), textX, textY, fontSize, BLACK);
            }

            if (board->cell[i][j].progress >= 1.0) {
                board->cell[i][j].beforeValue = 0;
                board->cell[i][j].toC = i;
                board->cell[i][j].toF = j;
            }        
        }
    }
}

void drawScore(TBoard board, int cellSize, int separation) {
    Rectangle rec;
    rec.height = cellSize;
    rec.width = cellSize * 2;
    rec.x = heightBoard - (cellSize * 2);
    rec.y = cellSize;
    int fontSize = cellSize - (cellSize / 5);
    std::string texto = std::to_string(board->score);
    int textX = rec.x + (rec.width * 0.5) - MeasureText(texto.c_str(), fontSize) * 0.5;
    int textY = rec.y + (rec.height * 0.5) - (fontSize * 0.5);
    DrawRectangleRounded(rec, 0.4, 1, GRAY);
    DrawText(texto.c_str(), textX, textY, fontSize, BLACK);
}

void moveLeft(TBoard board, unsigned int *seed) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int aux = j + 1;
            // Si el valor es 0 va a buscar el proximo valor distinto de 0 y lo va asignar a reasignar. 
            if (board->cell[j][i].value == 0) {
                while (aux < SIZE && board->cell[aux][i].value == board->cell[j][i].value) {
                    aux++;
                }
                if (aux < SIZE) {
                    // Actualizamos los valores para la animacion
                    board->statusAnimation = true;
                    board->cell[j][i].progress = 0;
                    if (board->cell[j][i].beforeValue == 0) {
                        board->cell[j][i].toF = i;
                        board->cell[j][i].toC = j;
                    }
                    board->cell[j][i].value = board->cell[aux][i].value;
                    board->cell[aux][i].progress = 0;
                    if (board->cell[aux][i].beforeValue == 0) {
                        board->cell[aux][i].beforeValue = board->cell[aux][i].value;
                        board->cell[aux][i].toF = i;
                        board->cell[aux][i].toC = j;
                    }
                    board->cell[aux][i].value = 0;
                    aux++;
                }
                while (aux < SIZE && board->cell[aux][i].value == 0) {
                    aux++;
                }
                if (aux < SIZE && board->cell[aux][i].value == board->cell[j][i].value){
                    // Actualizamos los valores para la animacion
                    board->statusAnimation = true;
                    board->cell[j][i].progress = 0;
                    if (board->cell[j][i].beforeValue == 0) {
                        board->cell[j][i].toF = i;
                        board->cell[j][i].toC = j;
                    }
                    board->cell[j][i].value = board->cell[j][i].value * 2;
                    board->cell[aux][i].progress = 0;
                    if (board->cell[aux][i].beforeValue == 0) {
                        board->cell[aux][i].beforeValue = board->cell[aux][i].value;
                        board->cell[aux][i].toF = i;
                        board->cell[aux][i].toC = j;
                    }
                    board->cell[aux][i].value = 0;
                }
            } else { // Si el valor es distinto de 0 va a ver si hay uno identico en lo que queda del arreglo.
                while (aux < SIZE && board->cell[aux][i].value == 0) {
                    aux++;
                }
                if (aux < SIZE && board->cell[aux][i].value == board->cell[j][i].value){
                    // Actualizamos los valores para la animacion
                    board->statusAnimation = true;
                    board->cell[j][i].progress = 0;
                    if (board->cell[j][i].beforeValue == 0) {
                        board->cell[j][i].beforeValue = board->cell[j][i].value;
                        board->cell[j][i].toF = i;
                        board->cell[j][i].toC = j;
                    }
                    board->cell[j][i].value = board->cell[j][i].value * 2;
                    board->cell[aux][i].progress = 0;
                    if (board->cell[aux][i].beforeValue == 0) {
                        board->cell[aux][i].beforeValue = board->cell[aux][i].value;
                        board->cell[aux][i].toF = i;
                        board->cell[aux][i].toC = j;
                    }
                    board->cell[aux][i].value = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila].value = getRandomNum(board, seed);
    board->cell[columna][fila].progress = 0;
    board->score = board->score + board->cell[columna][fila].value;
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
                    board->statusAnimation = true;
                    board->cell[j][i].progress = 0;
                    if (board->cell[j][i].beforeValue == 0) {
                        board->cell[j][i].toF = i;
                        board->cell[j][i].toC = j;
                    }
                    board->cell[j][i].value = board->cell[aux][i].value;
                    board->cell[aux][i].progress = 0;
                    if (board->cell[aux][i].beforeValue == 0) {
                        board->cell[aux][i].beforeValue = board->cell[aux][i].value;
                        board->cell[aux][i].toF = i;
                        board->cell[aux][i].toC = j;
                    }
                    board->cell[aux][i].value = 0;
                    aux--;
                }
                while (aux >= 0 && board->cell[aux][i].value == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[aux][i].value == board->cell[j][i].value){
                    board->statusAnimation = true;
                    board->cell[j][i].progress = 0;
                    board->cell[j][i].value = board->cell[j][i].value * 2;
                    if (board->cell[j][i].beforeValue == 0) {
                        board->cell[j][i].toF = i;
                        board->cell[j][i].toC = j;
                    }
                    board->cell[aux][i].progress = 0;
                    if (board->cell[aux][i].beforeValue == 0) {
                        board->cell[aux][i].beforeValue = board->cell[aux][i].value;
                        board->cell[aux][i].toF = i;
                        board->cell[aux][i].toC = j;
                    }
                    board->cell[aux][i].value = 0;
                }
            } else {
                while (aux >= 0 && board->cell[aux][i].value == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[aux][i].value == board->cell[j][i].value){
                    board->statusAnimation = true;
                    board->cell[j][i].progress = 0;
                    if (board->cell[j][i].beforeValue == 0) {
                        board->cell[j][i].beforeValue = board->cell[j][i].value;
                        board->cell[j][i].toF = i;
                        board->cell[j][i].toC = j;
                    }
                    board->cell[j][i].value = board->cell[j][i].value * 2;
                    board->cell[aux][i].progress = 0;
                    if (board->cell[aux][i].beforeValue == 0) {
                        board->cell[aux][i].beforeValue = board->cell[aux][i].value;
                        board->cell[aux][i].toF = i;
                        board->cell[aux][i].toC = j;
                    }
                    board->cell[aux][i].value = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila].value = getRandomNum(board, seed);
    board->cell[columna][fila].progress = 0;
    board->score = board->score + board->cell[columna][fila].value;
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
                    board->statusAnimation = true;
                    board->cell[i][j].progress = 0;
                    if (board->cell[i][j].beforeValue == 0) {
                        board->cell[i][j].toF = j;
                        board->cell[i][j].toC = i;
                    }
                    board->cell[i][j].value = board->cell[i][aux].value;
                    board->cell[i][aux].progress = 0;
                    if (board->cell[i][aux].beforeValue == 0) {
                        board->cell[i][aux].beforeValue = board->cell[i][aux].value;
                        board->cell[i][aux].toF = j;
                        board->cell[i][aux].toC = i;
                    }
                    board->cell[i][aux].value = 0;
                    aux++;
                }
                while (aux < SIZE && board->cell[i][aux].value == 0) {
                    aux++;
                }
                if (aux < SIZE && board->cell[i][aux].value == board->cell[i][j].value){
                    board->statusAnimation = true;
                    board->cell[i][j].progress = 0;
                    if (board->cell[i][j].beforeValue == 0) {
                        board->cell[i][j].toF = j;
                        board->cell[i][j].toC = i;
                    }
                    board->cell[i][j].value = board->cell[i][aux].value * 2;
                    board->cell[i][aux].progress = 0;
                    if (board->cell[i][aux].beforeValue == 0) {
                        board->cell[i][aux].beforeValue = board->cell[i][aux].value;
                        board->cell[i][aux].toF = j;
                        board->cell[i][aux].toC = i;
                    }
                    board->cell[i][aux].value = 0;
                }
            } else {
                while (aux < SIZE && board->cell[i][aux].value == 0) {
                    aux++;
                }
                if (aux < SIZE && board->cell[i][aux].value == board->cell[i][j].value){
                    board->statusAnimation = true;
                    board->cell[i][j].progress = 0;
                    if (board->cell[i][j].beforeValue == 0) {
                        board->cell[i][j].beforeValue = board->cell[i][j].value;
                        board->cell[i][j].toF = j;
                        board->cell[i][j].toC = i;
                    }
                    board->cell[i][j].value = board->cell[i][aux].value * 2;
                    board->cell[i][aux].progress = 0;
                    if (board->cell[i][aux].beforeValue == 0) {
                        board->cell[i][aux].beforeValue = board->cell[i][aux].value;
                        board->cell[i][aux].toF = j;
                        board->cell[i][aux].toC = i;
                    }
                    board->cell[i][aux].value = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila].value = getRandomNum(board, seed);
    board->cell[columna][fila].progress = 0;
    board->score = board->score + board->cell[columna][fila].value;
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
                    board->statusAnimation = true;
                    board->cell[i][j].progress = 0;
                    if (board->cell[i][j].beforeValue == 0) {
                        board->cell[i][j].toF = j;
                        board->cell[i][j].toC = i;
                    }
                    board->cell[i][j].value = board->cell[i][aux].value;
                    board->cell[i][aux].progress = 0;
                    if (board->cell[i][aux].beforeValue == 0) {
                        board->cell[i][aux].beforeValue = board->cell[i][aux].value;
                        board->cell[i][aux].toF = j;
                        board->cell[i][aux].toC = i;
                    }
                    board->cell[i][aux].value = 0;
                    aux--;
                }
                while (aux >= 0 && board->cell[i][aux].value == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[i][aux].value == board->cell[i][j].value){
                    board->statusAnimation = true;
                    board->cell[i][j].progress = 0;
                    if (board->cell[i][j].beforeValue == 0) {
                        board->cell[i][j].toF = j;
                        board->cell[i][j].toC = i;
                    }
                    board->cell[i][j].value = board->cell[i][aux].value * 2;
                    board->cell[i][aux].progress = 0;
                    if (board->cell[i][aux].beforeValue == 0) {
                        board->cell[i][aux].beforeValue = board->cell[i][aux].value;
                        board->cell[i][aux].toF = j;
                        board->cell[i][aux].toC = i;
                    }
                    board->cell[i][aux].value = 0;
                }
            } else {
                while (aux >= 0 && board->cell[i][aux].value == 0) {
                    aux--;
                }
                if (aux >= 0 && board->cell[i][aux].value == board->cell[i][j].value){
                    board->statusAnimation = true;
                    board->cell[i][j].progress = 0;
                    if (board->cell[i][j].beforeValue == 0) {
                        board->cell[i][j].beforeValue = board->cell[i][j].value;
                        board->cell[i][j].toF = j;
                        board->cell[i][j].toC = i;
                    }
                    board->cell[i][j].value = board->cell[i][aux].value * 2;
                    board->cell[i][aux].progress = 0;
                    if (board->cell[i][aux].beforeValue == 0) {
                        board->cell[i][aux].beforeValue = board->cell[i][aux].value;
                        board->cell[i][aux].toF = j;
                        board->cell[i][aux].toC = i;
                    }
                    board->cell[i][aux].value = 0;
                }
            }
        }
    }
    int fila, columna;
    getRandomCellFree(board, &fila, &columna, seed);
    board->cell[columna][fila].value = getRandomNum(board, seed);
    board->cell[columna][fila].progress = 0;
    board->score = board->score + board->cell[columna][fila].value;
}