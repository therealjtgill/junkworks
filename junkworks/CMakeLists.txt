set(
   junkwork_sources
   src/basic_udp.cpp
   src/udp_socket.cpp
)

set(
   junkwork_headers
   include/junkworks/basic_udp.hpp
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
