/* Specifies how the file "calendar_dates.txt" within a GTFS bundle is
   to be parsed and loaded into the database.

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

#ifndef __CALENDAR_DATES_H__
#define __CALENDAR_DATES_H__

#include "gtfs_file.h"

const gtfs_file_spec_t calendar_dates_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "service exception", "service exceptions" },

  /* The filename within the GTFS bundle to load from */
  "calendar_dates.txt",

  /* Whether the file is required to be present in a GTFS bundle or
     not */
  false,

  /* Field definitions */
  3,
  (gtfs_field_spec_t *[3]) {
    &(gtfs_field_spec_t) {"service_id",     TYPE_STRING, 255, true },
    &(gtfs_field_spec_t) {"date",           TYPE_DATE,     0, true },
    &(gtfs_field_spec_t) {"exception_type", TYPE_INTEGER,  0, true }
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE calendar_dates("
    "service_id VARCHAR(255), "
    "date DATE, "
    "exception_type TINYINT NOT NULL, "
    "PRIMARY KEY (service_id, date));",

  /* Insert a new record into the database */
  "INSERT INTO calendar_dates(service_id, date, exception_type) "
    "VALUES (?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
