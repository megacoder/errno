#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([errno], [1.2.1], [Tommy.Reynolds@MegaCoder])
AC_CONFIG_SRCDIR([gettext.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpopt':
AC_CHECK_LIB([popt], [main])

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([libintl.h limits.h locale.h netdb.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION([0.18.2])
AC_FUNC_MALLOC
AC_CHECK_FUNCS([setlocale strcasecmp strchr strerror strrchr])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([
        Makefile
        errno.spec
        po/Makefile.in
        src/Makefile
        intl/Makefile
])

AC_OUTPUT
