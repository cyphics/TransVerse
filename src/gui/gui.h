/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */
#if !defined(TRANSVERSE_GUI_H)
#define TRANSVERSE_GUI_H

#include <raylib.h>

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

typedef struct Point{
    int posX;
    int posY;
} Point;

int GuiListViewExDrag(Rectangle bounds,
                      const char **text,
                      int count,
                      int *focus,
                      int *scrollIndex,
                      int active,
                      void (DragGallBack(int)));
void StartGui();

#endif
