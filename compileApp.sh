#! /bin/bash

# Build data
preCompilePackage="component_archive"
executableFilename="smallsh"
mainFilename="main.c"

# Components
ioHandlerMethods="ioHandlerMethods"

# Cleans previously compiled files and created folders
function preCompileClean() {
  rm -rf $executableFilename *.o *.a
}

# Cleans space of temporary compile files
function postCompileClean() {
  rm -f *.o *.a
}

function generateModuleObjectsAndArchive() {
  # Create precompiled objects
  gcc --std=gnu99 -c constants/constants.c
  gcc --std=gnu99 -c ioHandlerMethods/ioHandlerMethods.c
  gcc --std=gnu99 -c subProcessHandlers/subProcessHandlers.c

  # Creates archive with objects
  ar -r ${preCompilePackage}.a constants.o ioHandlerMethods.o subProcessHandlers.o
}

# Compiles main
function compileMainAndArchive() {
  gcc --std=gnu99 -o $executableFilename $mainFilename ${preCompilePackage}.a
}

function main() {
  # Pre cleaning, removes leftovers from previous runs
  preCompileClean

  # Compilation step
  generateModuleObjectsAndArchive
  compileMainAndArchive

  # Pre cleaning to eliminate temporary files
  postCompileClean

  # Handles parameters to execute.
  # Param e -> triggers executable
  while getopts "ev" flag; do
    case $flag in
    e) ./$executableFilename ;;
    v) valgrind ./$executableFilename ;;
    esac
    shift
  done
}

# Execute this script passing params to main
main "$@"
