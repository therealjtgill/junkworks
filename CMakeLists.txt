cmake_minimum_required( VERSION 3.16 )

set( CMAKE_CXX_STANDARD 11 )

project(
   NETWORKING_DOODOO
   VERSION 0.0.1
)

include( ${CMAKE_SOURCE_DIR}/cmake/functions.cmake )

add_compile_definitions(
   NETWORKING_DOODOO
   PRIVATE
   SOURCE_DIR="${CMAKE_SOURCE_DIR}/.."
   BIN_DIR="${CMAKE_CURRENT_BINARY_DIR}"
)

set( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" )
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" )

add_subdirectory( demos/client_server )

add_subdirectory( junkwork )
