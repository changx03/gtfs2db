/* Specifies how the file "trips.txt" within a GTFS bundle is to be
   parsed and loaded into the database.

   Copyright (c) 2012 Simon South. All rights reserved. */

#ifndef __TRIPS_H__
#define __TRIPS_H__

#include "gtfs_file.h"

const gtfs_file_spec_t trips_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "trip", "trips" },

  /* The filename within the GTFS bundle to load from */
  "trips.txt",

  /* Field definitions */
  9,
  (gtfs_field_spec_t *[9]) {
    &(gtfs_field_spec_t) {"trip_id",               TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"route_id",              TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"service_id",            TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"trip_headsign",         TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"trip_short_name",       TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"direction_id",          TYPE_INTEGER,   0 },
    &(gtfs_field_spec_t) {"block_id",              TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"shape_id",              TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"wheelchair_accessible", TYPE_INTEGER,   0 }
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE trips("
    "trip_id VARCHAR(255) PRIMARY KEY, "
    "route_id VARCHAR(255) NOT NULL REFERENCES routes(route_id), "
    "service_id VARCHAR(255) NOT NULL, "
    "trip_headsign VARCHAR(255), "
    "trip_short_name VARCHAR(255), "
    "direction_id TINYINT, "
    "block_id VARCHAR(255), "
    "shape_id VARCHAR(255), "
    "wheelchair_accessible TINYINT);",

  /* Insert a new record into the database */
  "INSERT INTO trips(trip_id, route_id, service_id, trip_headsign, "
    "trip_short_name, direction_id, block_id, shape_id, "
    "wheelchair_accessible) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
