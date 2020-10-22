/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */
#if !defined(TRANSVERSE_GUI_H)

#define TRANSVERSE_GUI_H

#include <raylib.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 600
#define SCALE_FACTOR 1

// Hover animation
const float gradWidth = 30.0f;
const float gradHeight = 30.0f;
const int hoverLength = 6; // In frames
Rectangle staticRect = {0, 0, gradWidth, gradHeight};
Texture2D gradientTexture;
Color hovered = LIGHTGRAY;
Color unhovered = RAYWHITE;

int framesCounter = 0;

Vector2 mousePosition;
int mouseGesture;
int keyPressed;
bool mouseLeftPressed;
bool mouseLeftReleased;
bool mouseRightPressed;
bool mouseRightReleased;
const float defaultFontSize = 20.0;

enum Panel{
    TERMINAL_TAB,
    EDITOR_TAB,
    SHIP_TAB,
    WORKSHOP_TAB
};

typedef struct Interact{
    char *id;
    char *title;
    Rectangle rect;
    char *text;
    bool isHoverable;
    bool isEditable;
    int hoverIdx;
    bool isHovered;
    float fontSize;
} Interact;

void DrawInteract(Interact *tr);
int GuiListViewExDrag(Rectangle bounds, const char **text, int count, int *focus, int *scrollIndex, int active, void (DragGallBack(int)));
void StartGui();

#endif
