/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <time.h>
#include "helper.h"

bool AreStrEquals(char *ch1, char *ch2)
{
    return strcmp(ch1, ch2) == 0;
}

bool IsEmpty(char *str)
{
    return AreStrEquals(str, "");
}

void StartTimer(Timer *t, int duration)
{
    t->duration = duration;
    t->elapsed = 0;
    t->start = 0;
}

int time_idx;
bool IsTimerRunning(Timer *t)
{
    if (t->elapsed >= t->duration) {

        return false;
    }
    t->elapsed++;
    return true;
}