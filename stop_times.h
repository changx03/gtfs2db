/* Specifies how the file "stop_times.txt" within a GTFS bundle is to
   be parsed and loaded into the database.

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

#ifndef __STOP_TIMES_H__
#define __STOP_TIMES_H__

#include "gtfs_file.h"

const gtfs_file_spec_t stop_times_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "stop time", "stop times" },

  /* The filename within the GTFS bundle to load from */
  "stop_times.txt",

  /* Whether the file is required to be present in a GTFS bundle or
     not */
  true,

  /* Field definitions */
  9,
  (gtfs_field_spec_t *[9]) {
    &(gtfs_field_spec_t) {"trip_id",             TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"arrival_time",        TYPE_TIME,      8, true },
    &(gtfs_field_spec_t) {"departure_time",      TYPE_TIME,      8, true },
    &(gtfs_field_spec_t) {"stop_id",             TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"stop_sequence",       TYPE_INTEGER,   0, true },
    &(gtfs_field_spec_t) {"stop_headsign",       TYPE_STRING,  255, false },
    &(gtfs_field_spec_t) {"pickup_type",         TYPE_INTEGER,   0, false },
    &(gtfs_field_spec_t) {"drop_off_type",       TYPE_INTEGER,   0, false },
    &(gtfs_field_spec_t) {"shape_dist_traveled", TYPE_DOUBLE,    0, false }
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
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    /* Allow fast look-ups by stop ID */
    "CREATE INDEX stop_times_stop_id_index "
      "ON stop_times(stop_id);",
    NULL
  },
};

#endif
