/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <raylib.h>
#include <time.h>

#include "gui.h"
#include "game.h"
#include "game_handler.h"
#include "helper.cpp"
#include "gui_code.cpp"

bool isGameGuiInit = false;
float interfaceAnchorX = 800.0f;
float interfaceAnchorY = 0;

extern GameState state;
extern HandleList AvailableUpgrades;
extern Vector2 mousePosition;
extern bool mouseLeftPressed;
extern float fontSize;

int currentPanel = EDITOR_TAB;

float tabHeight = 30;


Rectangle mainPanelRect = {interfaceAnchorX + padding, interfaceAnchorY + padding,
                           800 - padding * 2, 500 - padding * 2};
Rectangle subPanelRect = {interfaceAnchorX + padding, interfaceAnchorY + padding + tabHeight + padding * 2,
                          800 - padding * 2, 560 - padding * 2};


Interact terminalTab {"termTab", "",
                      (Rectangle){mainPanelRect.x + padding, mainPanelRect.y + padding, 100, tabHeight},
                      "Terminal", true};
Interact editorTab {"editor", "", (Rectangle){terminalTab.rect.x + terminalTab.rect.width + padding, mainPanelRect.
                                              y + padding, 100, tabHeight},
                    "Editor", true};
Interact shipTab {"ship", "", (Rectangle){editorTab.rect.x + editorTab.rect.width + padding, mainPanelRect.y + padding, 100, tabHeight},
                  "Ship", true};
Interact workshopTab {"workshop", "", (Rectangle){shipTab.rect.x + shipTab.rect.width + padding, mainPanelRect.y + padding, 120, tabHeight},
                      "Workshop", true};

float availBoxWidth = 150;
float availBoxHeight = 150;

Rectangle availUpgradesRect = {subPanelRect.x + padding, subPanelRect.y + 30,
                         availBoxWidth, availBoxHeight};
Rectangle availStruct = {availUpgradesRect.x, availUpgradesRect.y,
                         availBoxWidth, availBoxHeight};
Rectangle availScience = {availUpgradesRect.x, availUpgradesRect.y,
                          availBoxWidth, availBoxHeight};
Rectangle availSoft = {availUpgradesRect.x, availUpgradesRect.y,
                       availBoxWidth, availBoxHeight};
Rectangle availIncrement = {availUpgradesRect.x, availUpgradesRect.y,
                            availBoxWidth, availBoxHeight};

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


void RefreshUpgradesLists()
{
    structCount = 0;
    scienceCount = 0;
    softCount = 0;
    incrementCount = 0;

    UpdateAvailableUpgrades();
    memset(availStructUpgrades, 0, sizeof(availStructUpgrades));
    memset(availScienceUpgrades, 0, sizeof(availScienceUpgrades));
    memset(availSoftUpgrades, 0, sizeof(availSoftUpgrades));
    memset(availIncrementUpgrades, 0, sizeof(availIncrementUpgrades));

    for (int i = 0; i < AvailableUpgrades.size; ++i) {
        UPGRADE handle = AvailableUpgrades.list[i];
        UpgradeInfo info = {0};
        GetInfo(handle, &info);
        if (AreStrEquals(info.type, "structure")) {
            availStructUpgrades[structCount] = info;
            availStructInter[structCount] =
                {"", "",
                 {availUpgradesRect.x + padding,
                  availUpgradesRect.y + padding +  (textRectHeight / 2 + padding) * structCount,
                  availUpgradesRect.width - 2 * padding, textRectHeight / 2},
                 availStructUpgrades[structCount].name, true, false};
            availStructInter[structCount].fontSize /= 2;
            structCount++;
        }
        if (AreStrEquals(info.type, "science")) {
            availScienceUpgrades[scienceCount] = info;
            printf("Sicence: %s\n", info.name);
            availScienceInter[scienceCount] =
                {"", "",
                 {availScience.x + padding,
                  availScience.y + padding +  (textRectHeight / 2 + padding) * scienceCount,
                  availScience.width - 2 * padding, textRectHeight / 2},
                 availScienceUpgrades[scienceCount].name, true, false};
            availScienceInter[scienceCount].fontSize /= 2;
            scienceCount++;
        }
        if (AreStrEquals(info.type, "software")) {
            availSoftUpgrades[softCount] = info;
            availSoftInter[softCount] =
                {"", "",
                 {availSoft.x + padding,
                  availSoft.y + padding +  (textRectHeight / 2 + padding) * softCount,
                  availSoft.width - 2 * padding, textRectHeight / 2},
                 availSoftUpgrades[softCount].name, true, false};
            availSoftInter[softCount].fontSize /= 2;
            softCount++;
        }
        if (AreStrEquals(info.type, "incremental")) {
            availIncrementUpgrades[incrementCount] = info;
            availIncrementInter[incrementCount] =
                {"", "",
                 {availIncrement.x + padding,
                  availIncrement.y + padding +  (textRectHeight / 2 + padding) * incrementCount,
                  availIncrement.width - 2 * padding, textRectHeight / 2},
                 availIncrementUpgrades[incrementCount].name, true, false};
            availIncrementInter[incrementCount].fontSize /= 2;
            incrementCount++;
        }
    }


}


void HandleMouseHoveredGame()
{
    int gesture = GetGestureDetected();

    terminalTab.isHovered = (CheckCollisionPointRec(mousePosition, terminalTab.rect));
    editorTab.isHovered = (CheckCollisionPointRec(mousePosition, editorTab.rect));
    shipTab.isHovered = (CheckCollisionPointRec(mousePosition, shipTab.rect));
    workshopTab.isHovered = (CheckCollisionPointRec(mousePosition, workshopTab.rect));
    for (int i = 0; i < structCount; ++i) {
        availStructInter[i].isHovered = (CheckCollisionPointRec(mousePosition, availStructInter[i].rect));
    }
    for (int i = 0; i < scienceCount; ++i) {
        availScienceInter[i].isHovered = (CheckCollisionPointRec(mousePosition, availScienceInter[i].rect));
    }
    for (int i = 0; i < softCount; ++i) {
        availSoftInter[i].isHovered = (CheckCollisionPointRec(mousePosition, availSoftInter[i].rect));
    }
    for (int i = 0; i < incrementCount; ++i) {
        availIncrementInter[i].isHovered = (CheckCollisionPointRec(mousePosition, availIncrementInter[i].rect));
    }


}
void HandleMouseClickGame()
{
    if (mouseLeftPressed) {
        if (CheckCollisionPointRec(mousePosition, editorTab.rect)) {
            ResizeEditor(availUpgradesRect.x + availUpgradesRect.width + padding * 4, availUpgradesRect.y);
            currentPanel = EDITOR_TAB;
        }

        if (CheckCollisionPointRec(mousePosition, terminalTab.rect)) {
            currentPanel = TERMINAL_TAB;
        }
        if (CheckCollisionPointRec(mousePosition, shipTab.rect)) {
            currentPanel = SHIP_TAB;
        }
        if (CheckCollisionPointRec(mousePosition, workshopTab.rect)) {
            currentPanel = WORKSHOP_TAB;
        }
    }


}



void DrawEditor()
{
    DrawText("Available softwares", availSoft.x + padding, availStruct.y - 10, fontSize / 2, DARKGRAY);
    DrawRectangleLinesEx(availSoft, 1, BLACK);
    for (int i = 0; i < softCount; ++i) {
        DrawInteract(&availSoftInter[i]);
    }
    TypeCode();
}

void DrawShip()
{
    DrawText("Incremental", availIncrement.x + padding, availStruct.y - 10, fontSize / 2, DARKGRAY);
    DrawRectangleLinesEx(availIncrement, 1, BLACK);
    for (int i = 0; i < incrementCount; ++i) {
        DrawInteract(&availIncrementInter[i]);
    }
}

void DrawWorkshop()
{
    DrawText("Structure", availStruct.x + padding, availStruct.y - 10, fontSize / 2, DARKGRAY);
    DrawRectangleLinesEx(availStruct, 1, BLACK);
    for (int i = 0; i < structCount; ++i) {
        DrawInteract(&availStructInter[i]);
    }
}

void DrawTerminal()
{
    DrawText("Science", availScience.x + padding, availStruct.y - 10, fontSize / 2, DARKGRAY);
    DrawRectangleLinesEx(availScience, 1, BLACK);
    for (int i = 0; i < scienceCount; ++i) {
        DrawInteract(&availScienceInter[i]);
    }


}


void DrawStaticInterface()
{
    DrawRectangleLinesEx(subPanelRect, 2, BLACK);
    DrawInteract(&terminalTab);
    DrawInteract(&editorTab);
    DrawInteract(&shipTab);
    DrawInteract(&workshopTab);
    switch (currentPanel) {
    case TERMINAL_TAB: {
        DrawTerminal();
        break;
    }
    case EDITOR_TAB:{
        DrawEditor();
        break;
    }
    case WORKSHOP_TAB:{
        DrawWorkshop();
        break;
    }
    case SHIP_TAB:{
        DrawShip();
        break;
    }

    default:
        break;
    }
}


void DrawGame()
{
    if (!isGameGuiInit) {
        ResizeEditor(availUpgradesRect.x + availUpgradesRect.width + padding * 4, availUpgradesRect.y);
        RefreshUpgradesLists();
        isGameGuiInit = true;
    }

    HandleMouseHoveredGame();
    HandleMouseClickGame();
    DrawStaticInterface();

    // DrawAnimation();
    // TypeCode();
    // DrawConfigMenu();
}
