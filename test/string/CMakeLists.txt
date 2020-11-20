cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.Test.string )

# Test configuration

include_directories(
     ${PROD_SOURCE_DIR}/include
     ${PROD_BINARY_DIR}/include
 )

set( PROD_SRC_FILES
     ${PROD_SOURCE_DIR}/sources/string.cpp
)

set( TEST_SRC_FILES
     string_test.cpp
)

# Generate test target

include( ../GenerateTest.cmake )
