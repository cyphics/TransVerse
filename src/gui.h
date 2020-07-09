/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */
#if !defined(GUI_H)

#define GUI_H

const float defaultFontSize = 20.0;

enum Panel{
    TERMINAL_TAB,
    EDITOR_TAB,
    SHIP_TAB,
    WORKSHOP_TAB
};

typedef struct Interact {
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
// void SartGui();

#endif
