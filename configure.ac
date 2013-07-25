#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([errno], [1.0.0], [Tommy Reynolds <Tommy.Reynolds@Megacoder.com>])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

#
AM_GNU_GETTEXT_VERSION([0.18.1])
AM_GNU_GETTEXT([external])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
# FIXME IT_PROG_INTLTOOL
LT_INIT([enable-shared])
LT_PREREQ([2.2.10])
AC_SUBST([LIBTOOL_DEPS])
AC_PROG_YACC

# Checks for libraries.
AC_CHECK_LIB([c], [getpid])
AC_CHECK_LIB([ncurses], [tgetstr])
AC_CHECK_LIB([readline], [readline])

# Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_CHECK_HEADERS([argz.h fcntl.h float.h inttypes.h langinfo.h libintl.h limits.h locale.h malloc.h netdb.h stddef.h stdint.h stdio_ext.h stdlib.h string.h strings.h sys/param.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify getcwd mempcpy munmap nl_langinfo setlocale stpcpy strcasecmp strchr strcspn strdup strerror strrchr strstr strtoul])

#
ALL_LINGUAS="en_US"
GETTEXT_PACKAGE=errno
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Define text domain.])
dnl AM_GLIB_GNU_GETTEXT
#
AM_MAINTAINER_MODE
#
if test "x$DATADIRNAME" != "x"; then
        localedir='${prefix}/${DATADIRNAME}/locale'
else
        localedir='${datadir}/locale'
fi
AC_ARG_WITH(localedir,
        [  --with-localedir=DIR    Locale directory],
        [localedir="$withval"])
AC_SUBST(localedir)

#
AC_CONFIG_FILES([
	Makefile
	errno.spec
	po/Makefile
	src/Makefile
])
AC_OUTPUT
