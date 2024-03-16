set(
   junkwork_sources
   src/basic_udp.cpp
)

set(
   junkwork_headers
   include/junkwork/basic_udp.hpp
)

set(
   target
   junkwork
)

set(
   junkwork_libs
)

make_library(
   "${target}" "${junkwork_sources}" "${junkwork_headers}" "${junkwork_libs}" ${CMAKE_CURRENT_SOURCE_DIR}/include
)
