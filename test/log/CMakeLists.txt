cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.Test.log )

# Test configuration

set( PROD_DIR ../../lib )

include_directories(
    ${PROD_DIR}/include
)

add_definitions( -DLOG_PRIORITY_MAX=7 )

set( PROD_SRC_FILES
     ${PROD_DIR}/sources/format_string.cpp
     ${PROD_DIR}/sources/runtime_error.cpp
     ${PROD_DIR}/sources/log.cpp
)

set( TEST_SRC_FILES
     log_test.cpp
     ../mocks/win32_os_mock.cpp
)

# Generate test target

include( ../GenerateTest.cmake )
