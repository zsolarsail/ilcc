
AM_CXXFLAGS += -I$(top_srcdir)


#EXTRA_DIST=

noinst_HEADERS = \
misc.h \
compile.h

bin_PROGRAMS = ilcc

ilcc_SOURCES = \
misc.cpp \
compile.cpp \
main.cpp


ilcc_LDADD = $(top_srcdir)/libilcc/libilcc.a


#xxxconfdir=$(sysconfdir)/x
#xxxconf_DATA=x.conf

