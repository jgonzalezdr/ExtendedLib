cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.Test.log )

# Test configuration

include_directories(
     ${PROD_SOURCE_DIR}/include
     ${PROD_BINARY_DIR}/include
 )

add_definitions( -DLOG_PRIORITY_MAX=7 )

set( PROD_SRC_FILES
     ${PROD_SOURCE_DIR}/sources/string.cpp
     ${PROD_SOURCE_DIR}/sources/runtime_error.cpp
     ${PROD_SOURCE_DIR}/sources/log.cpp
)

set( TEST_SRC_FILES
     log_test.cpp
     ${MOCKS_DIR}/win32_os_mock.cpp
)

# Generate test target

include( ../GenerateTest.cmake )
