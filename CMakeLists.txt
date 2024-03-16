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

# How to copy libraries (DLL's on Windows) over to binary directory
# https://stackoverflow.com/a/63158941

add_subdirectory( demos/client_server )

add_subdirectory( junkwork )

# set(
#    sources
#    client.cpp
#    # server.cpp
# )

# set(
#    headers

# )

# set(
#    target
#    networking_doodoo
# )

# add_executable(
#    ${target}
#    ${sources}
#    ${headers}
# )

# include_directories(
#    ${CMAKE_CURRENT_SOURCE_DIR}
# )

# target_include_directories(
#    ${target}
#    PUBLIC
#    ${CMAKE_CURRENT_SOURCE_DIR}
# )

# set_target_properties(
#    ${target}
#    PROPERTIES
#    LINKER_LANGUAGE
#    CXX
# )

# set(
#    link_libraries
# )

# # append wsock2 and ws2_32 to the link libraries variable if on Windows

# target_link_libraries(
#    ${target}
# )

# target_compile_options(
#    ${target}
#    PRIVATE
#    -Wall
#    -Wextra
#    -Wpedantic
#    -Wno-unused-function
#    # -Werror
# )
