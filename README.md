gtfs2db
=======

gtfs2db is a command-line utility written in C that generates an
[SQLite](https://www.sqlite.org/) database from a
[GTFS](https://developers.google.com/transit/gtfs/reference) feed (a ZIP file).

The database it generates is useful for quickly exploring downloaded GTFS data
and can serve as the data store underlying a transit application.

Requirements
------------

gtfs2db uses the [GLib](https://developer.gnome.org/glib/),
[libcsv](http://sourceforge.net/projects/libcsv/),
[libzip](http://www.nih.at/libzip/) and SQLite libraries. On Red Hat-based Linux
systems, including CentOS and Fedora, you can install the necessary packages with

    sudo yum install glib2 glib2-devel libcsv libcsv-devel libzip libzip-devel \
        sqlite sqlite-devel

On Ubuntu (15.04 and higher), run

    sudo apt-get install libglib2.0-0 libglib2.0-dev libcsv3 libcsv-dev \
        libzip2 libzip-dev libsqlite3-0 libsqlite3-dev

Installation and Usage
----------------------

Build the utility with

    ./build.sh

Currently no install script is provided, but the executable is self-contained
and can be moved to anywhere convenient on your system, such as /usr/local/bin:

    sudo mv -t /usr/local/bin/ ./gtfs2db
    sudo chmod a+rx /usr/local/bin/gtfs2db

To generate a database from a downloaded GTFS feed, invoke the utility like
this:

    gtfs2db ./google_transit.zip ./google_transit.sqlite

The generated database can then be opened at the command line with

    sqlite3 ./google_transit.sqlite
