#!/bin/bash
#
# Copyright (c) 2022 Adam Strzelecki, All rights reserved.
#
# Released under GPL v3 license.
#

set -e

dir=$(cd "$(dirname "$0")"; pwd)

builddir="$dir/build"
extdir="$dir/ext"
extsrcdir="$HOME/Code/Applications/scopy/src"
extlibdir="$extdir/lib"
extbuilddir="$extdir/build"

echo >&2 "builddir:    $builddir"
echo >&2 "extsrcdir:   $extsrcdir"
echo >&2 "extdir:      $extdir"
echo >&2 "extlibdir:   $extlibdir"
echo >&2 "extbuilddir: $extbuilddir"
# exit 1

build() {
	if [ -f "$builddir/build.ninja" ]; then
		echo >&2 $builddir is already configured
	else
		cmakeargs=(
			-GNinja
			-DCMAKE_BUILD_TYPE=RelWithDebInfo
			-DBUILD_SHARED_LIBS:BOOL=OFF
			-DCMAKE_PREFIX_PATH="$extdir"
			-S "$dir"
			-B "$builddir"
			)
		cmake "$@" "${cmakeargs[@]}"
	fi
	ninja -C "$builddir"
}

buildext() {
	name=$1
	lname=${name#lib}
	shift 1
	if [ -f "$extlibdir/lib$name.a" -o -d "$extlibdir/$lname.framework" ]; then
		echo >&2 $name is already built
	else
		if [ -f "$extbuilddir/$name/build.ninja" ]; then
			echo >&2 $name is already configured
		else
			cmakeargs=(
				-GNinja
				-DCMAKE_BUILD_TYPE=RelWithDebInfo
				-DBUILD_SHARED_LIBS:BOOL=OFF
				-DCMAKE_INSTALL_PREFIX="$extdir"
				-S "$src/$name"
				-B "$extbuilddir/$name"
				)
			cmake "$@" "${cmakeargs[@]}"
		fi
		ninja -C "$extbuilddir/$name" && ninja -C "$extbuilddir/$name" install
	fi
}

libiioargs=(
	-DWITH_TESTS:BOOL=OFF
	-DWITH_DOC:BOOL=OFF
	-DCSHARP_BINDINGS:BOOL=OFF
	-DPYTHON_BINDINGS:BOOL=OFF
	-DOSX_PACKAGE:BOOL=OFF
	-DOSX_INSTALL_FRAMEWORKSDIR="$extlibdir"
	)
buildext libiio "${libiioargs[@]}"

libm2kargs=(
	-DENABLE_PYTHON:BOOL=OFF
	)
buildext libm2k "${libm2kargs[@]}"

[ -f $dir/resources/Cutescope.icns ] || $dir/resources/png2icns $dir/resources/Cutescope.png

build
