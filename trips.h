/* Specifies how the file "trips.txt" within a GTFS bundle is to be
   parsed and loaded into the database.

   Copyright (c) 2012 Simon South <ssouth@simonsouth.com>.

   This file is part of gtfs2db.

   gtfs2db is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   gtfs2db is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with gtfs2db.  If not, see <http://www.gnu.org/licenses/>. */

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
  10,
  (gtfs_field_spec_t *[10]) {
    &(gtfs_field_spec_t) {"trip_id",               TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"route_id",              TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"service_id",            TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"trip_headsign",         TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"trip_short_name",       TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"direction_id",          TYPE_INTEGER,   0, false },
    &(gtfs_field_spec_t) {"block_id",              TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"shape_id",              TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"wheelchair_accessible", TYPE_INTEGER,   0, false },
    &(gtfs_field_spec_t) {"bikes_allowed",         TYPE_INTEGER,   0, false }
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
    "wheelchair_accessible TINYINT, "
    "bikes_allowed TINYINT);",

  /* Insert a new record into the database */
  "INSERT INTO trips(id, route_id, service_id, headsign, "
    "short_name, direction_id, block_id, shape_id, "
    "wheelchair_accessible, bikes_allowed) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
