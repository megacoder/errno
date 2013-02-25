errno_SOURCES	= errno.c errnames.c netnames.c x11names.c gcc-compat.h dict.h
bin_PROGRAMS	= errno
EXTRA_DIST	= get-err-names get-x11-names

AM_CPPFLAGS	= \
	-DLOCALEDIR=\""$(localedir)"\"

AM_LDFLAGS	=-lpopt

errnames.c: get-err-names
	./get-err-names -o $@

x11-names.c: get-x11-names
	./get-x11-names -o $@
