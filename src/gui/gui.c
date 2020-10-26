/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

//#include "../../lib/raygui/raygui.h"
#include "gui.h"
#include "gui_constants.h"
#include "game/gui_game.h"
#include "gui_config.h"
#include "../game_handler.h"
#include "gui_utils.h"

#define GAME_X_ANCHOR 800
#define GAME_Y_ANCHOR 0

char some_text[100] = "Some text";

void InitGui(){
    CreateDistanceUnitsList();
    InitGameUI(GAME_X_ANCHOR, GAME_Y_ANCHOR);
    InitConfigUI(0, 0);
}

void StartGui() {
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Transverse");
    InitGui();
    while (!WindowShouldClose()) {
        // Wait((u_time)GetFrameTime()); // TODO make sure cast is ok
        BeginDrawing();
        {
            float last_frame_time = GetFrameTime();
            Wait(last_frame_time);
            ClearBackground(RAYWHITE);
            DrawLineEx((Vector2) {800, 0}, (Vector2) {800, 600}, 1, LIGHTGRAY);
            DrawConfigPanel(0, 0);
            DrawGame();
        }
        EndDrawing();
    }
    CloseWindow();
}

