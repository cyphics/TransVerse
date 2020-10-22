//
// Created by cyphics on 7/19/20.
//
// Reference: https://libcheck.github.io/check/doc/check_html/index.html

#include <check.h>
#include <stdlib.h>
#include "test_game.h"
#include "test_storage.h"

int main(void)
{
    int number_failed;
    SRunner *sr;

    Suite *s = game_suite();
    sr = srunner_create(s);
    srunner_add_suite(sr, storage_test_suite());
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
