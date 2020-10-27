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

struct DataLine {
    char label[30];
    struct Rectangle bounds;
    int dropDownActive;
    bool dropDownEdit;
} DataLine;

static struct DataLine speedLine;
static struct DataLine accelerationLine;
static struct DataLine distanceLine;
static struct DataLine milestoneLine;

static Rectangle remainingTimeLabelRect;
static Rectangle remainingTimeValueRect;

static Rectangle milestonesRect;
static Rectangle mainDataRect;

void DrawDataLine(struct DataLine *line, u_phys value) {
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

void InitEngineScreen(int posX, int posY) {
    mainDataRect = (Rectangle) {posX + PADDING, posY + PADDING,
                                380, 180};
    milestonesRect = (Rectangle) {mainDataRect.x + 400, mainDataRect.y + PADDING,
                                  360, 600};

    float line_height = 30;

    speedLine.bounds = (Rectangle) {mainDataRect.x + PADDING, mainDataRect.y + PADDING,
                                    400, line_height};
    strcat(speedLine.label, "Speed: ");

    accelerationLine.bounds = speedLine.bounds;
    accelerationLine.bounds.y += 40;
    strcat(accelerationLine.label, "Acceleration: ");

    distanceLine.bounds = accelerationLine.bounds;
    distanceLine.bounds.y += 40;
    strcat(distanceLine.label, "Distance: ");

    remainingTimeLabelRect = distanceLine.bounds;
    remainingTimeLabelRect.y += 40;
    remainingTimeValueRect = remainingTimeLabelRect;
    remainingTimeValueRect.x += 80;



    milestoneLine.bounds.x = milestonesRect.x + PADDING;
    milestoneLine.bounds.width = distanceLine.bounds.width;
    milestoneLine.bounds.height = distanceLine.bounds.height;

}

void DrawEngine() {
    DrawRectangleLinesEx(mainDataRect, 1, BLACK);
    DrawRectangleLinesEx(milestonesRect, 1, BLACK);

    GuiLabel(remainingTimeLabelRect, "Remaining time: ");
    u_time remaining_time = GetRemainingTime();
    char time_to_str[100];
    TimeToStr(time_to_str, remaining_time);
//    sprintf(time_to_str, "%f", remaining_time);
    GuiLabel(remainingTimeValueRect, time_to_str);
    DrawDataLine(&distanceLine, GetTraveledDistance());
    DrawDataLine(&accelerationLine, GetCurrentAcceleration());
    DrawDataLine(&speedLine, GetCurrentSpeed());

    int num_milestones = sizeof(dist_mstone.list) / sizeof(dist_mstone.list[0]);
    milestoneLine.bounds.y = milestonesRect.y + PADDING;
    for (int i = 0; i < num_milestones; ++i) {
        strcpy(milestoneLine.label, dist_mstone.list[i].name);
        DrawDataLine(&milestoneLine, GetTraveledDistance() / dist_mstone.list[i].value);
        milestoneLine.bounds.y += 40;
    }
}