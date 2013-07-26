#!/bin/sh
exec 2>&1
SYSNAME=`uname`
if [ "x$SYSNAME" = "xDarwin" ] ; then
	LIBTOOLIZE=glibtoolize
else
	LIBTOOLIZE=libtoolize
fi
markdown2 README.md | tee README.html | lynx -dump -stdin >README
mkdir -p	m4
aclocal		--force -I m4
autoheader	--force
$LIBTOOLIZE	--force
autoconf	--force
automake	--add-missing --force-missing
