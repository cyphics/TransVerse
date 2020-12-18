/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */
#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gui_config.h"
#include "../helper.h"
#include "gui.h"
#include "gui_constants.h"
#include "gui_tools.h"
#include "../physics.h"
#include "config/gui_cheat.h"

Font font;
float fontSize = FONT_SIZE;
Vector2 mousePosition;
int keyPressed;

//extern struct GameState game_state;

float anchorX;
float anchorY;

Rectangle configTabRect;
char *configTabEntries[2] = {"Config", "Cheat"};
int configTabSelectedEntry = 1;



//const float FONT_SIZE = defaultFontSize * SCALE_FACTOR;
const int numUpgrades = MAX_UPGRADES_AMOUNT;

// Common
bool initDone = false;

bool isAboutToRemoveDependency;
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

const float addButtonWidth = 20;
Rectangle addUpgradeButton;
bool isAboutToAddNewUpgrade = false;
bool isAboutToRemoveUpgrade = false;

char *currentTypeList = "structure";
int amountSelectableUpgrades = 0;
Interact selectUpgradesList[MAX_UPGRADES_AMOUNT] = {};
bool isAboutToSelect = false;

// Upgrade edit panel
upgrade *currentUpgradeToEdit;

Rectangle displayNameRect;
Rectangle displayTypeRect;
Rectangle displayIncreaseRect;
Rectangle displayDescRect;
Rectangle displayDependRect;
Rectangle displayResourceRect;
Rectangle displayAmountRect;
Rectangle displayBoughtRect;
Rectangle displayMainRect;

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
char *typesList[] = {"science", "incremental", "structure", "software"};
char *resourcesList[] = {"energy", "code", "software", "copper", "steel"};

Rectangle gameStateRectangle;
Rectangle speedRect;
Interact speedInteract;
Rectangle saveRect;

Interact *editFieldsList[] = {&editNameInteract,
                              &editTypeInteract,
                              &editIncreaseInteract,
                              &editDescInteract,
                              &editDependenciesInteract,
                              &editResourceInteract,
                              &editResourceInteract,
                              &editAmountInteract,
                              &editBoughtInteract};

void BuildSelectUpgradesList(char *newTypeList) {
    currentTypeList = newTypeList;
    amountSelectableUpgrades = 0;
    memset(selectUpgradesList, 0, sizeof(selectUpgradesList));
    for (int i = 0; i < numUpgrades; ++i) {
        if (AreStrEquals(game_state.upgrades_list[i].type, currentTypeList)) {
            Rectangle r = {anchorX + PADDING,
                           anchorY + PADDING + (TEXT_RECT_HEIGHT + PADDING) * amountSelectableUpgrades,
                           selectUpgradeRectWidth, TEXT_RECT_HEIGHT};

            Interact inter = {};
            inter.rect = r;
            inter.text = game_state.upgrades_list[i].id;
            inter.isHoverable = true;
            inter.isEditable = false;
            inter.fontSize = fontSize;
            selectUpgradesList[amountSelectableUpgrades] = inter;
            amountSelectableUpgrades++;
        }
    }
//    selectMainRect.height = (TEXT_RECT_HEIGHT + PADDING) * amountSelectableUpgrades + 2 * PADDING;
    addUpgradeButton.y = anchorY + (TEXT_RECT_HEIGHT + PADDING) * amountSelectableUpgrades + 2 * PADDING + PADDING;
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
        if (IsEmpty(game_state.upgrades_list[i].id)) {
            memcpy(game_state.upgrades_list[i].id, "New upgrade", strlen("New ugprade"));
            memcpy(game_state.upgrades_list[i].type, currentTypeList, strlen(currentTypeList));
            break; // Avoid create new upgrade on all empty slots
        }
    }
    BuildSelectUpgradesList(currentTypeList);
}

void RemoveUpgrade() {
    int selectUpIdx = 0;
    for (int i = 0; i < numUpgrades; ++i) {
        upgrade *u = &game_state.upgrades_list[i];
        if (AreStrEquals(u->type, currentTypeList)) {
            if (selectUpgradesList[selectUpIdx].isHovered) {
                *u = (upgrade) {0};
                BuildSelectUpgradesList(currentTypeList);
            }
            selectUpIdx++;
        }

    }
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

void DrawGameState() {
    char speedStr[20];
    sprintf(speedStr, "%d", game_state.current_speed);
    speedInteract.text = speedStr;
    // DrawInteract(&speedInteract);
    DrawRectangleLinesEx(gameStateRectangle, 2, BLACK);

    char elapsedTime[20];
    sprintf(elapsedTime, "Elapsed time: %.1f", game_state.elapsed_time);
}



void BuildUI(int posX, int poxY) {
    anchorY = poxY;
    anchorX = posX;
    configTabRect = (Rectangle) {posX + PADDING, poxY + PADDING, 300, 30};



    // CONFIG
    float displayRectXAnchor = posX + MAX_UPGRADE_NAME_LENGTH + 20 * SCALE_FACTOR;
    float displayRectYAnchor = poxY;

    displayNameRect = (Rectangle) {displayRectXAnchor + PADDING,
                                   displayRectYAnchor + PADDING,
                                   selectMainRectWidth, TEXT_RECT_HEIGHT};
    displayTypeRect = (Rectangle) {displayNameRect.x + displayNameRect.width + PADDING,
                                   displayRectYAnchor + PADDING, 130 * SCALE_FACTOR, TEXT_RECT_HEIGHT};
    displayIncreaseRect = (Rectangle) {displayTypeRect.x + displayTypeRect.width + PADDING,
                                       displayRectYAnchor + PADDING, 50 * SCALE_FACTOR, TEXT_RECT_HEIGHT};
    displayDescRect = (Rectangle) {displayRectXAnchor + PADDING, displayRectYAnchor + PADDING + 40 * SCALE_FACTOR,
                                   displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width +
                                   2 * PADDING, TEXT_RECT_HEIGHT * 2};
    displayDependRect = (Rectangle) {displayRectXAnchor + PADDING,
                                     displayDescRect.y + displayDescRect.height + PADDING,
                                     displayNameRect.width, TEXT_RECT_HEIGHT};
    displayResourceRect = (Rectangle) {displayTypeRect.x,
                                       displayDependRect.y,
                                       displayTypeRect.width, TEXT_RECT_HEIGHT};
    displayAmountRect = (Rectangle) {displayIncreaseRect.x,
                                     displayDependRect.y,
                                     displayIncreaseRect.width, TEXT_RECT_HEIGHT};
    displayBoughtRect = (Rectangle) {displayResourceRect.x,
                                     displayDependRect.y + displayDependRect.height + PADDING * 2,
                                     50, displayNameRect.height};
    float displayMainRectWidth =
            displayNameRect.width + displayTypeRect.width + displayIncreaseRect.width + 4 * PADDING;
    float displayMainRectHeight = 300.0f * SCALE_FACTOR;
    displayMainRect = (Rectangle) {displayRectXAnchor, displayRectYAnchor,
                                   displayMainRectWidth, displayMainRectHeight};

    editNameInteract = (Interact) {"name", "name", displayNameRect, "", false, true};
    editTypeInteract = (Interact) {"type", "type", displayTypeRect, "", false, false};
    editIncreaseInteract = (Interact) {"increase", "increase", displayIncreaseRect, "", false, true};
    editDescInteract = (Interact) {"description", "description", displayDescRect, "", false, true};
    editDependenciesInteract = (Interact) {"dependencies", "dependencies", displayDependRect, "", false, false};
    editResourceInteract = (Interact) {"resource", "resource", displayResourceRect, "", false, false};
    editAmountInteract = (Interact) {"amount", "amount", displayAmountRect, "", false, true};
    editBoughtInteract = (Interact) {"bought", "bought", displayBoughtRect, "", false, true};

    gameStateRectangle = (Rectangle) {posX, displayMainRect.y + displayMainRect.height + PADDING,
                                      selectMainRectWidth + displayMainRectWidth + PADDING * 2, 200
    };

    speedRect = (Rectangle) {gameStateRectangle.x + PADDING,
                             gameStateRectangle.y + PADDING,
                             60 * SCALE_FACTOR,
                             TEXT_RECT_HEIGHT};
    speedInteract = (Interact) {"speed", "Speed", speedRect, "", false, true, .fontSize = fontSize};

    const float saveWidth = 100;
    saveRect = (Rectangle) {gameStateRectangle.x + gameStateRectangle.width - saveWidth,
                            gameStateRectangle.y + gameStateRectangle.height + PADDING,
                            saveWidth, 30};
    addUpgradeButton = (Rectangle) {posX, 0,
                                    addButtonWidth, addButtonWidth};
}


void InitConfigUI(Vector2 anchor) {
    InitCheatUI(anchor.x + 40, anchor.y + 80);
    BuildUI(anchor.x, anchor.y);

    BuildSelectUpgradesList("structure");
    SelectCurrentUpgrade(&game_state.upgrades_list[0]);
    font = GetFontDefault();
    for (int i = 0; i < sizeof(editFieldsList) / sizeof(editFieldsList[0]); ++i) {
        editFieldsList[i]->fontSize = fontSize;
    }
    initDone = true;
}

void DrawConfigPanel() {
    GuiTabs(configTabRect, configTabEntries, sizeof(configTabEntries) / sizeof(configTabEntries[0]), &configTabSelectedEntry);

    switch (configTabSelectedEntry) {
        case 0:
//            DrawStaticContent();
            break;
        case 1:
            DrawCheatsTab();
            break;
        default:
            break;
    }
}
