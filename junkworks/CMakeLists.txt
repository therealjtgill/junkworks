set(
   junkwork_sources
   src/basic_udp.c
   src/udp_socket.cpp
)

set(
   junkwork_headers
   include/junkworks/basic_udp.h
   include/junkworks/network_common.h
   include/junkworks/udp_socket.hpp
)

set(
   target
   junkworks
)

set(
   junkwork_libs
)

make_library(
   "${target}" "${junkwork_sources}" "${junkwork_headers}" "${junkwork_libs}" ${CMAKE_CURRENT_SOURCE_DIR}/include
)
