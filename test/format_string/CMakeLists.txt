cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.Test.format_string )

# Test configuration

set( PROD_DIR ../../lib )

include_directories(
    ${PROD_DIR}/include
)

set( PROD_SRC_FILES
     ${PROD_DIR}/sources/format_string.cpp
)

set( TEST_SRC_FILES
     format_string_test.cpp
)

# Generate test target

include( ../GenerateTest.cmake )
