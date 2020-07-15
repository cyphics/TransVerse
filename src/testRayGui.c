
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "../lib/raygui/raygui.h"

#include "game.h"
#include "types.h"
#include "helper.h"
#include "gui.h"
#include "game_handler.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define numUpgrades  MAX_UPGRADES_AMOUNT

// UI Elements related values

const float scaleFactor = 1;
const float fontSize = defaultFontSize * scaleFactor;
const float padding = 5 * scaleFactor;
const float anchorX = 20 * scaleFactor, anchorY = 20 * scaleFactor;
const float textRectHeight = 30 * scaleFactor;


const float maxUpgradeNameLength = 250 * scaleFactor;
const float selectUpgradeRectWidth = maxUpgradeNameLength;

const float selectMainRectX = anchorX;
const float selectMainRectY = anchorY;
const float selectMainRectWidth = selectUpgradeRectWidth + 2 * padding;
const float selectMainRectHeight = textRectHeight * numUpgrades + padding * (numUpgrades + 1);

const float typeTabWidth = selectMainRectWidth / 4;
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
const float editNameRectWidth      = selectMainRectWidth;

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
const float editMainRectY = editNameRectY - padding;;
const float editMainRectWidth = editNameRectWidth + editTypeRectWidth + editIncrRectWidth + 4 * padding;
const float editMainRectHeight = (textRectHeight + padding) * 5;

const float saveWidth = 100;

// Build rectangles

const Rectangle structRect    = {structRectX, structRectY, typeTabWidth, typeTabHeight};
const Rectangle scienceRect   = {scienceRectX, scienceRectY, typeTabWidth, typeTabHeight};
const Rectangle incrementRect = {incrementRectX, incrementRectY, typeTabWidth, typeTabHeight};
const Rectangle softRect      = {softRectX, softRectY, typeTabWidth, typeTabHeight};

Rectangle selectMainRect = {selectMainRectX, selectMainRectY, selectMainRectWidth, selectMainRectHeight};

const Rectangle editNameRect = {editNameRectX, editNameRectY, editNameRectWidth, textRectHeight};
const Rectangle editTypeRect = {editTypeRectX, editTypeRectY, editTypeRectWidth, textRectHeight};
const Rectangle editIncrRect = {editIncrRectX, editIncrRectY, editIncrRectWidth, textRectHeight};
const Rectangle editDescRect = {editDescRectX, editDescRectY, editDescRectWidth, editDescRectHeight};
const Rectangle editDepRect = {editDepRectX, editDepRectY, editDepRectWidth, textRectHeight};
const Rectangle editResRect = {editResRectX, editResRectY, editResRectWidth, textRectHeight};
const Rectangle editBoughtRect = {editBoughtRectX, editBoughtRectY, editBoughtRectWidth, textRectHeight};
const Rectangle editAmountRect = {editAmountRectX, editAmountRectY, editAmountRectWidth, textRectHeight};

const Rectangle editMainRect = {editMainRectX, editMainRectY, editMainRectWidth, editMainRectHeight};

const Rectangle gameStateRectangle = {anchorX, editMainRect.y + editMainRect.height + padding,
                                      selectMainRectWidth + editMainRectWidth + padding * 2, 200};
const Rectangle saveRect = {gameStateRectangle.x + gameStateRectangle.width - saveWidth,
                            gameStateRectangle.y + gameStateRectangle.height + padding,
                            saveWidth, 30};


char str[100] = "Some text";
char desc[100] = "Some desc";
int scrollIdx = 10;
const char *typesList[] = {"science", "incremental", "structure", "software"};
const char *resourcesList[] = {"energy", "code", "software", "copper", "steel"};
const char *itemsList[3] = {"One", "Two", "Three"};
const char *depsList[3] = {"OneD", "TwoD", "ThreeD"};
int active = 0;
int newlySelected = 0;
int selectedDependency = 0;
int focus = 0;

int dropTypeActive = 0;
bool dropTypeEditMode = false;
int dropResourceActive = 0;
bool dropResourceEditMode = false;
int valueBox002Value = 0;
double increaseValue = 0.0f;
bool valueBoxEditMode = false;

char dropTypesList[100];
char dropResourcesList[100];
int amountValue = 0;
char amountTxt[100];
int boughtValue = 0;
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

}


void TestRayGui(char *some_text)
{
    Vector2 mousePos = GetMousePosition();
    TextCopy(dropTypesList, TextJoin(typesList, 4, ";")); // TODO move to init method
    TextCopy(dropResourcesList, TextJoin(resourcesList, 4, ";")); // TODO move to init method


    if (GuiButton(scienceRect, "Science"))       BuildSelectUpgradesList("science");
    if (GuiButton(incrementRect, "Incremental")) BuildSelectUpgradesList("incremental");
    if (GuiButton(structRect, "Structure"))      BuildSelectUpgradesList("structure");
    if (GuiButton(softRect, "Software"))         BuildSelectUpgradesList("software");

    // active = GuiListView(selectMainRect, itemsList, &scrollIdx, active);
    newlySelected = GuiListViewEx(selectMainRect, itemsList, SIZEOF(itemsList), &focus, &scrollIdx, active);
    if (newlySelected != active && newlySelected >= 0) {
        printf("Selected %d: %s\n", newlySelected, itemsList[newlySelected]);
        active = newlySelected;
    }
    selectedDependency = GuiListViewEx(editDepRect, depsList, SIZEOF(depsList), &focus, &scrollIdx, active);

    if (GuiTextBox(editNameRect, str, 20, CheckCollisionPointRec(mousePos, editNameRect)));

    increaseValue = GuiDMValueBox(editIncrRect, increaseValue, 0.0f, 10.0f, 2,
                                  CheckCollisionPointRec(GetMousePosition(), editIncrRect));
    // GuiValueBox(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode)
    GuiValueBox(editAmountRect, amountTxt, &amountValue, 0, 1000000,
                CheckCollisionPointRec(GetMousePosition(), editAmountRect));
    GuiValueBox(editBoughtRect, boughtTxt, &boughtValue, 0, 1000000,
                CheckCollisionPointRec(GetMousePosition(), editBoughtRect));

    if (GuiTextBoxMulti(editDescRect, desc, 20, CheckCollisionPointRec(mousePos, editDescRect)));

    if (GuiButton(saveRect, "Save"))   SaveGame();
    if (GuiDropdownBox(editResRect, dropResourcesList, &dropResourceActive, dropResourceEditMode))
        dropResourceEditMode = !dropResourceEditMode;
    if (GuiDropdownBox(editTypeRect, dropTypesList, &dropTypeActive, dropTypeEditMode))
        dropTypeEditMode = !dropTypeEditMode;
}
