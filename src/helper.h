/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */
#if !defined(HELPER_H)
#define HELPER_H

typedef struct Timer
{
    int start = 0;
    int duration = 0; // in frames
    int elapsed = 0;
} Timer;


bool AreStrEquals(char *ch1, char *ch2);
bool IsEmpty(char *str);

#endif
