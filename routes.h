/* Specifies how the file "routes.txt" within a GTFS bundle is to be
   parsed and loaded into the database.

   Copyright (c) 2012 Simon South. All rights reserved. */

#ifndef __ROUTES_H__
#define __ROUTES_H__

#include "gtfs_file.h"

const gtfs_file_spec_t routes_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "route", "routes" },

  /* The filename within the GTFS bundle to load from */
  "routes.txt",

  /* Field definitions */
  9,
  (gtfs_field_spec_t *[9]) {
    &(gtfs_field_spec_t) {"route_id",         TYPE_STRING,   255 },
    &(gtfs_field_spec_t) {"agency_id",        TYPE_STRING,   255 },
    &(gtfs_field_spec_t) {"route_short_name", TYPE_STRING,   255 },
    &(gtfs_field_spec_t) {"route_long_name",  TYPE_STRING,   255 },
    &(gtfs_field_spec_t) {"route_desc",       TYPE_STRING,  1024 },
    &(gtfs_field_spec_t) {"route_type",       TYPE_INTEGER,    0 },
    &(gtfs_field_spec_t) {"route_url",        TYPE_STRING,   255 },
    &(gtfs_field_spec_t) {"route_color",      TYPE_STRING,     6 },
    &(gtfs_field_spec_t) {"route_text_color", TYPE_STRING,     6 }
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE routes("
    "route_id VARCHAR(255) PRIMARY KEY, "
    "agency_id VARCHAR(255) REFERENCES agencies, "
    "route_short_name VARCHAR(255) NOT NULL, "
    "route_long_name VARCHAR(255) NOT NULL, "
    "route_desc TEXT, "
    "route_type TINYINT NOT NULL, "
    "route_url VARCHAR(255), "
    "route_color CHAR(6), "
    "route_text_color CHAR(6));",

  /* Insert a new record into the database */
  "INSERT INTO routes(route_id, agency_id, route_short_name, "
    "route_long_name, route_desc, route_type, route_url, "
    "route_color, route_text_color) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
