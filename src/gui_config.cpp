/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */
#include <raylib.h>

#include "helper.h"
#include "game_handler.h"
#include "gui.h"

extern char *CONFIG_FILE;

// extern GameState state;
Font font;

const int numUpgrades = MAX_UPGRADES_AMOUNT;

// Common
bool initDone = false;
float scaleFactor = 1;
int framesCounter = 0;
float maxUpgradeNameLength = 250 * scaleFactor;
float padding = 5 * scaleFactor;
float fontSize = 20.0 * scaleFactor;
float xAnchor = 20 * scaleFactor, yAnchor = 20 * scaleFactor;
float textRectHeight = 30 * scaleFactor;
char currentUpgradeIncreaseString[6];
char currentUpgradeAmountString[10];
bool mouseOnText = false;

bool isDragging = false;
Interact draggedUpdateSprite;
upgrade *dragedUpgrade;


// Upgrade selection panel

float selectUpgradeRectWidth = maxUpgradeNameLength;
float selectMainRectWidth = selectUpgradeRectWidth + 2 * padding;
float selectMainRectHeight = textRectHeight * numUpgrades + padding * (numUpgrades + 1);
float tabWidth = selectMainRectWidth / 4;

Rectangle structureRect = {xAnchor,
                           yAnchor - textRectHeight/2,
                           tabWidth,
                           textRectHeight/2};
Rectangle scienceRect = {xAnchor + tabWidth,
                         yAnchor - textRectHeight/2,
                         tabWidth,
                         textRectHeight/2};Rectangle softwareRect = {xAnchor + tabWidth * 2,
                                                                     yAnchor - textRectHeight/2,
                                                                     tabWidth,
                                                                     textRectHeight/2};
Rectangle incrementalRect = {xAnchor + tabWidth * 3,
                             yAnchor - textRectHeight/2,
                             tabWidth, textRectHeight/2};

Rectangle selectMainRect = {xAnchor, yAnchor,
                            selectMainRectWidth, selectMainRectHeight};

float addButtonWidth = 20;
Rectangle addUpgradeButton = {xAnchor, selectMainRect.y + selectMainRect.height + padding,
                              addButtonWidth, addButtonWidth};
bool isAboutToAddNewUpgrade = false;
bool isAboutToRemoveUpgrade = false;

char *currentTypeList = "structure";
int amountSelectableUpgrades = 0;
Interact selectUpgradesList[numUpgrades] = {};
bool isAboutToSelect = false;

// Upgrade edit panel
upgrade *currentUpgradeToEdit;

float displayRectXAnchor = xAnchor + maxUpgradeNameLength + 20 * scaleFactor;
float displayRectYAnchor = yAnchor;
Rectangle displayNameRect = {displayRectXAnchor + padding,
                             displayRectYAnchor + padding,
                             selectMainRectWidth, textRectHeight};
Rectangle displayTypeRect = {displayNameRect.x + displayNameRect.width + padding,
                             displayRectYAnchor + padding, 130 * scaleFactor, textRectHeight};
Rectangle displayIncreaseRect = {displayTypeRect.x + displayTypeRect.width + padding,
                                 displayRectYAnchor + padding, 50 * scaleFactor, textRectHeight};
Rectangle displayDescRect = {displayRectXAnchor + padding, displayRectYAnchor + padding + 40 * scaleFactor,
                             displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 2 * padding, textRectHeight * 2};
Rectangle displayDependRect = {displayRectXAnchor + padding,
                               displayDescRect.y + displayDescRect.height + padding,
                               displayNameRect.width, textRectHeight};
Rectangle displayResourceRect = {displayTypeRect.x,
                                 displayDependRect.y,
                                 displayTypeRect.width, textRectHeight};
Rectangle displayAmountRect = {displayIncreaseRect.x,
                               displayDependRect.y,
                               displayIncreaseRect.width, textRectHeight};
float displayMainRectWidth = displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 4 * padding;
float displayMainRectHeight = 300.0f * scaleFactor;
Rectangle displayMainRect = {displayRectXAnchor, displayRectYAnchor,
                             displayMainRectWidth, displayMainRectHeight};
Rectangle saveRect = {displayRectXAnchor, displayRectYAnchor + displayMainRectWidth + padding,
                      100, 30};
Interact saveButton = {"saveBtn", "", saveRect,"Save"};
bool isAboutToSave = false;

Interact editNameInteract = {"name", "name", displayNameRect};
Interact editTypeInteract = {"type", "type", displayTypeRect};
Interact editIncreaseInteract = {"increase", "increase", displayIncreaseRect};
Interact editDescInteract = {"description", "description", displayDescRect};
Interact editDependenciesInteract = {"dependencies", "dependencies", displayDependRect};
Interact editResourceInteract = {"resource", "resource", displayResourceRect};
Interact editAmountInteract = {"amount", "amount", displayAmountRect};

char dependenciesStringToDisplay[MAX_DEPENDENCIES * 35];

char listDependenciesString[MAX_DEPENDENCIES][30];
int numberDependencies = 0;
bool isAboutToRemoveDependency = false;

// resource resouresList[MAX_RESOURCES_PER_PRICE];
// int numberResources = 0;
// bool isAboutToRemoveResource = false;

Interact *editFieldsList[] = { &editNameInteract,
                               &editTypeInteract,
                               &editIncreaseInteract,
                               &editDescInteract,
                               &editDependenciesInteract,
                               &editResourceInteract,
                               &editResourceInteract,
                               &editAmountInteract};

char dropDownMenuResult[30] = {};
char *typesList[] = {"science", "incremental", "knowledge", "structure", "software"};
char *resourcesList[] = {"energy", "code", "knowledge", "software", "copper", "steel"};

struct DropDownMenu {
    char *destination;
    char **items_list;
    int num_items;
    Rectangle rect;
    bool visible = false;
} DropDownMenu;


void BuildSelectUpgradesList(char *newTypeList)
{
    currentTypeList = newTypeList;
    amountSelectableUpgrades = 0;
    memset(selectUpgradesList, 0, sizeof(selectUpgradesList));
    for (int i = 0; i < numUpgrades; ++i) {
        if (AreStrEquals(state.upgrades_list[i].type, currentTypeList)) {
            selectUpgradesList[amountSelectableUpgrades] = {"", "",
                                                            {xAnchor + padding,
                                                             yAnchor + padding +  (textRectHeight + padding) * amountSelectableUpgrades,
                                                             selectUpgradeRectWidth, textRectHeight},
                                                            state.upgrades_list[i].id};
            amountSelectableUpgrades++;
        }
    }
    selectMainRect.height = (textRectHeight + padding) * amountSelectableUpgrades + 2 * padding;
    addUpgradeButton.y = selectMainRect.y + selectMainRect.height + padding;
}

void GenerateDependencyListString()
{
    memset(dependenciesStringToDisplay, 0, sizeof(dependenciesStringToDisplay));
    for (int i = 0; i < numberDependencies; ++i) {
        strcat(dependenciesStringToDisplay, listDependenciesString[i]);
        strcat(dependenciesStringToDisplay, "\n");
    }
    editDependenciesInteract.text =  dependenciesStringToDisplay;
    editDependenciesInteract.rect.height = (textRectHeight + padding) * numberDependencies;
    if (editDependenciesInteract.rect.height < textRectHeight) editDependenciesInteract.rect.height = textRectHeight;
}

void AddDependency(char *dependency)
{
    // Add dependency to the list
    for (int i = 0; i < numberDependencies; ++i) {
        if (AreStrEquals(listDependenciesString[i], dependency)) return;
    }

    strcat(listDependenciesString[numberDependencies], dependency);
    memcpy(currentUpgradeToEdit->dependencies[numberDependencies], dependency, strlen(dependency));

    numberDependencies++;

    GenerateDependencyListString();
}

void RemoveDependency(Vector2 mouse)
{
    // int item_idx = (mouse.y - DropDownMenu.rect.y) / textRectHeight;
    int dep_to_remove_idx = (mouse.y - editDependenciesInteract.rect.y ) / (textRectHeight + padding);
    printf("Removing dependency %s, idx %d\n", listDependenciesString[dep_to_remove_idx], dep_to_remove_idx);

    for (int i = dep_to_remove_idx; i < numberDependencies; ++i) {
        memcpy(listDependenciesString[i], listDependenciesString[i+1], sizeof(listDependenciesString[i+1]));
        memcpy(currentUpgradeToEdit->dependencies[i], currentUpgradeToEdit->dependencies[i+1],
               sizeof(currentUpgradeToEdit->dependencies[i+1]));
    }

    numberDependencies--;

    // Format the string to be displayed on screen
    GenerateDependencyListString();
}

void AddNewUpgrade()
{
    for (int i = 0; i < numUpgrades; ++i) {
        if (IsEmpty(state.upgrades_list[i].id)) {
            memcpy(state.upgrades_list[i].id, "New upgrade", strlen("New ugprade"));
            memcpy(state.upgrades_list[i].type, currentTypeList, strlen(currentTypeList));
            break; // Avoid create new upgrade on all empty slots
        }
    }
    BuildSelectUpgradesList(currentTypeList);
}

void RemoveUpgrade(Vector2 mouse)
{
    int selectUpIdx = 0;
    for (int i = 0; i < numUpgrades; ++i) {
        upgrade *u = &state.upgrades_list[i];
        if (AreStrEquals(u->type, currentTypeList)) {
            if (selectUpgradesList[selectUpIdx].isHovered) {
                *u = (upgrade){0};
                BuildSelectUpgradesList(currentTypeList);
            }
            selectUpIdx++;
        }

    }
}

void AddResource(resource res)
{

}

void RemoveResource(Vector2 mouse)
{

}


void OpenDropDownMenu(char *target, char **items_list, int num_items, float anchorX, float anchorY)
{
    // Set width of selector
    float width = 0;
    float length = 0;
    for (int i = 0; i < num_items; ++i) {
        length = MeasureText(items_list[i], fontSize);
        if (length > width) width = length;
    }
    width += padding * 2;

    DropDownMenu.destination = target;
    DropDownMenu.items_list = items_list;
    DropDownMenu.num_items = num_items;
    DropDownMenu.rect = (Rectangle){anchorX, anchorY, width, textRectHeight * num_items};
    DropDownMenu.visible = true;
}

void CloseDropDownMenu(Vector2 mouse)
{
    int item_idx = (mouse.y - DropDownMenu.rect.y) / textRectHeight;
    memset(DropDownMenu.destination, 0, sizeof(currentUpgradeToEdit->type));
    memcpy(DropDownMenu.destination,
           DropDownMenu.items_list[item_idx],
           strlen(DropDownMenu.items_list[item_idx]));
}

void SelectCurrentUpgrade(upgrade *original)
{
    currentUpgradeToEdit = original;
    sprintf(currentUpgradeIncreaseString, "%.2f", currentUpgradeToEdit->increase_factor);
    sprintf(currentUpgradeAmountString, "%d", currentUpgradeToEdit->initial_price.resources[0].amount);
    editNameInteract.text = original->id;
    editTypeInteract.text = original->type;
    editDescInteract.text = original->description;
    editResourceInteract.text = original->initial_price.resources[0].type;
    editAmountInteract.text = currentUpgradeAmountString;
    editIncreaseInteract.text = currentUpgradeIncreaseString;
    editDependenciesInteract.text = "";
    numberDependencies = 0;
    memset(listDependenciesString, 0, sizeof(listDependenciesString));
    for (int i = 0; i < MAX_DEPENDENCIES; ++i) {
        char *dep = original->dependencies[i];
        if (!IsEmpty(dep)) {
            AddDependency(dep);
        }
    }
}


void DrawDropDownMenu()
{
    Color c;
    for (int i = 0; i < DropDownMenu.num_items; ++i) {
        Rectangle r = {DropDownMenu.rect.x, DropDownMenu.rect.y + 30 * i,
                       DropDownMenu.rect.width, textRectHeight};
        if (CheckCollisionPointRec(GetMousePosition(), r)) c = WHITE;
        else c = LIGHTGRAY;
        DrawRectangleRec(r, c);
        DrawText(DropDownMenu.items_list[i],
                 DropDownMenu.rect.x + padding, DropDownMenu.rect.y + textRectHeight * i + padding,
                 fontSize, MAROON);
    }
    DrawRectangleLinesEx(DropDownMenu.rect, 1, DARKGRAY);
}

void DrawStaticContent()
{
    int i;
    DrawRectangleLinesEx(scienceRect, 1, BLACK);
    DrawRectangleLinesEx(structureRect, 1, BLACK);
    DrawRectangleLinesEx(softwareRect, 1, BLACK);
    DrawRectangleLinesEx(incrementalRect, 1, BLACK);
    DrawText("science", scienceRect.x + padding, scienceRect.y + padding / 2, fontSize/4, DARKGRAY);
    DrawText("structure", structureRect.x + padding, structureRect.y + padding / 2, fontSize/4, DARKGRAY);
    DrawText("software", softwareRect.x + padding, softwareRect.y + padding / 2, fontSize/4, DARKGRAY);
    DrawText("incremental", incrementalRect.x + padding, incrementalRect.y + padding / 2, fontSize/4, DARKGRAY);
    DrawRectangleLinesEx(selectMainRect, 2, BLACK);
    DrawRectangleLinesEx(addUpgradeButton, 2, BLACK);
    DrawText("+", addUpgradeButton.x + padding, addUpgradeButton.y + padding * 0.33, fontSize, MAROON);
    DrawRectangleLinesEx(displayMainRect, 2, BLACK);
    int fieldsElem = (sizeof(editFieldsList) / sizeof(editFieldsList[0]));
    for (i = 0; i < fieldsElem; ++i) DrawInteract(editFieldsList[i]);
    for (i = 0; i < amountSelectableUpgrades; ++i) DrawInteract(&selectUpgradesList[i]);


    // DrawRectangleLinesEx(saveRect, 2, BLACK);
    // DrawText("Save", saveRect.x + padding, saveRect.y + padding, fontSize, MAROON);
    DrawInteract(&saveButton);
    if (DropDownMenu.visible) {
        DrawDropDownMenu();
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
        if (CheckCollisionPointRec(mouse, editDependenciesInteract.rect)) {
            AddDependency(dragedUpgrade->id);
        }
        isDragging = false;

    }

    if (gesture == GESTURE_DRAG) {
        // Cancel pending actions
        isAboutToSelect = false;
        isAboutToSave = false;
        isAboutToRemoveDependency = false;
        isAboutToAddNewUpgrade = false;

        // Check and start dragging upgrade from list
        if (!isDragging) {
            for (int i = 0; i < numUpgrades; ++i) {
                Rectangle rec = selectUpgradesList[i].rect;
                if (CheckCollisionPointRec(mouse, rec)) {
                    isDragging = true;
                    dragedUpgrade = &state.upgrades_list[i];
                    draggedUpdateSprite.text = dragedUpgrade->id;
                }
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Prepare actions that will be triggered on release
        for (int i = 0; i < numUpgrades; ++i) {
            Rectangle rec = selectUpgradesList[i].rect;
            if (CheckCollisionPointRec(mouse, rec)) {
                isAboutToSelect = true;
            }
        }
        isAboutToSave = (CheckCollisionPointRec(mouse, saveButton.rect));
        isAboutToAddNewUpgrade = (CheckCollisionPointRec(mouse, addUpgradeButton));

        if (CheckCollisionPointRec(mouse, scienceRect)) BuildSelectUpgradesList("science");
        if (CheckCollisionPointRec(mouse, structureRect)) BuildSelectUpgradesList("structure");
        if (CheckCollisionPointRec(mouse, softwareRect)) BuildSelectUpgradesList("software");
        if (CheckCollisionPointRec(mouse, incrementalRect)) BuildSelectUpgradesList("incremental");
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {

        // Select upgrade in left panel
        if (isAboutToSelect) {
            for (int i = 0; i < amountSelectableUpgrades; ++i) {
                Rectangle rec = selectUpgradesList[i].rect;
                if (CheckCollisionPointRec(mouse, rec)) {
                    for (int j = 0; j < numUpgrades; ++j) {
                        if (AreStrEquals(selectUpgradesList[i].text, state.upgrades_list[j].id)) {
                            SelectCurrentUpgrade(&state.upgrades_list[j]);
                        }

                    }


                }
            }
            isAboutToSelect = false;
        }

        if (isAboutToAddNewUpgrade && CheckCollisionPointRec(mouse, addUpgradeButton)) {
            AddNewUpgrade();

        }

        // Select DropDownMenu item and close
        if (DropDownMenu.visible &&
            CheckCollisionPointRec(mouse, DropDownMenu.rect)) {
            CloseDropDownMenu(mouse);
        }
        DropDownMenu.visible = false;

        if (CheckCollisionPointRec(mouse, editTypeInteract.rect)){
            OpenDropDownMenu(editTypeInteract.text, typesList,
                             sizeof(typesList) / sizeof(typesList[0]),
                             editTypeInteract.rect.x, editTypeInteract.rect.y +
                             editTypeInteract.rect.height);
        } else if (CheckCollisionPointRec(mouse, editResourceInteract.rect)){
            OpenDropDownMenu(editResourceInteract.text, resourcesList,
                             sizeof(resourcesList) / sizeof(resourcesList[0]),
                             editResourceInteract.rect.x, editResourceInteract.rect.y +
                             editResourceInteract.rect.height);
        }

        else if (CheckCollisionPointRec(mouse, saveButton.rect) && isAboutToSave)
        {
            SaveGame();
            isAboutToSave = false;
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, editDependenciesInteract.rect)) {
            isAboutToRemoveDependency = true;
        }
        for (int i = 0; i < amountSelectableUpgrades; ++i) {
            if (selectUpgradesList[i].isHovered) {
                isAboutToRemoveUpgrade = true;
            }
        }

    }
    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)){
        if (isAboutToRemoveDependency){
            RemoveDependency(mouse);
            isAboutToRemoveDependency = false;
        }
        if (isAboutToRemoveUpgrade) {
            RemoveUpgrade(mouse);
        }
    }

}

void HandleMouseOver()
{
    Vector2 mouse = GetMousePosition();

    saveButton.isHovered = CheckCollisionPointRec(mouse, saveButton.rect);
    // Check left panel
    // for (int i = 0; i < numUpgrades; ++i) {
    //     selectUpgradesList[i].isHovered = CheckCollisionPointRec(GetMousePosition(), selectUpgradesList[i].rect);
    // }
    for (int i = 0; i < amountSelectableUpgrades; ++i) {
        selectUpgradesList[i].isHovered = CheckCollisionPointRec(GetMousePosition(), selectUpgradesList[i].rect);
    }
    printf("Upgrade %s hovered: %d\n", selectUpgradesList[6].text, selectUpgradesList[6].isHovered);


    // Check editable fields
    for (int i = 0; i < (sizeof(editFieldsList) / sizeof(editFieldsList[0])); ++i) {
        Interact editField = *editFieldsList[i];
        if (CheckCollisionPointRec(mouse, editField.rect))
        {
            if (AreStrEquals(editField.id, "name") ||
                AreStrEquals(editField.id, "increase") ||
                AreStrEquals(editField.id, "description") ||
                AreStrEquals(editField.id, "amount"))
            {
                editField.isHovered = true;
                framesCounter++;
                int cursorPos = strlen(editField.text);
                int key = GetKeyPressed();
                if (((framesCounter/20)%2) == 0)
                    DrawText("_", editField.rect.x + 6 + MeasureText(editField.text, fontSize),
                             editField.rect.y + 8, fontSize, MAROON);
                if (framesCounter > 200) framesCounter = 0;
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (cursorPos > 0) {
                        editField.text[cursorPos - 1] = '\0';
                    }
                }
                while(key > 0){
                    if (AreStrEquals(editField.id, "increase")) {
                        if ((key >= 46) && (key <= 57)) {
                            editField.text[cursorPos] = (char)key;
                            editField.text[cursorPos + 1] = '\0';
                            currentUpgradeToEdit->increase_factor = atof(editIncreaseInteract.text);
                        }
                    } else if (AreStrEquals(editField.id, "amount")) {
                        if ((key >= 48) && (key <= 57)) {
                            editField.text[cursorPos] = (char)key;
                            editField.text[cursorPos + 1] = '\0';
                            currentUpgradeToEdit->initial_price.resources[0].amount = strtol(editAmountInteract.text, NULL, 10);
                        }
                    }else if ((key >= 32) && (key <= 125)) {
                        editField.text[cursorPos] = (char)key;
                        editField.text[cursorPos + 1] = '\0';
                    }
                    key = GetKeyPressed();
                }
            }
        }
    }

    if (isDragging) {
        draggedUpdateSprite.rect.x = mouse.x;
        draggedUpdateSprite.rect.y = mouse.y;
        DrawInteract(&draggedUpdateSprite);
    }
}


void DrawConfigPanel(float panelWidth, float panelHeight)
{
    if (!initDone) {
        BuildSelectUpgradesList("structure");
        SelectCurrentUpgrade(&state.upgrades_list[0]);
        font = GetFontDefault();
        initDone = true;
    }

    HandleMouseOver();
    HandleMouseClick();
    DrawStaticContent();


}
