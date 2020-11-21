cmake_minimum_required( VERSION 3.1 )

project( Extended VERSION 1.0.0 )

if( NOT CMAKE_PROJECT_NAME MATCHES "${PROJECT_NAME}.*" )

    # Seems like we are being imported into another project

    if( TARGET_NAMESPACE )
        set( TARGET_NAMESPACE "${TARGET_NAMESPACE}." )
    endif()

    add_custom_target( ${TARGET_NAMESPACE}build )

    unset( CMAKE_MODULE_PATH )

endif()

option( BUILD_STATIC_LIB "Build static library" ON )
option( BUILD_SHARED_LIB "Build shared library" ON )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/" )

if( MSVC )
    include( VisualStudioHelper )

    generate_groups( ${CMAKE_CURRENT_SOURCE_DIR} sources )
    generate_groups( ${CMAKE_CURRENT_SOURCE_DIR} include )
endif( MSVC )

if( WIN32 )
    set( PLATFORM_DIR "msw" )
elseif( UNIX )
    set( PLATFORM_DIR "linux" )
endif()

#
# Source files
#

set( SRC_LIST
     sources/string.cpp
     sources/log.cpp
     sources/runtime_error.cpp
)

if( WIN32 )
    set( SRC_LIST ${SRC_LIST}
         sources/msw/helpers.cpp
         sources/msw/callback_dispatcher.cpp
    )

    if( NOT MINGW )
        set( SRC_LIST ${SRC_LIST}
             sources/msw/thread.cpp
        )
    endif( NOT MINGW )
endif( WIN32 )

#
# Header files
#

set( INC_LIST
     include/Extended/byte_vector.hpp
     include/Extended/broadcaster.hpp
     include/Extended/callback.hpp
     include/Extended/defs.hpp
     include/Extended/dispatched_callback.hpp
     include/Extended/string.hpp
     include/Extended/log.hpp
     include/Extended/log_common.hpp
     include/Extended/callback_dispatcher.hpp
     include/Extended/runtime_error.hpp
     include/Extended/thread.hpp
     include/Extended/${PLATFORM_DIR}/callback_dispatcher.hpp
     ${CMAKE_CURRENT_BINARY_DIR}/include/extended_config.hpp
)

if( WIN32 )
    set( INC_LIST ${INC_LIST}
         sources/msw/helpers.hpp
    )
endif( WIN32 )

#
# Project information
#

if( NOT PROJECT_VERSION_BUILD )
    set( PROJECT_VERSION_BUILD ${PROJECT_VERSION_TWEAK} )
endif()

include( GenerateProductVersion )

generate_product_version(
    PRODUCT_VERSION_FILES
    NAME ${PROJECT_NAME}
    FILE_DESCRIPTION "Extended C++ functionalities library"
    VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
    VERSION_MINOR ${PROJECT_VERSION_MINOR}
    VERSION_PATCH ${PROJECT_VERSION_PATCH}
    VERSION_BUILD ${PROJECT_VERSION_BUILD}
    VERSION_SUFFIX "${PROJECT_VERSION_SUFFIX}"
    COMPANY_NAME "www.gdr-sistemas.com"
    COMPANY_COPYRIGHT "(C) Copyright 2016 Jesus Gonzalez"
    PRIVATE_BUILD "${PRIVATE_BUILD_INFO}"
    SPECIAL_BUILD "${SPECIAL_BUILD_INFO}"
    PRERELEASE "${PRERELEASE}"
    PATCHED "${PATCHED_RELEASE}"
    HOMEPAGE "http://example.com/yourproduct/"
    MAINTAINER "Your Name <yourname@example.com>"
    EXPORT_TO_PARENT_SCOPE
)

#
# Configuration file definition
#

if( BUILD_SHARED_LIB AND NOT BUILD_STATIC_LIB )
    set( CONFIG_SHARED_LIB "\n#ifndef ${PROJECT_NAME}_SHARED_LIB\n#define ${PROJECT_NAME}_SHARED_LIB 1\n#endif\n" )
endif()

configure_file( templates/Config.hpp.in include/extended_config.hpp )

#
# Library definition
#

if( MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc" )
endif()

if ( BUILD_SHARED_LIB )
    #
    # Shared library definition
    #

    add_library( ${PROJECT_NAME} SHARED ${SRC_LIST} ${INC_LIST} ${PRODUCT_VERSION_FILES} )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_SHARED_LIB" )

    target_include_directories( ${PROJECT_NAME} PUBLIC include )
    target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include )

    #>>> Uncomment and edit the following commands to add compile definitions
    #target_compile_definitions( ${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_MY_DEFINITION" )

    add_dependencies( ${TARGET_NAMESPACE}build ${PROJECT_NAME} )

    #
    # C++ standard
    #

    set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11 )
    set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 1 )

    #
    # Shared library properties
    #

    set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_dbg" )
    set_target_properties( ${PROJECT_NAME} PROPERTIES COVERAGE_POSTFIX "_cov" )
    set_target_properties( ${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} )
endif()

if ( BUILD_STATIC_LIB )
    #
    # Static library definition
    #

    add_library( ${PROJECT_NAME}_static STATIC ${SRC_LIST} ${INC_LIST} ${PRODUCT_VERSION_FILES} )

    target_include_directories( ${PROJECT_NAME}_static PUBLIC include )
    target_include_directories( ${PROJECT_NAME}_static PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include )

    #>>> Uncomment and edit the following commands to add compile definitions
    #target_compile_definitions( ${PROJECT_NAME}_static PUBLIC "${PROJECT_NAME}_MY_DEFINITION" )

    add_dependencies( ${TARGET_NAMESPACE}build ${PROJECT_NAME}_static )

    #
    # C++ standard
    #

    set_property( TARGET ${PROJECT_NAME}_static PROPERTY CXX_STANDARD 11 )
    set_property( TARGET ${PROJECT_NAME}_static PROPERTY CXX_STANDARD_REQUIRED 1 )

    #
    # Static library properties
    #

    set_target_properties( ${PROJECT_NAME}_static PROPERTIES DEBUG_POSTFIX "_dbg" )
    set_target_properties( ${PROJECT_NAME}_static PROPERTIES COVERAGE_POSTFIX "_cov" )
    set_target_properties( ${PROJECT_NAME}_static PROPERTIES VERSION ${PROJECT_VERSION} )
endif()

#
# Install
#

include( InstallersSetup.cmake )

if ( BUILD_SHARED_LIB )
    if( WIN32 )
        install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION lib )
        install( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib )
    else()
        install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib )
    endif()
endif()

if ( BUILD_STATIC_LIB )
    install( TARGETS ${PROJECT_NAME}_static ARCHIVE DESTINATION lib )
endif()

install( DIRECTORY include/ DESTINATION include ) 
install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/ DESTINATION include ) 

include( DocsTargets.cmake )
