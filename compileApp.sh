#! /bin/bash

# Build data
preCompilePackage="component_archive"
executableFilename="smallsh"
mainFilename="main.c"

# Components
declare -a componentList=(
  "constants"
  "ioHandlerMethods"
  "subProcessHandlers"
  "builtinFunctions"
)

# Cleans previously compiled files and created folders
function preCompileClean() {
  rm -rf $executableFilename *.o *.a
}

# Cleans space of temporary compile files
function postCompileClean() {
  rm -f *.o *.a
}

function generatePrecompiledObject() {
  gcc --std=gnu99 -c ${1}/${1}.c
}

function generateModuleObjectsAndArchive() {
  # Stores object list
  precompileObjectList=""

  # Create precompiled objects and list
  for component in ${componentList[@]}; do
    precompileObjectList+=" ${component}.o"
    generatePrecompiledObject $component
  done

  # Creates archive with objects
  ar -r ${preCompilePackage}.a $precompileObjectList
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
  # Param triggers:
  #  e -> run executable
  #  v -> run executable with valgrind leak analysis
  #  t -> run executable with testscript
  while getopts "ev" flag; do
    case $flag in
    e) ./$executableFilename ;;
    v) valgrind ./$executableFilename ;;
    t) ./p3testscript 2>&1 ;;
    esac
    shift
  done
}

# Execute this script passing params to main
main "$@"
