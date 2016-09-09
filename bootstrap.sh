#!/bin/sh
(
	autoreconf -fvim
	./configure
	make dist
	rm -rf RPM
	mkdir -p RPM/{SOURCES,RPMS,SRPMS,BUILD,SPECS}
	rpmbuild							\
		-D"_topdir ${PWD}/RPM"					\
		-D"_sourcedir ${PWD}"					\
		-D"_specdir ${PWD}"					\
		-ba							\
		errno.spec
) 2>&1 | tee bootstrap.log
