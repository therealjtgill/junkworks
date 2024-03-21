set(
   junkwork_sources
   src/basic_udp.c
   src/client.cpp
   src/ipv4add.cpp
   src/server.cpp
   src/udp_socket_wrapper.cpp
   src/udp_socket.cpp
)

set(
   junkwork_headers
   include/junkworks/basic_udp.h
   include/junkworks/client.hpp
   include/junkworks/ipv4add.hpp
   include/junkworks/network_common.h
   include/junkworks/server.hpp
   include/junkworks/types.hpp
   include/junkworks/udp_socket_wrapper.hpp
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
