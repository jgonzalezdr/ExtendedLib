cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.Test.dptch_callback )

# Test configuration

include_directories(
    ${PROD_SOURCE_DIR}/include
    ${PROD_BINARY_DIR}/include
)

set( TEST_SRC_FILES 
     dispatched_callback_test.cpp
)

# Generate test target

include( ../GenerateTest.cmake )
