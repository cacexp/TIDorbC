SUBDIRS = . 

AM_CPPFLAGS = \
	-I$(top_srcdir)/source/TIDThr/tidthr/include \
	-I$(top_srcdir)/source/TIDSocket/tidsocket/include \
	-I$(top_srcdir)/source/TIDorbC/orb/include \
	-I$(top_builddir)/source/TIDorbC/orb/IDL/01_orb

CLEANFILES = *.C *.h

idl_cpp_headers = \
	FullValueDescription_ext.h \
	FullValueDescription.h

idl_cpp_sources = \
	FullValueDescription.C 

instdir=$(includedir)/CORBA/ValueDef

inst_HEADERS = \
	$(idl_cpp_headers)

noinst_LTLIBRARIES = libTIDorbIdl_orb_CORBA_ValueDef.la

libTIDorbIdl_orb_CORBA_ValueDef_la_SOURCES = $(idl_cpp_headers) $(idl_cpp_sources)
