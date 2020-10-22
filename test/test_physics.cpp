
// /* ========================================================================
//    $File: $
//    $Date: $
//    $Revision: $
//    $Creator: Thierry Raeber $
//    ======================================================================== */

// #include <string.h>

// #include "catch2/catch.hpp"
// #include "physics.h"

// TEST_CASE("Times to strings")
// {

//   char result[100] = "";

//   char *expected;

//   SECTION("Time = 0")
//     {
//       expected = "0 second";
//       TimeToStr(result, 0);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("Time = 1")
//     {
//       expected = "1 second";
//       TimeToStr(result, 1);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("Time = 59 seconds")
//     {
//       expected = "59 seconds";
//       TimeToStr(result, 59);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("One minute")
//     {
//       expected = "1 minute";
//       TimeToStr(result, 60);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("One minute, one second")
//     {
//       expected = "1 minute, 1 second";
//       TimeToStr(result, 61);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("One minute, 59 seconds")
//     {
//       expected = "1 minute, 59 seconds";
//       TimeToStr(result, 119);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("59 minutes, 59 seconds")
//     {
//       expected = "59 minutes, 59 seconds";
//       TimeToStr(result, 3599);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("1 hour")
//     {
//       expected = "1 hour";
//       TimeToStr(result, 3600);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("1 hour, 4 minutes")
//     {
//       expected = "1 hour, 4 minutes";
//       TimeToStr(result, 3840);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("1 hour, 4 minutes, 40 seconds")
//     {
//       expected = "1 hour, 4 minutes";
//       TimeToStr(result, 3880);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("23h59")
//     {
//       expected = "23 hours, 59 minutes";
//       TimeToStr(result, 3600 * 24 - 1);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("1 day")
//     {
//       expected = "1 day";
//       TimeToStr(result, 3600 * 24);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("almost 1 year")
//     {
//       expected = "364 days, 23 hours";
//       TimeToStr(result, 3600 * 24 * 365- 1);
//       REQUIRE(strcmp(result, expected) == 0);
//     }
//   SECTION("1 year")
//     {
//       expected = "1 year";
//       TimeToStr(result, 3600 * 24 * 366);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("years max")
//     {
//       expected = "20970000 years";
//       TimeToStr(result, time_std.galactic_year.value * 0.09);
//       REQUIRE(strcmp(result, expected) == 0);
//     }

//   SECTION("0.1 galactic year")
//     {
//       expected = "0.1 galactic year";
//       TimeToStr(result, time_std.galactic_year.value * 0.1);
//       REQUIRE(strcmp(result, expected) == 0);
//     }
//   SECTION("1 galactic year")
//     {
//       expected = "1 galactic year";
//       TimeToStr(result, time_std.galactic_year.value);
//       REQUIRE(strcmp(result, expected) == 0);
//     }
//   SECTION("1.5 g.y.")
//     {
//       expected = "1.5 galactic year";
//       TimeToStr(result, time_std.galactic_year.value* 1.5);
//       REQUIRE(strcmp(result, expected) == 0);
//     }
//   SECTION("1.9 g.y.")
//     {
//       expected = "1.9 galactic year";
//       TimeToStr(result, time_std.galactic_year.value * 1.9);
//       REQUIRE(strcmp(result, expected) == 0);
//     }
// }

// TEST_CASE("RemainingTime()")
// {

//   char time_str[100] = "";
//   char *expected;

//   SECTION("all == 0")
//     {
//       u_time t = RemainingTime(0, 0, 0);
//       TimeToStr(time_str, t);
//       expected = "0 second";
//       REQUIRE(strcmp(time_str, expected) == 0);
//     }

//   SECTION("unitary no acceleration")
//     {
//       u_time t = RemainingTime(1, 1, 0);
//       TimeToStr(time_str, t);
//       expected = "1 second";
//       REQUIRE(strcmp(time_str, expected) == 0);
//     }

//   SECTION("x100 no acceleration")
//     {
//       u_time t = RemainingTime(100, 100, 0);
//       TimeToStr(time_str, t);
//       expected = "1 second";
//       REQUIRE(strcmp(time_str, expected) == 0);
//     }

//   SECTION("infinity")
//     {
//       u_time t = RemainingTime(1, 0, 0);
//       TimeToStr(time_str, t);
//       expected = "infinity";
//       REQUIRE(strcmp(time_str, expected) == 0);
//       REQUIRE(t == INFINITY);
//     }
// }
