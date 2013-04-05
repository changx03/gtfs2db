/* Specifies how the file "calendar.txt" within a GTFS bundle is to be
   parsed and loaded into the database.

   Copyright (c) 2012 Simon South. All rights reserved. */

#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include "gtfs_file.h"

const gtfs_file_spec_t calendar_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "service schedule", "service schedules" },

  /* The filename within the GTFS bundle to load from */
  "calendar.txt",

  /* Field definitions */
  10,
  (gtfs_field_spec_t *[10]) {
    &(gtfs_field_spec_t) {"service_id", TYPE_STRING,  255 },
    &(gtfs_field_spec_t) {"monday",     TYPE_BOOLEAN,   0 },
    &(gtfs_field_spec_t) {"tuesday",    TYPE_BOOLEAN,   0 },
    &(gtfs_field_spec_t) {"wednesday",  TYPE_BOOLEAN,   0 },
    &(gtfs_field_spec_t) {"thursday",   TYPE_BOOLEAN,   0 },
    &(gtfs_field_spec_t) {"friday",     TYPE_BOOLEAN,   0 },
    &(gtfs_field_spec_t) {"saturday",   TYPE_BOOLEAN,   0 },
    &(gtfs_field_spec_t) {"sunday",     TYPE_BOOLEAN,   0 },
    &(gtfs_field_spec_t) {"start_date", TYPE_DATE,      0 },
    &(gtfs_field_spec_t) {"end_date",   TYPE_DATE,      0 }
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
