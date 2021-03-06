/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "types.h"
#include "physics.h"

void CreateTimeSegment(char *str_ptr, int amount, char *unit) {
    if (amount > 0) {
        sprintf(str_ptr, "%d ", amount);
        strcat(str_ptr, unit);
        if (amount > 1) strcat(str_ptr, "s");
    } else str_ptr[0] = '\0';
}

void TimeToStr(char *str_ptr, u_time t) {
    /*
      Build the str_ptral string. The return statements ensure we only give 2
      units of time maximum. Printing "2 days, 3 hours" is enough. Printing
      "2 days, 3 hours, 25 minutes" is too much.
    */
    if (t == INFINITY) {
        strcat(str_ptr, "infinity");
        return;
    }

    u_time total_double = t;

    u_time galac_year = total_double / time_std.id.galactic_year.value;

    if (galac_year >= 0.1) {
        sprintf(str_ptr, "%.2g ", galac_year);
        strcat(str_ptr, time_std.id.galactic_year.name);
        return;
    }

    long remaining_time = total_double;
    int seconds;
    int minutes;
    int hours;
    int days;
    int years;

    years = (remaining_time / time_std.id.year.value);
    remaining_time %= (long) time_std.id.year.value;   // TODO check dangerous (long) cast
    days = floor(remaining_time / time_std.id.day.value);
    remaining_time %= (long) time_std.id.day.value;
    hours = floor(remaining_time / time_std.id.hour.value);
    remaining_time %= (long) time_std.id.hour.value;
    minutes = floor(remaining_time / time_std.id.minute.value);
    remaining_time %= (long) time_std.id.minute.value;
    seconds = remaining_time;

    char years_str[TIME_SEGMENT_SIZE];
    char days_str[TIME_SEGMENT_SIZE];
    char hours_str[TIME_SEGMENT_SIZE];
    char minutes_str[TIME_SEGMENT_SIZE];
    char seconds_str[TIME_SEGMENT_SIZE];

    CreateTimeSegment(years_str, years, "year");
    CreateTimeSegment(days_str, days, "day");
    CreateTimeSegment(hours_str, hours, "hour");
    CreateTimeSegment(minutes_str, minutes, "minute");
    CreateTimeSegment(seconds_str, seconds, "second");

    // printf("years segment: %s\n", years_str);
    // printf("days segment: %s\n", days_str);
    // printf("hours segment: %s\n", hours_str);
    // printf("minmutes segment: %s\n", minutes_str);
    // printf("seconds segment: %s\n", seconds_str);


    if ((str_ptr[0] != '\0') && years > 0) strcat(str_ptr, ", ");
    strcat(str_ptr, years_str);
    if (years > 10) return;
    if ((str_ptr[0] != '\0') && days > 0) strcat(str_ptr, ", ");
    strcat(str_ptr, days_str);
    if (years > 0) return;
    if ((str_ptr[0] != '\0') && hours > 0) strcat(str_ptr, ", ");
    strcat(str_ptr, hours_str);
    if (days > 0) return;
    if ((str_ptr[0] != '\0') && minutes > 0) strcat(str_ptr, ", ");
    strcat(str_ptr, minutes_str);
    if (hours > 0) return;
    if ((str_ptr[0] != '\0') && seconds > 0) strcat(str_ptr, ", ");
    strcat(str_ptr, seconds_str);

    if (str_ptr[0] == '\0') strcat(str_ptr, "0 second");
}


u_time
RemainingTime(u_distance distance_to_travel,
              u_speed speed,
              u_acceleration acceleration) {
    /**
       Return Time() object === time it takes to travel distance with given speed and acceleration
       d=v0t+(at^2/2)d=v0t+(at^2/2)
       =>
       t = ( -2v0 +- sqrt( pow(2v0) + 4a * 2d ) ) / 2a
    */

    if (distance_to_travel <= 0) return 0;
    if (speed <= 0) return INFINITY;
    if (acceleration < 0) return INFINITY;
    if (acceleration == 0) {
        if (speed == 0) return INFINITY;
        return distance_to_travel / speed;
    }

    double delta = sqrt(pow(2 * speed, 2) + 8 * acceleration * distance_to_travel);
    double numerator_one = -2 * speed + delta;
    double numerator_two = -2 * speed - delta;
    double denominator = 2 * acceleration;

    double answer_one = numerator_one / denominator;
    double answer_two = numerator_two / denominator;

    if (answer_one > answer_two) {
        return answer_one;
    }
    return answer_two;
}

u_distance
TraveledDistance(u_speed initial_speed,
                 u_time duration,
                 u_acceleration acceleration) {

    /**
     *  Return the distance traveled at current speed, during given duration
     *   dist = init_speed * duration + 1/2 acceleration * duration^2
     */
    return initial_speed * duration + acceleration * 0.5 * duration * duration;

}
