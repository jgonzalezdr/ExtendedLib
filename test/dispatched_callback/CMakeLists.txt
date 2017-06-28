cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.Test.dptch_callback )

# Test configuration

set( PROD_DIR ../../lib )

include_directories(
    ${PROD_DIR}/include
)

set( TEST_SRC_FILES 
     dispatched_callback_test.cpp
)

# Generate test target

include( ../GenerateTest.cmake )
