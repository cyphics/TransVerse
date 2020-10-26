//
// Created by cyphics on 26/10/2020.
//

#include <raylib.h>
#include <stdio.h>
#include "gui_engine.h"
#include "../gui_constants.h"
#include "../../game.h"
#include "../gui_tools.h"
#include "../gui_utils.h"

//static char speedUnitsList[150];

static Rectangle mainDataRect;
static Rectangle speedLabelRect;
static Rectangle speedValueRect;
static Rectangle speedUnitDropdownRect;
static int speedDropDownActive = 0;
static bool speedDropdownEdit = false;
static Rectangle speedUnitRect;

static Rectangle milestonesRect;

void InitEngineScreen(int posX, int posY) {
    mainDataRect = (Rectangle) {posX + PADDING, posY + PADDING,
                                300, 100};
    float labelWidth = 80;
    float value_width = 250;
    float unit_select_width = 80;
    float unit_width = 20;
    float line_height = 30;
    speedLabelRect = (Rectangle) {mainDataRect.x + PADDING, mainDataRect.y + PADDING,
                                  labelWidth, line_height};
    speedValueRect = (Rectangle) {speedLabelRect.x + speedLabelRect.width + PADDING, mainDataRect.y + PADDING,
                                  value_width, line_height};
    speedUnitDropdownRect = (Rectangle) {speedValueRect.x + speedValueRect.width + PADDING, mainDataRect.y + PADDING,
                                         unit_select_width, line_height};
    speedUnitRect = (Rectangle) {speedUnitDropdownRect.x + speedUnitDropdownRect.width + PADDING,
                                 mainDataRect.y + PADDING,
                                 unit_width, line_height};


    milestonesRect = (Rectangle) {mainDataRect.x, mainDataRect.y + mainDataRect.height + PADDING,
                                  300, 400};
}

void DrawEngine() {
    DrawRectangleLinesEx(mainDataRect, 1, BLACK);
    DrawRectangleLinesEx(milestonesRect, 1, BLACK);

    int v_space = 20;
    int x_pos = milestonesRect.x + PADDING;
    char currentSpeed[100];
    char currentAcceleration[20];
    sprintf(currentSpeed, "%.30f", (float) GetCurrentSpeed() / distance_std.list[speedDropDownActive].value);
    sprintf(currentAcceleration, "%.3f", (float) GetCurrentAcceleration());
    GuiLabel(speedLabelRect, "Speed: ");
    GuiLabel(speedValueRect, currentSpeed);
    GuiLabel(speedUnitRect, "/ second");
//    GuiLabelBox(x_pos, mainDataRect.y + PADDING, "Speed", currentSpeed);
//    GuiLabelBox(x_pos, mainDataRect.y + v_space + PADDING, "Distance", "0m");
//    GuiLabelBox(x_pos, mainDataRect.y + (v_space + PADDING) * 2, "Acceleration", currentAcceleration);
    if (GuiDropdownBox(speedUnitDropdownRect, DistanceUnitsListString, &speedDropDownActive, speedDropdownEdit))speedDropdownEdit = !speedDropdownEdit;

}