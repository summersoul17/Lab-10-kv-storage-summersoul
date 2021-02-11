cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_COVERAGE "Build code coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
  SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
  FILEPATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake
)

# TODO: rename project and delete this comment
project(kv-storage VERSION 0.1.0)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(gflags)
find_package(gflags CONFIG REQUIRED)

hunter_add_package(rocksdb)
find_package(RocksDB CONFIG REQUIRED)

hunter_add_package(Boost COMPONENTS program_options log thread system filesystem program_options)
find_package(Boost CONFIG REQUIRED program_options log log_setup thread system filesystem program_options)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

#=======================================================LOGGER======================================================
add_library(logger INTERFACE)

target_include_directories( logger INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/logger
        )

target_link_libraries(logger INTERFACE
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        )
#===================================================================================================================

#=======================================================CONSOLE=====================================================
add_library(console STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/console.cpp)

target_include_directories(console PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/console
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/PicoSHA2
        )

target_link_libraries(console
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        )
#===================================================================================================================

#=======================================================STORAGE=====================================================
add_library(storage STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/storage.cpp)

target_include_directories(storage PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/storage
        )

target_link_libraries(storage RocksDB::rocksdb)
#===================================================================================================================

target_link_libraries(console logger storage)

add_executable(dbcs
  ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp
)

target_link_libraries(dbcs console)

if(BUILD_DOCS)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/_docs)
  set(DOXYGEN_EXAMPLE_PATH examples)
  set(DOXYGEN_GENERATE_HTML NO)
  set(DOXYGEN_GENERATE_MAN YES)
  find_package(Doxygen)
  doxygen_add_docs(docs include)
  install(DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/man DESTINATION share)
  # add_dependencies(install docs) need to call target docs before install or package
endif()

include(CPackConfig.cmake)
