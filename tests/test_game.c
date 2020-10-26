
#include <stdlib.h>
#include <stdint.h>
#include <check.h>
#include <game.h>
#include <storage.h>

static void setup(void)
{

}

static void teardown(void)
{

}

START_TEST(price_to_string) {
    resource res_list[4] = {{"energy", 10},
                            {"copper", 40}};
    char price_str[200] = {};
    price a_price;
    memcpy(a_price.resources, res_list, sizeof(res_list));
    PriceToStr(price_str, a_price);
    ck_assert_str_eq(price_str,
                     "<Price>\n<Resource>\n<type>energy</type>\n<amount>10</amount>\n</Resource>\n<Resource>\n<type>copper</type>\n<amount>40</amount>\n</Resource>\n</Price>\n");
}END_TEST

Suite * game_test_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Game");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, price_to_string);
    suite_add_tcase(s, tc_core);


    return s;
}



