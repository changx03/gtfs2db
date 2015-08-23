/* Specifies how the file "routes.txt" within a GTFS bundle is to be
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

#ifndef __ROUTES_H__
#define __ROUTES_H__

#include "gtfs_file.h"

const gtfs_file_spec_t routes_file_spec = {
  /* The name of this GTFS object, both singular and plural forms */
  { "route", "routes" },

  /* The filename within the GTFS bundle to load from */
  "routes.txt",

  /* Whether the file is required to be present in a GTFS bundle or
     not */
  true,

  /* Field definitions */
  9,
  (gtfs_field_spec_t *[9]) {
    &(gtfs_field_spec_t) {"route_id",         TYPE_STRING,   255, true },
    &(gtfs_field_spec_t) {"agency_id",        TYPE_STRING,   255, false },
    &(gtfs_field_spec_t) {"route_short_name", TYPE_STRING,   255, true },
    &(gtfs_field_spec_t) {"route_long_name",  TYPE_STRING,   255, true },
    &(gtfs_field_spec_t) {"route_desc",       TYPE_STRING,  1024, false },
    &(gtfs_field_spec_t) {"route_type",       TYPE_INTEGER,    0, true },
    &(gtfs_field_spec_t) {"route_url",        TYPE_STRING,   255, false },
    &(gtfs_field_spec_t) {"route_color",      TYPE_STRING,     6, false },
    &(gtfs_field_spec_t) {"route_text_color", TYPE_STRING,     6, false }
  },

  /* SQL statements */

  /* Create the corresponding table in the database */
  "CREATE TABLE routes("
    "id VARCHAR(255) PRIMARY KEY, "
    "agency_id VARCHAR(255) REFERENCES agencies(id), "
    "short_name VARCHAR(255) NOT NULL, "
    "long_name VARCHAR(255) NOT NULL, "
    "desc TEXT, "
    "route_type TINYINT NOT NULL, "
    "url VARCHAR(255), "
    "color CHAR(6), "
    "text_color CHAR(6));",

  /* Insert a new record into the database */
  "INSERT INTO routes(id, agency_id, short_name, long_name, desc, "
    "route_type, url, color, text_color)"
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);",

  /* Define indices on the table for quick lookups */
  (const char *[]) {
    NULL
  },
};

#endif
