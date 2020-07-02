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
    TERMINAL,
    EDITOR,
    SHIP
};

struct Interact {
    char *id;
    char *title;
    Rectangle rect;
    char *text;
    bool isHoverable;
    bool isEditable;
    int hoverIdx = 0;
    bool isHovered = false;
    float fontSize = defaultFontSize;
};

void DrawInteract(Interact *tr);
// void SartGui();

#endif
