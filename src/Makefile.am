errno_SOURCES	= dict.h errnames.c errno.c gcc-compat.h netnames.c        \
         signames.c tcp_states.h x11names.c webnames.c pamnames.c
bin_PROGRAMS	= errno
EXTRA_DIST	= get-err-names get-x11-names get-net-names get-sig-names \
		  get-web-names rfc1945.txt

AM_CPPFLAGS	= \
	-DLOCALEDIR=\""$(localedir)"\"

AM_LDFLAGS	=-lpopt

errnames.c: get-err-names
	./get-err-names -o $@

x11names.c: get-x11-names
	./get-x11-names -o $@

netnames.c: get-net-names
	./get-net-names -o $@

signames.c: get-sig-names
	./get-sig-names -o $@

webnames.c: get-web-names rfc1945.txt
	./get-web-names -o $@ rfc1945.txt
