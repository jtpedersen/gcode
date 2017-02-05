cmake_minimum_required(VERSION 3.1.0)

PROJECT("Gcode Parsing")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)

set(QT57Path "Path for qt5.7" CACHE STRING "/media/jacob/0b25e20a-d821-4863-86a4-2c990267cefe/Qt5.7/5.7/gcc_64/lib/cmake/Qt5/")

message("QT57Path:${QT57Path}")

find_package(Qt5 REQUIRED
  COMPONENTS Core Gui Widgets 3DCore 3DExtras 3DRender 3DInput
  CONFIG
  PATHS ${QT57Path}
  NO_DEFAULT_PATH)
add_subdirectory(src)


SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET(CMAKE_CXX_FLAGS_DEBUG "-g -p -O0 -Wall -pedantic -Werror ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")

OPTION(ASAN "Use adresss sanitizer" OFF)
IF (ASAN)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
ENDIF()

ADD_EXECUTABLE(
  gcodester
  main.cc
  )
