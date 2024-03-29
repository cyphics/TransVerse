//
// Created by cyphics on 22/10/2020.
//

#ifndef TRANSVERSE_GUI_TOOLS_H
#define TRANSVERSE_GUI_TOOLS_H


#include <raylib.h>
#include "../types.h"

// PUBLIC METHODS
// Label control
void GuiLabel(Rectangle bounds, const char *text);
void GuiTabs(Rectangle bounds, char **entries, int num_entries, int *current_entry);
//void GuiLabelBox(int posX, int posY, char *label, char *content);
bool GuiTextEdit(Rectangle bounds, char *text, bool editMode);
bool GuiButton(Rectangle bounds, const char *text, bool is_enabled);
bool GuiButtonTab(Rectangle bounds, const char *text, bool is_selected);
//void DrawInteract(Interact *tr);
bool GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool editMode);

#endif //TRANSVERSE_GUI_TOOLS_H
