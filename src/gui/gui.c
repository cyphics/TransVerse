/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include "gui.h"
#include "gui_constants.h"
#include "game/gui_game.h"
#include "gui_config.h"
#include "../game_handler.h"
#include "gui_utils.h"

void InitGui(){
    Vector2 config_panel_anchor = {0, 0};
    Vector2 game_panel_anchor = {600, 0};
    CreateDistanceUnitsList();
    InitGameUI(game_panel_anchor);
    InitConfigUI(config_panel_anchor);
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
            DrawConfigPanel();
            DrawGame();
        }
        EndDrawing();
    }
    CloseWindow();
}
