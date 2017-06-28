cmake_minimum_required( VERSION 3.1 )

project( ExtendedLib.AllTests )

enable_testing()

#set( COVERAGE_EXCLUDED \"*/libs/*\" \"*/sources/CAL/*\" \"*/sources/EOL/CAL/*\" )

include( CommonTestTargets.cmake )

# Test modules

add_subdirectory( format_string )
add_subdirectory( callback )
add_subdirectory( dispatched_callback )
add_subdirectory( broadcaster )
add_subdirectory( callback_dispatcher_msw )
add_subdirectory( log )
add_subdirectory( runtime_error )
