/* Specifies how the file "stop_times.txt" within a GTFS bundle is to
   be parsed and loaded into the database.

   Copyright (c) 2012 Simon South. All rights reserved. */

#ifndef __STOP_TIMES_H__
#define __STOP_TIMES_H__

#include "gtfs_file.h"

const gtfs_file_spec_t stop_times_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "stop time", "stop times" },

  /* The filename within the GTFS bundle to load from */
  "stop_times.txt",

  /* Field definitions */
  9,
  (gtfs_field_spec_t *[9]) {
    &(gtfs_field_spec_t) {"trip_id",             TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"arrival_time",        TYPE_TIME,      8 },
    &(gtfs_field_spec_t) {"departure_time",      TYPE_TIME,      8 },
    &(gtfs_field_spec_t) {"stop_id",             TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"stop_sequence",       TYPE_INTEGER,   0 },
    &(gtfs_field_spec_t) {"stop_headsign",       TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"pickup_type",         TYPE_INTEGER,   0 },
    &(gtfs_field_spec_t) {"drop_off_type",       TYPE_INTEGER,   0 },
    &(gtfs_field_spec_t) {"shape_dist_traveled", TYPE_DOUBLE,    0 }
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE stop_times("
    "trip_id VARCHAR(255) NOT NULL REFERENCES trips(id), "
    "arrival_time INTEGER, "
    "departure_time INTEGER, "
    "stop_id VARCHAR(255) NOT NULL REFERENCES stops(id), "
    "stop_sequence INTEGER NOT NULL, "
    "stop_headsign VARCHAR(255), "
    "pickup_type TINYINT, "
    "drop_off_type TINYINT, "
    "shape_dist_traveled DECIMAL);",

  /* Insert a new record into the database */
  "INSERT INTO stop_times(trip_id, arrival_time, departure_time, "
    "stop_id, stop_sequence, stop_headsign, pickup_type, "
    "drop_off_type, shape_dist_traveled) "
    " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
