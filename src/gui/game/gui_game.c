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

#include "../gui.h"
#include "../../game.h"
#include "../../game_handler.h"
#include "../../helper.h"
#include "../gui_config.h"
#include "gui_game.h"
#include "../gui_constants.h"
#include "../gui_tools.h"
#include "gui_engine.h"
#include "../gui_utils.h"

const float tab_height = 30;

Rectangle types_tab_rect;
char *types_tabs_labels[5] = {"Terminal", "Editor", "Ship", "Workshop", "Engine"};
int types_currently_selected_tab = 4;

static Rectangle dashboard_border_rect;
static struct DataLine dashboard_speed_line;
static struct DataLine dashboard_acceleration_line;
static struct DataLine dashboard_distance_line;
static Rectangle remaining_time_label_rect;
static Rectangle remaining_time_value_rect;

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
Rectangle main_panel_rect;
Rectangle game_sub_border;

struct DataLine {
    char label[30];
    struct Rectangle bounds;
    int dropDownActive;
    bool dropDownEdit;
} DataLine;

static struct DataLine milestoneLine;
static Rectangle milestonesRect;

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
    for (int i = 0; i < available_upgrades.size; ++i) {
        UPGRADE handle = available_upgrades.list[i];
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





void InitGameUI(Point anchor) {

    game_sub_border.x = anchor.posX;
    game_sub_border.y = anchor.posY;

    Point dashboard_anchor = {game_sub_border.x + PADDING, game_sub_border.y + PADDING};

    main_panel_rect = (Rectangle) {anchor.posX + PADDING, anchor.posY + PADDING,
                                   800 - PADDING * 2, 400 - PADDING * 2};

    // DASHBOARD
    float line_height = 30;
    dashboard_speed_line.bounds = (Rectangle) {dashboard_anchor.posX + PADDING, dashboard_anchor.posY + PADDING,
                                               400, line_height};
    dashboard_acceleration_line.bounds = dashboard_speed_line.bounds;
    dashboard_acceleration_line.bounds.y += 40;
    dashboard_distance_line.bounds = dashboard_speed_line.bounds;
    dashboard_distance_line.bounds.x += dashboard_speed_line.bounds.width + 40;
    dashboard_border_rect.x = dashboard_anchor.posX;
    dashboard_border_rect.y = dashboard_anchor.posY;
    dashboard_border_rect.width = dashboard_speed_line.bounds.width + dashboard_distance_line.bounds.width + 3 * PADDING;
    dashboard_border_rect.height = 2 * line_height + 4 * PADDING;

    strcat(dashboard_speed_line.label, "Speed: ");
    strcat(dashboard_acceleration_line.label, "Acceleration: ");
    strcat(dashboard_distance_line.label, "Distance: ");

    remaining_time_label_rect = dashboard_distance_line.bounds;
    remaining_time_label_rect.y += 40;
    remaining_time_value_rect = remaining_time_label_rect;
    remaining_time_value_rect.x += 80;


    // TABS
    types_tab_rect.x = dashboard_border_rect.x  + PADDING;
    types_tab_rect.y = dashboard_border_rect.y + dashboard_border_rect.height+ PADDING;
    types_tab_rect.width = 600;
    types_tab_rect.height = tab_height;


    availUpgradesRect = (Rectangle) {game_sub_border.x + PADDING, game_sub_border.y + 30,
                                     availBoxWidth, availBoxHeight};
    availStruct = (Rectangle) {availUpgradesRect.x, availUpgradesRect.y,
                               availBoxWidth, availBoxHeight};
    availScience = (Rectangle) {availUpgradesRect.x, availUpgradesRect.y,
                                availBoxWidth, availBoxHeight};
    availSoft = (Rectangle) {availUpgradesRect.x, availUpgradesRect.y,
                             availBoxWidth, availBoxHeight};
    availIncrement = (Rectangle) {availUpgradesRect.x, availUpgradesRect.y,
                                  availBoxWidth, availBoxHeight};

    InitEngineScreen(types_tab_rect.x, types_tab_rect.y + types_tab_rect.height);

//    ResizeEditor(availUpgradesRect.x + availUpgradesRect.width + padding * 4, availUpgradesRect.y);
    RefreshUpgradesLists();
}



void InitEngineScreen(int anchorX, int anchorY) {

    milestonesRect.x = anchorX;
    milestonesRect.y = anchorY;
    milestoneLine.bounds.x = milestonesRect.x + PADDING;
    milestoneLine.bounds.width = dashboard_distance_line.bounds.width;
    milestoneLine.bounds.height = dashboard_distance_line.bounds.height;

}

void DrawValueWithSelectableUnit(struct DataLine *line, u_phys value) {
    float label_width = 120;
    float value_width = 60;
    float unit_select_width = 100;
    float unit_width = 20;
    float line_height = line->bounds.height;
    Rectangle labelRect = {line->bounds.x, line->bounds.y, label_width, line_height};
    Rectangle valueRect = {labelRect.x + labelRect.width + PADDING, line->bounds.y, value_width, line_height};
    Rectangle unitDropdownRect = {valueRect.x + valueRect.width + PADDING, line->bounds.y, unit_select_width,
                                  line_height};
    Rectangle unitRect = {unitDropdownRect.x + unitDropdownRect.width + PADDING, line->bounds.y, unit_width,
                          line_height};


    char data_to_string[100];

    sprintf(data_to_string, "%.3e", value / distance_std.list[line->dropDownActive].value);
    GuiLabel(labelRect, line->label);
    GuiLabel(valueRect, data_to_string);
    GuiLabel(unitRect, "/ second");

    if (GuiDropdownBox(unitDropdownRect, DistanceUnitsListString, &line->dropDownActive, line->dropDownEdit))
        line->dropDownEdit = !line->dropDownEdit;
}

void DrawEngineTab() {
    DrawRectangleLinesEx(milestonesRect, 1, BLACK);
    int num_milestones = sizeof(dist_mstone.list) / sizeof(dist_mstone.list[0]);
    milestoneLine.bounds.y = milestonesRect.y + PADDING;
    for (int i = 0; i < num_milestones; ++i) {
        strcpy(milestoneLine.label, dist_mstone.list[i].name);
        DrawValueWithSelectableUnit(&milestoneLine, GetTraveledDistance() / dist_mstone.list[i].value);
        milestoneLine.bounds.y += 40;
    }
}




void DrawEditorTab() {
    DrawText("Available softwares", availSoft.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availSoft, 1, BLACK);
//    for (int i = 0; i < softCount; ++i) {
//        DrawInteract(&availSoftInter[i]);
//    }
//    TypeCode();
}

void DrawShipTab() {
    DrawText("Incremental", availIncrement.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availIncrement, 1, BLACK);
//    for (int i = 0; i < incrementCount; ++i) {
//        DrawInteract(&availIncrementInter[i]);
//    }
}

void DrawWorkshopTab() {
    DrawText("Structure", availStruct.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availStruct, 1, BLACK);
//    for (int i = 0; i < structCount; ++i) {
//        DrawInteract(&availStructInter[i]);
//    }
}

void DrawTerminalTab() {
    DrawText("Science", availScience.x + PADDING, availStruct.y - 10, FONT_SIZE / 2, DARKGRAY);
    DrawRectangleLinesEx(availScience, 1, BLACK);
//    for (int i = 0; i < scienceCount; ++i) {
//        DrawInteract(&availScienceInter[i]);
//    }
}

void DrawDashboard(){
    DrawRectangleLinesEx(dashboard_border_rect, 1, BLACK);
    GuiLabel(remaining_time_label_rect, "Remaining time: ");
    u_time remaining_time = GetRemainingTime();
    char time_to_str[100];
    TimeToStr(time_to_str, remaining_time);
    GuiLabel(remaining_time_value_rect, time_to_str);
    DrawValueWithSelectableUnit(&dashboard_distance_line, GetTraveledDistance());
    DrawValueWithSelectableUnit(&dashboard_acceleration_line, GetCurrentAcceleration());
    DrawValueWithSelectableUnit(&dashboard_speed_line, GetCurrentSpeed());
}

void DrawGame() {
    static char some_text[100] = "Some text";
    static bool isPressed = false;

    // Static content
    DrawDashboard();
    DrawRectangleLinesEx(game_sub_border, 2, BLACK);
    types_currently_selected_tab = GuiTabs(types_tab_rect,
                                           types_tabs_labels,
                                           sizeof(types_tabs_labels) / sizeof(types_tabs_labels[0]),
                                           types_currently_selected_tab);


    // Dynamic content
    switch (types_currently_selected_tab) {
        case 0: {
            DrawTerminalTab();
            break;
        }
        case 1: {
            DrawEditorTab();
            break;
        }
        case 2: {
            DrawShipTab();
            break;
        }
        case 3: {
            DrawWorkshopTab();
            break;
        }
        case 4: {
            DrawEngineTab();
        }
        default:
            break;
    }

}

void Foo(char i){

}
