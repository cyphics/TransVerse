/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <raylib.h>

const char message[128] = "This sample illustrates a text writing\nanimation effect! Check it out! ;)";
int lines = 0;
// Color backgroundColor = {50, 48, 47, 255};
Color backgroundColor = DARKBROWN;
// Color textColor = {179, 182, 38, 255};
Color textColor = BEIGE;
Color textColorBack = {136, 137, 41, 255};
char symbol;
// Font editorFont = GetFontDefault();
float editorAnchorX, editorAnchorY;
Rectangle codeContainer;
int codeFontSize = 10;
int codeFontPadding = 2;
char buffer[200000];
int codePosition = 0;
int textStep = 1;

void OpenFile(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error, file not loaded\n");
    } else printf("File loaded successfully\n");
    if (fp) {
        while ((symbol = getc(fp)) != EOF) {
            strcat(buffer, &symbol);
        }
        fclose(fp);
    }
}

void ResizeEditor(float x, float y)
{
    editorAnchorX = x;
    editorAnchorY = y;

    codeContainer = (Rectangle){ editorAnchorX, editorAnchorY, 300, 250};
    OpenFile("../assets/3dfirmware.txt");
}

void TypeCode()
{

    DrawRectangleLinesEx(codeContainer, 1, BLACK);
    if ((keyPressed >= 32) && (keyPressed <= 125)) {
        codePosition+= textStep;
    }
    if (IsKeyPressed(KEY_ENTER)) codePosition = 0;
    if (IsKeyPressed(KEY_SPACE)) textStep++;

    const char *textToDraw = TextSubtext(buffer, 0, codePosition);

    DrawText(textToDraw, editorAnchorX + padding, editorAnchorY + padding, codeFontSize, textColorBack);

}
