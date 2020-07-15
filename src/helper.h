/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */
#if !defined(HELPER_H)
#define HELPER_H

#define SIZEOF(A) (sizeof(A)/sizeof(A[0]))

typedef struct Timer
{
    int start;
    int duration; // in frames
    int elapsed;
} Timer;


bool AreStrEquals(char *ch1, char *ch2);
bool IsEmpty(char *str);

#endif
