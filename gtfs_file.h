/* Data definitions related to a GTFS file---that is, a file contained
   within a GTFS bundle (ZIP file)---and how it is to be parsed and
   loaded into the database.

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

#ifndef __GTFS_FILE_H__
#define __GTFS_FILE_H__

#include <sqlite3.h>
#include <stdbool.h>
#include <stddef.h>

/* Represents a name of a GTFS object, in singular and plural forms */
typedef struct {
  const char *singular;
  const char *plural;
} gtfs_object_name_t;

/* Represents a field type in a GTFS file and the database---this
   controls how the field value is parsed and how it is bound to the
   SQLite3 INSERT statement */
typedef enum {
  TYPE_BOOLEAN,
  TYPE_INTEGER,
  TYPE_DOUBLE,
  TYPE_STRING,
  TYPE_DATE,    /* in "YYYYMMDD" format */
  TYPE_TIME     /* in "H:MM:SS" or "HH:MM:SS" format */
} gtfs_field_type_t;

/* Represents the value of a field parsed from a GTFS file */
typedef union {
  bool boolean_value;
  int integer_value;
  double double_value;
  char *string_value;
  struct tm date_value;
  int time_value;
} gtfs_field_value_t;

/* Defines a field present in a GTFS file and stored in the
   database */
typedef struct {
  const char *name;
  gtfs_field_type_t type;
  unsigned int length;
  bool required;
} gtfs_field_spec_t;

/* Specifies a GTFS file (contained within a GTFS bundle) and how it
   is to be parsed */
typedef struct {
  /* The name of the GTFS object */
  gtfs_object_name_t name;

  /* The name of the file containing these objects */
  const char *filename;

  /* A flag indicating whether the file is required to be present in a
     GTFS bundle or not */
  bool required;

  /* The number of fields contained in each record (object) and an
     array containing a specifier for each */
  unsigned int num_fields;
  gtfs_field_spec_t **field_specs;

  /* SQL create-table and insert-object statements, plus an array of
     statements used to create the needed indices on the table */
  const char *create_table_stmt_str;
  const char *insert_stmt_str;
  const char **create_index_stmt_strs;
} gtfs_file_spec_t;

#endif
