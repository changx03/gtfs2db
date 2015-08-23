#!/bin/sh

# A simple build script for gtfs2db until a proper makefile is
# available.
#
# Copyright (c) 2012 Simon South <ssouth@simonsouth.com>.
#
# This file is part of gtfs2db.
#
# gtfs2db is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# gtfs2db is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gtfs2db.  If not, see <http://www.gnu.org/licenses/>. */

gcc -std=c99 -O2 main.c -L/usr/local/lib `pkg-config --cflags --libs glib-2.0` -lcsv -lsqlite3 -lzip -o gtfs2db
