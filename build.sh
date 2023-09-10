#!/bin/sh
set -o errexit
set -o nounset
USAGE="Usage: (basename $0) [ debug | release ]"

CMAKE=cmake
CMAKE_LISTS_FILE=./
BUILD=./build
TYPE=DEBUG
BUILD_DIR=$BUILD/debug

for arg; do
  case "$arg" in
    --help|-h)    echo $USAGE; exit 0;;
    debug)        TYPE=DEBUG; BUILD_DIR=$BUILD/debug ;;
    release)      TYPE=RELEASE; BUILD_DIR=$BUILD/release ;;
    *)            echo -e "unknown option $arg\n$USAGE" >&2; exit 1 ;;
  esac
done

echo "cleaning build dir\n"
#rm -rf $BUILD_DIR
./clean.sh

echo "configuring ..."
$CMAKE -S $CMAKE_LISTS_FILE -B $BUILD_DIR -DCMAKE_BUILD_TYPE=$TYPE
echo " DONE\n"

echo "compiling ..."
$CMAKE --build $BUILD_DIR
echo " DONE\n"
