#!/bin/sh

SOURCE_DIR=`pwd`
BUILD_DIR=${SOURCE_DIR}/build
LIB_DIR=${SOURCE_DIR}/liblwf
TEST_DIR=${SOURCE_DIR}/test
EXAMPLE_DIR=${SOURCE_DIR}/example

cd $LIB_DIR
make $*

cd $TEST_DIR
make $*

cd $EXAMPLE_DIR/httpserver
make $*

