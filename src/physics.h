#if !defined(PHYSICS_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */

#define PHYSICS_H

#include "types.h"

#define TIME_SEGMENT_SIZE 100

typedef struct physics_unit
{
  char *name;
  u_phys value;
} physics_unit;

typedef struct distance_standard
{
  physics_unit yomtometer;
  physics_unit zeptometer;
  physics_unit attometer;
  physics_unit femtometer;
  physics_unit picometer;
  physics_unit nanometer;
  physics_unit micrometer;
  physics_unit millimeter;
  physics_unit meter;
  physics_unit kilometer;
  physics_unit astro;
  physics_unit parsec;
  physics_unit lightyear;
} distance_standard;

static const distance_standard dist_std =
  {
   {"yomtometer",1},
   {"zeptometer", 1000},
   {"attometer", 1e6},
   {"femtometer", 1e9},
   {"picometer", 1e12},
   {"nanometer", 1e15},
   {"micrometer", 1e18},
   {"millimeter", 1e21},
   {"meter", 1e24},
   {"kilometer", 1e27},
   {"astro", 1.495978707e+35},
   {"parsec", 3.0857e+40},
   {"lightyear", 9.4607304725808e+39},
  };

typedef struct distance_milestones
{
  physics_unit radius_proton;
  physics_unit radius_gold_nucleus;
  physics_unit radius_hydrogen;
  physics_unit radius_carbon;
  physics_unit angstrom;
  physics_unit diameter_adn_helix;
  physics_unit red_lightwave;
  physics_unit diameter_hair;
  physics_unit human_height;
  physics_unit earth_diameter;
  physics_unit sun_diameter;
  physics_unit earth_to_sun;
  physics_unit proxima_centauri;
  physics_unit milky_way_diameter;
  physics_unit universe_diamete;
} distance_milestones;

static const distance_milestones dist_mstone =
  {
   {"proton radius", 8.33e+8},
   {"gold nucleus radius", 8.45e+9},
   {"radius_hydrogen", 2.5e+13},
   {"radius_carbon", 6e+13},
   {"angstrom", 1e+14},
   {"diameter_adn_helix", 2e+15},
   {"red_lightwave", 7e+17},
   {"diameter_hair", 7e+19},
   {"human_height", 1.75e+24},
   {"earth_diameter", 1.2742e+31},
   {"sun_diameter", 1.391e+33},
   {"earth_to_sun", 1.495978707e+35},
   {"proxima_centauri", 4.014e+40},
   {"milky_way_diameter", 9.999992109517905e+44},
   {"universe_diameter", 8.798479339500144e+50},
  };



typedef struct time_standards
{
  physics_unit second;
  physics_unit minute;
  physics_unit hour;
  physics_unit day;
  physics_unit year;
  physics_unit galactic_year;
} time_standards;

static const time_standards time_std =
  {
   {"second", 1},
   {"minute", 60},
   {"hour", 3600},
   {"day", 86400},
   {"year", 31540000},
   {"galactic year", 7348820000000000}
  };


void TimeToStr(char *origin, u_time t);


u_time
RemainingTime(u_distance distance_to_travel,
              u_speed current_speed,
              u_acceleration current_acceleration);

u_distance
TraveledDistance(u_speed initial_speed,
                 u_time duration,
                 u_acceleration acceleration);

#endif
