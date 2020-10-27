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

#define GAME_X_ANCHOR 600
#define GAME_Y_ANCHOR 0

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
        float last_frame_time = GetFrameTime();
        Wait(last_frame_time);
        BeginDrawing();
        {

            ClearBackground(RAYWHITE);
            DrawLineEx((Vector2) {600, 0}, (Vector2) {600, 600}, 1, LIGHTGRAY);
            DrawConfigPanel(0, 0);
            DrawGame();
        }
        EndDrawing();
    }
    CloseWindow();
}

