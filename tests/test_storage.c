//
// Created by cyphics on 7/19/20.
//

#include <stdlib.h>
#include <stdint.h>
#include <check.h>
#include <storage.h>
// #include "game.h"
//#include "includes.c"

static void setup(void)
{

}

static void teardown(void)
{

}

START_TEST(price_to_string_simple) {
    char *segment= "<Rand>\n</Rand>\n<Upgrade>\n<entry>blah</entry>\n</Upgrade>";
    char result[1000] = {};
    result[0] = '\0';
    GetXmlSegment(result, segment, "Upgrade");
    ck_assert_str_eq(result, "<entry>blah</entry>\n");
}END_TEST



Suite * storage_test_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Storage");

    /* Core test case */
    tc_core = tcase_create("XML creation");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, price_to_string_simple);
    suite_add_tcase(s, tc_core);

    return s;
}
