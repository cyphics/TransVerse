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

bool is_in_dashboard = false;
bool print_debug = false;

static const float tab_height = 30;

static Rectangle types_tab_rect;
static char *types_tabs_labels[5] = {"Terminal", "Editor", "Ship", "Workshop", "Engine"};
static int types_currently_selected_tab = 0;

static Rectangle dashboard_border_rect;
static struct DataLine dashboard_speed_line;
static struct DataLine dashboard_acceleration_line;
static struct DataLine dashboard_distance_line;
static Rectangle remaining_time_label_rect;
static Rectangle remaining_time_value_rect;

static const float availBoxWidth = 150;
static const float availBoxHeight = 150;

#define MAX_UPGRADE_PER_TYPE 5

static UpgradeInfo available_structure_upgrades[MAX_UPGRADE_PER_TYPE];
static UpgradeInfo available_science_upgrades[MAX_UPGRADE_PER_TYPE];
static UpgradeInfo available_software_upgrades[MAX_UPGRADE_PER_TYPE];
static UpgradeInfo available_incremental_upgrades[MAX_UPGRADE_PER_TYPE];

static int number_structure_upgrades = 0;
static int number_science_upgrades = 0;
static int number_soft_upgrades = 0;
static int number_incremental_upgrades = 0;

static int upgrade_rect_width = 100;
static int upgrade_rect_height = 30;

static Interact availStructInter[MAX_UPGRADE_PER_TYPE] = {};
static Interact availScienceInter[MAX_UPGRADE_PER_TYPE] = {};
static Interact availSoftInter[MAX_UPGRADE_PER_TYPE] = {};
static Interact availIncrementInter[MAX_UPGRADE_PER_TYPE] = {};
static Rectangle main_panel_rect;
static Rectangle game_sub_border;

static bool is_ugprade_clicked;
static Vector2 upgrade_list_anchor;

static struct DataLine {
    char label[30];
    struct Rectangle bounds;
    int dropDownActive;
    bool dropDownEdit;
} DataLine;

static struct DataLine milestoneLine;
static Rectangle milestonesRect;

void RefreshUpgradesLists() {
    number_structure_upgrades = 0;
    number_science_upgrades = 0;
    number_soft_upgrades = 0;
    number_incremental_upgrades = 0;

    UpdateAvailableUpgrades();

    memset(available_structure_upgrades, 0, sizeof(available_structure_upgrades));
    memset(available_science_upgrades, 0, sizeof(available_science_upgrades));
    memset(available_software_upgrades, 0, sizeof(available_software_upgrades));
    memset(available_incremental_upgrades, 0, sizeof(available_incremental_upgrades));

    for (int i = 0; i < available_upgrades.size; ++i) {
        UPGRADE handle = available_upgrades.list[i];
        UpgradeInfo info = {0};
        GetInfo(handle, &info);
        if (AreStrEquals(info.type, "structure")) {
            available_structure_upgrades[number_structure_upgrades] = info;
            number_structure_upgrades++;
        }
        if (AreStrEquals(info.type, "science")) {
            available_science_upgrades[number_science_upgrades] = info;
            number_science_upgrades++;
        }
        if (AreStrEquals(info.type, "software")) {
            available_software_upgrades[number_soft_upgrades] = info;
            number_soft_upgrades++;
        }
        if (AreStrEquals(info.type, "incremental")) {
            available_incremental_upgrades[number_incremental_upgrades] = info;
            number_incremental_upgrades++;
        }
    }
}

void InitGameUI(Vector2 anchor) {

    game_sub_border.x = anchor.x;
    game_sub_border.y = anchor.y;

    Vector2 dashboard_anchor = {game_sub_border.x + PADDING, game_sub_border.y + PADDING};

    main_panel_rect = (Rectangle) {anchor.x + PADDING, anchor.y + PADDING,
                                   800 - PADDING * 2, 400 - PADDING * 2};

    // DASHBOARD
    float line_height = 30;
    dashboard_speed_line.bounds = (Rectangle) {dashboard_anchor.x + PADDING, dashboard_anchor.y + PADDING,
                                               400, line_height};
    dashboard_acceleration_line.bounds = dashboard_speed_line.bounds;
    dashboard_acceleration_line.bounds.y += 40;
    dashboard_distance_line.bounds = dashboard_speed_line.bounds;
    dashboard_distance_line.bounds.x += dashboard_speed_line.bounds.width + 40;
    dashboard_border_rect.x = dashboard_anchor.x;
    dashboard_border_rect.y = dashboard_anchor.y;
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

    InitEngineScreen(types_tab_rect.x, types_tab_rect.y + types_tab_rect.height);

    upgrade_list_anchor.x = types_tab_rect.x;
    upgrade_list_anchor.y = types_tab_rect.y + types_tab_rect.height + PADDING;
    RefreshUpgradesLists();
}

void InitEngineScreen(int anchorX, int anchorY) {
    milestonesRect.x = anchorX;
    milestonesRect.y = anchorY;
    milestoneLine.bounds.x = milestonesRect.x + PADDING;
    milestoneLine.bounds.width = dashboard_distance_line.bounds.width;
    milestoneLine.bounds.height = dashboard_distance_line.bounds.height;
}

void DrawUpgrade(UpgradeInfo info, Vector2 anchor, bool *isClicked) {
    Rectangle bounds = {anchor.x, anchor.y, upgrade_rect_width, upgrade_rect_height};
    GuiButton(bounds, info.name, true);
    *isClicked = false;
}

void DrawUpgradesList(UpgradeInfo *upgrades_list, int list_length, Vector2 anchor){
    bool isClicked;

    for (int i = 0; i < list_length; ++i) {
        Vector2 p = {anchor.x,   anchor.y + (upgrade_rect_height + PADDING)* i};
        UpgradeInfo info = upgrades_list[i];
        DrawUpgrade(info, p, &isClicked);

        if(isClicked){
            printf("Upgrade %s purchased", info.name);
            /* PurchaseUpgrade(info.handle) */
        }
    }
}

void DrawValueWithSelectableUnit(struct DataLine *line, u_phys value) {
    float label_width = 120;
    float value_width = 60;
    float unit_select_width = 100;
    float unit_width = 20;
    float line_height = line->bounds.height;
    Rectangle label_rect = {line->bounds.x, line->bounds.y, label_width, line_height};
    Rectangle value_rect = {label_rect.x + label_rect.width + PADDING, line->bounds.y, value_width, line_height};
    Rectangle unit_dropdown_rect = {value_rect.x + value_rect.width + PADDING, line->bounds.y, unit_select_width,
                                  line_height};
    Rectangle unit_rect = {unit_dropdown_rect.x + unit_dropdown_rect.width + PADDING, line->bounds.y, unit_width,
                          line_height};
    char data_to_string[100];
    sprintf(data_to_string, "%.3e", value / distance_std.list[line->dropDownActive].value);
    GuiLabel(label_rect, line->label);
    GuiLabel(value_rect, data_to_string);
    GuiLabel(unit_rect, "/ second");
    if(is_in_dashboard) print_debug = true;
    if (GuiDropdownBox(unit_dropdown_rect, DistanceUnitsListString, &line->dropDownActive, line->dropDownEdit))
        line->dropDownEdit = !line->dropDownEdit;
    print_debug = false;
}

void DrawTerminalTab() {
    DrawText("Science",
             upgrade_list_anchor.x + PADDING,
             upgrade_list_anchor.y - 10,
             FONT_SIZE / 2,
             DARKGRAY);
    /* DrawRectangleLinesEx((Recta), 1, BLACK); */
    DrawUpgradesList(available_science_upgrades, number_science_upgrades, upgrade_list_anchor);
}

void DrawEditorTab() {
    DrawText("Available softwares",
             upgrade_list_anchor.x + PADDING,
             upgrade_list_anchor.y - 10,
             FONT_SIZE / 2, DARKGRAY);
    /* DrawRectangleLinesEx(available_software_rect, 1, BLACK); */
    DrawUpgradesList(available_software_upgrades, number_soft_upgrades, upgrade_list_anchor);
}

void DrawShipTab() {
    DrawText("Incremental",
             upgrade_list_anchor.x + PADDING,
             upgrade_list_anchor.y - 10,
             FONT_SIZE / 2, DARKGRAY);
    /* DrawRectangleLinesEx(available_incremental_rect, 1, BLACK); */
    DrawUpgradesList(available_incremental_upgrades, number_incremental_upgrades, upgrade_list_anchor);
}

void DrawWorkshopTab() {
    DrawText("Structure",
             upgrade_list_anchor.x + PADDING,
             upgrade_list_anchor.y - 10,
             FONT_SIZE / 2, DARKGRAY);
    /* DrawRectangleLinesEx(available_structure_rect, 1, BLACK); */
    DrawUpgradesList(available_structure_upgrades, number_structure_upgrades, upgrade_list_anchor);
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


void DrawDashboard(){

    DrawRectangleLinesEx(dashboard_border_rect, 1, BLACK);
    GuiLabel(remaining_time_label_rect, "Remaining time: ");
    u_time remaining_time = GetRemainingTime();
    char time_to_str[100];
    TimeToStr(time_to_str, remaining_time);
    GuiLabel(remaining_time_value_rect, time_to_str);
    is_in_dashboard = true;
    DrawValueWithSelectableUnit(&dashboard_distance_line, GetTraveledDistance());
    is_in_dashboard = false;
    DrawValueWithSelectableUnit(&dashboard_acceleration_line, GetCurrentAcceleration());
    DrawValueWithSelectableUnit(&dashboard_speed_line, GetCurrentSpeed());

}

void DrawGame() {
    static char some_text[100] = "Some text";
    static bool isPressed = false;

    // Static content
    DrawDashboard();
    DrawRectangleLinesEx(game_sub_border, 2, BLACK);
    GuiTabs(types_tab_rect,
            types_tabs_labels,
            sizeof(types_tabs_labels) / sizeof(types_tabs_labels[0]),
            &types_currently_selected_tab);

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
