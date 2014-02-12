/* Data definitions related to a GTFS file---that is, a file contained
   within a GTFS bundle (ZIP file)---and how it is to be parsed and
   loaded into the database.

   Copyright (c) 2012 Simon South. All rights reserved. */

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
