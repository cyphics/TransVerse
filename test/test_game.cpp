/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <assert.h>
#include <string.h>

#include "test_main.h"
#include "storage.h"

void run_test_game(void)
{
    resource res_list[4] = {{"energy", 10}, {"copper", 40}};
    char price_str[200] = {};
    price a_price;
    memcpy(a_price.resources, res_list, sizeof(res_list));
    PriceToStr(price_str, a_price);
    assert(strcmp(price_str, "<Price>\n<Resource>\n<type>energy</type>\n<amount>10</amount>\n</Resource>\n<Resource>\n<type>copper</type>\n<amount>40</amount>\n</Resource>\n</Price>\n") == 0);


    char segment[] = "<Rand>\n</Rand>\n<Upgrade>\n<entry>blah</entry>\n</Upgrade>";
    char result[1000] = {};
    result[0] = '\0';
    GetXmlSegment(result, segment, "Upgrade");
    assert(strcmp(result, "<entry>blah</entry>\n") == 0);
}
