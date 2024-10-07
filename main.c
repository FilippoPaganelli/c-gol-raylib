#include <stdio.h>
#include <string.h>
#include <math.h>
#include "raylib.h"

#define COLS          20
#define ROWS          20
#define NUM_CELLS     COLS*ROWS
#define CELL_SIZE     35
#define GRID_PADDING  10
#define PANEL_HEIGHT  175
#define SCREEN_WIDTH  COLS*CELL_SIZE + 2*GRID_PADDING
#define SCREEN_HEIGHT ROWS*CELL_SIZE + 2*GRID_PADDING + PANEL_HEIGHT

#define ALIVE         true
#define DEAD          false

#define FONT_SIZE     22

#define MODE_DRAW     (1 << 0)  // 0001 - Draw
#define MODE_PLAY     (1 << 1)  // 0010 - Play

// Keeps the current cells state
bool cells[NUM_CELLS];

void init_glider() {
    cells[COLS*1 + 4] = ALIVE;
    cells[COLS*2 + 5] = ALIVE;
    cells[COLS*3 + 3] = ALIVE;
    cells[COLS*3 + 4] = ALIVE;
    cells[COLS*3 + 5] = ALIVE;
}

void next_state() {
    bool next_cells[NUM_CELLS];
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

                DrawRectangle(startX + GRID_PADDING, startY + GRID_PADDING + PANEL_HEIGHT,
                              width, height,
                              RAYWHITE);
            }
        }
    }
}

void draw_grid() {
    for (size_t i=0; i <= ROWS; ++i) {
        DrawLine(i*CELL_SIZE + GRID_PADDING, 0 + GRID_PADDING + PANEL_HEIGHT,
                 i*CELL_SIZE + GRID_PADDING, COLS*CELL_SIZE + GRID_PADDING + PANEL_HEIGHT,
                 GRAY);
    }
    for (size_t j=0; j <= COLS; ++j) {
        DrawLine(0 + GRID_PADDING, j*CELL_SIZE + GRID_PADDING + PANEL_HEIGHT,
                 ROWS*CELL_SIZE + GRID_PADDING, j*CELL_SIZE + GRID_PADDING + PANEL_HEIGHT,
                 GRAY);
    }
}

void handle_drawing_mode() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int x = GetMouseX();
        int y = GetMouseY();

        if (GRID_PADDING < x && x < (GRID_PADDING + COLS*CELL_SIZE) &&
            (GRID_PADDING + PANEL_HEIGHT) < y && y < (GRID_PADDING + PANEL_HEIGHT + ROWS*CELL_SIZE)) {
            size_t col = ceil((x - GRID_PADDING) / CELL_SIZE);
            size_t row = ceil((y - GRID_PADDING - PANEL_HEIGHT) / CELL_SIZE);
            size_t index = row + col*COLS;
            cells[index] = !cells[index];
        }
    }
}

void show_current_mode (int modes) {
    char text[50];
    if (modes & MODE_DRAW) sprintf(text, "Current mode: < DRAW >");
    else if (modes & MODE_PLAY) sprintf(text, "Current mode: < PLAY >");
    else sprintf(text, "Current mode: < --- >");

    DrawText(text, 10, 10 + 4*(13 + FONT_SIZE), FONT_SIZE, SKYBLUE);
}

int main(void) {
    int modes = 0; // Modes bitmask

    memset(cells, DEAD, sizeof(bool) * NUM_CELLS);
    init_glider();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life in Raylib!");
    SetTargetFPS(10);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_D)) { modes &= ~MODE_PLAY; modes |= MODE_DRAW; }
        if (IsKeyDown(KEY_P)) { modes &= ~MODE_DRAW; modes |= MODE_PLAY; }
        if (IsKeyDown(KEY_R)) { modes = 0; memset(cells, DEAD, sizeof(bool) * NUM_CELLS); }

        BeginDrawing();
          ClearBackground(BLACK);

          // DrawTextEx(customFont, "This should be Iosevka...", (Vector2){ 10.0f, 10.0f }, FONT_SIZE, 3, YELLOW);
          DrawText("This is a demo of Conway's Game of Life with Raylib!", 10, 10 + 0*(13 + FONT_SIZE), FONT_SIZE, RAYWHITE);
          DrawText("Press (d) to enter DRAW mode and design the state.",   10, 10 + 1*(13 + FONT_SIZE), FONT_SIZE, GOLD);
          DrawText("Press (r) to RESET the grid to an empty state.",       10, 10 + 2*(13 + FONT_SIZE), FONT_SIZE, GOLD);
          DrawText("Press (p) to PLAY from the current state.",            10, 10 + 3*(13 + FONT_SIZE), FONT_SIZE, GOLD);

          if (modes & MODE_DRAW) {
              handle_drawing_mode();
          }
          else {
              next_state();
          }

          show_current_mode(modes);
          draw_grid();
          draw_state();
        EndDrawing();
    }

    //UnloadFont(customFont);
    CloseWindow();

    return 0;
}
