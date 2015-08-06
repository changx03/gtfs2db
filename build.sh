#!/bin/sh

# A simple build script for gtfs2db until a proper makefile is
# available.
#
# Copyright (c) 2012 Simon South. All rights reserved.

gcc -std=c99 -O2 main.c -L/usr/local/lib `pkg-config --cflags --libs glib-2.0` -lcsv -lsqlite3 -lzip -o gtfs2db
