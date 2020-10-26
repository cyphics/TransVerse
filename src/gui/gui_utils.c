//
// Created by cyphics on 26/10/2020.
//

#include <stdio.h>
#include "gui_utils.h"

char DistanceUnitsListString[150] = "";

void CreateDistanceUnitsList() {
    int num_units = sizeof(distance_std.list) / sizeof(distance_std.list[0]);
    for (int i = 0; i < num_units; ++i) {
        strcat(DistanceUnitsListString, distance_std.list[i].name);
        if (i < num_units - 1) strcat(DistanceUnitsListString, ";");
    }
    printf(DistanceUnitsListString);

}