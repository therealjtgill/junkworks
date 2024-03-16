function( make_executable target_name sources headers links )
   message( STATUS "making executable for ${target_name}" )

   set( target ${target_name} )

   # How to copy libraries (DLL's on Windows) over to binary directory
   # https://stackoverflow.com/a/63158941

   add_executable(
      ${target}
      ${sources}
      ${headers}
   )

   set_target_properties(
      ${target}
      PROPERTIES
      LINKER_LANGUAGE CXX
   )

   target_link_libraries(
      ${target}
      ${links}
   )

   target_compile_options(
      ${target}
      PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wno-unused-function
      # -Werror
   )   
endfunction()

function( make_library target_name sources headers links include_dir )
   message( STATUS "making library for ${target_name}" )

   set( target ${target_name} )

   # How to copy libraries (DLL's on Windows) over to binary directory
   # https://stackoverflow.com/a/63158941

   add_library(
      ${target}
      SHARED
      ${sources}
      ${headers}
   )

   set_target_properties(
      ${target}
      PROPERTIES
      LINKER_LANGUAGE CXX
   )

   include_directories(
      ${include_dir}
   )
   
   target_include_directories(
      ${target}
      PUBLIC
      ${include_dir}
   )

   target_link_libraries(
      ${target}
      ${links}
   )

   target_compile_options(
      ${target}
      PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wno-unused-function
      # -Werror
   )
endfunction()
