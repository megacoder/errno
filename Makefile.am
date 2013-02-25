SUBDIRS	= po src

EXTRA_DIST	= autogen.sh LICENSE README.md

ACLOCAL_AMFLAGS = -I m4

LIBTOOL_DEPS = @LIBTOOL_DEPS@

libtool:	${LIBTOOL_DEPS}
	${SHELL} ./config.status libtool
