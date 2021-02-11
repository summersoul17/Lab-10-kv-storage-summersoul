# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.12)
project(PicoSHA2)

set(CMAKE_CXX_STANDARD 17)

# Add tasks subprojects
add_library(PicoSHA2 STATIC ${SOURCE_LIB}picosha2.h)