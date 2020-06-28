/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */
#include <raylib.h>
#include <stb.h>

#include "edit_mode.h"
#include "gui_code.cpp"
#include "gui_config.cpp"

const int screenWidth = 800;
const int screenHeight = 600;

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126

void SartGui()
{
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    DrawEditMode(screenWidth, screenHeight);
    // DrawConfigMenu();
    // TypeCode();
    CloseWindow();
}
