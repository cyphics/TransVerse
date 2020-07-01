/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */
#if !defined(GUI_H)

#define GUI_H

struct Interact {
    char *id;
    char *title;
    Rectangle rect;
    char *text;
    bool isHovered = false;
    void *onClic;
    int hoverIdx = 0;
};

void DrawInteract(Interact *tr);
// void SartGui();

#endif
