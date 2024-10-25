#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include "raylib.h"

#define COLS          20
#define ROWS          20
#define NUM_CELLS     (COLS*ROWS)
#define CELL_SIZE     35
#define GRID_PADDING  10
#define PANEL_HEIGHT  175
#define SCREEN_WIDTH  (COLS*CELL_SIZE + 2*GRID_PADDING)
#define SCREEN_HEIGHT (ROWS*CELL_SIZE + 2*GRID_PADDING + PANEL_HEIGHT)

#define FONT_SIZE            22
#define NOTIFICATION_TIMEOUT 3.0f
#define NOTIF_TEXT_LENGTH    128

#define MODE_DRAW  (1 << 0)  // 0001 - Draw
#define MODE_PLAY  (1 << 1)  // 0010 - Play
#define MODE_NOTIF (1 << 2)  // 0100 - Notification

#define ALIVE true
#define DEAD  false

// Keeps the current cells state
bool cells[NUM_CELLS] = {0};
// Keeps the state to rewind to
bool rewind_cells[NUM_CELLS] = {0};

// Modes bitmask
int modes = 0;

void init_glider() {
    cells[COLS*1 + 4] = ALIVE;
    cells[COLS*2 + 5] = ALIVE;
    cells[COLS*3 + 3] = ALIVE;
    cells[COLS*3 + 4] = ALIVE;
    cells[COLS*3 + 5] = ALIVE;
}

void random_state() {
    srand(time(NULL));
    for (size_t i = 0; i < NUM_CELLS; ++i) {
        int rand_value = (rand()%3)/2; // Reduce likelihood of getting ALIVE cells
        cells[i] = (bool) rand_value;
    }
    push_notification("Generated random state");
}

void next_state() {
    bool next_cells[(COLS + 2)*(ROWS + 2)] = {0};
    
    // Copy cells into next_cells with padding
    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLS; ++j) {
            size_t src_index = j + i * COLS;
            size_t dest_index = (j + 1) + (i + 1) * (COLS + 2);
            next_cells[dest_index] = cells[src_index];
        }
    }

    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLS; ++j) {
            size_t cell_index = j + i * COLS;
            size_t padded_index = (j + 1) + (i + 1) * (COLS + 2);
            
            int neighbours = 0;
            
            // Count neighbours using padded indices
            neighbours += next_cells[padded_index - (COLS + 2) - 1]; // Top-left
            neighbours += next_cells[padded_index - (COLS + 2)];     // Top
            neighbours += next_cells[padded_index - (COLS + 2) + 1]; // Top-right
            neighbours += next_cells[padded_index - 1];              // Left
            neighbours += next_cells[padded_index + 1];              // Right
            neighbours += next_cells[padded_index + (COLS + 2) - 1]; // Bottom-left
            neighbours += next_cells[padded_index + (COLS + 2)];     // Bottom
            neighbours += next_cells[padded_index + (COLS + 2) + 1]; // Bottom-right
            
            // Apply Game of Life rules
            if (next_cells[padded_index] == ALIVE) {
                cells[cell_index] = (neighbours == 2 || neighbours == 3) ? ALIVE : DEAD;
            } else {
                cells[cell_index] = (neighbours == 3) ? ALIVE : DEAD;
            }
        }
    }
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

void handle_draw_mode() {
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

const char *get_mode_text (int modes) {
    static char text[10];

    if (modes & MODE_DRAW) sprintf(text, "< DRAW >");
    else if (modes & MODE_PLAY) sprintf(text, "< PLAY >");
    else sprintf(text, "< --- >");

    return text;
}

// Notifications logic
double start_time = 0;
char notification_text[NOTIF_TEXT_LENGTH];
void show_notification() {
    const double time_passed = GetTime() - start_time;
    if (time_passed >= NOTIFICATION_TIMEOUT) {
        start_time = GetTime();
        modes &= ~MODE_NOTIF;
        return;
    }

    float alpha = 1;
    Color color_bg = DARKGRAY;
    Color color_fg = SKYBLUE;
    // Fade in at the beginning
    if (time_passed <= NOTIFICATION_TIMEOUT*0.1f) {
        alpha = (0 + time_passed)/(NOTIFICATION_TIMEOUT*0.1f);
        color_bg = Fade(DARKGRAY, alpha);
        color_fg = Fade(SKYBLUE, alpha);
    }
    // Fade out at the end
    else if (time_passed >= NOTIFICATION_TIMEOUT*0.8f) {
        alpha = (NOTIFICATION_TIMEOUT - time_passed)/(NOTIFICATION_TIMEOUT - NOTIFICATION_TIMEOUT*0.8f);
        color_bg = Fade(DARKGRAY, alpha);
        color_fg = Fade(SKYBLUE, alpha);
    }

    int msg_length = MeasureText(notification_text, FONT_SIZE);
    Rectangle rec = {
        .x = SCREEN_WIDTH - msg_length - 20 - GRID_PADDING,
        .y = 10 + 4*(13 + FONT_SIZE) - 10/2,
        .width = 20 + msg_length,
        .height = FONT_SIZE + 10
    };
    DrawRectangleRounded(rec, 0.3f, 0, color_bg);
    DrawText(notification_text, SCREEN_WIDTH - msg_length - 20 - GRID_PADDING + 20/2, 10 + 4*(13 + FONT_SIZE), FONT_SIZE, color_fg);
}

void push_notification(const char *text) {
    sprintf(notification_text, "%s", text);
    modes |= MODE_NOTIF;
    start_time = GetTime();
}

void save_to_file() {
    const char *output_file = "./state.gol";
    TraceLog(LOG_INFO, "Saving current state to file: %s", output_file);

    FILE *f = fopen(output_file, "w");
    if (f == NULL) {
        TraceLog(LOG_ERROR, "Could not open file: %s", output_file);
        push_notification("Error saving to file");
        return;
    }

    if (fwrite(cells, sizeof(bool), NUM_CELLS, f) < 0) {
        int err = errno;
        TraceLog(LOG_ERROR, "Could not write to file: %s. Cause: %s", output_file, strerror(errno));
        push_notification("Error saving to file");
    }

    TraceLog(LOG_INFO, "Saved current state to file: %s", output_file);
    push_notification("Saved state to file");
    fclose(f);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            TraceLog(LOG_INFO, "Usage: %s [preset file]", argv[0]);
            return 0;
        }

        const char *preset_file = argv[1];
        TraceLog(LOG_INFO, "Loading preset file: %s", preset_file);
        
        FILE *f = fopen(preset_file, "r");
        if (f == NULL) {
            TraceLog(LOG_ERROR, "Could not open file: %s", preset_file);
            TraceLog(LOG_INFO, "Starting app with default Glider instead");
            push_notification("Error loading state from file");
            init_glider();
        }
        else {
            fread(cells, sizeof(bool), NUM_CELLS, f);
            TraceLog(LOG_INFO, "Loaded state from file: %s", preset_file);
            push_notification("Loaded state from file");
            fclose(f);
        }
    }
    else {
        init_glider();
        push_notification("Loaded Glider");
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life with Raylib!");
    SetTargetFPS(10);

    // Event loop
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_D)) {
            modes &= ~MODE_PLAY; modes |= MODE_DRAW;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            if (modes & MODE_PLAY) {
                modes &= ~MODE_PLAY; modes |= MODE_DRAW;
            }
            else {
                modes &= ~MODE_DRAW; modes |= MODE_PLAY;
                // Take a snapshot of the current state for 'rewind'
                memcpy(rewind_cells, cells, sizeof(cells));
            }
        }
        if (IsKeyPressed(KEY_R)) {
            modes = 0; memset(cells, DEAD, sizeof(bool) * NUM_CELLS);
        }
        if (IsKeyPressed(KEY_W)) {
            modes = 0; memcpy(cells, rewind_cells, sizeof(rewind_cells));
            push_notification("Rewinded to previous state");
        }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_S)) {
            modes = 0;
            save_to_file();
        }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_R)) {
            modes = 0;
            random_state();
        }
        const char *mode_text = get_mode_text(modes);

        BeginDrawing();
          ClearBackground(BLACK);

          DrawText("This is a demo of Conway's Game of Life with Raylib!",     10, 10 + 0*(13 + FONT_SIZE), FONT_SIZE, RAYWHITE);
          DrawText("( SPACE )  PLAY/PAUSE & save state,    ( d )  DRAW mode.", 10, 10 + 1*(13 + FONT_SIZE), FONT_SIZE, GOLD);
          DrawText("( w )  REWIND to saved state,    ( r )  RESET canvas.",    10, 10 + 2*(13 + FONT_SIZE), FONT_SIZE, GOLD);
          DrawText("( R )  Random state,    ( S )  SAVE canvas to file.",      10, 10 + 3*(13 + FONT_SIZE), FONT_SIZE, GOLD);
          DrawText(mode_text,                                                  10, 10 + 4*(13 + FONT_SIZE), FONT_SIZE, SKYBLUE);

          draw_grid();
          draw_state();

          if (modes & MODE_DRAW) {
              handle_draw_mode();
          }
          else if (modes & MODE_PLAY) {
              next_state();
          }

          // Handle notifications
          if (modes & MODE_NOTIF) {
              show_notification();
          }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
