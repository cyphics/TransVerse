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
const float gradWidth = 30.0f;
const float gradHeight = 30.0f;
const int hoverLength = 6; // In frames
Rectangle staticRect = {0, 0, gradWidth, gradHeight};
Texture2D gradientTexture;
Color hovered = LIGHTGRAY;
Color unhovered = RAYWHITE;

int framesCounter = 0;

Vector2 mousePosition;
int mouseGesture;
int keyPressed;
bool mouseLeftPressed;
bool mouseLeftReleased;
bool mouseRightPressed;
bool mouseRightReleased;

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126

void SartGui()
{

    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "Transverse");
    Image grad = GenImageGradientH(gradWidth, gradHeight, hovered, unhovered);
    gradientTexture = LoadTextureFromImage(grad);
    UnloadImage(grad);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            // Store input data
            keyPressed = GetKeyPressed();
            mousePosition = GetMousePosition();
            mouseGesture = GetGestureDetected();
            mouseLeftPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            mouseLeftReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
            mouseRightPressed = IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
            mouseRightReleased = IsMouseButtonReleased(MOUSE_RIGHT_BUTTON);

            ClearBackground(RAYWHITE);
            DrawLineEx((Vector2){800, 0}, (Vector2){800, 600}, 1, LIGHTGRAY);
            DrawConfigPanel();
            DrawGame();
        }
        EndDrawing();
    }
    CloseWindow();
}

void DrawHoverAnimLayer(Interact *i)
{
    if (i->isHoverable) {
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

        DrawTexturePro(gradientTexture, staticRect, intersect, (Vector2){}, 0, WHITE); // TODO check if the vector2 is correct
        // DrawRectangleLinesEx(movingRectangle, 1, BLACK);
    }
    if (i->isEditable && i->isHovered) {
        framesCounter++;


        if (((framesCounter/20)%2) == 0)
            DrawText("_", i->rect.x + 6 + MeasureText(i->text, fontSize),
                     i->rect.y + 8, fontSize, MAROON);
        if (framesCounter > 200) framesCounter = 0;


    }

}

// void RescaleInteract(Interact *i, float scale)
// {
//     i->rect.height *= scale;
//     i->rect.width *= scale;
//     i->fontSize *= scale;
// }

void DrawInteract(Interact *i)
{
    if (i->isHovered) {
        i->hoverIdx++;
        if (i->hoverIdx > hoverLength) {
            i->hoverIdx = hoverLength;
        }
        // DrawRectangleRec(i->rect, LIGHTGRAY);
    }
    if (!i->isHovered) {
        i->hoverIdx--;
        if (i->hoverIdx < 0) {
            i->hoverIdx = 0;
        }
    }
    DrawHoverAnimLayer(i);
    DrawRectangleLinesEx(i->rect, 1, BLACK);
    DrawText(i->title, i->rect.x + padding, i->rect.y, i->fontSize / 2, GRAY);
    DrawText(i->text, i->rect.x + padding, i->rect.y + i->rect.height / 4, i->fontSize, MAROON);
}
