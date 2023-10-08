SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/src/lib -I$(top_builddir)/src/lib
AM_CPPFLAGS += $(BOOST_INCLUDES) $(MYSQL_CPPFLAGS)

AM_CXXFLAGS = $(KEA_CXXFLAGS)

CLEANFILES = *.gcno *.gcda

lib_LTLIBRARIES = libkea-ubus.la
libkea_ubus_la_SOURCES  = ubus.cc ubus.h

libkea_ubus_la_LIBADD  = $(top_builddir)/src/lib/database/libkea-database.la
libkea_ubus_la_LIBADD += $(top_builddir)/src/lib/cc/libkea-cc.la
libkea_ubus_la_LIBADD += $(top_builddir)/src/lib/asiolink/libkea-asiolink.la
libkea_ubus_la_LIBADD += $(top_builddir)/src/lib/log/libkea-log.la
libkea_ubus_la_LIBADD += $(top_builddir)/src/lib/util/libkea-util.la
libkea_ubus_la_LIBADD += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
libkea_ubus_la_LIBADD += $(LOG4CPLUS_LIBS) $(BOOST_LIBS)

libkea_ubus_la_LDFLAGS = -no-undefined -version-info 51:0:0

libkea_ubus_la_LDFLAGS += $(UBUS_LIBS)

# Specify the headers for copying into the installation directory tree.
libkea_ubus_includedir = $(pkgincludedir)/ubus
libkea_ubus_include_HEADERS = \
	ubus.h
