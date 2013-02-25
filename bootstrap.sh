#!/bin/sh
./autogen.sh
./configure
make -j10 dist
rclean
mv *z ~/rpm/SOURCES
rpmbuild -ba *spec
