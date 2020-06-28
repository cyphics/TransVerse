/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

bool AreStrEquals(char *ch1, char *ch2)
{
    return strcmp(ch1, ch2) == 0;
}

bool IsEmpty(char *str)
{
    return AreStrEquals(str, "");
}
