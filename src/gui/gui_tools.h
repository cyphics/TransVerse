//
// Created by cyphics on 22/10/2020.
//

#ifndef TRANSVERSE_GUI_TOOLS_H
#define TRANSVERSE_GUI_TOOLS_H


#include <raylib.h>

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define NUM_CONTROLS                    16      // Number of standard controls
#define NUM_PROPS_DEFAULT               16      // Number of standard properties
#define NUM_PROPS_EXTENDED               8      // Number of extended properties

#define TEXTEDIT_CURSOR_BLINK_FRAMES    20      // Text edit controls cursor blink timming

// Gui control text alignment
typedef enum {
    GUI_TEXT_ALIGN_LEFT = 0,
    GUI_TEXT_ALIGN_CENTER,
    GUI_TEXT_ALIGN_RIGHT,
} GuiTextAlignment;

// DEFAULT properties
typedef enum {
    TEXT_SIZE = 16,
    TEXT_SPACING,
    LINE_COLOR,
    BACKGROUND_COLOR,
} GuiDefaultProperty;

// Gui controls
typedef enum {
    DEFAULT = 0,
    LABEL,          // LABELBUTTON
    BUTTON,         // IMAGEBUTTON
    DROPDOWNBOX,
    TEXTBOX,        // TEXTBOXMULTI
    VALUEBOX,
    SPINNER,
    LISTVIEW
} GuiControl;

// Gui base properties for every control
typedef enum {
    BORDER_COLOR_NORMAL = 0,
    BASE_COLOR_NORMAL,
    TEXT_COLOR_NORMAL,
    BORDER_COLOR_FOCUSED,
    BASE_COLOR_FOCUSED,
    TEXT_COLOR_FOCUSED,
    BORDER_COLOR_PRESSED,
    BASE_COLOR_PRESSED,
    TEXT_COLOR_PRESSED,
    BORDER_COLOR_DISABLED,
    BASE_COLOR_DISABLED,
    TEXT_COLOR_DISABLED,
    BORDER_WIDTH,
    TEXT_PADDING,
    TEXT_ALIGNMENT,
    RESERVED
} GuiControlProperty;

// TextBox / TextBoxMulti / ValueBox / Spinner
typedef enum {
    TEXT_INNER_PADDING = 16
} GuiTextBoxProperty;

// DropdownBox
typedef enum {
    ARROW_PADDING = 16,
    DROPDOWN_ITEMS_PADDING
} GuiDropdownBoxProperty;

// Gui control property style color element
typedef enum { BORDER = 0, BASE, TEXT, OTHER } GuiPropertyElement;

static Font guiFont = { 0 };            // Gui current font (WARNING: highly coupled to raylib)
static float guiAlpha = 1.0f;           // Gui element transpacency on drawing

static bool guiStyleLoaded = false;     // Style loaded flag for lazy style initialization
static unsigned int guiStyle[NUM_CONTROLS*(NUM_PROPS_DEFAULT + NUM_PROPS_EXTENDED)] = { 0 };

static void GuiLoadStyleDefault(void);
// Get control style property value
static int GuiGetStyle(int control, int property);
// Gui get text width using default font
static int GetTextWidth(const char *text);
// Get text bounds considering control bounds
static Rectangle GetTextBounds(int control, Rectangle bounds);
// Gui draw rectangle using default raygui plain style with borders
static void GuiDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color color);

// PUBLIC METHODS
void GuiDrawText(const char *text, Rectangle bounds, int alignment, Color tint);
int GuiTabs(Rectangle bounds, char **entries, int num_entries, int current_entry);
void GuiLabelBox(int posX, int posY, char *label, char *content);
bool GuiTextEdit(Rectangle bounds, char *text, bool editMode);
bool GuiButton(Rectangle bounds, const char *text);
//void DrawInteract(Interact *tr);
bool GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool editMode);
#endif //TRANSVERSE_GUI_TOOLS_H
