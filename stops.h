/* Specifies how the file "stops.txt" within a GTFS bundle is to be
   parsed and loaded into the database.

   Copyright (c) 2012 Simon South. All rights reserved. */

#ifndef __STOPS_H__
#define __STOPS_H__

#include "gtfs_file.h"

const gtfs_file_spec_t stops_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "stop", "stops" },

  /* The filename within the GTFS bundle to load from */
  "stops.txt",

  /* Field definitions */
  12,
  (gtfs_field_spec_t *[12]) {
    &(gtfs_field_spec_t) {"stop_id",             TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"stop_code",           TYPE_STRING,   16 },
    &(gtfs_field_spec_t) {"stop_name",           TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"stop_desc",           TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"stop_lat",            TYPE_DOUBLE,    0 },
    &(gtfs_field_spec_t) {"stop_lon",            TYPE_DOUBLE,    0 },
    &(gtfs_field_spec_t) {"zone_id",             TYPE_STRING,   16 },
    &(gtfs_field_spec_t) {"stop_url",            TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"location_type",       TYPE_INTEGER,   0 },
    &(gtfs_field_spec_t) {"parent_station",      TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"stop_timezone",       TYPE_STRING,   64 },
    &(gtfs_field_spec_t) {"wheelchair_boarding", TYPE_INTEGER,   0 },
    
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE stops("
    "stop_id VARCHAR(255) PRIMARY KEY, "
    "stop_code VARCHAR(16), "
    "stop_name VARCHAR(255) NOT NULL, "
    "stop_desc VARCHAR(255), "
    "stop_lat DECIMAL(8,6) NOT NULL, "
    "stop_lon DECIMAL(9,6) NOT NULL, "
    "zone_id VARCHAR(16), "
    "stop_url VARCHAR(255), "
    "location_type TINYINT, "
    "parent_station VARCHAR(255) REFERENCES stops(stop_id), "
    "stop_timezone VARCHAR(64), "
    "wheelchair_boarding TINYINT);",

  /* Insert a new record into the database */
  "INSERT INTO stops(stop_id, stop_code, stop_name, stop_desc, "
    "stop_lat, stop_lon, zone_id, stop_url, location_type, "
    "parent_station, stop_timezone, wheelchair_boarding) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
