/* Converts a GTFS bundle (in ZIP-file format) to a SQLite 3 database.

   Requires glib2, libcsv (http://libcsv.sourceforge.net/) and libzip
   (http://www.nih.at/libzip/).

   Copyright (c) 2012 Simon South. All rights reserved. */

#define _XOPEN_SOURCE  /* include the definition of "strptime" */

#include <assert.h>
#include <errno.h>
#include <csv.h>
#include <glib.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zip.h>

#include "gtfs_file.h"
#include "agency.h"
#include "calendar.h"
#include "calendar_dates.h"
#include "routes.h"
#include "stops.h"
#include "trips.h"
#include "stop_times.h"

/* The maximum number of records (i.e., INSERT statements) to include
   in a single database transaction */
#define RECORDS_PER_TRANSACTION 2048

/* The size, in bytes, of the buffer used to read CSV data from the
   GTFS ZIP file */
#define BUFFER_SIZE 20 * 1024

/* The maximum number of columns (fields) contained in any GTFS-bundle
   member file */
#define MAX_COLUMNS 16

/* ---------------------------------------------------------------- */

/* A structure that represents the current state of parsing a file
   within a GTFS bundle */
typedef struct {
  /* Handles to the SQLite 3 database and GTFS ZIP files,
     respectively */
  sqlite3 *db;
  const gtfs_file_spec_t *gtfs_file_spec;

  /* A mapping between column numbers in the file and field numbers in
     the GTFS-file spec---this accounts for the fact the order of
     fields in each record may vary between GTFS bundles */
  unsigned int field_for_column[MAX_COLUMNS];

  /* TRUE if the header (i.e., first) row has already been parsed;
     FALSE otherwise */
  bool header_parsed;

  /* The number of fields parsed for the current record */
  unsigned int fields_parsed;

  /* The number of records parsed for the current file */
  unsigned long records_parsed;

  /* The number of objects loaded into the database from the current
     file */
  unsigned long objects_loaded;

  /* The pre-compiled INSERT statement used to insert rows from the
     GTFS file currently being parsed */
  sqlite3_stmt *insert_stmt;
} gtfs_parsing_state_t;

/* ---------------------------------------------------------------- */

/* Precompiled "BEGIN TRANSACTION" and "END TRANSACTION" statements,
   used to group inserted records into batches before being written
   out to disk */
sqlite3_stmt *begin_transaction_stmt, *end_transaction_stmt;

/* The set of GTFS-file specifiers; together these specify how the
   bundle as a whole should be processed */
const gtfs_file_spec_t *gtfs_file_specs[] = {
  &agency_file_spec,
  &calendar_file_spec,
  &calendar_dates_file_spec,
  &routes_file_spec,
  &stops_file_spec,
  &trips_file_spec,
  &stop_times_file_spec,
  NULL
};

/* ---------------------------------------------------------------- */

/* Creates a table to hold data from the GTFS file currently being
   processed */
inline static int create_table(sqlite3 *db,
                               const gtfs_file_spec_t *gtfs_file_spec,
                               char **errmsg) {
  return sqlite3_exec(db,
                      gtfs_file_spec->create_table_stmt_str,
                      NULL,
                      NULL,
                      errmsg);
}

/* Inserts one object into the database from the GTFS file currently
   being parsed */
inline static int
prepare_insert_stmt(sqlite3 *db,
                    const gtfs_file_spec_t *gtfs_file_spec,
                    sqlite3_stmt **insert_stmt) {
  return sqlite3_prepare_v2(db,
                            gtfs_file_spec->insert_stmt_str,
                            strlen(gtfs_file_spec->insert_stmt_str),
                            insert_stmt,
                            NULL);
}

/* Invoked by the CSV parser each time a field has been parsed */
static void field_parsed(void *val, size_t len, void *data) {
  static const char *date_format = "%Y%m%d";

  struct tm parsed_date;
  char iso8601_date_str[24];
  unsigned int iso8601_date_str_len;
  char *hours, *minutes, *seconds;

  gtfs_parsing_state_t *parsing_state = (gtfs_parsing_state_t *)data;
  const gtfs_file_spec_t *gtfs_file_spec =
    parsing_state->gtfs_file_spec;

  int field_number;
  bool field_name_matched;
  gtfs_field_spec_t *field_spec;
  int sqlite_result;
  int max_len;

  if(parsing_state->header_parsed) {
    /* We've parsed the header already; this field contains real data */

    field_number =
      parsing_state->field_for_column[parsing_state->fields_parsed];
    field_spec =
      parsing_state->gtfs_file_spec->field_specs[field_number];

    /* Bind this value to our INSERT statement */
    switch(field_spec->type) {
    case TYPE_BOOLEAN:
    case TYPE_INTEGER:
      sqlite_result = sqlite3_bind_int(parsing_state->insert_stmt,
                                       field_number + 1,
                                       atoi((const char *)val));
      break;

    case TYPE_DOUBLE:
      sqlite_result = sqlite3_bind_double(parsing_state->insert_stmt,
                                          field_number + 1,
                                          atof((const char *)val));
      break;

    case TYPE_STRING:
      assert(len <= field_spec->length);
      if(len == 0) {
        /* We insert NULL values instead of empty strings */
        sqlite_result = sqlite3_bind_null(parsing_state->insert_stmt,
                                          field_number + 1);
      }
      else {
        max_len = len > field_spec->length? field_spec->length: len;
        sqlite_result = sqlite3_bind_text(parsing_state->insert_stmt,
                                          field_number + 1,
                                          val,
                                          max_len,
                                          SQLITE_TRANSIENT);
      }
      break;

    case TYPE_DATE:
      assert(strptime(val, date_format, &parsed_date));
      assert(iso8601_date_str_len = strftime(iso8601_date_str,
                                             sizeof(iso8601_date_str),
                                             "%F",
                                             &parsed_date));
      sqlite_result = sqlite3_bind_text(parsing_state->insert_stmt,
                                        field_number + 1,
                                        iso8601_date_str,
                                        iso8601_date_str_len,
                                        SQLITE_TRANSIENT);
      break;

    case TYPE_TIME:
      if(len == 0) {
        /* We insert NULL values for missing arrival or departure
           times in stop times  */
        sqlite_result = sqlite3_bind_null(parsing_state->insert_stmt,
                                          field_number + 1);
      }
      else {
        /* Convert the "H:MM:SS" or "HH:MM:SS" format to a number of
           seconds since midnight */
        /* TODO: This may not correctly handle the shift to or from
           daylight saving time */
        hours = strtok(val, ":");
        minutes = strtok(NULL, ":");
        seconds = strtok(NULL, ":");

        assert(strtok(NULL, ":") == NULL);

        sqlite_result = sqlite3_bind_int(parsing_state->insert_stmt,
                                         field_number + 1,
                                         atoi(hours) * 3600 +
                                         atoi(minutes) * 60 +
                                         atoi(seconds));
      }
      break;
    }

    if(sqlite_result != SQLITE_OK) {
      fprintf(stderr,
              "field_parsed: "
              "Error binding value \"%.*s\" to %s: %s\n",
              len,
              val,
              field_spec->name,
              sqlite3_errmsg(parsing_state->db));
    }
  }
  else {
    /* We're still parsing the header; use this header field to update
       our column-number-to-field-number mapping */

    assert(parsing_state->fields_parsed < MAX_COLUMNS);

    /* Search for this field's number by name */
    field_number = 0;
    field_name_matched = false;
    while(!field_name_matched &&
          field_number < gtfs_file_spec->num_fields) {
      field_name_matched =
        (strncmp(gtfs_file_spec->field_specs[field_number++]->name,
                 val,
                 len) == 0);
    }

    /* TODO: Better error-handling. Add fields to the
       gtfs_parsing_state_t structure to indicate whether an error has
       occurred, and a description of the error? */
    assert(field_name_matched);

    /* Add the mapping */
    parsing_state->field_for_column[parsing_state->fields_parsed] =
      field_number - 1;
  }

  /* Another field parsed from the current record */
  parsing_state->fields_parsed++;
}

/* Invoked by the CSV parser each time a row has been parsed */
static void record_parsed(int eor, void *data) {
  gtfs_parsing_state_t *parsing_state = (gtfs_parsing_state_t *)data;

  if(parsing_state->header_parsed) {
    /* End and start a new transaction if we've reached our limit on
       the number of objects in this one */
    if(parsing_state->objects_loaded % RECORDS_PER_TRANSACTION == 0) {
        assert(sqlite3_step(end_transaction_stmt) == SQLITE_DONE);
        assert(sqlite3_reset(end_transaction_stmt) == SQLITE_OK);

        assert(sqlite3_step(begin_transaction_stmt) == SQLITE_DONE);
        assert(sqlite3_reset(begin_transaction_stmt) == SQLITE_OK);
    }

    /* Insert the parsed record into the database */
    if(sqlite3_step(parsing_state->insert_stmt) == SQLITE_DONE) {
      /* Another object loaded to the database */
      parsing_state->objects_loaded++;
    }
    else {
      fprintf(stderr,
              "record_parsed: "
              "Error inserting record: %s\n",
              sqlite3_errmsg(parsing_state->db));
    }

    /* Unbind the values from the INSERT statement and reset it */
    sqlite3_clear_bindings(parsing_state->insert_stmt);
    if(sqlite3_reset(parsing_state->insert_stmt) != SQLITE_OK) {
      fprintf(stderr,
              "record_parsed: "
              "Error resetting INSERT statement: %s\n",
              sqlite3_errmsg(parsing_state->db));
    }

    /* Another record parsed */
    parsing_state->records_parsed++;
  }
  else {
    /* We've now finished parsing the header---our
       column-number-to-field-number mapping should be complete */
    parsing_state->header_parsed = true;
  }

  /* Reset our fields-parsed (per record) counter */
  parsing_state->fields_parsed = 0;
}

/* Loads a GTFS file (that is, a file contained within a GTFS bundle)
   according to the provided GTFS-file specifier */
long load_gtfs_file(const gtfs_file_spec_t *gtfs_file_spec,
                    struct zip *gtfs_zip,
                    sqlite3 *db,
                    struct csv_parser *csv) {
  long result = -1;
  struct zip_file *gtfs_zip_member;
  char *errmsg;
  sqlite3_stmt *insert_stmt;
  char buf[BUFFER_SIZE];
  int bytes_read;
  gtfs_parsing_state_t parsing_state;
  bool parsing_error;

  /* Open the file within the GTFS bundle---note this should always
     succeed as the main routine has validated the bundle contains the
     needed files */
  if(gtfs_zip_member = zip_fopen(gtfs_zip,
                                 gtfs_file_spec->filename,
                                 0)) {
    /* Create the corresponding table in the database */
    if(create_table(db, gtfs_file_spec, &errmsg) == SQLITE_OK) {
      /* Prepare the INSERT statement */
      if(prepare_insert_stmt(db,
                             gtfs_file_spec,
                             &insert_stmt) == SQLITE_OK) {
        /* We're just about ready to parse---reset our parsing state */
        memset(&parsing_state, 0, sizeof(parsing_state));
        parsing_state.db = db;
        parsing_state.gtfs_file_spec = gtfs_file_spec;
        parsing_state.insert_stmt = insert_stmt;

        /* Start a new transaction in the database */
        assert(sqlite3_step(begin_transaction_stmt) == SQLITE_DONE);
        assert(sqlite3_reset(begin_transaction_stmt) == SQLITE_OK);

        /* Now parse the CSV file */
        bytes_read = zip_fread(gtfs_zip_member,
                               buf,
                               BUFFER_SIZE);
        while(bytes_read > 0 && !parsing_error) {
          /* Parse this data, invoking our callback functions as
             each field or record is parsed */
          parsing_error =
            csv_parse(csv,
                      buf,
                      bytes_read,
                      field_parsed,
                      record_parsed,
                      &parsing_state)
            != bytes_read;

          if(parsing_error) {
            fprintf(stderr,
                    "load_gtfs_file: "
                    "Error parsing CSV data: %s\n",
                    csv_strerror(csv_error(csv)));
          }
          else {
            bytes_read = zip_fread(gtfs_zip_member,
                                   buf,
                                   BUFFER_SIZE);
          }
        }

        /* End this final database transaction */
        assert(sqlite3_step(end_transaction_stmt) == SQLITE_DONE);
        assert(sqlite3_reset(end_transaction_stmt) == SQLITE_OK);

        /* Finalize the CSV parser */
        assert(csv_fini(csv,
                        field_parsed,
                        record_parsed,
                        &parsing_state) == 0);

        /* Free our prepared INSERT statement */
        if(sqlite3_finalize(insert_stmt) != SQLITE_OK) {
          fprintf(stderr,
                  "load_gtfs_file: "
                  "Error finalizing INSERT statement: %s\n",
                  sqlite3_errmsg(db));
        }

        /* Return the number of objects loaded to our caller */
        result = parsing_state.objects_loaded;
      }
      else {
        fprintf(stderr,
                "Error preparing INSERT statement: %s\n",
                sqlite3_errmsg(db));
      }
    }
    else {
      fprintf(stderr,
              "Error creating database table: %s\n",
              errmsg);
    }

    /* All done---close the GTFS member file */
    assert(zip_fclose(gtfs_zip_member) == 0);
  }
  else {
    fprintf(stderr,
            "Error opening ZIP member \"%s\": %s\n",
            gtfs_file_spec->filename,
            zip_strerror(gtfs_zip));
  }

  /* Return the number of objects loaded into the database, or -1 on
     error */
  return result;
}

/* Validates a GTFS bundle before it is loaded---at the moment, this
   simply checks to make sure the bundle contains the files we expect
   to load */
bool validate_gtfs_bundle(struct zip *gtfs_zip) {
  const gtfs_file_spec_t *gtfs_file_spec;
  bool result = true;
  int index;
  const char *filename;

  /* Make sure the GTFS bundle contains all the required files */
  index = 0;
  while(gtfs_file_spec = gtfs_file_specs[index++]) {
    filename = gtfs_file_spec->filename;
    if(zip_name_locate(gtfs_zip, filename, 0) == -1) {
      fprintf(stderr,
              "Error: Bundle is missing required file \"%s\".\n",
              filename);
      result = false;
    }
  }

  return result;
}

/* Main entry point for gtfs2db */
int main(int argc, char *argv[]) {
  int result = 1;

  char *gtfs_path;
  char *db_path;

  struct zip *gtfs_zip;
  struct zip_file *gtfs_zip_member;
  const char *zip_member_name;
  int zip_error, system_error;
  int zip_member_count, zip_member_index;
  char zip_error_str[256];

  sqlite3 *db;

  const gtfs_file_spec_t *gtfs_file_spec;
  int gtfs_file_specs_index;

  struct csv_parser csv;
  bool parsing_error;

  GTimer *parsing_timer;
  gdouble parsing_time_elapsed;
  long objects_loaded;
  const char *object_name;

  if(argc > 2) {
    /* Get our parameters */
    gtfs_path = argv[1];
    db_path = argv[2];

    /* Open and process the GTFS bundle (ZIP file) */
    gtfs_zip = zip_open(gtfs_path, ZIP_CHECKCONS, &zip_error);
    if(gtfs_zip) {
      /* List the bundle's contents */
      puts("Bundle contents:");
      zip_member_count = zip_get_num_files(gtfs_zip);
      for(zip_member_index = 0;
          zip_member_index < zip_member_count;
          zip_member_index++) {
        zip_member_name = zip_get_name(gtfs_zip, zip_member_index, 0);
        if(zip_member_name) {
          printf("  %s\n", zip_member_name);
        }
      }

      /* Validate the GTFS bundle before continuing */
      if(validate_gtfs_bundle(gtfs_zip)) {
        /* Create and open the database */
        if(sqlite3_open(db_path, &db) == SQLITE_OK) {
          /* Precompile our "BEGIN TRANSACTION" and "END TRANSACTION"
             statements */
          assert(sqlite3_prepare_v2(db,
                                    "BEGIN TRANSACTION",
                                    strlen("BEGIN TRANSACTION"),
                                    &begin_transaction_stmt,
                                    NULL) == SQLITE_OK);
          assert(sqlite3_prepare_v2(db,
                                    "END TRANSACTION",
                                    strlen("END TRANSACTION"),
                                    &end_transaction_stmt,
                                    NULL) == SQLITE_OK);

          /* Initialize our CSV parser */
          assert(csv_init(&csv, CSV_STRICT | CSV_APPEND_NULL) == 0);

          /* Now step through our data structure that specifies files
             to parse and how they should be parsed, parsing each
             file */
          parsing_error = false;
          gtfs_file_specs_index = 0;
          while((gtfs_file_spec = gtfs_file_specs[gtfs_file_specs_index++]) &&
                !parsing_error) {
            printf("Processing \"%s\": ", gtfs_file_spec->filename);
            fflush(stdout);

            parsing_timer = g_timer_new();
            objects_loaded = load_gtfs_file(gtfs_file_spec,
                                            gtfs_zip,
                                            db,
                                            &csv);
            g_timer_stop(parsing_timer);
            parsing_time_elapsed = g_timer_elapsed(parsing_timer,
                                                   NULL);

            /* If parsing was successful, output the number of objects
               loaded and the time it took */
            if(objects_loaded > -1) {
              object_name = objects_loaded == 1?
                gtfs_file_spec->name.singular:
                gtfs_file_spec->name.plural;
              printf("%lu %s added in %.2f seconds",
                     objects_loaded,
                     object_name,
                     parsing_time_elapsed);
              if(parsing_time_elapsed > 0) {
                printf(" (%.2fms/%s)",
                       (parsing_time_elapsed * 1000) / objects_loaded,
                       gtfs_file_spec->name.singular);
              }
            }
            else {
              parsing_error = TRUE;
            }

            puts("");
          }

          /* Free our CSV parser */
          csv_free(&csv);

          /* Free our "BEGIN TRANSACTION" and "END TRANSACTION"
             statements */
          assert(sqlite3_finalize(begin_transaction_stmt) ==
                 SQLITE_OK);
          assert(sqlite3_finalize(end_transaction_stmt) ==
                 SQLITE_OK);

          /* Close the database */
          puts("GTFS bundle loaded.");
          assert(sqlite3_close(db) == SQLITE_OK);

          /* Success! */
          result = 0;
        }
        else {
          fprintf(stderr,
                  "Error creating database \"%s\": %s",
                  db_path,
                  sqlite3_errmsg(db));
        }
      }

      /* All done; close the GTFS ZIP file and exit */
      assert(zip_close(gtfs_zip) == 0);
    }
    else {
      /* Couldn't open the GTFS bundle; print an error message and
         exit */
      zip_error_to_str(zip_error_str, 256, zip_error, errno);
      fprintf(stderr,
              "Error opening ZIP file: %s\n",
              zip_error_str);
    }
  }
  else {
    /* Print out our usage and exit */
    puts("Usage: gtfs2db gtfs-file db-file");
  }

  return result;
}
