cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib )
set( PROJECT_VERSION_MAJOR 0 )
set( PROJECT_VERSION_MINOR 9 )
set( PROJECT_VERSION_PATCH 0 )

option( BUILD_SHARED_LIBS "Build as a shared library" ON )

if( MSVC )
    include( cmake/VSHelper.cmake )

    generate_groups( ${CMAKE_CURRENT_SOURCE_DIR} sources )
    generate_groups( ${CMAKE_CURRENT_SOURCE_DIR} include )
endif( MSVC )

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DEXTENDEDLIB_DEBUG" )

include_directories( include )

include( CheckFunctionExists )

if( WIN32 )
    set( PLATFORM_DIR "msw" )
elseif( UNIX )
    set( PLATFORM_DIR "linux" )
endif()

set( SRC_LIST
     sources/format_string.cpp
     sources/log.cpp
     sources/runtime_error.cpp
)

set( INC_LIST
     include/ExtendedLib/byte_vector.h
     include/ExtendedLib/broadcaster.h
     include/ExtendedLib/callback.h
     include/ExtendedLib/defs.h
     include/ExtendedLib/dispatched_callback.h
     include/ExtendedLib/format_string.h
     include/ExtendedLib/log.h
     include/ExtendedLib/log_common.h
     include/ExtendedLib/memory.h
     include/ExtendedLib/callback_dispatcher.h
     include/ExtendedLib/runtime_error.h
     include/ExtendedLib/shared_lib.h
     include/ExtendedLib/shared_ptr.h
     include/ExtendedLib/thread.h
     include/ExtendedLib/${PLATFORM_DIR}/callback_dispatcher.h
)

if( WIN32 )
    set( SRC_LIST ${SRC_LIST}
         sources/msw/helpers.cpp
         sources/msw/callback_dispatcher.cpp
    )

    set( INC_LIST ${INC_LIST}
         sources/msw/helpers.h
    )

    if( NOT MINGW )
        set( SRC_LIST ${SRC_LIST}
             sources/msw/thread.cpp
        )
    endif( NOT MINGW )
endif( WIN32 )


if( BUILD_SHARED_LIBS )
    add_definitions( -DEXTENDEDLIB_BUILD_SHARED_LIB )
endif()

set( PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

include( cmake/generate_product_version.cmake )

generate_product_version(
    PRODUCT_VERSION_FILES
    NAME ${PROJECT_NAME}
    VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
    VERSION_MINOR ${PROJECT_VERSION_MINOR}
    VERSION_PATCH ${PROJECT_VERSION_PATCH}
    COMPANY_NAME "www.gdr-sistemas.com"
    COMPANY_COPYRIGHT "(C) Copyright 2016 Jesus Gonzalez"
)

add_library( ${PROJECT_NAME} ${SRC_LIST} ${INC_LIST} ${PRODUCT_VERSION_FILES} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11 )
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 1 )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_dbg" )
set_target_properties( ${PROJECT_NAME} PROPERTIES COVERAGE_POSTFIX "_cov" )
set_target_properties( ${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} )
if( WIN32 )
    set_target_properties( ${PROJECT_NAME} PROPERTIES PREFIX "" )
    set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORT_PREFIX "" )
else()
    set_target_properties( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Extended" )
endif( WIN32 )

add_dependencies( build ${PROJECT_NAME} )

install( TARGETS ${PROJECT_NAME} DESTINATION lib )

install( DIRECTORY include/ DESTINATION include )

include( DocsTargets.cmake )
