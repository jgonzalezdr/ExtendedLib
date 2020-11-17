cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.Test.callback_dptchr_msw )

# Test configuration

include_directories(
     ${PROD_SOURCE_DIR}/include
     ${PROD_BINARY_DIR}/include
 )

set( PROD_SRC_FILES
     ${PROD_SOURCE_DIR}/sources/msw/callback_dispatcher.cpp
     ${PROD_SOURCE_DIR}/sources/msw/helpers.cpp
)

set( TEST_SRC_FILES
     callback_dispatcher_test.cpp
     ${MOCKS_DIR}/runtime_error_mock.cpp
)

# Generate test target

include( ../GenerateTest.cmake )
