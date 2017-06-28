cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.Top )

option( COVERAGE "Enable coverage analysis" ON )

if( COVERAGE AND NOT MSVC )

    set( CMAKE_CONFIGURATION_TYPES "Debug;Release;Coverage" CACHE STRING "" FORCE )

    set( CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_COVERAGE} -g -O0 -fprofile-arcs -ftest-coverage" )
    set( CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} -g -O0 -fprofile-arcs -ftest-coverage" )
    set( CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_COVERAGE} -fprofile-arcs -ftest-coverage" )
    
else()

    set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE )
    
endif()

if( WIN32 AND NOT MSVC )
    add_definitions( -DWIN32 )
endif()

add_custom_target( build )

add_subdirectory( lib )
add_subdirectory( test )
