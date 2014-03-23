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

  /* Whether the file is required to be present in a GTFS bundle or
     not */
  true,

  /* Field definitions */
  9,
  (gtfs_field_spec_t *[9]) {
    &(gtfs_field_spec_t) {"trip_id",               TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"route_id",              TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"service_id",            TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"trip_headsign",         TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"trip_short_name",       TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"direction_id",          TYPE_INTEGER,   0, false },
    &(gtfs_field_spec_t) {"block_id",              TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"shape_id",              TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"wheelchair_accessible", TYPE_INTEGER,   0, false }
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE trips("
    "id VARCHAR(255) PRIMARY KEY, "
    "route_id VARCHAR(255) NOT NULL REFERENCES routes(id), "
    "service_id VARCHAR(255) NOT NULL, "
    "headsign VARCHAR(255), "
    "short_name VARCHAR(255), "
    "direction_id TINYINT, "
    "block_id VARCHAR(255), "
    "shape_id VARCHAR(255), "
    "wheelchair_accessible TINYINT);",

  /* Insert a new record into the database */
  "INSERT INTO trips(id, route_id, service_id, headsign, "
    "short_name, direction_id, block_id, shape_id, "
    "wheelchair_accessible) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
