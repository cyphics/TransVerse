/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */
#include <raylib.h>

#include "helper.h"
#include "game_handler.h"

extern char *CONFIG_FILE;

// extern GameState state;
Font font;
const int numUpgrades = MAX_UPGRADES_AMOUNT;

struct TextRect {
    char *id;
    Rectangle rect;
    char* text;
};

// Common
float scaleFactor = 1;
int framesCounter = 0;
float maxUpgradeNameLength = 250 * scaleFactor;
float padding = 5 * scaleFactor;
float fontSize = 20.0 * scaleFactor;
float xAnchor = 20 * scaleFactor, yAnchor = 20 * scaleFactor;
float textRectHeight = 30 * scaleFactor;
char currentUpgradeIncreaseString[6];
bool mouseOnText = false;

// Upgrade selection panel

float selectUpgradeRectWidth = maxUpgradeNameLength;
float selectMainRectWidth = selectUpgradeRectWidth + 2 * padding;
float selectMainRectHeight = textRectHeight * numUpgrades + padding * (numUpgrades + 1);

Rectangle selectMainRect = {xAnchor, yAnchor,
                            selectMainRectWidth, selectMainRectHeight};

Rectangle selectRectList[numUpgrades] = {};

// Upgrade edit panel
upgrade *displayCurrentUpgrade;

float displayRectXAnchor = xAnchor + maxUpgradeNameLength + 20 * scaleFactor;
float displayRectYAnchor = yAnchor;
Rectangle displayNameRect = {displayRectXAnchor + padding,
                             displayRectYAnchor + padding,
                             selectMainRectWidth, 30};
Rectangle displayTypeRect = {displayNameRect.x + displayNameRect.width + padding,
                             displayRectYAnchor + padding, 120 * scaleFactor, 30 * scaleFactor};
Rectangle displayIncreaseRect = {displayTypeRect.x + displayTypeRect.width + padding,
                                 displayRectYAnchor + padding, 50 * scaleFactor, 30 * scaleFactor};
Rectangle displayDescRect = {displayRectXAnchor + padding, displayRectYAnchor + padding + 40 * scaleFactor,
                             displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 2 * padding, 60 * scaleFactor};

float displayMainRectWidth = displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 4 * padding;
float displayMainRectHeight = 300.0f * scaleFactor;
Rectangle displayMainRect = {displayRectXAnchor, displayRectYAnchor,
                             displayMainRectWidth, displayMainRectHeight};
Rectangle displayDependenciesRect = {};
Rectangle displayPriceRect = {};
Rectangle displaySaveRect = {displayRectXAnchor, displayRectYAnchor + displayMainRectWidth + padding,
                             100, 30};

TextRect nameRect = {"name", displayNameRect};
TextRect typeRect = {"type", displayTypeRect};
TextRect incrRect = {"inc", displayIncreaseRect, currentUpgradeIncreaseString};
TextRect descRect = {"desc", displayDescRect};

TextRect *displayRectList[] = { &nameRect, &typeRect, &incrRect, &descRect};
char newTypeReceiver[30] = {};
char *typesList[] = {"science", "incremental", "knowledge", "structure"};

struct SelectMenu {
    char *destination;
    char **items_list;
    int num_items;
    Rectangle rect;
    bool visible = false;
} SelectMenu;

void DrawSelectionMenu()
{
    Color c;
    if (SelectMenu.visible) {
        for (int i = 0; i < SelectMenu.num_items; ++i) {
            Rectangle r = {SelectMenu.rect.x, SelectMenu.rect.y + 30 * i, SelectMenu.rect.width, textRectHeight};
            if (CheckCollisionPointRec(GetMousePosition(), r)) c = WHITE;
            else c = LIGHTGRAY;
            DrawRectangleRec(r, c);
            DrawText(SelectMenu.items_list[i],
                     SelectMenu.rect.x + padding, SelectMenu.rect.y + textRectHeight * i + padding,
                     fontSize, MAROON);

        }
        DrawRectangleLinesEx(SelectMenu.rect, 1, DARKGRAY);
    }
}

void OpenSelectMenu(char *destination, char **items_list, int num_items, float anchorX, float anchorY)
{
    // Set width of selector
    float width = 0;
    float length = 0;
    for (int i = 0; i < num_items; ++i) {
        length = MeasureText(items_list[i], fontSize);
        if (length > width) width = length;
    }
    width += padding * 2;

    SelectMenu.destination = destination;
    SelectMenu.items_list = items_list;
    SelectMenu.num_items = num_items;
    SelectMenu.rect = (Rectangle){anchorX, anchorY, width, textRectHeight * num_items};
    SelectMenu.visible = true;
}

void DrawTextRect(TextRect tr)
{
    DrawRectangleLinesEx(tr.rect, 1, BLACK);
    DrawText(tr.text, tr.rect.x + padding, tr.rect.y + padding, fontSize, MAROON);
}

void SelectCurrentUpgrade(upgrade *original)
{
    displayCurrentUpgrade = original;
    sprintf(currentUpgradeIncreaseString, "%.2f", displayCurrentUpgrade->increase_factor);
    nameRect.text = original->id;
    typeRect.text = original->type;
    descRect.text = original->description;

}

void DrawSelectUpgradeMenu()
{
    int i;

    DrawRectangleLinesEx(selectMainRect, 2, BLACK);

    for (i = 0; i < numUpgrades; ++i) {
        Rectangle rec = selectRectList[i];
        if (CheckCollisionPointRec(GetMousePosition(), rec)) {
            DrawRectangleRec(rec, LIGHTGRAY);
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                SelectCurrentUpgrade(&state.upgrades_list[i]);
            }
        }
        DrawRectangleLinesEx(selectRectList[i], 1, BLACK);
        upgrade cur = state.upgrades_list[i];
        DrawTextRec(font, cur.id,
                    (Rectangle){rec.x + padding, rec.y + padding, rec.width, rec.height},
                    fontSize, 2.0f, true, MAROON);
    }
}

void DrawDisplayUpgrade()
{
    // Save button
    if (CheckCollisionPointRec(GetMousePosition(), displaySaveRect)) {
        DrawRectangleRec(displaySaveRect, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SaveGame();
        }
    }

    DrawRectangleLinesEx(displayMainRect, 2, BLACK);

    for (int i = 0; i < (sizeof(displayRectList) / sizeof(displayRectList[0])); ++i) {

        TextRect tr = *displayRectList[i];


        if (CheckCollisionPointRec(GetMousePosition(), tr.rect)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText)
        {
            if (AreStrEquals(tr.id, "name") ||
                AreStrEquals(tr.id, "inc") ||
                AreStrEquals(tr.id, "desc"))
            {
                framesCounter++;
                int cursorPos = strlen(tr.text);
                int key = GetKeyPressed();
                while(key > 0)
                {
                    if ((key >= 32) && (key <= 125)) {
                        tr.text[cursorPos] = (char)key;
                        tr.text[cursorPos + 1] = '\0';
                    }
                    key = GetKeyPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    tr.text[cursorPos - 1] = '\0';
                }
                if (((framesCounter/20)%2) == 0)
                    DrawText("_", tr.rect.x + 6 + MeasureText(tr.text, fontSize),
                             tr.rect.y + 8, fontSize, MAROON);
                if (framesCounter > 200) framesCounter = 0;

            } else if (AreStrEquals(tr.id, "type")) {
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    OpenSelectMenu(newTypeReceiver,
                                   typesList, sizeof(typesList) / sizeof(typesList[0]),
                                   tr.rect.x, tr.rect.y + tr.rect.height);
                }
            }

        }

        DrawTextRect(tr);
    }

    DrawRectangleLinesEx(displaySaveRect, 2, BLACK);
    DrawText("Save", displaySaveRect.x + padding, displaySaveRect.y + padding, fontSize, MAROON);
    DrawSelectionMenu();
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        !CheckCollisionPointRec(GetMousePosition(), SelectMenu.rect)) {
        SelectMenu.visible = false;
    }
}

void DrawEditMode(float panelWidth, float panelHeight)
{


    for (int i = 0; i < numUpgrades; ++i) {
        selectRectList[i] = {xAnchor + padding,
                             yAnchor + padding +  (textRectHeight + padding) * i,
                             selectUpgradeRectWidth, textRectHeight};
    }

    SelectCurrentUpgrade(&state.upgrades_list[0]);


    font = GetFontDefault();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawSelectUpgradeMenu();
            DrawDisplayUpgrade();
        }
        EndDrawing();
    }

}
