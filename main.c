#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "raylib.h"

#define COLS      20
#define ROWS      20
#define NUM_CELLS COLS*ROWS
#define CELL_SIZE 30

#define ALIVE     1
#define DEAD      0

int cells[NUM_CELLS];

void init_glider() {
    memset(cells, DEAD, NUM_CELLS);
    cells[COLS*1 + 4] = ALIVE;
    cells[COLS*2 + 5] = ALIVE;
    cells[COLS*3 + 3] = ALIVE;
    cells[COLS*3 + 4] = ALIVE;
    cells[COLS*3 + 5] = ALIVE;
}

void next_state() {
    int next_cells[NUM_CELLS];
    memcpy(next_cells, cells, sizeof(cells));

    for (size_t i=1; i < ROWS-1; ++i) {
        for (size_t j=1; j < COLS-1; ++j) {
            size_t k = j + i*COLS;
            int neighbours = 0;

            if (cells[k + 1 + COLS] == ALIVE) neighbours++;
            if (cells[k + 1] == ALIVE)        neighbours++;
            if (cells[k + 1 - COLS] == ALIVE) neighbours++;
            if (cells[k + 0 + COLS] == ALIVE) neighbours++;
            if (cells[k + 0 - COLS] == ALIVE) neighbours++;
            if (cells[k - 1 + COLS] == ALIVE) neighbours++;
            if (cells[k - 1] == ALIVE)        neighbours++;
            if (cells[k - 1 - COLS] == ALIVE) neighbours++;

            if (cells[k] == ALIVE) {
                if (neighbours < 2 || neighbours > 3) next_cells[k] = DEAD;
            }
            if (cells[k] == DEAD) {
                if (neighbours == 3) next_cells[k] = ALIVE;
            }
       }
    }

    memcpy(cells, next_cells, sizeof(cells));
}

void draw_state() {
    for (size_t i=0; i < ROWS; ++i) {
        for (size_t j=0; j < COLS; ++j) {
            if (cells[j + i*COLS]) {
                int startX = i*CELL_SIZE + 1;
                int startY = j*CELL_SIZE + 1;
                int width  = CELL_SIZE - 3;
                int height = CELL_SIZE - 3;
                DrawRectangle(startX, startY, width, height, WHITE);
            }
        }
    }
}

void draw_grid() {
    for (size_t i=0; i < ROWS; ++i) {
        DrawLine(i*CELL_SIZE, 0, i*CELL_SIZE, COLS*CELL_SIZE, GRAY);
    }
    for (size_t j=0; j < COLS; ++j) {
        DrawLine(0, j*CELL_SIZE, ROWS*CELL_SIZE, j*CELL_SIZE, GRAY);
    }
}

int main(void) {
    init_glider();

    InitWindow(COLS*CELL_SIZE, ROWS*CELL_SIZE, "Glider in Raylib!");

    while (!WindowShouldClose()) {
        BeginDrawing();
          ClearBackground(BLACK);

          draw_grid();
          draw_state();
        EndDrawing();

        usleep(500*1000);
        next_state();
    }

    CloseWindow();

    return 0;
}
