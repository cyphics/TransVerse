//
// Created by cyphics on 22/10/2020.
//

#ifndef TRANSVERSE_GUI_GAME_H
#define TRANSVERSE_GUI_GAME_H

//
// Created by cyphics on 22/10/2020.
//

bool isGameGuiInit = false;
const float interfaceAnchorX = 800.0f;
const float interfaceAnchorY = 0;

//extern GameState state;
extern HandleList AvailableUpgrades;
extern Vector2 mousePosition;
bool mouseLeftPressed = false;

int currentPanel = EDITOR_TAB;

const float tabHeight = 30;




Interact terminalTab;
Interact editorTab;
Interact shipTab;
Interact workshopTab;

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

#endif //TRANSVERSE_GUI_GAME_H
