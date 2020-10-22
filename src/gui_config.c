/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */
#include <raylib.h>
#include "gui_config.h"

#include "helper.h"
#include "game_handler.h"
#include "gui.h"
#include "gui_constants.h"

extern char *CONFIG_FILE;
Font font;

//const float FONT_SIZE = defaultFontSize * SCALE_FACTOR;
#define numUpgrades  MAX_UPGRADES_AMOUNT

const float anchorX = 20 * SCALE_FACTOR, anchorY = 20 * SCALE_FACTOR;

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

const float selectUpgradeRectWidth = MAX_UPGRADE_NAME_LENGTH;
const float selectMainRectWidth = selectUpgradeRectWidth + 2 * PADDING;
const float selectMainRectHeight = TEXT_RECT_HEIGHT * numUpgrades + PADDING * (numUpgrades + 1);
const float selectTypeTabWidth = selectMainRectWidth / 4;

// Rectangle structRect = {anchorX,
//                            anchorY - textRectHeight/2,
//                            selectTypeTabWidth,
//                            textRectHeight/2};
// Rectangle scienceRect = {anchorX + selectTypeTabWidth,
//                          anchorY - textRectHeight/2,
//                          selectTypeTabWidth,
//                          textRectHeight/2};Rectangle softwareRect = {anchorX + selectTypeTabWidth * 2,
//                                                                      anchorY - textRectHeight/2,
//                                                                      selectTypeTabWidth,
//                                                                      textRectHeight/2};
// Rectangle incrementalRect = {anchorX + selectTypeTabWidth * 3,
//                              anchorY - textRectHeight/2,
//                              selectTypeTabWidth, textRectHeight/2};

// Rectangle selectMainRect = {anchorX, anchorY,
//                             selectMainRectWidth, selectMainRectHeight};

const float addButtonWidth = 20;
Rectangle addUpgradeButton = {anchorX, 0,
                              addButtonWidth, addButtonWidth};
bool isAboutToAddNewUpgrade = false;
bool isAboutToRemoveUpgrade = false;

char *currentTypeList = "structure";
int amountSelectableUpgrades = 0;
Interact selectUpgradesList[numUpgrades] = {};
bool isAboutToSelect = false;

// Upgrade edit panel
upgrade *currentUpgradeToEdit;

const float displayRectXAnchor = anchorX + MAX_UPGRADE_NAME_LENGTH + 20 * SCALE_FACTOR;
const float displayRectYAnchor = anchorY;
const Rectangle displayNameRect = {displayRectXAnchor + PADDING,
                                   displayRectYAnchor + PADDING,
                                   selectMainRectWidth, TEXT_RECT_HEIGHT};
const Rectangle displayTypeRect;
const Rectangle displayIncreaseRect;
const Rectangle displayDescRect;
const Rectangle displayDependRect;
const Rectangle displayResourceRect;
const Rectangle displayAmountRect;
const Rectangle displayBoughtRect ;
const Rectangle displayMainRect;

Interact editNameInteract;
Interact editTypeInteract;
Interact editIncreaseInteract;
Interact editDescInteract;
Interact editDependenciesInteract;
Interact editResourceInteract;
Interact editAmountInteract;
Interact editBoughtInteract;


char dependenciesStringToDisplay[MAX_DEPENDENCIES * 35];

char listDependenciesString[MAX_DEPENDENCIES][30];
int numberDependencies = 0;
bool isAboutToRemoveItem = false;

// resource resouresList[MAX_RESOURCES_PER_PRICE];
// int numberResources = 0;
// bool isAboutToRemoveResource = false;


char dropDownMenuResult[30] = {};
const char *typesList[] = {"science", "incremental", "structure", "software"};
const char *resourcesList[] = {"energy", "code", "software", "copper", "steel"};

const Rectangle gameStateRectangle = {anchorX, displayMainRect.y + displayMainRect.height + PADDING,
                                      selectMainRectWidth + displayMainRectWidth + PADDING * 2, 200
};

const Rectangle speedRect = {gameStateRectangle.x + PADDING,
                             gameStateRectangle.y + PADDING,
                             60 * SCALE_FACTOR,
                             TEXT_RECT_HEIGHT};
Interact speedInteract = {"speed", "Speed", speedRect, "", false, true, .fontSize = defaultFontSize};

const float saveWidth = 100;
const Rectangle saveRect = {gameStateRectangle.x + gameStateRectangle.width - saveWidth,
                            gameStateRectangle.y + gameStateRectangle.height + PADDING,
                            saveWidth, 30};

Interact *editFieldsList[] = {&editNameInteract,
                              &editTypeInteract,
                              &editIncreaseInteract,
                              &editDescInteract,
                              &editDependenciesInteract,
                              &editResourceInteract,
                              &editResourceInteract,
                              &editAmountInteract,
                              &editBoughtInteract};

struct DropDownMenu {
    char *destination;
    const char **items_list;
    int num_items;
    Rectangle rect;
    bool visible;
} DropDownMenu;

// GuiDMProperty prop[] = {
//     PBOOL("Bool", 0, true),
//     PSECTION("#102#SECTION", 0, 2),
//     PINT("Int", 0, 123),
//     PFLOAT("Float", 0, 0.99f),
//     PTEXT("Text", 0, (char*)&(char[30]){"Hello!"}, 30),
//     PSELECT("Select", 0, "ONE;TWO;THREE;FOUR", 0),
//     PINT_RANGE("Int Range", 0, 32, 1, 0, 100),
//     PRECT("Rect", 0, 0, 0, 100, 200),
//     PVEC2("Vec2", 0, 20, 20),
//     PVEC3("Vec3", 0, 12, 13, 14),
//     PVEC4("Vec4", 0, 12, 13, 14, 15),
//     PCOLOR("Color", 0, 0, 255, 0, 255),
// };

void initiateUI();

void buildUI();

void BuildSelectUpgradesList(char *newTypeList) {
    currentTypeList = newTypeList;
    amountSelectableUpgrades = 0;
    memset(selectUpgradesList, 0, sizeof(selectUpgradesList));
    for (int i = 0; i < numUpgrades; ++i) {
        if (AreStrEquals(state.upgrades_list[i].type, currentTypeList)) {
            Rectangle r = {anchorX + PADDING,
                           anchorY + PADDING + (TEXT_RECT_HEIGHT + PADDING) * amountSelectableUpgrades,
                           selectUpgradeRectWidth, TEXT_RECT_HEIGHT};

            Interact inter = {};
            inter.rect = r;
            inter.text = state.upgrades_list[i].id;
            inter.isHoverable = true;
            inter.isEditable = false;
            inter.fontSize = defaultFontSize;
            selectUpgradesList[amountSelectableUpgrades] = inter;
            amountSelectableUpgrades++;
        }
    }
    selectMainRect.height = (TEXT_RECT_HEIGHT + PADDING) * amountSelectableUpgrades + 2 * PADDING;
    addUpgradeButton.y = selectMainRect.y + selectMainRect.height + PADDING;
}

void GenerateDependencyListString() {
    memset(dependenciesStringToDisplay, 0, sizeof(dependenciesStringToDisplay));
    for (int i = 0; i < numberDependencies; ++i) {
        strcat(dependenciesStringToDisplay, listDependenciesString[i]);
        strcat(dependenciesStringToDisplay, "\n");
    }
    editDependenciesInteract.text = dependenciesStringToDisplay;
    editDependenciesInteract.rect.height = (TEXT_RECT_HEIGHT + PADDING) * numberDependencies;
    if (editDependenciesInteract.rect.height < TEXT_RECT_HEIGHT)
        editDependenciesInteract.rect.height = TEXT_RECT_HEIGHT;
}

void AddDependency(char *dependency) {
    // Add dependency to the list
    for (int i = 0; i < numberDependencies; ++i) {
        if (AreStrEquals(listDependenciesString[i], dependency)) return;
    }

    strcat(listDependenciesString[numberDependencies], dependency);
    memcpy(currentUpgradeToEdit->dependencies[numberDependencies], dependency, strlen(dependency));

    numberDependencies++;

    GenerateDependencyListString();
}

void RemoveDependency() {
    // int item_idx = (mousePosition.y - DropDownMenu.rect.y) / textRectHeight;
    int dep_to_remove_idx = (mousePosition.y - editDependenciesInteract.rect.y) / (TEXT_RECT_HEIGHT + PADDING);
    printf("Removing dependency %s, idx %d\n", listDependenciesString[dep_to_remove_idx], dep_to_remove_idx);

    for (int i = dep_to_remove_idx; i < numberDependencies; ++i) {
        memcpy(listDependenciesString[i], listDependenciesString[i + 1], sizeof(listDependenciesString[i + 1]));
        memcpy(currentUpgradeToEdit->dependencies[i], currentUpgradeToEdit->dependencies[i + 1],
               sizeof(currentUpgradeToEdit->dependencies[i + 1]));
    }

    numberDependencies--;

    // Format the string to be displayed on screen
    GenerateDependencyListString();
}

void AddNewUpgrade() {
    for (int i = 0; i < numUpgrades; ++i) {
        if (IsEmpty(state.upgrades_list[i].id)) {
            memcpy(state.upgrades_list[i].id, "New upgrade", strlen("New ugprade"));
            memcpy(state.upgrades_list[i].type, currentTypeList, strlen(currentTypeList));
            break; // Avoid create new upgrade on all empty slots
        }
    }
    BuildSelectUpgradesList(currentTypeList);
}

void RemoveUpgrade() {
    int selectUpIdx = 0;
    for (int i = 0; i < numUpgrades; ++i) {
        upgrade *u = &state.upgrades_list[i];
        if (AreStrEquals(u->type, currentTypeList)) {
            if (selectUpgradesList[selectUpIdx].isHovered) {
                *u = (upgrade) {0};
                BuildSelectUpgradesList(currentTypeList);
            }
            selectUpIdx++;
        }

    }
}

void AddResource(resource res) {

}

void RemoveResource() {

}


void OpenDropDownMenu(char *target, const char **items_list, int num_items, float anchorX, float anchorY) {
    // Set width of selector
    float width = 0;
    float length = 0;
    for (int i = 0; i < num_items; ++i) {
        length = MeasureText(items_list[i], FONT_SIZE);
        if (length > width) width = length;
    }
    width += PADDING * 2;

    DropDownMenu.destination = target;
    DropDownMenu.items_list = items_list;
    DropDownMenu.num_items = num_items;
    DropDownMenu.rect = (Rectangle) {anchorX, anchorY, width, TEXT_RECT_HEIGHT * num_items};
    DropDownMenu.visible = true;
}

void CloseDropDownMenu() {
    int item_idx = (mousePosition.y - DropDownMenu.rect.y) / TEXT_RECT_HEIGHT;
    memset(DropDownMenu.destination, 0, sizeof(currentUpgradeToEdit->type));
    memcpy(DropDownMenu.destination,
           DropDownMenu.items_list[item_idx],
           strlen(DropDownMenu.items_list[item_idx]));
}

void SelectCurrentUpgrade(upgrade *original) {
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


void DrawDropDownMenu() {
    Color c;
    for (int i = 0; i < DropDownMenu.num_items; ++i) {
        Rectangle r = {DropDownMenu.rect.x, DropDownMenu.rect.y + 30 * i,
                       DropDownMenu.rect.width, TEXT_RECT_HEIGHT};
        if (CheckCollisionPointRec(mousePosition, r)) c = WHITE;
        else c = LIGHTGRAY;
        DrawRectangleRec(r, c);
        DrawText(DropDownMenu.items_list[i],
                 DropDownMenu.rect.x + PADDING, DropDownMenu.rect.y + TEXT_RECT_HEIGHT * i + PADDING,
                 FONT_SIZE, MAROON);
    }
    DrawRectangleLinesEx(DropDownMenu.rect, 1, DARKGRAY);
}


void DrawGameState() {
    char speedStr[20];
    sprintf(speedStr, "%d", state.current_speed);
    speedInteract.text = speedStr;
    // DrawInteract(&speedInteract);
    DrawRectangleLinesEx(gameStateRectangle, 2, BLACK);

    char elapsedTime[20];
    sprintf(elapsedTime, "Elapsed time: %.1f", state.elapsed_time);
}

void DrawStaticContent() {
    int i;

    DrawRectangleLinesEx(addUpgradeButton, 2, BLACK);
    DrawText("+", addUpgradeButton.x + PADDING, addUpgradeButton.y + PADDING * 0.33, FONT_SIZE, MAROON);
    DrawGameState();
    int fieldsElem = (sizeof(editFieldsList) / sizeof(editFieldsList[0]));
    for (i = 0; i < fieldsElem; ++i) DrawInteract(editFieldsList[i]);
    for (i = 0; i < amountSelectableUpgrades; ++i) DrawInteract(&selectUpgradesList[i]);
    if (DropDownMenu.visible) {
        DrawDropDownMenu();
    }

    // GuiGroupBox(selectMainRect, "Upgrades");
    // GuiGroupBox(displayMainRect, "Edit upgrade");
    // GuiLabel(speedRect, "Speed");

    // int style = GuiGetStyle(SCROLLBAR, BORDER_WIDTH);
    // style = GuiCheckBox((Rectangle){ 565, 280, 20, 20 }, "ARROWS_VISIBLE", GuiGetStyle(SCROLLBAR, ARROWS_VISIBLE));
    // GuiSetStyle(SCROLLBAR, ARROWS_VISIBLE, style);


    // if (GuiButton(scienceRect, "Science")) {
    //     BuildSelectUpgradesList("science");
    // }

    // if (GuiButton(incrementalRect, "Incremental")) {
    //     BuildSelectUpgradesList("incremental");
    // }

    // if (GuiButton(structRect, "Structure")) {
    //     BuildSelectUpgradesList("structure");
    // }

    // if (GuiButton(softwareRect, "Software")) {
    //     BuildSelectUpgradesList("software");
    // }

    // if (GuiButton(saveRect, "Save")) {
    //     SaveGame();
    // };



}

void HandleMouseClick() {

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
        isAboutToRemoveItem = false;
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
        isAboutToAddNewUpgrade = (CheckCollisionPointRec(mousePosition, addUpgradeButton));
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

        if (CheckCollisionPointRec(mousePosition, editTypeInteract.rect)) {
            OpenDropDownMenu(editTypeInteract.text, typesList,
                             sizeof(typesList) / sizeof(typesList[0]),
                             editTypeInteract.rect.x, editTypeInteract.rect.y +
                                                      editTypeInteract.rect.height);
        } else if (CheckCollisionPointRec(mousePosition, editResourceInteract.rect)) {
            OpenDropDownMenu(editResourceInteract.text, resourcesList,
                             sizeof(resourcesList) / sizeof(resourcesList[0]),
                             editResourceInteract.rect.x, editResourceInteract.rect.y +
                                                          editResourceInteract.rect.height);
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
    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        if (isAboutToRemoveDependency) {
            RemoveDependency();
            isAboutToRemoveDependency = false;
        }
        if (isAboutToRemoveUpgrade) {
            RemoveUpgrade();
        }
    }
}

void HandleMouseOver() {

    int i;
    for (i = 0; i < amountSelectableUpgrades; ++i) {
        selectUpgradesList[i].isHovered = CheckCollisionPointRec(GetMousePosition(), selectUpgradesList[i].rect);
    }

    for (i = 0; i < (sizeof(editFieldsList) / sizeof(editFieldsList[0])); ++i) {
        Interact *editField = editFieldsList[i];
        editField->isHovered = CheckCollisionPointRec(mousePosition, editField->rect);
        if (editField->isHovered && editField->isEditable) {
            int cursorPos = strlen(editField->text);
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (cursorPos > 0) {
                    editField->text[cursorPos - 1] = '\0';
                }
            }
            while (keyPressed > 0) {
                if (AreStrEquals(editField->id, "increase")) {
                    if ((keyPressed >= 46) && (keyPressed <= 57)) {
                        editField->text[cursorPos] = (char) keyPressed;
                        editField->text[cursorPos + 1] = '\0';
                        currentUpgradeToEdit->increase_factor = atof(editIncreaseInteract.text);
                    }
                } else if (AreStrEquals(editField->id, "amount")) {
                    if ((keyPressed >= 48) && (keyPressed <= 57)) {
                        editField->text[cursorPos] = (char) keyPressed;
                        editField->text[cursorPos + 1] = '\0';
                        currentUpgradeToEdit->initial_price.resources[0].amount = strtol(editAmountInteract.text, NULL,
                                                                                         10);
                    }
                } else if (AreStrEquals(editField->id, "bought")) {
                    if ((keyPressed >= 48) && (keyPressed <= 57)) {
                        editField->text[cursorPos] = (char) keyPressed;
                        editField->text[cursorPos + 1] = '\0';
                        currentUpgradeToEdit->amount_bought = strtol(editBoughtInteract.text, NULL, 10);
                    }
                } else if ((keyPressed >= 32) && (keyPressed <= 125)) {
                    editField->text[cursorPos] = (char) keyPressed;
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


void DrawConfigPanel(float panelWidth, float panelHeight) {
    if (!initDone) {
        initiateUI();
    }

    HandleMouseOver();
    HandleMouseClick();
    DrawStaticContent();


}

void initiateUI() {
    buildUI();

    BuildSelectUpgradesList("structure");
    SelectCurrentUpgrade(&state.upgrades_list[0]);
    font = GetFontDefault();
    for (int i = 0; i < sizeof(editFieldsList) / sizeof(editFieldsList[0]); ++i) {
        editFieldsList[i]->fontSize = defaultFontSize;
    }
    initDone = true;
}

void buildUI() {
    float displayRectXAnchor = anchorX + MAX_UPGRADE_NAME_LENGTH + 20 * SCALE_FACTOR;
    float displayRectYAnchor = anchorY;
    displayNameRect = {displayRectXAnchor + PADDING,
                                       displayRectYAnchor + PADDING,
                                       selectMainRectWidth, TEXT_RECT_HEIGHT};
    displayTypeRect = {displayNameRect.x + displayNameRect.width + PADDING,
                                       displayRectYAnchor + PADDING, 130 * SCALE_FACTOR, TEXT_RECT_HEIGHT};
    displayIncreaseRect = {displayTypeRect.x + displayTypeRect.width + PADDING,
                                           displayRectYAnchor + PADDING, 50 * SCALE_FACTOR, TEXT_RECT_HEIGHT};
    displayDescRect = {displayRectXAnchor + PADDING, displayRectYAnchor + PADDING + 40 * SCALE_FACTOR,
                                       displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width +
                                       2 * PADDING, TEXT_RECT_HEIGHT * 2};
    displayDependRect = {displayRectXAnchor + PADDING,
                                         displayDescRect.y + displayDescRect.height + PADDING,
                                         displayNameRect.width, TEXT_RECT_HEIGHT};
    displayResourceRect = {displayTypeRect.x,
                                           displayDependRect.y,
                                           displayTypeRect.width, TEXT_RECT_HEIGHT};
    displayAmountRect = {displayIncreaseRect.x,
                                         displayDependRect.y,
                                         displayIncreaseRect.width, TEXT_RECT_HEIGHT};
    displayBoughtRect = {displayResourceRect.x,
                                         displayDependRect.y + displayDependRect.height + PADDING * 2,
                                         50, displayNameRect.height};
    displayMainRectWidth =
            displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 4 * PADDING;
    displayMainRectHeight = 300.0f * SCALE_FACTOR;
    displayMainRect = {displayRectXAnchor, displayRectYAnchor,
                                       displayMainRectWidth, displayMainRectHeight};

    editNameInteract = (Interact){"name", "name", displayNameRect, "", false, true};
    editTypeInteract = (Interact){"type", "type", displayTypeRect, "", false, false};
    editIncreaseInteract = (Interact){"increase", "increase", displayIncreaseRect, "", false, true};
    editDescInteract = (Interact){"description", "description", displayDescRect, "", false, true};
    editDependenciesInteract = (Interact){"dependencies", "dependencies", displayDependRect, "", false, false};
    editResourceInteract = (Interact){"resource", "resource", displayResourceRect, "", false, false};
    editAmountInteract = (Interact){"amount", "amount", displayAmountRect, "", false, true};
    editBoughtInteract = (Interact){"bought", "bought", displayBoughtRect, "", false, true};
}
