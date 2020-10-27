//
// Created by cyphics on 26/10/2020.
//

#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "gui_cheat.h"
#include "../gui_tools.h"
#include "../../helper.h"
#include "../../game.h"
#include "../../physics.h"
#include "../gui_constants.h"
#include "../gui_utils.h"

struct ConfigLine {
    char label[40];
    char valueString[40];
    bool valueEditMode;
    int dropDownActive;
    bool dropDownEditMode;
    u_phys *game_value;
    Rectangle bounds;
} ConfigLine = {"", "", false, 0, false};

static struct ConfigLine speedLine;
struct ConfigLine accelerationLine;
struct ConfigLine distanceLine;
int mult_counter = 0;

void InitCheatUI(int posX, int posY) {
    strcat(speedLine.label, "Speed: ");
    strcat(speedLine.valueString, "0");
    speedLine.bounds = (struct Rectangle) {posX, posY, 200, 30};;
    speedLine.game_value = &game_state.current_speed;

    strcat(accelerationLine.label, "Acceleration: ");
    strcat(accelerationLine.valueString, "0");
    accelerationLine.bounds = speedLine.bounds;
    accelerationLine.bounds.y += 50.0f;
    accelerationLine.game_value = &game_state.current_acceleration;

    strcat(distanceLine.label, "Distance: ");
    strcat(distanceLine.valueString, "0");
    distanceLine.bounds = accelerationLine.bounds;
    distanceLine.bounds.y += 50.0f;
    distanceLine.game_value = &game_state.traveled_distance;


}

void DrawConfigLine(struct ConfigLine *line) {
    float line_height = 30.0f;
    float label_width = 70;
    Rectangle LabelRect = (Rectangle) {line->bounds.x, line->bounds.y,
                                       label_width, line_height};
    Rectangle ValueRect = (Rectangle) {LabelRect.x + LabelRect.width + PADDING,
                                       LabelRect.y,
                                       50, line_height};
    Rectangle DropdownUnitRect = (Rectangle) {ValueRect.x + ValueRect.width + PADDING,
                                              ValueRect.y,
                                              100, line_height};
    Rectangle SetButtonRect = (Rectangle) {DropdownUnitRect.x + DropdownUnitRect.width + PADDING,
                                           DropdownUnitRect.y,
                                           40, line_height};
    Rectangle AddButtonRect = (Rectangle) {SetButtonRect.x + SetButtonRect.width + PADDING,
                                           SetButtonRect.y,
                                           40, line_height};

    Rectangle MultiplyButtonRect = (Rectangle) {AddButtonRect.x + AddButtonRect.width + PADDING,
                                                AddButtonRect.y,
                                                40, line_height};

    GuiLabel(LabelRect, line->label);
    if (GuiTextEdit(ValueRect, line->valueString, line->valueEditMode)) line->valueEditMode = !line->valueEditMode;
    if (GuiButton(SetButtonRect, "SET")) {
        if (IsStringValidDouble(line->valueString)) {
            *(line->game_value) = StringToDouble(line->valueString) * distance_std.list[line->dropDownActive].value;
            printf(" set to %s!\n", line->valueString);
        } else printf(" value %s of wrong format. Aborting conversion\n", line->valueString);
    }
    if (GuiButton(AddButtonRect, "ADD")) {
        if (IsStringValidDouble(line->valueString)) {
            *line->game_value += StringToDouble(line->valueString) * distance_std.list[line->dropDownActive].value;
            printf(" added by %s!\n", line->valueString);
        } else printf(" value %s of wrong format. Aborting conversion\n", line->valueString);
    }
    if (GuiButton(MultiplyButtonRect, "MULT")) {
        if (IsStringValidDouble(line->valueString)) {
            *line->game_value *= StringToDouble(line->valueString);
            mult_counter++;
            printf(" multiply by %s for the %d time!\n", line->valueString, mult_counter);
        } else printf(" value %s of wrong format. Aborting conversion\n", line->valueString);
    }

    if (GuiDropdownBox(DropdownUnitRect, DistanceUnitsListString, &(line->dropDownActive),
                       line->dropDownEditMode))
        line->dropDownEditMode = !line->dropDownEditMode;
}

void DrawCheatsTab() {
    DrawConfigLine(&distanceLine);
    DrawConfigLine(&accelerationLine);
    DrawConfigLine(&speedLine);


}