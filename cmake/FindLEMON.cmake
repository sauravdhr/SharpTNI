# Try to find the LEMON library
#
#   LEMON_FOUND
#   LEMON_INCLUDE_DIRS
#   LEMON_LIBRARIES
#   LEMON_DEFINITIONS

find_package(PkgConfig QUIET)
pkg_check_modules(PC_LEMON QUIET lemon)
set(LEMON_DEFINITIONS ${PC_LEMON_CFLAGS_OTHER})

find_path(LEMON_INCLUDE_DIR lemon/core.h
  HINTS ${PC_LEMON_INCLUDEDIR} ${PC_LEMON_INCLUDE_DIRS}
  PATHS "$ENV{HOME}/lemon"
  PATH_SUFFIXES include lemon)

find_library(LEMON_LIBRARY NAMES emon libemon
  HINTS ${PC_LEMON_LIBDIR} ${PC_LEMON_LIBRARY_DIRS}
  PATHS "$ENV{HOME}/lemon"
  PATH_SUFFIXES lib lemon)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LEMON DEFAULT_MSG
    LEMON_LIBRARY LEMON_INCLUDE_DIR)

mark_as_advanced(LEMON_INCLUDE_DIR LEMON_LIBRARY)

set(LEMON_LIBRARIES ${LEMON_LIBRARY})
set(LEMON_INCLUDE_DIRS ${LEMON_INCLUDE_DIR})
