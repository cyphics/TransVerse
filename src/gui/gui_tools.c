//
// Created by cyphics on 22/10/2020.
//

#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "gui_tools.h"
#include "gui.h"
#include "gui_constants.h"

// Gui control state
typedef enum {
    GUI_STATE_NORMAL = 0,
    GUI_STATE_FOCUSED,
    GUI_STATE_PRESSED,
    GUI_STATE_DISABLED,
    GUI_STATE_SELECTED
} GuiControlState;

const float gradWidth = 30.0f;
const float gradHeight = 30.0f;
const int hoverLength = 6; // In frames
Rectangle staticRect = {0, 0, gradWidth, gradHeight};
Texture2D gradientTexture;
Color hovered = LIGHTGRAY;
Color unhovered = RAYWHITE;
int framesCounter = 0;

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
    SECONDARY_BACKGROUND_COLOR,
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

// Button types
typedef enum {
    BUTTON_DEFAULT = 0,
    BUTTON_TAB
} GuiButtonType;

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
    BORDER_COLOR_SELECTED,
    BASE_COLOR_SELECTED,
    TEXT_COLOR_SELECTED,
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

// Gui control property style background_color element
typedef enum {
    BORDER = 0,
    BASE,
    TEXT,
    NUMBER_PROPERTIES } GuiPropertyElement;

static Font guiFont = { 0 };            // Gui current font (WARNING: highly coupled to raylib)
static float guiAlpha = 1.0f;           // Gui element transpacency on drawing

static bool guiStyleLoaded = false;     // Style loaded flag for lazy style initialization
static unsigned int guiStyle[NUM_CONTROLS*(NUM_PROPS_DEFAULT + NUM_PROPS_EXTENDED)] = { 0 };


// Set control style property value

static void GuiSetStyle(int control, int property, int value);
static void GuiLoadStyleDefault(void);

static void GuiSetStyle(int control, int property, int value) {
    if (!guiStyleLoaded) GuiLoadStyleDefault();
    guiStyle[control * (NUM_PROPS_DEFAULT + NUM_PROPS_EXTENDED) + property] = value;

    // Default properties are propagated to all controls
    if ((control == 0) && (property < NUM_PROPS_DEFAULT)) {
        for (int i = 1; i < NUM_CONTROLS; i++)
            guiStyle[i * (NUM_PROPS_DEFAULT + NUM_PROPS_EXTENDED) + property] = value;
    }
}
static void GuiLoadStyleDefault(void) {
    // We set this variable first to avoid cyclic function calls
    // when calling GuiSetStyle() and GuiGetStyle()
    guiStyleLoaded = true;

    // Initialize default LIGHT style property values
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x838383ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0xc9c9c9ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x686868ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0x5bb2d9ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0xc9effeff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x6c9bbcff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0x0492c7ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x97e8ffff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0x368bafff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, 0xb5c1c2ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, 0xe6e9e9ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0xaeb7b8ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_SELECTED, 0x838383ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_SELECTED, 0x838383ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_SELECTED, 0xc9c9c9ff);
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x90abb5ff);       // DEFAULT specific property
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0xf5f5f5ff);       // DEFAULT specific property
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);                       // WARNING: Some controls use other values
    GuiSetStyle(DEFAULT, TEXT_PADDING, 0);                       // WARNING: Some controls use other values
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER); // WARNING: Some controls use other values
    GuiSetStyle(DEFAULT, TEXT_PADDING, 0);   // WARNING: Some controls use other values

    // Initialize control-specific property values
    // NOTE: Those properties are in default list but require specific values by control type
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 5);
    GuiSetStyle(TEXTBOX, TEXT_INNER_PADDING, 4);
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);

    // Initialize extended property values
    // NOTE: By default, extended property values are initialized to 0
    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);                // DEFAULT, shared by all controls
    GuiSetStyle(DEFAULT, TEXT_SPACING, 1);
    GuiSetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_PADDING, 2);
    GuiSetStyle(DROPDOWNBOX, ARROW_PADDING, 16);
    guiFont = GetFontDefault();     // Initialize default font
}

// Get control style property value

int GuiGetStyle(int control, int property) {
    if (!guiStyleLoaded) GuiLoadStyleDefault();
    return guiStyle[control * (NUM_PROPS_DEFAULT + NUM_PROPS_EXTENDED) + property];
}

static int GetTextWidth(const char *text) {
    Vector2 size = {0};

    if ((text != NULL) && (text[0] != '\0'))
        size = MeasureTextEx(guiFont, text, GuiGetStyle(DEFAULT, TEXT_SIZE), GuiGetStyle(DEFAULT, TEXT_SPACING));

    // TODO: Consider text icon width here???

    return (int) size.x;
}

// Get text bounds considering control bounds
static Rectangle GetTextBounds(int control, Rectangle bounds) {
    Rectangle textBounds = bounds;

    textBounds.x = bounds.x + GuiGetStyle(control, BORDER_WIDTH);
    textBounds.y = bounds.y + GuiGetStyle(control, BORDER_WIDTH);
    textBounds.width = bounds.width - 2 * GuiGetStyle(control, BORDER_WIDTH);
    textBounds.height = bounds.height - 2 * GuiGetStyle(control, BORDER_WIDTH);

    // Consider TEXT_PADDING properly, depends on control type and TEXT_ALIGNMENT
    switch (control) {
//        case COMBOBOX: bounds.width -= (GuiGetStyle(control, COMBO_BUTTON_WIDTH) + GuiGetStyle(control, COMBO_BUTTON_PADDING)); break;
        case VALUEBOX:
            break;   // NOTE: ValueBox text value always centered, text padding applies to label
        default: {
            if (GuiGetStyle(control, TEXT_ALIGNMENT) == GUI_TEXT_ALIGN_RIGHT)
                textBounds.x -= GuiGetStyle(control, TEXT_PADDING);
            else textBounds.x += GuiGetStyle(control, TEXT_PADDING);
        }
            break;
    }

    // TODO: Special cases (no label): COMBOBOX, DROPDOWNBOX, LISTVIEW (scrollbar?)
    // More special cases (label side): CHECKBOX, SLIDER, VALUEBOX, SPINNER

    return textBounds;
}

void DrawHoverable(Rectangle bounds, GuiControlState state) {
    float alpha;
    if (state == GUI_STATE_FOCUSED) {
        alpha = 0.8f;
    } else {
        alpha = 0.2f;
    }
    DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, Fade(BLUE, alpha));
}

// Gui draw text using default font
void GuiDrawText(const char *text, Rectangle bounds, int alignment, Color tint) {
#define TEXT_VALIGN_PIXEL_OFFSET(h)  ((int)h%2)     // Vertical alignment for pixel perfect

    if ((text != NULL) && (text[0] != '\0')) {
        Vector2 position = {bounds.x, bounds.y};

        // NOTE: We get text size after icon been processed
        int textWidth = GetTextWidth(text);
        int textHeight = GuiGetStyle(DEFAULT, TEXT_SIZE);

        // Check guiTextAlign global variables
        switch (alignment) {
            case GUI_TEXT_ALIGN_LEFT: {
                position.x = bounds.x;
                position.y = bounds.y + bounds.height / 2 - textHeight / 2 + TEXT_VALIGN_PIXEL_OFFSET(bounds.height);
            }
                break;
            case GUI_TEXT_ALIGN_CENTER: {
                position.x = bounds.x + bounds.width / 2 - textWidth / 2;
                position.y = bounds.y + bounds.height / 2 - textHeight / 2 + TEXT_VALIGN_PIXEL_OFFSET(bounds.height);
            }
                break;
            case GUI_TEXT_ALIGN_RIGHT: {
                position.x = bounds.x + bounds.width - textWidth;
                position.y = bounds.y + bounds.height / 2 - textHeight / 2 + TEXT_VALIGN_PIXEL_OFFSET(bounds.height);
            }
                break;
            default:
                break;
        }

        // NOTE: Make sure we get pixel-perfect coordinates,
        // In case of decimals we got weird text positioning
        position.x = (float) ((int) position.x);
        position.y = (float) ((int) position.y);

        DrawTextEx(guiFont, text, position, GuiGetStyle(DEFAULT, TEXT_SIZE), GuiGetStyle(DEFAULT, TEXT_SPACING), tint);
        //---------------------------------------------------------------------------------
    }
}

// Gui draw rectangle using default raygui plain style with borders
static void GuiDrawRectangle(Rectangle bounds, int border_width, Color border_color, Color background_color) {
    if (background_color.a > 0) {
        DrawRectangleRec(bounds, background_color);
    }

    if (border_width > 0) {
        DrawRectangle(bounds.x, bounds.y, bounds.width, border_width, border_color);
        DrawRectangle(bounds.x, bounds.y + border_width, border_width, bounds.height - 2 * border_width, border_color);
        DrawRectangle(bounds.x + bounds.width - border_width,
                      bounds.y + border_width, border_width,
                      bounds.height - 2 * border_width, border_color);
        DrawRectangle(bounds.x, bounds.y + bounds.height - border_width, bounds.width, border_width, border_color);
    }

}

bool GuiTextEdit(Rectangle bounds, char *text, bool edit_mode) {
    static int framesCounter = 0;

    bool is_clicked = false;

    Rectangle cursor = {
            bounds.x + GuiGetStyle(TEXTBOX, TEXT_PADDING) + GetTextWidth(text) + 2,
            bounds.y + bounds.height / 2 - GuiGetStyle(DEFAULT, TEXT_SIZE),
            1,
            GuiGetStyle(DEFAULT, TEXT_SIZE) * 2
    };

    Vector2 mouse_point = GetMousePosition();
    GuiControlState state = GUI_STATE_NORMAL;

    if (edit_mode) {
        state = GUI_STATE_PRESSED;
        framesCounter++;

        int key = GetKeyPressed();      // Returns codepoint as Unicode
        int keyCount = strlen(text);

        // Only allow keys in range [32..125]
        if (keyCount < (GuiGetStyle(TEXTBOX, TEXT_SIZE) - 1)) {
            int maxWidth = (bounds.width - (GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING) * 2));

            if ((GetTextWidth(text) < (maxWidth - GuiGetStyle(DEFAULT, TEXT_SIZE))) && (key >= 32)) {
                int byteLength = 0;
                const char *textUtf8 = CodepointToUtf8(key, &byteLength);

                for (int i = 0; i < byteLength; i++) {
                    text[keyCount] = textUtf8[i];
                    keyCount++;
                }
                text[keyCount] = '\0';
            }
        }

        // Delete text
        if (keyCount > 0) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                keyCount--;
                text[keyCount] = '\0';
                framesCounter = 0;
                if (keyCount < 0) keyCount = 0;
            } else if (IsKeyDown(KEY_BACKSPACE)) {
                if ((framesCounter > TEXTEDIT_CURSOR_BLINK_FRAMES) && (framesCounter % 2) == 0) keyCount--;
                text[keyCount] = '\0';
                if (keyCount < 0) keyCount = 0;
            }
        }

        if (IsKeyPressed(KEY_ENTER) ||
            (!CheckCollisionPointRec(mouse_point, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            is_clicked = true;

        // Check text alignment to position cursor properly
        int textAlignment = GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT);
        if (textAlignment == GUI_TEXT_ALIGN_CENTER) cursor.x = bounds.x + GetTextWidth(text) / 2 + bounds.width / 2 + 1;
        else if (textAlignment == GUI_TEXT_ALIGN_RIGHT)
            cursor.x = bounds.x + bounds.width - GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING);
    } // if (edit_mode)
    else {
        if (CheckCollisionPointRec(mouse_point, bounds)) {
            state = GUI_STATE_FOCUSED;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) is_clicked = true;
        }
    }

    if (is_clicked) framesCounter = 0;

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == GUI_STATE_PRESSED) {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH),
                         Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state * NUMBER_PROPERTIES))), guiAlpha),
                         Fade(GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)), guiAlpha));

        // Draw blinking cursor
        if (edit_mode && ((framesCounter / 20) % 2 == 0))
            GuiDrawRectangle(cursor, 0, BLANK, Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)), guiAlpha));
    } else if (state == GUI_STATE_DISABLED) {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH),
                         Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state * NUMBER_PROPERTIES))), guiAlpha),
                         Fade(GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_DISABLED)), guiAlpha));
    } else
        GuiDrawRectangle(bounds, 1, Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state * NUMBER_PROPERTIES))), guiAlpha),
                         BLANK); // TODO understand

    GuiDrawText(text, GetTextBounds(TEXTBOX, bounds), GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT),
                Fade(GetColor(GuiGetStyle(TEXTBOX, TEXT + (state * NUMBER_PROPERTIES))), guiAlpha));
    //--------------------------------------------------------------------

    return is_clicked;
}

bool GuiButtonAbstract(Rectangle bounds, const char *text, int button_type,
                       bool is_selected, bool is_enabled) {
    GuiControlState state = GUI_STATE_NORMAL;
    if(is_selected) state = GUI_STATE_SELECTED;
    if(!is_enabled) state = GUI_STATE_DISABLED;
    bool is_clicked = false;

    // Update control
    //--------------------------------------------------------------------
    Vector2 mouse_point = GetMousePosition();

    // Check button state
    if (CheckCollisionPointRec(mouse_point, bounds)) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = GUI_STATE_PRESSED;
        else state = GUI_STATE_FOCUSED;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) is_clicked = true;
    }

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH),
                     Fade(GetColor(GuiGetStyle(BUTTON, BORDER + (state * NUMBER_PROPERTIES))), guiAlpha),
                     Fade(GetColor(GuiGetStyle(BUTTON, BASE + (state * NUMBER_PROPERTIES))), guiAlpha));
    GuiDrawText(text, GetTextBounds(BUTTON, bounds), GuiGetStyle(BUTTON, TEXT_ALIGNMENT),
                Fade(GetColor(GuiGetStyle(BUTTON, TEXT + (state * 4))), guiAlpha));
    //------------------------------------------------------------------

    return is_clicked;
}

bool GuiButton(Rectangle bounds, const char *text, bool is_enabled) {
    return GuiButtonAbstract(bounds, text, BUTTON_DEFAULT, false, is_enabled);
}

bool GuiButtonTab(Rectangle bounds, const char *text, bool is_selected) {
    return GuiButtonAbstract(bounds, text, BUTTON_TAB, is_selected, true);
}

void GuiTabs(Rectangle bounds, char **entries, int num_entries, int *current_entry) {
    float padding = 4.0f;
    float tab_width = (bounds.width - (padding * (num_entries + 1))) / num_entries;
    float tab_height = bounds.height - padding * 2;
    float tab_y_anchor = bounds.y + padding;
    Vector2 mouse_pos = GetMousePosition();
    bool is_selected;

    DrawRectangleLinesEx(bounds, 2, BLACK);
    for (int i = 0; i < num_entries; ++i) {
        is_selected = false;
        float x_anchor = bounds.x + padding + (tab_width + padding) * i;
        Rectangle tab_rect = {x_anchor, tab_y_anchor, tab_width, tab_height};
        if(*current_entry == i)
            is_selected = true;
        if(GuiButtonTab(tab_rect, entries[i], is_selected))
            *current_entry = i;
    }
}

// Split controls text into multiple strings
// Also check for multiple columns (required by GuiToggleGroup())
static const char **GuiTextSplit(const char *text, int *count, int *textRow) {
    // NOTE: Current implementation returns a copy of the provided string with '\0' (string end delimiter)
    // inserted between strings defined by "delimiter" parameter. No memory is dynamically allocated,
    // all used memory is static... it has some limitations:
    //      1. Maximum number of possible split strings is set by TEXTSPLIT_MAX_TEXT_ELEMENTS
    //      2. Maximum size of text to split is TEXTSPLIT_MAX_TEXT_LENGTH
    // NOTE: Those definitions could be externally provided if required

#if !defined(TEXTSPLIT_MAX_TEXT_LENGTH)
#define TEXTSPLIT_MAX_TEXT_LENGTH      1024
#endif

#if !defined(TEXTSPLIT_MAX_TEXT_ELEMENTS)
#define TEXTSPLIT_MAX_TEXT_ELEMENTS     128
#endif

    static const char *result[TEXTSPLIT_MAX_TEXT_ELEMENTS] = {NULL};
    static char buffer[TEXTSPLIT_MAX_TEXT_LENGTH] = {0};
    memset(buffer, 0, TEXTSPLIT_MAX_TEXT_LENGTH);

    result[0] = buffer;
    int counter = 1;

    if (textRow != NULL) textRow[0] = 0;

    // Count how many substrings we have on text and point to every one
    for (int i = 0; i < TEXTSPLIT_MAX_TEXT_LENGTH; i++) {
        buffer[i] = text[i];
        if (buffer[i] == '\0') break;
        else if ((buffer[i] == ';') || (buffer[i] == '\n')) {
            result[counter] = buffer + i + 1;

            if (textRow != NULL) {
                if (buffer[i] == '\n') textRow[counter] = textRow[counter - 1] + 1;
                else textRow[counter] = textRow[counter - 1];
            }

            buffer[i] = '\0';   // Set an end of string at this point

            counter++;
            if (counter == TEXTSPLIT_MAX_TEXT_ELEMENTS) break;
        }
    }

    *count = counter;

    return result;
}

// Panel control
void GuiPanel(Rectangle bounds)
{
#define PANEL_BORDER_WIDTH   1

    GuiControlState state = GUI_STATE_NORMAL;

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, PANEL_BORDER_WIDTH, Fade(GetColor(GuiGetStyle(DEFAULT, (state == GUI_STATE_DISABLED)? BORDER_COLOR_DISABLED: LINE_COLOR)), guiAlpha),
                     Fade(GetColor(GuiGetStyle(DEFAULT, (state == GUI_STATE_DISABLED)? BASE_COLOR_DISABLED : BACKGROUND_COLOR)), guiAlpha));
    //--------------------------------------------------------------------
}

// Dropdown Box control
// NOTE: Returns mouse click
bool GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool edit_mode) {

    GuiControlState state = GUI_STATE_NORMAL;
    int selected_item = *active;
    int focused_item = -1;

    // Get substrings items from text (items pointers, lengths and count)
    int item_counter = 0;
    const char **items = GuiTextSplit(text, &item_counter, NULL);

    Rectangle open_bounds = bounds;
    open_bounds.height = (item_counter + 1) * (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_PADDING));

    Rectangle item_bounds = bounds;

    bool is_clicked = false;

    // Update control
    //--------------------------------------------------------------------

    Vector2 mouse_point = GetMousePosition();

    if (edit_mode) {
        state = GUI_STATE_PRESSED;

        // Check if mouse has been clicked or released outside limits
        if (!CheckCollisionPointRec(mouse_point, open_bounds)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) is_clicked = true;
        }

        // Check if already selected item has been clicked again
        if (CheckCollisionPointRec(mouse_point, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) is_clicked = true;

        // Check focused and selected item
        for (int i = 0; i < item_counter; i++) {

            item_bounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_PADDING));

            if (CheckCollisionPointRec(mouse_point, item_bounds)) {
                focused_item = i;
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    selected_item = i;
                    is_clicked = true;     // Item selected, change to edit_mode = false
                }
                break;
            }
        }

        item_bounds = bounds;
    } else {
        if (CheckCollisionPointRec(mouse_point, bounds)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                is_clicked = true;
                state = GUI_STATE_PRESSED;
            } else state = GUI_STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (edit_mode) GuiPanel(open_bounds);
    Rectangle tmp_rect = GetTextBounds(DEFAULT, bounds);
    GuiDrawRectangle(bounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH),
                     Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BORDER + state * NUMBER_PROPERTIES)), guiAlpha),
                     Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BASE + state * NUMBER_PROPERTIES)), guiAlpha));
    GuiDrawText(items[selected_item], GetTextBounds(DEFAULT, bounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT),
                Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + state * NUMBER_PROPERTIES)), guiAlpha));

    if (edit_mode) {
        // Draw visible items
        for (int i = 0; i < item_counter; i++) {
            // Update item rectangle y position for next item
            item_bounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_PADDING));

            if (i == selected_item) {
                GuiDrawRectangle(item_bounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH),
                                 Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_PRESSED)), guiAlpha),
                                 Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED)), guiAlpha));
                GuiDrawText(items[i], GetTextBounds(DEFAULT, item_bounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT),
                            Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED)), guiAlpha));
            } else if (i == focused_item) {
                GuiDrawRectangle(item_bounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH),
                                 Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED)), guiAlpha),
                                 Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED)), guiAlpha));
                GuiDrawText(items[i], GetTextBounds(DEFAULT, item_bounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT),
                            Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_FOCUSED)), guiAlpha));
            } else
                GuiDrawText(items[i], GetTextBounds(DEFAULT, item_bounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT),
                            Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL)), guiAlpha));
        }
    }

    // TODO: Avoid this function, use icon instead or 'v'
    DrawTriangle((Vector2) {bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING),
                            bounds.y + bounds.height / 2 - 2},
                 (Vector2) {bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING) + 5,
                            bounds.y + bounds.height / 2 - 2 + 5},
                 (Vector2) {bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING) + 10,
                            bounds.y + bounds.height / 2 - 2},
                 Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state * NUMBER_PROPERTIES))), guiAlpha));

    *active = selected_item;
    return is_clicked;
}

// Label control
void GuiLabel(Rectangle bounds, const char *text)
{
    GuiControlState state = GUI_STATE_NORMAL;

    // Update control
    //--------------------------------------------------------------------
    // ...
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawText(text, GetTextBounds(LABEL, bounds), GuiGetStyle(LABEL, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(LABEL, (state == GUI_STATE_DISABLED)? TEXT_COLOR_DISABLED : TEXT_COLOR_NORMAL)), guiAlpha));
    //--------------------------------------------------------------------
}
