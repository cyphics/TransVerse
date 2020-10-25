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
} GuiControlState;

const float gradWidth = 30.0f;
const float gradHeight = 30.0f;
const int hoverLength = 6; // In frames
Rectangle staticRect = {0, 0, gradWidth, gradHeight};
Texture2D gradientTexture;
Color hovered = LIGHTGRAY;
Color unhovered = RAYWHITE;
int framesCounter = 0;

// Set control style property value
void GuiSetStyle(int control, int property, int value) {
    if (!guiStyleLoaded) GuiLoadStyleDefault();
    guiStyle[control * (NUM_PROPS_DEFAULT + NUM_PROPS_EXTENDED) + property] = value;

    // Default properties are propagated to all controls
    if ((control == 0) && (property < NUM_PROPS_DEFAULT)) {
        for (int i = 1; i < NUM_CONTROLS; i++)
            guiStyle[i * (NUM_PROPS_DEFAULT + NUM_PROPS_EXTENDED) + property] = value;
    }
}
// Get control style property value

void GuiLoadStyleDefault(void) {
    // We set this variable first to avoid cyclic function calls
    // when calling GuiSetStyle() and GuiGetStyle()
    guiStyleLoaded = true;

    // Initialize default LIGHT style property values
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

    guiFont = GetFontDefault();     // Initialize default font
}

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

bool GuiTextEdit(Rectangle bounds, char *text, bool editMode) {
    static int framesCounter = 0;

    bool pressed = false;

    Rectangle cursor = {
            bounds.x + GuiGetStyle(TEXTBOX, TEXT_PADDING) + GetTextWidth(text) + 2,
            bounds.y + bounds.height / 2 - GuiGetStyle(DEFAULT, TEXT_SIZE),
            1,
            GuiGetStyle(DEFAULT, TEXT_SIZE) * 2
    };

    Vector2 mousePoint = GetMousePosition();
    GuiControlState state = GUI_STATE_NORMAL;

    if (editMode) {
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
            (!CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            pressed = true;

        // Check text alignment to position cursor properly
        int textAlignment = GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT);
        if (textAlignment == GUI_TEXT_ALIGN_CENTER) cursor.x = bounds.x + GetTextWidth(text) / 2 + bounds.width / 2 + 1;
        else if (textAlignment == GUI_TEXT_ALIGN_RIGHT)
            cursor.x = bounds.x + bounds.width - GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING);
    } // if (editMode)
    else {
        if (CheckCollisionPointRec(mousePoint, bounds)) {
            state = GUI_STATE_FOCUSED;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) pressed = true;
        }
    }

    if (pressed) framesCounter = 0;

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == GUI_STATE_PRESSED) {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH),
                         Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state * 3))), guiAlpha),
                         Fade(GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)), guiAlpha));

        // Draw blinking cursor
        if (editMode && ((framesCounter / 20) % 2 == 0))
            GuiDrawRectangle(cursor, 0, BLANK, Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)), guiAlpha));
    } else if (state == GUI_STATE_DISABLED) {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH),
                         Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state * 3))), guiAlpha),
                         Fade(GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_DISABLED)), guiAlpha));
    } else
        GuiDrawRectangle(bounds, 1, Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state * 3))), guiAlpha),
                         BLANK); // TODO understand

    GuiDrawText(text, GetTextBounds(TEXTBOX, bounds), GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT),
                Fade(GetColor(GuiGetStyle(TEXTBOX, TEXT + (state * 3))), guiAlpha));
    //--------------------------------------------------------------------

    return pressed;
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
static void GuiDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color color) {
    if (color.a > 0) {
        // Draw rectangle filled with color
        DrawRectangle(rec.x, rec.y, rec.width, rec.height, color);
    }

    if (borderWidth > 0) {
        // Draw rectangle border lines with color
        DrawRectangle(rec.x, rec.y, rec.width, borderWidth, borderColor);
        DrawRectangle(rec.x, rec.y + borderWidth, borderWidth, rec.height - 2 * borderWidth, borderColor);
        DrawRectangle(rec.x + rec.width - borderWidth, rec.y + borderWidth, borderWidth, rec.height - 2 * borderWidth,
                      borderColor);
        DrawRectangle(rec.x, rec.y + rec.height - borderWidth, rec.width, borderWidth, borderColor);
    }

    // TODO: For n-patch-based style we would need: [state] and maybe [control]
    // In this case all controls drawing logic should be moved to this function... I don't like it...
}

bool GuiButton(Rectangle bounds, const char *text) {
    GuiControlState state = GUI_STATE_NORMAL;
    bool pressed = false;

    // Update control
    //--------------------------------------------------------------------
    Vector2 mousePoint = GetMousePosition();

    // Check button state
    if (CheckCollisionPointRec(mousePoint, bounds)) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = GUI_STATE_PRESSED;
        else state = GUI_STATE_FOCUSED;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) pressed = true;
    }

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH),
                     Fade(GetColor(GuiGetStyle(BUTTON, BORDER + (state * 3))), guiAlpha),
                     Fade(GetColor(GuiGetStyle(BUTTON, BASE + (state * 3))), guiAlpha));
    GuiDrawText(text, GetTextBounds(BUTTON, bounds), GuiGetStyle(BUTTON, TEXT_ALIGNMENT),
                Fade(GetColor(GuiGetStyle(BUTTON, TEXT + (state * 3))), guiAlpha));
    //------------------------------------------------------------------

    return pressed;
}

void GuiLabelBox(int posX, int posY, char *label, char *content) {
    int fontSize = FONT_SIZE;
    DrawText(label, posX, posY, fontSize, BLACK);
    int text_size = MeasureText(label, fontSize);
    int spacing = 10;
    DrawLine(posX + text_size + spacing, posY, posX + text_size, posY + 10, LIGHTGRAY);
    DrawText(content, posX + text_size + spacing * 2, posY, fontSize, LIGHTGRAY);
}

int GuiTabs(Rectangle bounds, char **entries, int num_entries, int current_entry) {
    float padding = 4.0f;
    float tab_width = (bounds.width - (padding * (num_entries + 1))) / num_entries;
    float tab_height = bounds.height - padding * 2;
    float tab_y_anchor = bounds.y + padding;
    Vector2 mousePos = GetMousePosition();

    DrawRectangleLinesEx(bounds, 2, BLACK);
    for (int i = 0; i < num_entries; ++i) {

        float x_anchor = bounds.x + padding + (tab_width + padding) * i;
        Rectangle tabRect = {x_anchor, tab_y_anchor, tab_width, tab_height};
        if (i == current_entry)
            DrawRectangleRec(tabRect, LIGHTGRAY);
        if (CheckCollisionPointRec(mousePos, tabRect)) {
            DrawRectangleRec(tabRect, Fade(BLUE, 0.5f));
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                current_entry = i;
            }
        }
        DrawRectangleLinesEx(tabRect, 1, BLACK);
        DrawText(entries[i], x_anchor + padding, tab_y_anchor, FONT_SIZE, GRAY);
    }
    return current_entry;
}