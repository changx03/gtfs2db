/* Specifies how the file "stops.txt" within a GTFS bundle is to be
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

#ifndef __STOPS_H__
#define __STOPS_H__

#include "gtfs_file.h"

const gtfs_file_spec_t stops_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "stop", "stops" },

  /* The filename within the GTFS bundle to load from */
  "stops.txt",

  /* Whether the file is required to be present in a GTFS bundle or
     not */
  true,

  /* Field definitions */
  12,
  (gtfs_field_spec_t *[12]) {
    &(gtfs_field_spec_t) {"stop_id",             TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"stop_code",           TYPE_STRING,   16, false },
    &(gtfs_field_spec_t) {"stop_name",           TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"stop_desc",           TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"stop_lat",            TYPE_DOUBLE,    0, true },
    &(gtfs_field_spec_t) {"stop_lon",            TYPE_DOUBLE,    0, true },
    &(gtfs_field_spec_t) {"zone_id",             TYPE_STRING,   16, false },
    &(gtfs_field_spec_t) {"stop_url",            TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"location_type",       TYPE_INTEGER,   0, false },
    &(gtfs_field_spec_t) {"parent_station",      TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"stop_timezone",       TYPE_STRING,   64, false },
    &(gtfs_field_spec_t) {"wheelchair_boarding", TYPE_INTEGER,   0, false },
    
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE stops("
    "id VARCHAR(255) PRIMARY KEY, "
    "code VARCHAR(16), "
    "name VARCHAR(255) NOT NULL, "
    "desc VARCHAR(255), "
    "lat DECIMAL(8,6) NOT NULL, "
    "lon DECIMAL(9,6) NOT NULL, "
    "zone_id VARCHAR(16), "
    "url VARCHAR(255), "
    "location_type TINYINT, "
    "parent_station VARCHAR(255) REFERENCES stops(id), "
    "timezone VARCHAR(64), "
    "wheelchair_boarding TINYINT);",

  /* Insert a new record into the database */
  "INSERT INTO stops(id, code, name, desc, lat, lon, zone_id, url, "
    "location_type, parent_station, timezone, wheelchair_boarding) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    /* Allow fast lookups by stop-code */
    "CREATE UNIQUE INDEX stops_code_id_index "
      "ON stops(code, id)",
    NULL
  },
};

#endif
