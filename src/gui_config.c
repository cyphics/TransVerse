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
extern Vector2 mousePosition;
extern int keyPressed;
Font font;

const float scaleFactor = 1;
const float fontSize = defaultFontSize * scaleFactor;
#define numUpgrades  MAX_UPGRADES_AMOUNT
const float maxUpgradeNameLength = 250 * scaleFactor;
const float padding = 5 * scaleFactor;
const float xAnchor = 20 * scaleFactor, yAnchor = 20 * scaleFactor;
const float textRectHeight = 30 * scaleFactor;

// Common
bool initDone = false;


char currentUpgradeIncreaseString[6];
char currentUpgradeAmountString[10];
char currentUpgradeBoughtString[10];
bool mouseOnText = false;

bool isDragging = false;
Interact draggedUpdateSprite;
upgrade *dragedUpgrade;


// Upgrade selection panel

const float selectUpgradeRectWidth = maxUpgradeNameLength;
const float selectMainRectWidth = selectUpgradeRectWidth + 2 * padding;
const float selectMainRectHeight = textRectHeight * numUpgrades + padding * (numUpgrades + 1);
const float tabWidth = selectMainRectWidth / 4;

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

const float addButtonWidth = 20;
Rectangle addUpgradeButton = {xAnchor, 0,
                              addButtonWidth, addButtonWidth};
bool isAboutToAddNewUpgrade = false;
bool isAboutToRemoveUpgrade = false;

char *currentTypeList = "structure";
int amountSelectableUpgrades = 0;
Interact selectUpgradesList[numUpgrades] = {};
bool isAboutToSelect = false;

// Upgrade edit panel
upgrade *currentUpgradeToEdit;

const float displayRectXAnchor = xAnchor + maxUpgradeNameLength + 20 * scaleFactor;
const float displayRectYAnchor = yAnchor;
const Rectangle displayNameRect = {displayRectXAnchor + padding,
                                   displayRectYAnchor + padding,
                                   selectMainRectWidth, textRectHeight};
const Rectangle displayTypeRect = {displayNameRect.x + displayNameRect.width + padding,
                                   displayRectYAnchor + padding, 130 * scaleFactor, textRectHeight};
const Rectangle displayIncreaseRect = {displayTypeRect.x + displayTypeRect.width + padding,
                                       displayRectYAnchor + padding, 50 * scaleFactor, textRectHeight};
const Rectangle displayDescRect = {displayRectXAnchor + padding, displayRectYAnchor + padding + 40 * scaleFactor,
                                   displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 2 * padding, textRectHeight * 2};
const Rectangle displayDependRect = {displayRectXAnchor + padding,
                                     displayDescRect.y + displayDescRect.height + padding,
                                     displayNameRect.width, textRectHeight};
const Rectangle displayResourceRect = {displayTypeRect.x,
                                       displayDependRect.y,
                                       displayTypeRect.width, textRectHeight};
const Rectangle displayAmountRect = {displayIncreaseRect.x,
                                     displayDependRect.y,
                                     displayIncreaseRect.width, textRectHeight};
const Rectangle displayBoughtRect = {displayResourceRect.x,
                                     displayDependRect.y + displayDependRect.height + padding * 2,
                                     50, displayNameRect.height};
const float displayMainRectWidth = displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 4 * padding;
const float displayMainRectHeight = 300.0f * scaleFactor;
Rectangle displayMainRect = {displayRectXAnchor, displayRectYAnchor,
                             displayMainRectWidth, displayMainRectHeight};
const Rectangle saveRect = {displayRectXAnchor, displayRectYAnchor + displayMainRectWidth + padding,
                            100, 30};
Interact saveButton = {"saveBtn", "", saveRect,"Save", true, false};
bool isAboutToSave = false;

Interact editNameInteract = {"name", "name", displayNameRect, "", false, true};
Interact editTypeInteract = {"type", "type", displayTypeRect, "", false, false};
Interact editIncreaseInteract = {"increase", "increase", displayIncreaseRect, "", false, true};
Interact editDescInteract = {"description", "description", displayDescRect, "", false, true};
Interact editDependenciesInteract = {"dependencies", "dependencies", displayDependRect, "", false, false};
Interact editResourceInteract = {"resource", "resource", displayResourceRect, "", false, false};
Interact editAmountInteract = {"amount", "amount", displayAmountRect, "", false, true};
Interact editBoughtInteract = {"bought", "bought", displayBoughtRect, "", false, true};


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
                               &editAmountInteract,
                               &editBoughtInteract};

char dropDownMenuResult[30] = {};
char *typesList[] = {"science", "incremental", "structure", "software"};
char *resourcesList[] = {"energy", "code", "software", "copper", "steel"};

struct DropDownMenu {
    char *destination;
    char **items_list;
    int num_items;
    Rectangle rect;
    bool visible;
} DropDownMenu;


void BuildSelectUpgradesList(char *newTypeList)
{
    currentTypeList = newTypeList;
    amountSelectableUpgrades = 0;
    memset(selectUpgradesList, 0, sizeof(selectUpgradesList));
    for (int i = 0; i < numUpgrades; ++i) {
        if (AreStrEquals(state.upgrades_list[i].type, currentTypeList)) {
            Rectangle r = {xAnchor + padding,
                           yAnchor + padding +  (textRectHeight + padding) * amountSelectableUpgrades,
                           selectUpgradeRectWidth, textRectHeight};

            Interact inter = {};
            inter.rect = r;
            inter.text = state.upgrades_list[i].id;
            inter.isHoverable = true;
            inter.isEditable = false;
            selectUpgradesList[amountSelectableUpgrades] = inter;
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

void RemoveDependency()
{
    // int item_idx = (mousePosition.y - DropDownMenu.rect.y) / textRectHeight;
    int dep_to_remove_idx = (mousePosition.y - editDependenciesInteract.rect.y ) / (textRectHeight + padding);
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

void RemoveUpgrade()
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

void RemoveResource()
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

void CloseDropDownMenu()
{
    int item_idx = (mousePosition.y - DropDownMenu.rect.y) / textRectHeight;
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
    sprintf(currentUpgradeBoughtString, "%d", currentUpgradeToEdit->amount_bought);
    editNameInteract.text = original->id;
    editTypeInteract.text = original->type;
    editDescInteract.text = original->description;
    editResourceInteract.text = original->initial_price.resources[0].type;
    editAmountInteract.text = currentUpgradeAmountString;
    editIncreaseInteract.text = currentUpgradeIncreaseString;
    editBoughtInteract.text = currentUpgradeBoughtString;
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
        if (CheckCollisionPointRec(mousePosition, r)) c = WHITE;
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

    if (isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Check if dropped in dependency list
        if (CheckCollisionPointRec(mousePosition, editDependenciesInteract.rect)) {
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
                if (CheckCollisionPointRec(mousePosition, rec)) {
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
            if (CheckCollisionPointRec(mousePosition, rec)) {
                isAboutToSelect = true;
            }
        }
        isAboutToSave = (CheckCollisionPointRec(mousePosition, saveButton.rect));
        isAboutToAddNewUpgrade = (CheckCollisionPointRec(mousePosition, addUpgradeButton));

        if (CheckCollisionPointRec(mousePosition, scienceRect)) BuildSelectUpgradesList("science");
        if (CheckCollisionPointRec(mousePosition, structureRect)) BuildSelectUpgradesList("structure");
        if (CheckCollisionPointRec(mousePosition, softwareRect)) BuildSelectUpgradesList("software");
        if (CheckCollisionPointRec(mousePosition, incrementalRect)) BuildSelectUpgradesList("incremental");
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {

        // Select upgrade in left panel
        if (isAboutToSelect) {
            for (int i = 0; i < amountSelectableUpgrades; ++i) {
                Rectangle rec = selectUpgradesList[i].rect;
                if (CheckCollisionPointRec(mousePosition, rec)) {
                    for (int j = 0; j < numUpgrades; ++j) {
                        if (AreStrEquals(selectUpgradesList[i].text, state.upgrades_list[j].id)) {
                            SelectCurrentUpgrade(&state.upgrades_list[j]);
                        }
                    }
                }
            }
            isAboutToSelect = false;
        }

        if (isAboutToAddNewUpgrade && CheckCollisionPointRec(mousePosition, addUpgradeButton)) {
            AddNewUpgrade();
        }

        // Select DropDownMenu item and close
        if (DropDownMenu.visible &&
            CheckCollisionPointRec(mousePosition, DropDownMenu.rect)) {
            CloseDropDownMenu();
        }
        DropDownMenu.visible = false;

        if (CheckCollisionPointRec(mousePosition, editTypeInteract.rect)){
            OpenDropDownMenu(editTypeInteract.text, typesList,
                             sizeof(typesList) / sizeof(typesList[0]),
                             editTypeInteract.rect.x, editTypeInteract.rect.y +
                             editTypeInteract.rect.height);
        } else if (CheckCollisionPointRec(mousePosition, editResourceInteract.rect)){
            OpenDropDownMenu(editResourceInteract.text, resourcesList,
                             sizeof(resourcesList) / sizeof(resourcesList[0]),
                             editResourceInteract.rect.x, editResourceInteract.rect.y +
                             editResourceInteract.rect.height);
        }

        else if (CheckCollisionPointRec(mousePosition, saveButton.rect) && isAboutToSave)
        {
            SaveGame();
            isAboutToSave = false;
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (CheckCollisionPointRec(mousePosition, editDependenciesInteract.rect)) {
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
            RemoveDependency();
            isAboutToRemoveDependency = false;
        }
        if (isAboutToRemoveUpgrade) {
            RemoveUpgrade();
        }
    }

}

void HandleMouseOver()
{
    saveButton.isHovered = CheckCollisionPointRec(mousePosition, saveButton.rect);

    int i;
    for (i = 0; i < amountSelectableUpgrades; ++i) {
        selectUpgradesList[i].isHovered = CheckCollisionPointRec(GetMousePosition(), selectUpgradesList[i].rect);
    }

    for (i = 0; i < (sizeof(editFieldsList) / sizeof(editFieldsList[0])); ++i) {
        Interact *editField = editFieldsList[i];
        editField->isHovered = CheckCollisionPointRec(mousePosition, editField->rect);
        if (editField->isHovered && editField->isEditable)
        {
            int cursorPos = strlen(editField->text);
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (cursorPos > 0) {
                    editField->text[cursorPos - 1] = '\0';
                }
            }
            while(keyPressed > 0){
                if (AreStrEquals(editField->id, "increase")) {
                    if ((keyPressed >= 46) && (keyPressed <= 57)) {
                        editField->text[cursorPos] = (char)keyPressed;
                        editField->text[cursorPos + 1] = '\0';
                        currentUpgradeToEdit->increase_factor = atof(editIncreaseInteract.text);
                    }
                } else if (AreStrEquals(editField->id, "amount")) {
                    if ((keyPressed >= 48) && (keyPressed <= 57)) {
                        editField->text[cursorPos] = (char)keyPressed;
                        editField->text[cursorPos + 1] = '\0';
                        currentUpgradeToEdit->initial_price.resources[0].amount = strtol(editAmountInteract.text, NULL, 10);
                    }
                } else if (AreStrEquals(editField->id, "bought")) {
                    if ((keyPressed >= 48) && (keyPressed <= 57)) {
                        editField->text[cursorPos] = (char)keyPressed;
                        editField->text[cursorPos + 1] = '\0';
                        currentUpgradeToEdit->amount_bought = strtol(editBoughtInteract.text, NULL, 10);
                    }
                }else if ((keyPressed >= 32) && (keyPressed <= 125)) {
                    editField->text[cursorPos] = (char)keyPressed;
                    editField->text[cursorPos + 1] = '\0';
                }
                keyPressed = GetKeyPressed();
            }
        }
    }

    if (isDragging) {
        draggedUpdateSprite.rect.x = mousePosition.x;
        draggedUpdateSprite.rect.y = mousePosition.y;
        DrawInteract(&draggedUpdateSprite);
    }
}


void DrawConfigPanel()
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
