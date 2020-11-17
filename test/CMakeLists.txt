cmake_minimum_required( VERSION 3.3 )

option( ENABLE_TEST "Enable building tests" ON )
option( COVERAGE_VERBOSE "Coverage analysis is performed in verbose mode" OFF )

if( ENABLE_TEST )

    enable_testing()

    include( CommonTestTargets.cmake )

    set( PROD_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../lib )
    set( PROD_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../lib )
    set( MOCKS_DIR ${CMAKE_CURRENT_LIST_DIR}/mocks )

    #
    # Test modules
    #

    add_subdirectory( format_string )
    add_subdirectory( callback )
    add_subdirectory( dispatched_callback )
    add_subdirectory( broadcaster )
    add_subdirectory( callback_dispatcher_msw )
    add_subdirectory( log )
    add_subdirectory( runtime_error )

endif()
