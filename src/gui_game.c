/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <raylib.h>
#include <time.h>
#include <string.h>

#include "gui.h"
#include "game.h"
#include "game_handler.h"
#include "helper.h"
//#include "gui_code.h"
#include "gui_config.h"
#include "gui_game.h"
#include "gui_constants.h"

Rectangle mainPanelRect ;
Rectangle subPanelRect ;


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
    Interact inter;
    Rectangle r;
    for (int i = 0; i < AvailableUpgrades.size; ++i) {
        UPGRADE handle = AvailableUpgrades.list[i];
        UpgradeInfo info = {0};
        GetInfo(handle, &info);
        if (AreStrEquals(info.type, "structure")) {
            availStructUpgrades[structCount] = info;
            r = (Rectangle){availUpgradesRect.x + PADDING,
                            availUpgradesRect.y + PADDING + (TEXT_RECT_HEIGHT / 2 + PADDING) * structCount,
                            availUpgradesRect.width - 2 * PADDING,
                            TEXT_RECT_HEIGHT / 2};
            inter = (Interact){};
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
            r = (Rectangle){availScience.x + PADDING,
                            availScience.y + PADDING + (TEXT_RECT_HEIGHT / 2 + PADDING) * scienceCount,
                            availScience.width - 2 * PADDING,
                            TEXT_RECT_HEIGHT / 2};
            inter = (Interact){};
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
            Rectangle r = (Rectangle){availSoft.x + PADDING,
                                      availSoft.y + PADDING + (TEXT_RECT_HEIGHT / 2 + PADDING) * softCount,
                                      availSoft.width - 2 * PADDING, TEXT_RECT_HEIGHT / 2};
            inter = (Interact){};
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
            Rectangle r = (Rectangle){availIncrement.x + PADDING,
                                      availIncrement.y + PADDING + (TEXT_RECT_HEIGHT / 2 + PADDING) * incrementCount,
                                      availIncrement.width - 2 * PADDING, TEXT_RECT_HEIGHT / 2};
            inter = (Interact){};
            inter.text = availIncrementUpgrades[incrementCount].name;
            inter.isHoverable = true;
            inter.isEditable = false;
            inter.fontSize = FONT_SIZE / 2;
            availIncrementInter[incrementCount] = inter;
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
//            ResizeEditor(availUpgradesRect.x + availUpgradesRect.width + padding * 4, availUpgradesRect.y);
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
    DrawText("Available softwares", availSoft.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availSoft, 1, BLACK);
    for (int i = 0; i < softCount; ++i) {
        DrawInteract(&availSoftInter[i]);
    }
//    TypeCode();
}

void DrawShip()
{
    DrawText("Incremental", availIncrement.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availIncrement, 1, BLACK);
    for (int i = 0; i < incrementCount; ++i) {
        DrawInteract(&availIncrementInter[i]);
    }
}

void DrawWorkshop()
{
    DrawText("Structure", availStruct.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availStruct, 1, BLACK);
    for (int i = 0; i < structCount; ++i) {
        DrawInteract(&availStructInter[i]);
    }
}

void DrawTerminal()
{
    DrawText("Science", availScience.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
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

void InitGameUI()
{
    mainPanelRect  = (Rectangle) {interfaceAnchorX + PADDING, interfaceAnchorY + PADDING,
                                  800 - PADDING * 2, 500 - PADDING * 2};
    subPanelRect = (Rectangle){interfaceAnchorX + PADDING, interfaceAnchorY + PADDING + tabHeight + PADDING * 2,
                    800 - PADDING * 2, 560 - PADDING * 2};
    Rectangle terminalRect = {mainPanelRect.x + PADDING,
                          mainPanelRect.y + PADDING,
                              100, tabHeight};
    terminalTab = (Interact){"termTab", "", terminalRect, "Terminal", true};
    Rectangle editorRect = {terminalTab.rect.x + terminalTab.rect.width + PADDING,
                            mainPanelRect.y + PADDING,
                            100, tabHeight};
    editorTab = (Interact){"editor", "", editorRect, "Editor", true};
    Rectangle shipRect = {editorTab.rect.x + editorTab.rect.width + PADDING,
                          mainPanelRect.y + PADDING,
                          100, tabHeight};
    shipTab = (Interact){"ship", "", shipRect, "Ship", true};
    Rectangle workshopRect = {shipTab.rect.x + shipTab.rect.width + PADDING,
                              mainPanelRect.y + PADDING,
                              120, tabHeight};
    workshopTab = (Interact){"workshop", "", workshopRect, "Workshop", true};

    availUpgradesRect = (Rectangle){subPanelRect.x + PADDING, subPanelRect.y + 30,
                                    availBoxWidth, availBoxHeight};
    availStruct = (Rectangle){availUpgradesRect.x, availUpgradesRect.y,
                             availBoxWidth, availBoxHeight};
    availScience = (Rectangle){availUpgradesRect.x, availUpgradesRect.y,
                              availBoxWidth, availBoxHeight};
    availSoft = (Rectangle){availUpgradesRect.x, availUpgradesRect.y,
                           availBoxWidth, availBoxHeight};
    availIncrement = (Rectangle){availUpgradesRect.x, availUpgradesRect.y,
                                availBoxWidth, availBoxHeight};


//    ResizeEditor(availUpgradesRect.x + availUpgradesRect.width + padding * 4, availUpgradesRect.y);
    RefreshUpgradesLists();
    isGameGuiInit = true;
}
void DrawGame()
{
    if (!isGameGuiInit) {
        InitGameUI();
    }

    HandleMouseHoveredGame();
    HandleMouseClickGame();
    DrawStaticInterface();

    // DrawAnimation();
    // TypeCode();
    // DrawConfigMenu();
}
