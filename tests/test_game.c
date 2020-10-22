
#include <stdlib.h>
#include <stdint.h>
#include <check.h>

static void setup(void)
{

}

static void teardown(void)
{

}


START_TEST (test_name)
{
    ck_assert_int_eq(5, 5);
}
END_TEST



Suite * game_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Game");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_name);
    suite_add_tcase(s, tc_core);


    return s;
}



