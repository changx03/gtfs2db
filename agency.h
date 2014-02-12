/* Specifies how the file "agency.txt" within a GTFS bundle is to be
   parsed and loaded into the database.

   Copyright (c) 2012 Simon South. All rights reserved. */

#ifndef __AGENCY_H__
#define __AGENCY_H__

#include "gtfs_file.h"

const gtfs_file_spec_t agency_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "agency", "agencies" },

  /* The filename within the GTFS bundle to load from */
  "agency.txt",

  /* Field definitions */
  7,
  (gtfs_field_spec_t *[7]) {
    &(gtfs_field_spec_t) {"agency_id",       TYPE_STRING, 255, false },
    &(gtfs_field_spec_t) {"agency_name",     TYPE_STRING, 255, true },
    &(gtfs_field_spec_t) {"agency_url",      TYPE_STRING, 255, true },
    &(gtfs_field_spec_t) {"agency_timezone", TYPE_STRING,  64, true },
    &(gtfs_field_spec_t) {"agency_lang",     TYPE_STRING,   2, false },
    &(gtfs_field_spec_t) {"agency_phone",    TYPE_STRING, 255, false },
    &(gtfs_field_spec_t) {"agency_fare_url", TYPE_STRING, 255, false }
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE agencies("
    "id VARCHAR(255), "
    "name VARCHAR(255) NOT NULL, "
    "url VARCHAR(255) NOT NULL, "
    "timezone VARCHAR(64) NOT NULL, "
    "lang CHAR(2), "
    "phone VARCHAR(32), "
    "fare_url VARCHAR(255));",

  /* Insert a new record into the database */
  "INSERT INTO agencies(id, name, url, timezone, lang, phone, "
    "fare_url) VALUES (?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
