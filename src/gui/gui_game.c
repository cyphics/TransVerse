/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <raylib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "gui.h"
#include "../game.h"
#include "../game_handler.h"
#include "../helper.h"
//#include "gui_code.h"
#include "gui_config.h"
#include "gui_game.h"
#include "gui_constants.h"
#include "gui_tools.h"

bool isGameGuiInit = false;

extern Vector2 mousePosition;
bool mouseLeftPressed = false;

int currentPanel = EDITOR_TAB;

const float tabHeight = 30;

Rectangle typesTabRect;
char *typesTabsLabels[5] = {"Terminal", "Editor", "Ship", "Workshop", "Engine"};
int typesSelectedTab = 4;

Rectangle terminalRect;
Rectangle editorRect;
Rectangle shipRect;
Rectangle workshopRect;

const float availBoxWidth = 150;
const float availBoxHeight = 150;

Rectangle availUpgradesRect;
Rectangle availStruct;
Rectangle availScience;
Rectangle availSoft;
Rectangle availIncrement;

#define max_available_ugprade_per_type 5

UpgradeInfo availStructUpgrades[max_available_ugprade_per_type];
int structCount = 0;
UpgradeInfo availScienceUpgrades[max_available_ugprade_per_type];
int scienceCount = 0;
UpgradeInfo availSoftUpgrades[max_available_ugprade_per_type];
int softCount = 0;
UpgradeInfo availIncrementUpgrades[max_available_ugprade_per_type];
int incrementCount = 0;
Interact availStructInter[max_available_ugprade_per_type] = {};
Interact availScienceInter[max_available_ugprade_per_type] = {};
Interact availSoftInter[max_available_ugprade_per_type] = {};
Interact availIncrementInter[max_available_ugprade_per_type] = {};
Rectangle mainPanelRect;
Rectangle subPanelRect;

Rectangle engineDataRect;
Rectangle engineMilestonesRect;

void RefreshUpgradesLists() {
    structCount = 0;
    scienceCount = 0;
    softCount = 0;
    incrementCount = 0;

    UpdateAvailableUpgrades();
    memset(availStructUpgrades, 0, sizeof(availStructUpgrades));
    memset(availScienceUpgrades, 0, sizeof(availScienceUpgrades));
    memset(availSoftUpgrades, 0, sizeof(availSoftUpgrades));
    memset(availIncrementUpgrades, 0, sizeof(availIncrementUpgrades));
    Interact inter;
    Rectangle r;
    for (int i = 0; i < AvailableUpgrades.size; ++i) {
        UPGRADE handle = AvailableUpgrades.list[i];
        UpgradeInfo info = {0};
        GetInfo(handle, &info);
        if (AreStrEquals(info.type, "structure")) {
            availStructUpgrades[structCount] = info;
            r = (Rectangle) {availUpgradesRect.x + PADDING,
                             availUpgradesRect.y + PADDING + (TEXT_RECT_HEIGHT / 2 + PADDING) * structCount,
                             availUpgradesRect.width - 2 * PADDING,
                             TEXT_RECT_HEIGHT / 2};
            inter = (Interact) {};
            inter.rect = r;
            inter.text = availStructUpgrades[structCount].name;
            inter.isHoverable = true;
            inter.isEditable = false;
            inter.fontSize = FONT_SIZE / 2;
            availStructInter[structCount] = inter;
            structCount++;
        }
        if (AreStrEquals(info.type, "science")) {
            availScienceUpgrades[scienceCount] = info;
            r = (Rectangle) {availScience.x + PADDING,
                             availScience.y + PADDING + (TEXT_RECT_HEIGHT / 2 + PADDING) * scienceCount,
                             availScience.width - 2 * PADDING,
                             TEXT_RECT_HEIGHT / 2};
            inter = (Interact) {};
            inter.rect = r;
            inter.text = availScienceUpgrades[scienceCount].name;
            inter.isHoverable = true;
            inter.isEditable = false;
            inter.fontSize = FONT_SIZE / 2;
            availScienceInter[scienceCount] = inter;
            scienceCount++;
        }
        if (AreStrEquals(info.type, "software")) {
            availSoftUpgrades[softCount] = info;
            Rectangle r = (Rectangle) {availSoft.x + PADDING,
                                       availSoft.y + PADDING + (TEXT_RECT_HEIGHT / 2 + PADDING) * softCount,
                                       availSoft.width - 2 * PADDING, TEXT_RECT_HEIGHT / 2};
            inter = (Interact) {};
            inter.rect = r;
            inter.text = availSoftUpgrades[softCount].name;
            inter.isHoverable = true;
            inter.isEditable = false;
            inter.fontSize = FONT_SIZE / 2;
            availSoftInter[softCount] = inter;
            softCount++;
        }
        if (AreStrEquals(info.type, "incremental")) {
            availIncrementUpgrades[incrementCount] = info;
            Rectangle r = (Rectangle) {availIncrement.x + PADDING,
                                       availIncrement.y + PADDING + (TEXT_RECT_HEIGHT / 2 + PADDING) * incrementCount,
                                       availIncrement.width - 2 * PADDING, TEXT_RECT_HEIGHT / 2};
            inter = (Interact) {};
            inter.text = availIncrementUpgrades[incrementCount].name;
            inter.isHoverable = true;
            inter.isEditable = false;
            inter.fontSize = FONT_SIZE / 2;
            availIncrementInter[incrementCount] = inter;
            incrementCount++;
        }
    }


}

void DrawEditor() {
    DrawText("Available softwares", availSoft.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availSoft, 1, BLACK);
//    for (int i = 0; i < softCount; ++i) {
//        DrawInteract(&availSoftInter[i]);
//    }
//    TypeCode();
}

void DrawShip() {
    DrawText("Incremental", availIncrement.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availIncrement, 1, BLACK);
//    for (int i = 0; i < incrementCount; ++i) {
//        DrawInteract(&availIncrementInter[i]);
//    }
}

void DrawWorkshop() {
    DrawText("Structure", availStruct.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availStruct, 1, BLACK);
//    for (int i = 0; i < structCount; ++i) {
//        DrawInteract(&availStructInter[i]);
//    }
}

void DrawTerminal() {
    DrawText("Science", availScience.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availScience, 1, BLACK);
//    for (int i = 0; i < scienceCount; ++i) {
//        DrawInteract(&availScienceInter[i]);
//    }
}

void DrawEngine() {
    DrawRectangleLinesEx(engineDataRect, 1, BLACK);
    int v_space = 20;
    int x_pos = engineMilestonesRect.x + PADDING;
    char currentSpeed[20];
    char currentAcceleration[20];
    sprintf(currentSpeed, "%.3f", (float)GetCurrentSpeed());
    sprintf(currentAcceleration, "%.3f", (float)GetCurrentAcceleration());
    GuiLabelBox(x_pos, engineDataRect.y + PADDING, "Speed", currentSpeed);
    GuiLabelBox(x_pos, engineDataRect.y + v_space + PADDING, "Distance", "0m");
    GuiLabelBox(x_pos, engineDataRect.y + (v_space + PADDING) * 2, "Acceleration", currentAcceleration);

    DrawRectangleLinesEx(engineMilestonesRect, 1, BLACK);
}

void InitGameUI(int posX, int posY) {

    mainPanelRect = (Rectangle) {posX + PADDING, posY + PADDING,
                                 800 - PADDING * 2, 500 - PADDING * 2};
    subPanelRect = (Rectangle) {posX + PADDING, posY + PADDING + tabHeight + PADDING * 2,
                                800 - PADDING * 2, 560 - PADDING * 2};

    engineDataRect = (Rectangle) {subPanelRect.x + PADDING, subPanelRect.y + PADDING,
                                  300, 100};
    engineMilestonesRect = (Rectangle) {engineDataRect.x, engineDataRect.y + engineDataRect.height + PADDING,
                                        300, 400};


    typesTabRect = (Rectangle) {mainPanelRect.x + PADDING,
                            mainPanelRect.y + PADDING,
                                600, tabHeight};
    terminalRect = (Rectangle) {mainPanelRect.x + PADDING,
                                mainPanelRect.y + PADDING,
                                100, tabHeight};
    editorRect = (Rectangle) {terminalRect.x + terminalRect.width + PADDING,
                              mainPanelRect.y + PADDING,
                              100, tabHeight};
    shipRect = (Rectangle) {editorRect.x + editorRect.width + PADDING,
                            mainPanelRect.y + PADDING,
                            100, tabHeight};
    workshopRect = (Rectangle) {shipRect.x + shipRect.width + PADDING,
                                mainPanelRect.y + PADDING,
                                120, tabHeight};

    availUpgradesRect = (Rectangle) {subPanelRect.x + PADDING, subPanelRect.y + 30,
                                     availBoxWidth, availBoxHeight};
    availStruct = (Rectangle) {availUpgradesRect.x, availUpgradesRect.y,
                               availBoxWidth, availBoxHeight};
    availScience = (Rectangle) {availUpgradesRect.x, availUpgradesRect.y,
                                availBoxWidth, availBoxHeight};
    availSoft = (Rectangle) {availUpgradesRect.x, availUpgradesRect.y,
                             availBoxWidth, availBoxHeight};
    availIncrement = (Rectangle) {availUpgradesRect.x, availUpgradesRect.y,
                                  availBoxWidth, availBoxHeight};


//    ResizeEditor(availUpgradesRect.x + availUpgradesRect.width + padding * 4, availUpgradesRect.y);
    RefreshUpgradesLists();
    isGameGuiInit = true;
}

void DrawGame(int posX, int posY) {
    static char some_text[100] = "Some text";
    static bool isPressed = false;

    if (!isGameGuiInit) {
        InitGameUI(posX, posY);
    }

    typesSelectedTab = GuiTabs(typesTabRect, typesTabsLabels, sizeof(typesTabsLabels) / sizeof(typesTabsLabels[0]), typesSelectedTab);
    switch (typesSelectedTab) {
        case 0: {
            DrawTerminal();
            break;
        }
        case 1: {
            DrawEditor();
            break;
        }
        case 2: {
            DrawShip();
            break;
        }
        case 3: {
            DrawWorkshop();
            break;
        }
        case 4: {
            DrawEngine();
        }
        default:
            break;
    }

    DrawRectangleLinesEx(subPanelRect, 2, BLACK);
}
