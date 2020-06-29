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
    char id[20];
    Rectangle rect;
    char text[200];
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

bool isDragging = false;
TextRect draggedRect;
upgrade *dragedUpgrade;


// Upgrade selection panel

float selectUpgradeRectWidth = maxUpgradeNameLength;
float selectMainRectWidth = selectUpgradeRectWidth + 2 * padding;
float selectMainRectHeight = textRectHeight * numUpgrades + padding * (numUpgrades + 1);

Rectangle selectMainRect = {xAnchor, yAnchor,
                            selectMainRectWidth, selectMainRectHeight};

Rectangle selectRectList[numUpgrades] = {};
bool isAboutToSelect = false;

// Upgrade edit panel
upgrade *displayCurrentUpgrade;

float displayRectXAnchor = xAnchor + maxUpgradeNameLength + 20 * scaleFactor;
float displayRectYAnchor = yAnchor;
Rectangle displayNameRect = {displayRectXAnchor + padding,
                             displayRectYAnchor + padding,
                             selectMainRectWidth, 30};
Rectangle displayTypeRect = {displayNameRect.x + displayNameRect.width + padding,
                             displayRectYAnchor + padding, 140 * scaleFactor, textRectHeight * scaleFactor};
Rectangle displayIncreaseRect = {displayTypeRect.x + displayTypeRect.width + padding,
                                 displayRectYAnchor + padding, 50 * scaleFactor, 30 * scaleFactor};
Rectangle displayDescRect = {displayRectXAnchor + padding, displayRectYAnchor + padding + 40 * scaleFactor,
                             displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 2 * padding, 60 * scaleFactor};
Rectangle displayDependRect = {displayRectXAnchor + padding,
                               displayDescRect.y + displayDescRect.height + padding,
                               displayNameRect.width, textRectHeight};

float displayMainRectWidth = displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 4 * padding;
float displayMainRectHeight = 300.0f * scaleFactor;
Rectangle displayMainRect = {displayRectXAnchor, displayRectYAnchor,
                             displayMainRectWidth, displayMainRectHeight};
Rectangle displayDependenciesRect = {};
Rectangle displayPriceRect = {};
Rectangle saveRect = {displayRectXAnchor, displayRectYAnchor + displayMainRectWidth + padding,
                      100, 30};
bool isAboutToSave = false;

TextRect nameRect = {"name", displayNameRect};
TextRect typeRect = {"type", displayTypeRect};
TextRect incrRect = {"increase", displayIncreaseRect};
TextRect descRect = {"description", displayDescRect};
TextRect dependRect = {"dependencies", displayDependRect};
char depsList[MAX_DEPENDENCIES * 35];

char depend_list[MAX_DEPENDENCIES][30];
int depend_list_idx = 0;
bool isAboutToRemoveDependency = false;

TextRect *displayRectList[] = { &nameRect, &typeRect, &incrRect, &descRect, &dependRect};
char newTypeReceiver[30] = {};
char *typesList[] = {"science", "incremental", "knowledge", "structure", "software"};

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
    DrawText(tr.id, tr.rect.x + padding, tr.rect.y + padding/3, fontSize / 2, GRAY);
    DrawText(tr.text, tr.rect.x + padding, tr.rect.y + padding * 2, fontSize, MAROON);
}

void GenerateDependencyListString()
{
    memset(depsList, 0, sizeof(depsList));
    for (int i = 0; i < depend_list_idx; ++i) {
        strcat(depsList, depend_list[i]);
        strcat(depsList, "\n");
    }
    memcpy(dependRect.text, depsList, sizeof(depsList));
    dependRect.rect.height = (textRectHeight + padding) * depend_list_idx;
    if (dependRect.rect.height < textRectHeight) dependRect.rect.height = textRectHeight;
}

void AddDependency(char *dependency)
{
    // Add dependency to the list
    for (int i = 0; i < depend_list_idx; ++i) {
        if (AreStrEquals(depend_list[i], dependency)) return;
    }

    strcat(depend_list[depend_list_idx], dependency);
    memcpy(displayCurrentUpgrade->dependencies[depend_list_idx], dependency, strlen(dependency));

    depend_list_idx++;

    GenerateDependencyListString();
}

void RemoveDependency(Vector2 mouse)
{
    // int item_idx = (mouse.y - SelectMenu.rect.y) / textRectHeight;
    int dep_to_remove_idx = (mouse.y - dependRect.rect.y ) / (textRectHeight + padding);
    printf("Removing dependency %s, idx %d\n", depend_list[dep_to_remove_idx], dep_to_remove_idx);

    for (int i = dep_to_remove_idx; i < depend_list_idx; ++i) {
        memcpy(depend_list[i], depend_list[i+1], sizeof(depend_list[i+1]));
        memcpy(displayCurrentUpgrade->dependencies[i], displayCurrentUpgrade->dependencies[i+1],
               sizeof(displayCurrentUpgrade->dependencies[i+1]));
    }

    depend_list_idx--;

    // Format the string to be displayed on screen
    GenerateDependencyListString();
}

void SelectCurrentUpgrade(upgrade *original)
{
    displayCurrentUpgrade = original;
    sprintf(currentUpgradeIncreaseString, "%.2f", displayCurrentUpgrade->increase_factor);
    strncpy(nameRect.text, original->id, sizeof(original->id));
    strncpy(typeRect.text, original->type, sizeof(original->type));
    strncpy(descRect.text, original->description, sizeof(original->description));
    depend_list_idx = 0;
    memset(dependRect.text, 0, sizeof(dependRect.text));
    memset(depend_list, 0, sizeof(depend_list));
    for (int i = 0; i < MAX_DEPENDENCIES; ++i) {
        char *dep = original->dependencies[i];
        if (!AreStrEquals(dep, "")) {
            AddDependency(dep);
        }
    }
}

void DrawSelectUpgradeMenu()
{
    int i;

    DrawRectangleLinesEx(selectMainRect, 2, BLACK);

    for (i = 0; i < numUpgrades; ++i) {
        Rectangle rec = selectRectList[i];
        if (CheckCollisionPointRec(GetMousePosition(), rec)) {
            DrawRectangleRec(rec, LIGHTGRAY);
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
    Vector2 mouse = GetMousePosition();
    DrawRectangleLinesEx(displayMainRect, 2, BLACK);

    for (int i = 0; i < (sizeof(displayRectList) / sizeof(displayRectList[0])); ++i) {

        TextRect tr = *displayRectList[i];


        if (CheckCollisionPointRec(mouse, tr.rect)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText)
        {
            if (AreStrEquals(tr.id, "name") ||
                AreStrEquals(tr.id, "increase") ||
                AreStrEquals(tr.id, "description"))
            {
                framesCounter++;
                int cursorPos = strlen(tr.text);
                int key = GetKeyPressed();
                if (((framesCounter/20)%2) == 0)
                    DrawText("_", tr.rect.x + 6 + MeasureText(tr.text, fontSize),
                             tr.rect.y + 8, fontSize, MAROON);
                if (framesCounter > 200) framesCounter = 0;
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    tr.text[cursorPos - 1] = '\0';
                }
                if (AreStrEquals(tr.id, "increas")) {
                    while(key > 0)
                    {
                        if ((key >= 46) && (key <= 57)) {
                            tr.text[cursorPos] = (char)key;
                            tr.text[cursorPos + 1] = '\0';
                            displayCurrentUpgrade->increase_factor = atof(incrRect.text);
                        }
                        key = GetKeyPressed();
                    }
                } else {
                    while(key > 0)
                    {
                        if ((key >= 32) && (key <= 125)) {
                            tr.text[cursorPos] = (char)key;
                            tr.text[cursorPos + 1] = '\0';
                            printf("key: %d\n", key);
                        }
                        key = GetKeyPressed();
                    }

                }

            } else if (AreStrEquals(tr.id, "type")) {
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {

                }
            }

        }

        DrawTextRect(tr);
    }

    if (CheckCollisionPointRec(mouse, saveRect)) {
        DrawRectangleRec(saveRect, LIGHTGRAY);
    }


    DrawRectangleLinesEx(saveRect, 2, BLACK);
    DrawText("Save", saveRect.x + padding, saveRect.y + padding, fontSize, MAROON);
    DrawSelectionMenu();

    if (isDragging) {
        draggedRect.rect.x = mouse.x;
        draggedRect.rect.y = mouse.y;
        DrawTextRect(draggedRect);
    }
}


void HandleMouseClick()
{

    /* On mouse click down, activate pending action,
       only activated on button release.
       Pending action is cancelled on dragged gesture
    */

    int gesture = GetGestureDetected();
    Vector2 mouse = GetMousePosition();

    if (isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Check if dropped in dependency list
        if (CheckCollisionPointRec(mouse, dependRect.rect)) {
            AddDependency(dragedUpgrade->id);
        }
        isDragging = false;

    }

    if (gesture == GESTURE_DRAG) {
        // Cancel pending actions
        isAboutToSelect = false;
        isAboutToSave = false;
        isAboutToRemoveDependency = false;

        if (!isDragging) {
            for (int i = 0; i < numUpgrades; ++i) {
                Rectangle rec = selectRectList[i];
                if (CheckCollisionPointRec(mouse, rec)) {
                    isDragging = true;
                    dragedUpgrade = &state.upgrades_list[i];
                    strncpy(draggedRect.text, dragedUpgrade->id, sizeof(dragedUpgrade->id));
                }
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, dependRect.rect)) {
            isAboutToRemoveDependency = true;
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Prepare actions that will be triggered on release
        for (int i = 0; i < numUpgrades; ++i) {
            Rectangle rec = selectRectList[i];
            if (CheckCollisionPointRec(mouse, rec)) {
                isAboutToSelect = true;
            }
        }
        if (CheckCollisionPointRec(mouse, saveRect)) {
            isAboutToSave = true;
        }

    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {

        // Select upgrade in left panel
        if (isAboutToSelect) {
            for (int i = 0; i < numUpgrades; ++i) {
                Rectangle rec = selectRectList[i];
                if (CheckCollisionPointRec(mouse, rec)) {
                    SelectCurrentUpgrade(&state.upgrades_list[i]);
                }
            }
            isAboutToSelect = false;
        }


        // Select new type
        if (SelectMenu.visible &&
            CheckCollisionPointRec(mouse, SelectMenu.rect)) {
            int item_idx = (mouse.y - SelectMenu.rect.y) / textRectHeight;
            memset(displayCurrentUpgrade->type, 0, sizeof(displayCurrentUpgrade->type));
            memcpy(displayCurrentUpgrade->type,
                   SelectMenu.items_list[item_idx],
                   strlen(SelectMenu.items_list[item_idx]));
        }


        SelectMenu.visible = false;

        // Close opened MenuConfig must be after closure test
        if (CheckCollisionPointRec(mouse, typeRect.rect)){
            OpenSelectMenu(newTypeReceiver,
                           typesList, sizeof(typesList) / sizeof(typesList[0]),
                           typeRect.rect.x, typeRect.rect.y + typeRect.rect.height);
        }
        else if (CheckCollisionPointRec(mouse, saveRect) && isAboutToSave)
        {
            SaveGame();
            isAboutToSave = false;
        }
    }

    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        if (isAboutToRemoveDependency) {
            RemoveDependency(mouse);
        }
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
            HandleMouseClick();
        }
        EndDrawing();
    }

}
