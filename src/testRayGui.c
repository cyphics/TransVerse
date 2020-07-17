#include <raylib.h>

#include "../lib/raygui/raygui.h"

#include "game.h"
#include "types.h"
#include "helper.h"
#include "gui.h"
#include "game_handler.h"

#define numUpgrades  MAX_UPGRADES_AMOUNT

// UI Elements related values

const float scaleFactor = 1;
const float fontSize = defaultFontSize * scaleFactor;
const float padding = 5 * scaleFactor;
const float anchorX = 20 * scaleFactor, anchorY = 20 * scaleFactor;
const float textRectHeight = 30 * scaleFactor;


const float maxUpgradeNameLength = 250 * scaleFactor;
const float selectUpgradeRectWidth = maxUpgradeNameLength;

const float upgradesListRectX = anchorX;
const float upgradesListRectY = anchorY;
const float upgradesListRectWidth = selectUpgradeRectWidth + 2 * padding;
float upgradesListRectHeight = textRectHeight * numUpgrades + padding * (numUpgrades + 1);

const float typeTabWidth = upgradesListRectWidth / 4;
const float typeTabHeight = textRectHeight / 2;
const float structRectX = anchorX;
const float structRectY = anchorY - textRectHeight/2;
const float scienceRectX = anchorX + typeTabWidth;
const float scienceRectY = anchorY - textRectHeight/2;
const float incrementRectX = anchorX + typeTabWidth * 3;
const float incrementRectY = anchorY - textRectHeight/2;
const float softRectX = anchorX + typeTabWidth * 2;
const float softRectY = anchorY - textRectHeight/2;

const float editRectXAnchor = anchorX + maxUpgradeNameLength + 20 * scaleFactor;
const float editRectYAnchor = anchorY;

const float editNameRectX          = editRectXAnchor + padding;
const float editNameRectY          = editRectYAnchor + padding;
const float editNameRectWidth      = upgradesListRectWidth;

const float editTypeRectX      = editNameRectX + editNameRectWidth + padding;
const float editTypeRectY      = editRectYAnchor + padding;
const float editTypeRectWidth  = 80 * scaleFactor;

const float editIncrRectX      = editTypeRectX + editTypeRectWidth + padding;
const float editIncrRectY      = editRectYAnchor + padding;
const float editIncrRectWidth  = 50 * scaleFactor;


const float editDescRectX          = editRectXAnchor + padding;
const float editDescRectY          = editRectYAnchor + padding + 40 * scaleFactor;
const float editDescRectWidth      = editNameRectWidth + editTypeRectWidth + editIncrRectWidth + padding * 2;
const float editDescRectHeight      = textRectHeight * 2;

const float editDepRectX           = editRectXAnchor + padding;
const float editDepRectY           = editDescRectY + editDescRectHeight + padding;
const float editDepRectWidth       = editNameRectWidth;
float editDepRectHeight       = 100;

const float editResRectX           = editTypeRectX;
const float editResRectY           = editDepRectY;
const float editResRectWidth       = editTypeRectWidth;

const float editAmountRectX        = editIncrRectX;
const float editAmountRectY        = editDepRectY;
const float editAmountRectWidth    = editIncrRectWidth;

const float editBoughtRectX        = editResRectX;
const float editBoughtRectY        = editDepRectY;
const float editBoughtRectWidth    = 50;

const float editMainRectX = editNameRectX - padding;
const float editMainRectY = editNameRectY - padding;
const float editMainRectWidth = editNameRectWidth + editTypeRectWidth + editIncrRectWidth + 4 * padding;
const float editMainRectHeight = (textRectHeight + padding) * 5;

const float gameStateRectX = editRectXAnchor;
const float gameStateRectY = editMainRectY + editMainRectHeight + padding;
const float gameStateRectWidth = editMainRectWidth;
const float gameStateRectHeight = (textRectHeight + padding) * 5;

const float saveRectX = gameStateRectX;
const float saveRectHeight = textRectHeight;
const float saveRectY = gameStateRectY + gameStateRectHeight + padding;
const float saveRectWidth = 100;


// Build rectangles

const Rectangle structRect    = {structRectX, structRectY, typeTabWidth, typeTabHeight};
const Rectangle scienceRect   = {scienceRectX, scienceRectY, typeTabWidth, typeTabHeight};
const Rectangle incrementRect = {incrementRectX, incrementRectY, typeTabWidth, typeTabHeight};
const Rectangle softRect      = {softRectX, softRectY, typeTabWidth, typeTabHeight};

Rectangle upgradesListRect = {upgradesListRectX, upgradesListRectY, upgradesListRectWidth, 0};

const Rectangle editNameRect = {editNameRectX, editNameRectY, editNameRectWidth, textRectHeight};
const Rectangle editTypeRect = {editTypeRectX, editTypeRectY, editTypeRectWidth, textRectHeight};
const Rectangle editIncrRect = {editIncrRectX, editIncrRectY, editIncrRectWidth, textRectHeight};
const Rectangle editDescRect = {editDescRectX, editDescRectY, editDescRectWidth, textRectHeight };
Rectangle editDepRect = {editDepRectX, editDepRectY, editDepRectWidth, textRectHeight * 2};
const Rectangle editResRect = {editResRectX, editResRectY, editResRectWidth, textRectHeight};
const Rectangle editBoughtRect = {editBoughtRectX, editBoughtRectY, editBoughtRectWidth, textRectHeight};
const Rectangle editAmountRect = {editAmountRectX, editAmountRectY, editAmountRectWidth, textRectHeight};

const Rectangle editMainRect = {editMainRectX, editMainRectY, editMainRectWidth, editMainRectHeight};

const Rectangle gameStateRectangle = {gameStateRectX, gameStateRectY, gameStateRectWidth, gameStateRectHeight};
const Rectangle saveRect = {saveRectX, saveRectY, saveRectWidth, saveRectHeight};


const char *typesList[] = {"science", "incremental", "structure", "software"};
const char *resourcesList[] = {"energy", "code", "software", "copper", "steel"};

const char *depsList[3] = {"OneD", "TwoD", "ThreeD"};

int scrollIdx = 0;
int selectUpgradeFocus = 0;
int selectUpgradeActive = 0;
int selectedUpgradeId = 0;
int depsListactive = 0;
int depsListfocus = 0;
int selectedDependencyId = 0;

int dropTypeActive = 0;

bool dropTypeEditMode = false;
int dropResourceActive = 0;
bool dropResourceEditMode = false;
bool valueBoxEditMode = false;

char dropTypesList[100];
char dropResourcesList[100];
char amountTxt[100];
char boughtTxt[100];

upgrade *currentUpgradeToEdit;
char *currentTypeList = "structure";
char *selectableUpgradesList[15];
int amountSelectableUpgrades = 0;


void BuildSelectUpgradesList(char *newTypeList)
{
    currentTypeList = newTypeList;
    amountSelectableUpgrades = 0;
    // memset(selectUpgradesList, 0, sizeof(selectUpgradesList));
    for (int i = 0; i < numUpgrades; ++i) {
        if (AreStrEquals(state.upgrades_list[i].type, currentTypeList)) {
            selectableUpgradesList[amountSelectableUpgrades] = state.upgrades_list[i].id;
            amountSelectableUpgrades++;
        }
    }
    upgradesListRect.height = (textRectHeight + padding) * (float)amountSelectableUpgrades;

}

void SelectCurrentUpgrade(upgrade *original)
{
    currentUpgradeToEdit = original;
    int numDependencies = SIZEOF(currentUpgradeToEdit->dependencies);
    editDepRectHeight = textRectHeight * (float)numDependencies;
    // numberDependencies = 0;
    // memset(listDependenciesString, 0, sizeof(listDependenciesString));
    // for (int i = 0; i < MAX_DEPENDENCIES; ++i) {
    //     char *dep = original->dependencies[i];
    //     if (!IsEmpty(dep)) {
    //         AddDependency(dep);
    //     }
    // }
}

void InitConfig()
{
    BuildSelectUpgradesList("structure");
    SelectCurrentUpgrade(&state.upgrades_list[0]);
}

void DragDependencyCallback(int dependencyIndex)
{

    if (CheckCollisionPointRec(GetMousePosition(), editDepRect)) {
        char *name = selectableUpgradesList[dependencyIndex];
        printf("Received %s\n", name);

    }
}

void AddDependency(int dependencyIndex)
{
    // Add dependency to the list

    // for (int i = 0; i < numberDependencies; ++i) {
    //     if (AreStrEquals(listDependenciesString[i], dependency)) return;
    // }

    // strcat(listDependenciesString[numberDependencies], dependency);
    // memcpy(currentUpgradeToEdit->dependencies[numberDependencies], dependency, strlen(dependency));

    // numberDependencies++;

    // GenerateDependencyListString();
}


void TestRayGui(char *some_text)
{
    Vector2 mousePos = GetMousePosition();
    TextCopy(dropTypesList, TextJoin(typesList, 4, ";")); // TODO move to init method
    TextCopy(dropResourcesList, TextJoin(resourcesList, 4, ";")); // TODO move to init method

    GuiGroupBox(editMainRect, "Current upgrade");
    GuiGroupBox(gameStateRectangle, "Game State");

    if (GuiButton(scienceRect, "Science"))       BuildSelectUpgradesList("science");
    if (GuiButton(incrementRect, "Incremental")) BuildSelectUpgradesList("incremental");
    if (GuiButton(structRect, "Structure"))      BuildSelectUpgradesList("structure");
    if (GuiButton(softRect, "Software"))         BuildSelectUpgradesList("software");

    GuiTextBox(editNameRect, currentUpgradeToEdit->id, 20, CheckCollisionPointRec(mousePos, editNameRect));

    currentUpgradeToEdit->increase_factor = GuiDMValueBox(editIncrRect, currentUpgradeToEdit->increase_factor, 0.0f, 10.0f, 2,
                                                          CheckCollisionPointRec(GetMousePosition(), editIncrRect));
    GuiValueBox(editAmountRect,
                amountTxt,
                &currentUpgradeToEdit->initial_price.resources[0].amount,
                0, 1000000,
                CheckCollisionPointRec(GetMousePosition(), editAmountRect));

    GuiValueBox(editBoughtRect,
                boughtTxt,
                &currentUpgradeToEdit->amount_bought,
                0, 1000000,
                CheckCollisionPointRec(GetMousePosition(), editBoughtRect));

    if (GuiTextBoxMulti(editDescRect, currentUpgradeToEdit->description, 200, CheckCollisionPointRec(mousePos, editDescRect)));

    if (GuiButton(saveRect, "Save"))   SaveGame();
    if (GuiDropdownBox(editResRect, dropResourcesList, &dropResourceActive, dropResourceEditMode))
        dropResourceEditMode = !dropResourceEditMode;
    if (GuiDropdownBox(editTypeRect, dropTypesList, &dropTypeActive, dropTypeEditMode))
        dropTypeEditMode = !dropTypeEditMode;

    // selectedDependencyId = GuiListViewEx(editDepRect,
    //                                      (const char**)currentUpgradeToEdit->dependencies,
    //                                      SIZEOF(currentUpgradeToEdit->dependencies),
    //                                      &depsListfocus, &scrollIdx, depsListactive);
    // List of selectable upgrades
    // Must be drawn last so it can draw dragged item over the rest
    selectedUpgradeId = GuiListViewExDrag(upgradesListRect,
                                          (const char**)selectableUpgradesList,
                                          amountSelectableUpgrades,
                                          &selectUpgradeFocus, &scrollIdx, selectUpgradeActive,
                                          DragDependencyCallback);
    if (selectedUpgradeId != selectUpgradeActive && selectedUpgradeId >= 0) {
        for (int i = 0; i < numUpgrades; ++i) {
            if (AreStrEquals(selectableUpgradesList[selectedUpgradeId], state.upgrades_list[i].id)) {
                SelectCurrentUpgrade(&state.upgrades_list[i]);
            }
        }
        selectUpgradeActive = selectedUpgradeId;
    }
}
