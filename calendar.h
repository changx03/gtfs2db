/* Specifies how the file "calendar.txt" within a GTFS bundle is to be
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

#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include "gtfs_file.h"

const gtfs_file_spec_t calendar_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "service schedule", "service schedules" },

  /* The filename within the GTFS bundle to load from */
  "calendar.txt",

  /* Whether the file is required to be present in a GTFS bundle or
     not */
  true,

  /* Field definitions */
  10,
  (gtfs_field_spec_t *[10]) {
    &(gtfs_field_spec_t) {"service_id", TYPE_STRING,  255, true },
    &(gtfs_field_spec_t) {"monday",     TYPE_BOOLEAN,   0, true },
    &(gtfs_field_spec_t) {"tuesday",    TYPE_BOOLEAN,   0, true },
    &(gtfs_field_spec_t) {"wednesday",  TYPE_BOOLEAN,   0, true },
    &(gtfs_field_spec_t) {"thursday",   TYPE_BOOLEAN,   0, true },
    &(gtfs_field_spec_t) {"friday",     TYPE_BOOLEAN,   0, true },
    &(gtfs_field_spec_t) {"saturday",   TYPE_BOOLEAN,   0, true },
    &(gtfs_field_spec_t) {"sunday",     TYPE_BOOLEAN,   0, true },
    &(gtfs_field_spec_t) {"start_date", TYPE_DATE,      0, true },
    &(gtfs_field_spec_t) {"end_date",   TYPE_DATE,      0, true }
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE calendars("
    "service_id VARCHAR(255) PRIMARY KEY, "
    "monday BOOLEAN NOT NULL, "
    "tuesday BOOLEAN NOT NULL, "
    "wednesday BOOLEAN NOT NULL, "
    "thursday BOOLEAN NOT NULL, "
    "friday BOOLEAN NOT NULL, "
    "saturday BOOLEAN NOT NULL, "
    "sunday BOOLEAN NOT NULL, "
    "start_date DATE NOT NULL, "
    "end_date DATE NOT NULL);",

  /* Insert a new record into the database */
  "INSERT INTO calendars(service_id, monday, tuesday, wednesday, "
    "thursday, friday, saturday, sunday, start_date, end_date) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
