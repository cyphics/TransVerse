//
// Created by cyphics on 25/10/2020.
//

#include <physics.h>
#include <stdio.h>
#include "test_physics.h"

START_TEST(time_to_string) {
    char result[100] = "";
    TimeToStr(result, 0);
    ck_assert_str_eq(result, "0 second");

    result[0] = '\0';
    TimeToStr(result, 1);
    ck_assert_str_eq(result, "1 second");

    result[0] = '\0';
    TimeToStr(result, 59);
    ck_assert_str_eq(result, "59 seconds");

    result[0] = '\0';
    TimeToStr(result, 60);
    ck_assert_str_eq(result, "1 minute");

    result[0] = '\0';
    TimeToStr(result, 61);
    ck_assert_str_eq(result, "1 minute, 1 second");

    result[0] = '\0';
    TimeToStr(result, 119);
    ck_assert_str_eq(result, "1 minute, 59 seconds");

    result[0] = '\0';
    TimeToStr(result, 60 * 60 - 1);
    ck_assert_str_eq(result, "59 minutes, 59 seconds");

    result[0] = '\0';
    TimeToStr(result, 60 * 60);
    ck_assert_str_eq(result, "1 hour");

    result[0] = '\0';
    TimeToStr(result, 64 * 60);
    ck_assert_str_eq(result, "1 hour, 4 minutes");

    result[0] = '\0';
    TimeToStr(result, 64 * 60 + 1);
    ck_assert_str_eq(result, "1 hour, 4 minutes"); // should print max 2 last levels

    result[0] = '\0';
    TimeToStr(result, 24 * 60 * 60);
    ck_assert_str_eq(result, "1 day");

    result[0] = '\0';
    TimeToStr(result, 24 * 60 * 60 * 366);
    ck_assert_str_eq(result, "1 year");

    result[0] = '\0';
    TimeToStr(result, 24 * 60 * 60 * 366);
    ck_assert_str_eq(result, "1 year");

    result[0] = '\0';
    TimeToStr(result, time_std.id.galactic_year.value * 0.099);
    ck_assert_str_eq(result, "23067000 years");

}

END_TEST

START_TEST(print_list_std) {
    int num_items = sizeof(time_std.list) / sizeof(time_std.list[0]);
    char output[10000];
    output[0] = '\0';
    for (int i = 0; i < num_items; ++i) {
        strcat(output, time_std.list[i].name);
        strcat(output, ";");
    }
    ck_assert_str_eq("second;minute;hour;day;year;galactic year;", output);

    num_items = sizeof(distance_std.list) / sizeof(distance_std.list[0]);
    output[0] = '\0';
    for (int i = 0; i < num_items; ++i) {
        strcat(output, distance_std.list[i].name);
        strcat(output, ";");
    }
    ck_assert_str_eq("yomtometer;zeptometer;attometer;femtometer;picometer;nanometer;micrometer;millimeter;meter;kilometer;astro;parsec;lightyear;", output);

    num_items = sizeof(dist_mstone.list) / sizeof(dist_mstone.list[0]);
    output[0] = '\0';
    printf("%d\n", num_items);
    for (int i = 0; i < num_items; ++i) {
        strcat(output, dist_mstone.list[i].name);
        strcat(output, ";");
    }
    ck_assert_str_eq("proton radius;gold nucleus radius;radius_hydrogen;radius_carbon;angstrom;diameter_adn_helix;red_lightwave;diameter_hair;human_height;earth_diameter;sun_diameter;earth_to_sun;proxima_centauri;milky_way_diameter;universe_diameter;", output);
}

END_TEST

Suite *physics_test_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Physics");

    /* Core test case */
    tc_core = tcase_create("Test physics computations");

//    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, time_to_string);
    tcase_add_test(tc_core, print_list_std);
    suite_add_tcase(s, tc_core);

    return s;
}