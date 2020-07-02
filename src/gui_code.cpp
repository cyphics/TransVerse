/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

const char message[128] = "This sample illustrates a text writing\nanimation effect! Check it out! ;)";
int lines = 0;
// Color backgroundColor = {50, 48, 47, 255};
Color backgroundColor = DARKBROWN;
// Color textColor = {179, 182, 38, 255};
Color textColor = BEIGE;
Color textColorBack = {136, 137, 41, 255};
char symbol;
Font editorFont = GetFontDefault();
float editorAnchorX, editorAnchorY;
Rectangle codeContainer;
int codeFontSize = 20;
int codeFontPadding = 2;
char buffer[200000];
int codePosition = 0;
    int textStep = 1;

void OpenFile()
{
    FILE *fp = fopen("../assets/3dfirmware.txt", "r");
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

    codeContainer = { 25, 25, editorAnchorX, editorAnchorY};
    OpenFile();
}

void TypeCode()
{

        int key = GetKeyPressed();
        if ((key >= 32) && (key <= 125)) {
            codePosition+= textStep;
        }
        if (IsKeyPressed(KEY_ENTER)) codePosition = 0;
        if (IsKeyPressed(KEY_SPACE)) textStep++;

        const char *textToDraw = TextSubtext(buffer, 0, codePosition);

        DrawText(textToDraw, editorAnchorX, editorAnchorY, codeFontSize, textColorBack);



        // DrawTextRec(editorFont, textToDraw, codeContainer, codeFontSize, codeFontPadding, true, textColor);


        // DrawText("PRESS [ENTER] to RESTART!", 240, 500, 20, LIGHTGRAY);
        // DrawText("PRESS [SPACE] to SPEED UP!", 239, 540, 20, LIGHTGRAY);


        //----------------------------------------------------------------------------------
}
