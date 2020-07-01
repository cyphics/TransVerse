/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */
#include <raylib.h>
#include <stb/stb.h>

#include "gui_config.cpp"
#include "gui_game.cpp"

const int screenWidth = 1600;
const int screenHeight = 600;

// Hover animation
float gradWidth = 30.0f;
float gradHeight = 30.0f;
int hoverLength = 10; // In frames
Rectangle staticRect = {0, 0, gradWidth, gradHeight};
Texture2D gradientTexture;
Color hovered = LIGHTGRAY;
Color unhovered = RAYWHITE;

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126

void SartGui()
{

    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    Image grad = GenImageGradientH(gradWidth, gradHeight, hovered, unhovered);
    gradientTexture = LoadTextureFromImage(grad);
    UnloadImage(grad);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {

            ClearBackground(RAYWHITE);
            DrawConfigPanel(screenWidth, screenHeight);
            DrawLineEx((Vector2){800, 0}, (Vector2){800, 600}, 1, LIGHTGRAY);
            DrawGame();
        }
        EndDrawing();
    }
    CloseWindow();
}

void DrawAnimationLayer(Interact *i)
{
    float dist = (i->rect.x + i->rect.width) - (i->rect.x - gradWidth);
    float offset = dist / hoverLength * i->hoverIdx;

    Rectangle colorIn = {i->rect.x - i->rect.width - gradWidth + offset, i->rect.y,
                         i->rect.width + gradWidth, i->rect.height};
    Rectangle colorInDrawn = GetCollisionRec(colorIn, i->rect);


    Rectangle colorOut = {i->rect.x + offset, i->rect.y,
                          i->rect.width, i->rect.height};
    Rectangle colorOutDrawn = GetCollisionRec(colorOut, i->rect);

    DrawRectangleRec(colorInDrawn, hovered);
    DrawRectangleRec(colorOutDrawn, unhovered);

    float rectX, rectWidth;
    float rectXBase = i->rect.x + 50;

    Rectangle movingRectangle = {i->rect.x - gradWidth + offset, i->rect.y,
                                 gradWidth, gradHeight};
    Rectangle intersect = GetCollisionRec(i->rect, movingRectangle);

    // DrawRectangleLinesEx(movingRectangle, 1, BLACK);

    Rectangle sourceRect = {rectX, i->rect.y + gradHeight + 20, rectWidth, gradHeight};

    DrawTexturePro(gradientTexture, staticRect, intersect, Vector2(), 0, WHITE);
    // DrawRectangleLinesEx(movingRectangle, 1, BLACK);
}

void DrawInteract(Interact *i)
{
    if (i->isHovered) {
        i->hoverIdx++;
        if (i->hoverIdx > hoverLength) {
            i->hoverIdx = hoverLength;
        }
        DrawRectangleRec(i->rect, LIGHTGRAY);
    }
    if (!i->isHovered) {
        i->hoverIdx--;
        if (i->hoverIdx < 0) {
            i->hoverIdx = 0;
        }
    }
    DrawAnimationLayer(i);
    DrawRectangleLinesEx(i->rect, 1, BLACK);
    DrawText(i->title, i->rect.x + padding, i->rect.y + padding/3, fontSize / 2, GRAY);
    DrawText(i->text, i->rect.x + padding, i->rect.y + padding * 2, fontSize, MAROON);
}
